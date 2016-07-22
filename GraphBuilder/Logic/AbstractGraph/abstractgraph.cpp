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

#include "abstractgraph.h"
#include "point_to_line.h"

#define NOT_IN_RANGE(X, FROM, TO) ((X) < (FROM) || (X) > (TO))

#define CHANGES_BEGIN \
	if(!changesDeepFlag){emit aboutToChange();}\
	++changesDeepFlag;

#define CHANGES_END \
	--changesDeepFlag;\
	if(!changesDeepFlag){emit changed();}

AbstractGraph::AbstractGraph(QObject *parrent):
	QObject(parrent),
	oriented(false), changesDeepFlag(0)
{}

AbstractGraph::~AbstractGraph(){
	while(vertexList.count())
		delete vertexList.takeLast();
}

void AbstractGraph::setOriented(bool io){
	if(io == oriented) return;

	CHANGES_BEGIN
	emit atOrientChanged();

	if(io){
		oriented = io;
		emit orientChanged();
		CHANGES_END
		return;
	}
				// if switch to not oriented - add ribs
	for(int i=1; i<=vertexCount(); ++i){
		const Tvertex &ver = *vertexList[i-1];

		for(int j=0; j<ver.ribs.count(); ++j){
			int to = vertexList.indexOf(ver[j].to)+1;

			if(!isRibExist(ABrib(to, i)))
				addRib(ABrib(to, i), TribInfo(ver[j]));
			else
				updateRib(ABrib(to, i), TribInfo(ver[j]));

		}
	}

	oriented = io;

	emit orientChanged();
	CHANGES_END
}

int AbstractGraph::ribCount() const{
	int c = 0;
	for(int i=0; i<vertexCount(); ++i)
		c += vertexList[i]->ribs.count();

	if(!oriented)	return c/2;
	else			return c;
}

bool AbstractGraph::addVertex(int px, int py, const TvertexInfo &vi){
	if(vi.radius < 0) return false;

	CHANGES_BEGIN
	emit atVertexAdd(vertexList.count()+1);

	Tvertex *newVer = new Tvertex(vi);
	newVer->x = px; newVer->y = py;
	vertexList << newVer;

	emit vertexAdded(vertexList.count());
	CHANGES_END

	return true;
}

bool AbstractGraph::updateVertex(int v, const TvertexInfo &vi){
	if(NOT_IN_RANGE(v, 1, vertexCount()) || vi.radius < 0)
		return false;

	CHANGES_BEGIN
	emit atVertexUpdate(v);

	Tvertex &ver = *vertexList[v-1];
	ver.setProperties(vi);

	emit vertexUpdated(v);
	CHANGES_END

			return true;
}

bool AbstractGraph::moveVertex(int v, int px, int py){
	if(NOT_IN_RANGE(v, 1, vertexCount()))
		return false;

	CHANGES_BEGIN
	emit atVertexMove(v);

	Tvertex &ver = *vertexList[v-1];
	ver.x = px;
	ver.y = py;

	emit vertexMoved(v);
	CHANGES_END

	return true;
}

bool AbstractGraph::removeVertex(int v){
	if(NOT_IN_RANGE(v, 1, vertexCount()))
		return false;

	CHANGES_BEGIN
	emit atVertexRemove(v);

				// Delete all ribs to v
	for(int i=1; i <= vertexCount(); ++i){
		if(isRibExist(ABrib(i,v)))
			removeRib(ABrib(i,v));
	}
				// Delete all ribs from v
	if(oriented){
		Tvertex &ver = *vertexList[v-1];
		for(int i=0; i<ver.ribs.count(); ++i)
			removeRib(ABrib(v, vertexList.indexOf(ver[i].to)+1));
	}

	delete vertexList.takeAt(v-1);

	emit vertexRemoved(v);
	CHANGES_END

	return true;
}

bool AbstractGraph::addRib(const ABrib r, const TribInfo &ri){
	if(NOT_IN_RANGE(r.from, 1, vertexCount())
		|| NOT_IN_RANGE(r.to, 1, vertexCount())
		|| (r.from == r.to))
	{
		return false;
	}

	if(isRibExist(r)){		// Update if exist
		updateRib(r, ri);
		return true;
	}

	CHANGES_BEGIN
	emit atRibAdd(r);

	Tvertex &Vfrom = *vertexList[r.from-1];
	Tvertex &Vto = *vertexList[r.to-1];

	Trib rib(ri);		// Add a -> b rib
	rib.to = &Vto;
	Vfrom.ribs << rib;

	if(!oriented){		// Add a <- b rib
		rib.to = &Vfrom;
		Vto.ribs << rib;
	}

	emit ribAdded(r);
	CHANGES_END

	return true;
}

bool AbstractGraph::updateRib(const ABrib r, const TribInfo &ri){
	int rab = findABrib(r);			// a -> b rib
	if(rab < 0) return false;

	CHANGES_BEGIN
	emit atRibUpdate(r);

	(*vertexList[r.from-1])[rab].setProperties(ri);

				// a <- b rib
	if(!oriented){
		rab = findABrib(ABrib(r.to, r.from));
		(*vertexList[r.to-1])[rab].setProperties(ri);
	}

	emit ribUpdated(r);
	CHANGES_END

	return true;
}

bool AbstractGraph::removeRib(const ABrib r){
	int rab = findABrib(r);			// Remove rib a -> b
	if(rab < 0) return false;

	CHANGES_BEGIN
	emit atRibRemove(r);

	vertexList[r.from-1]->ribs.remove(rab);

	if(!oriented){			// Remove rib a <- b
		rab = findABrib(ABrib(r.to, r.from));
		vertexList[r.to-1]->ribs.remove(rab);
	}

	emit ribRemoved(r);
	CHANGES_END

	return true;
}

void AbstractGraph::clear(){
	CHANGES_BEGIN
	emit atClear();

	while(vertexCount())
		delete vertexList.takeLast();

	emit cleared();
	CHANGES_END
}

AbstractGraph *AbstractGraph::clone() const{
	AbstractGraph *ngr = new AbstractGraph;
	ngr->setGraphModel(getGraphModel());
	return ngr;
}

GraphModel AbstractGraph::getGraphModel() const{
	GraphModel gr;
	gr.oriented = oriented;

	for(int i=0; i<vertexCount(); ++i){
		gr.vertexes << TvertexInfo(*vertexList[i]);
		for(int j=0; j<vertexList[i]->ribs.count(); ++j){
			gr.ribs << TribInfo((*vertexList[i])[j],  i+1,
				   vertexList.indexOf((*vertexList[i])[j].to)+1);
		}
	}

	return gr;
}

void AbstractGraph::setGraphModel(const GraphModel &gr){
	CHANGES_BEGIN
	clear();
	setOriented(gr.oriented);

	for(int i=0; i<gr.vertexes.count(); ++i)
		addVertex(gr.vertexes[i].x, gr.vertexes[i].y, gr.vertexes[i]);

	for(int i=0; i<gr.ribs.count(); ++i)
		addRib(ABrib(gr.ribs[i].from, gr.ribs[i].to), gr.ribs[i]);

	CHANGES_END
}

TvertexInfo AbstractGraph::getVertexInfo(int v) const{
	if(NOT_IN_RANGE(v, 1, vertexCount()))
		return TvertexInfo();

	return TvertexInfo(*vertexList[v-1], v);
}

TribInfo AbstractGraph::getRibInfo(const ABrib &r) const{
	int rab = findABrib(r);
	if(rab < 0) return TribInfo();

	return TribInfo((*vertexList[r.from-1])[rab], r.from, r.to);
}

QRect AbstractGraph::vertexToRound(int v) const{
	if(NOT_IN_RANGE(v, 1, vertexCount()))
		return QRect();

	const Tvertex &ver = *vertexList[v-1];
	return QRect(QPoint(ver.x-ver.radius, ver.y-ver.radius),
				 QPoint(ver.x+ver.radius, ver.y+ver.radius));
}

QLine AbstractGraph::ribToLine(const ABrib &r) const{
	int rab = findABrib(r);
	if(rab < 0) return QLine();

	const Tvertex &Vfrom = *vertexList[r.from-1];
	const Tvertex &Vto = *vertexList[r.to-1];

	int vx = Vto.x - Vfrom.x;
	int vy = Vto.y - Vfrom.y;
	double l = sqrt(sqr(vx) + sqr(vy));

	QPoint p1(Vfrom.x + vx*Vfrom.radius/l, Vfrom.y + vy*Vfrom.radius/l);
	QPoint p2(Vto.x   - vx*Vto.radius/l,   Vto.y   - vy*Vto.radius/l);

	return QLine(p1, p2);
}

int AbstractGraph::findVertex(int px, int py, int r, int exept) const{
	for(int i=1; i <= vertexCount(); ++i){
		const Tvertex &ver = *vertexList[i-1];

					// Miss hidden or exept vertexes
		if(ver.isHidden || (i == exept)) continue;

					// If rounds cross
		if(sqrt(sqr(px-ver.x) + sqr(py-ver.y)) <= ver.radius + r)
			return i;
	}

	return 0;
}

ABrib AbstractGraph::findRib(int px, int py, int r) const{
	QPoint m(px,py);
	QLine rLine; ABrib rib;
	double l;

	GraphModel gr = getGraphModel();
	for(int i=0; i<gr.ribs.count(); ++i){
		rib = ABrib(gr.ribs[i].from, gr.ribs[i].to);

					// Miss hidden ribs
		if(gr.ribs[i].hidden || vertexList[rib.from-1]->isHidden
							 || vertexList[rib.to  -1]->isHidden)
			continue;

		rLine = ribToLine(rib);
		l = pointToLine(m, rLine);
		if(l <= r) return rib;
	}

	return ABrib();
}

int AbstractGraph::findABrib(const ABrib &r) const{
	if(NOT_IN_RANGE(r.from, 1, vertexCount())
		|| NOT_IN_RANGE(r.to, 1, vertexCount()))
	{
		return -1;
	}

	return findABrib(*vertexList[r.from-1], *vertexList[r.to-1]);
}

int AbstractGraph::findABrib(const Tvertex &va, const Tvertex &vb) const{
	for(int i=0; i < va.ribs.count(); ++i){		// Find a -> b rib
		if(va[i].to == &vb)
			return i;
	}
	return -1;
}
