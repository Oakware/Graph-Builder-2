/*****************************************************************************
 * Copyright © 2016 Oakware - All Rights Reserved
 *
 * Github : <https://github.com/Oakware>
 * Authors: Ivan Kondratuk <i.o.kondratuk@gmail.com>
 *
 * This file is part of Graph Builder.
 *
 * Graph Builder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graph Builder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graph Builder.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "graphview.h"

#define GV_DEF_TIMER_INTERVAL 300
#define sqr(A) ((A)*(A))

#define GRAPH_CD_FUNC(GR, func) {\
	func(GR, &AbstractGraph::destroyed, this, &GraphView::graphDestroyed);\
	func(GR, &AbstractGraph::changed,	this, &GraphView::graphUpdate);\
	func(GR, &AbstractGraph::atClear,   this, &GraphView::stopEditing);\
\
	func(GR, &AbstractGraph::vertexUpdated,  this, &GraphView::graphVertexUpdate);\
	func(GR, &AbstractGraph::atVertexRemove, this, &GraphView::graphVertexRemove);\
\
	func(GR, &AbstractGraph::ribUpdated,  this, &GraphView::graphRibUpdate);\
	func(GR, &AbstractGraph::atRibRemove, this, &GraphView::graphRibRemove);\
}

#define GRAPH_CONNECT(GR)    GRAPH_CD_FUNC(GR, connect)
#define GRAPH_DISCONNECT(GR) GRAPH_CD_FUNC(GR, disconnect)

GraphView::GraphView(QQuickItem *parent):
	QQuickItem(parent),
	graph(NULL), verSel(0),
	editable(true), currentState(Normal),
	backColor(Qt::white), minimumSize(100, 100)
{
	setFlag(ItemHasContents);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::AllButtons);

	moveTimer.setInterval(GV_DEF_TIMER_INTERVAL);
	moveTimer.setSingleShot(true);
	connect(&moveTimer, SIGNAL(timeout()), this, SLOT(moveStart()));
}

void GraphView::setGraph(AbstractGraph *gr){
	if(gr == graph) return;
	stopEditing();

	if(graph) GRAPH_DISCONNECT(graph)
	graph = gr;
	if(graph) GRAPH_CONNECT(graph)

	graphUpdate();
	emit graphChanged();
}

void GraphView::setEditable(bool ie){
	if(ie == editable) return;
	editable = ie;
	if(!editable){
		stopEditing();
		graphUpdate();
	}

	emit editableChanged();
}

void GraphView::setBackColor(const QColor &col){
	if(backColor == col) return;
	backColor = col;
	graphUpdate();
	update();
	emit backColorChanged();
}

void GraphView::setSelectedVertex(int v){
	if(verSel == v) return;
	verSel = v;
	emit vertexSelected(v);
}

void GraphView::setSelectedRib(const ABrib &r){
	if(ribSel == r) return;
	ribSel = r;
	emit ribSelected(r);
}

void GraphView::setState(GraphView::GVState st){
	if(currentState == st) return;
	currentState = st;
	emit stateChanged();
}

void GraphView::setMinimumSize(QSize sz){
	if(minimumSize == sz) return;
	minimumSize = sz;
	emit minimumSizeChanged();
}

void GraphView::setStdVertex(const TvertexInfo &vi){
	if(vi.radius < 0) return;
	stdVertex = vi;
}

void GraphView::setStdRib(const TribInfo &ri){
	stdRib = ri;
}

void GraphView::graphUpdate(){
	if(graph == NULL){
		setMinimumSize(QSize(100, 100));
		graphRemove();
		return;
	}

	updateVertexes();
	updateRibs();
}

void GraphView::graphDestroyed(){
	graph = NULL;
	graphRemove();
	emit graphChanged();
}

void GraphView::graphVertexUpdate(int ver){
	if(ver == verSel && graph->vertexList[verSel-1]->isHidden)
		stopEditing();
}

void GraphView::graphVertexRemove(int){
	stopEditing();
}

void GraphView::graphRibUpdate(ABrib r){
	if((r == ribSel) || (!graph->isOriented() &&
							r.notOrientEqual(ribSel)))
	{
		TribInfo ri = graph->getRibInfo(r);
		if(ri.hidden) setSelectedRib();
	}
}

void GraphView::graphRibRemove(ABrib r){
	if((r == ribSel) || (!graph->isOriented() &&
							r.notOrientEqual(ribSel)))
		{setSelectedRib();}
}

bool GraphView::borderCross(int x, int y, int r){
	return (x-r < 0 || x+r >= width() ||
			y-r < 0 || y+r >= height());
}

void GraphView::setRVSelected(int px, int py){
	int ver = graph->findVertex(px, py);
	setSelectedVertex(ver);

	if(!ver)
		setSelectedRib(graph->findRib(px, py, 7));
	else
		setSelectedRib();
}

void GraphView::stopEditing(){
	moveTimer.stop();
	if(currentState == VertexMoving)
		moveEnd();
	else if(currentState == RibAdd)
		delete addRibItem;

	setState(Normal);
	setSelectedVertex();
	setSelectedRib();
}

void GraphView::moveStart(){
	if(currentState != Normal) return;
	setState(VertexMoving);
	graphUpdate();
}

void GraphView::moveEnd(){
	setState(Normal);
	graphUpdate();
}

QSGNode *GraphView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData*){
	QSGSimpleRectNode *node = 0;
	node = static_cast<QSGSimpleRectNode*>(oldNode);
	if(!node)
		node = new QSGSimpleRectNode;

	node->setRect(boundingRect());
	node->setColor(backColor);

	node->markDirty(QSGNode::DirtyGeometry);
	return node;
}

void GraphView::updateVertexes(){
	QSize minSize(100, 100);
	QQmlEngine *eng = qmlEngine(this);

	if(graph->vertexCount() > vertexItems.count()){		// Add new
		int dif = graph->vertexCount() - vertexItems.count();
		QQmlComponent vc(eng, QUrl("qrc:/VertexItem.qml"));
		while(dif--){
			QQuickItem *vitm = qobject_cast<QQuickItem*>(vc.create());
			vitm->setParentItem(this);
			vertexItems << vitm;
		}
	}else if(vertexItems.count() > graph->vertexCount()){	// Remove old
		int dif = vertexItems.count() - graph->vertexCount();
		while(dif--)
			delete vertexItems.takeLast();
	}

	int c = 1;
	foreach(QQuickItem *vi, vertexItems){		// Vertexes
		const Tvertex &v = *graph->vertexList[c-1];
		if(!v.isHidden){
			if(v.x+v.radius+1 > minSize.width())  minSize.setWidth(v.x+v.radius+1);
			if(v.y+v.radius+1 > minSize.height()) minSize.setHeight(v.y+v.radius+1);
		}

		setVertexParams(vi, c++);
	}

	if(currentState != VertexMoving)	// Min size
		setMinimumSize(minSize);
}

void GraphView::updateRibs(){
	QQmlEngine *eng = qmlEngine(this);
	QQmlComponent rc(eng, QUrl("qrc:/RibItem.qml"));
	int c = 0;

	for(int i=1; i <= graph->vertexCount(); ++i){
		for(int j=0; j < graph->vertexList[i-1]->ribs.count(); ++j){
			if(c >= ribItems.count()){		// Create new
				QQuickItem *ritm = qobject_cast<QQuickItem*>(rc.create());
				ritm->setParentItem(this);
				ribItems << ritm;
			}

			setRibParams(ribItems[c++], i, j);
		}
	}

	if(ribItems.count() > c){			// Remove other
		int dif = ribItems.count() - c;
		while(dif--)
			delete ribItems.takeLast();
	}
}

void GraphView::graphRemove(){
	setCursor(Qt::ArrowCursor);
	while(vertexItems.count())
		delete vertexItems.takeFirst();
	while(ribItems.count())
		delete ribItems.takeFirst();
}

void GraphView::setVertexParams(QQuickItem *vi, int ver){
	const Tvertex &vdr = *graph->vertexList[ver-1];
	if(vdr.isHidden){
		vi->setProperty("isHidden", true);
		return;
	}else
		vi->setProperty("isHidden", false);

	vi->setProperty("number", ver);
	vi->setProperty("posX", vdr.x);
	vi->setProperty("posY", vdr.y);
	vi->setProperty("rad", vdr.radius);
	vi->setProperty("vtext", vdr.text);

	QColor col = vdr.color;
	if(ver == verSel){
		vi->setProperty("isSelected", true);
		if(currentState == VertexMoving || currentState == RibSelect
										|| currentState == RibRemove)
			col = QColor(vdr.color).lighter();
	}else
		vi->setProperty("isSelected", false);
	vi->setProperty("col", col);

	if(col.value() > 200 && col.lightness() > 200)
		vi->setProperty("isTextBlack", true);
	else
		vi->setProperty("isTextBlack", false);

	if(backColor.value() > 200 && backColor.lightness() > 200)
		vi->setProperty("isBorderBlack", true);
	else
		vi->setProperty("isBorderBlack", false);
}

void GraphView::setRibParams(QQuickItem *ri, int ver, int rib){
	Tvertex &vfrom = *graph->vertexList[ver-1];
	Tvertex &vto = *((*graph->vertexList[ver-1])[rib].to);

	if(vfrom[rib].isHidden || vfrom.isHidden || vto.isHidden){
		ri->setProperty("isHidden", true);
		return;
	}else
		ri->setProperty("isHidden", false);

	ri->setProperty("weight", vfrom[rib].weight);
	ri->setProperty("col", vfrom[rib].color);
	ri->setProperty("rtext", vfrom[rib].text);

	ABrib r(ver, graph->vertexList.indexOf(&vto)+1);
	QLine ribL(graph->ribToLine(r));

	ri->setProperty("posX", ribL.x1());
	ri->setProperty("posY", ribL.y1());

	int x = ribL.dx(); int y = ribL.dy();
	double l = sqrt(sqr(x)+sqr(y));
	ri->setProperty("length", l);
	ri->setProperty("alpha", atan2(y, x)*180/M_PI);

	if(r == ribSel)
		ri->setProperty("isSelected", true);
	else
		ri->setProperty("isSelected", false);

	if(backColor.value() > 200 && backColor.lightness() > 200)
		ri->setProperty("isTextBlack", true);
	else
		ri->setProperty("isTextBlack", false);

	if(graph->isOriented()){
		ri->setProperty("isOriented", true);
		ri->setProperty("isTextVisible", true);
	}else{
		ri->setProperty("isOriented", false);
		if(r.from < r.to)
			ri->setProperty("isTextVisible", true);
		else
			ri->setProperty("isTextVisible", false);
	}
}

void GraphView::setupAddRibParams(){
	QQmlEngine *eng = qmlEngine(this);
	QQmlComponent rc(eng, QUrl("qrc:/RibItem.qml"));
	addRibItem = qobject_cast<QQuickItem*>(rc.create());
	addRibItem->setParentItem(this);

	int rx = graph->vertexList[ribFromVer-1]->x;
	int ry = graph->vertexList[ribFromVer-1]->y;
	addRibItem->setProperty("posX", rx);
	addRibItem->setProperty("posY", ry);

	addRibItem->setProperty("col", stdRib.color);
	addRibItem->setProperty("isOriented", true);
	addRibItem->setProperty("isHidden", false);
	updateAddRibParams();
}

void GraphView::updateAddRibParams(){
	if(!ribFromVer) return;

	int rx = graph->vertexList[ribFromVer-1]->x;
	int ry = graph->vertexList[ribFromVer-1]->y;

	int x = curX-rx; int y = curY-ry;
	double l = sqrt(sqr(x)+sqr(y));
	addRibItem->setProperty("length", l);
	addRibItem->setProperty("alpha", atan2(y, x)*180/M_PI);
}

void GraphView::mousePressEvent(QMouseEvent *ev){
	if(!(editable && graph)) return;

	curX = ev->x();			// Set selected
	curY = ev->y();

	if(ev->button() != Qt::LeftButton){
		if(currentState == Normal){
			setRVSelected(curX, curY);
			graphUpdate();
		}

		if(ev->button() == Qt::RightButton)
			openContextMenu();
		return;
	}

	setRVSelected(curX, curY);
	if(currentState == RibAdd) cmAddRibEnd();	// Add rib
	else if(currentState == RibSelect) cmSelectRibEnd();	// Select rib
	else if(currentState == RibRemove) cmRemoveRibEnd();	// Remove rib

	if(verSel > 0) moveTimer.start();	// Move timer start
	graphUpdate();
}

void GraphView::mouseReleaseEvent(QMouseEvent *ev){
	if(ev->button() != Qt::LeftButton) return;

	moveTimer.stop();
	if(currentState == VertexMoving)	// Move end
		moveEnd();
}

void GraphView::mouseDoubleClickEvent(QMouseEvent *ev){
	if(!(editable && graph) || ev->button() != Qt::LeftButton)
		return;

	if(verSel == 0) cmAddVertex();	// Add vertex
	else cmAddRibBegin();			// Rib add state
}

void GraphView::mouseMoveEvent(QMouseEvent *ev){
	if(moveTimer.isActive()){			// Stop moving
		if(graph->findVertex(ev->x(), ev->y()) != verSel)
			moveTimer.stop();
	}

	if(currentState == VertexMoving){			// Vertex move
		if(!verSel) return;

		Tvertex &ver = *graph->vertexList[verSel-1];
		int xto, yto;

		if(ev->x()-ver.radius < 0)
			xto = ver.radius;
		else if(ev->x()+ver.radius >= width())
			xto = width()-ver.radius-1;
		else
			xto = ev->x();

		if(ev->y()-ver.radius < 0)
			yto = ver.radius;
		else if(ev->y()+ver.radius >= height())
			yto = height()-ver.radius-1;
		else
			yto = ev->y();
		graph->moveVertex(verSel, xto, yto);
	}
}

void GraphView::hoverMoveEvent(QHoverEvent *ev){
	QPoint pos = ev->pos();
	if(currentState == RibAdd){			// Set current position
		curX = pos.x(); curY = pos.y();
		updateAddRibParams();
	}else if(graph){
		if(graph->findVertex(pos.x(), pos.y()) ||
			graph->findRib(pos.x(), pos.y(), 7).from)
		{
			setCursor(Qt::CrossCursor);
		}else
			setCursor(Qt::ArrowCursor);
	}
}

void GraphView::cmAddVertex(){
	if(!(editable && graph) || currentState != Normal
		|| borderCross(curX, curY, stdVertex.radius))
	{
		return;
	}

	graph->addVertex(curX, curY, stdVertex);
	setSelectedRib();
	setSelectedVertex(graph->vertexCount());
	graphUpdate();
}

void GraphView::cmRemoveVertex(){
	if(!(editable && graph) || currentState != Normal)
		return;

	graph->removeVertex(verSel);
}

void GraphView::cmAddRibBegin(){
	if(!(editable && graph) || currentState != Normal || !verSel)
		return;

	setState(RibAdd);
	ribFromVer = verSel;
	setupAddRibParams();
}

void GraphView::cmSelectRibBegin(){
	if(!(editable && graph) || currentState != Normal || !verSel)
		return;

	setState(RibSelect);
	ribFromVer = verSel;
	graphUpdate();
}

void GraphView::cmRemoveRibBegin(){
	if(!(editable && graph) || currentState != Normal || !verSel)
		return;

	setState(RibRemove);
	ribFromVer = verSel;
	graphUpdate();
}

void GraphView::cmRemoveRib(){
	if(!(editable && graph) || currentState != Normal)
		return;

	graph->removeRib(ribSel);
}

void GraphView::openContextMenu(){
	if(currentState == Normal)
		emit contextMenu();
}

void GraphView::cmAddRibEnd(){
	setState(Normal);
	delete addRibItem;	// Remove add rib

	if(verSel > 0)
		graph->addRib(ABrib(ribFromVer, verSel), stdRib);
}

void GraphView::cmSelectRibEnd(){
	setState(Normal);
	if(verSel > 0){
		int vs = verSel;
		if(!graph->isRibExist(ABrib(ribFromVer, vs)))
			return;
		setSelectedVertex();
		setSelectedRib(ABrib(ribFromVer, vs));
		graphUpdate();
	}
}

void GraphView::cmRemoveRibEnd(){
	setState(Normal);
	if(verSel > 0)
		graph->removeRib(ABrib(ribFromVer, verSel));
}
