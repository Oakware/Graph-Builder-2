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

#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QtQuick>
#include "../AbstractGraph/abstractgraph.h"

class GraphView: public QQuickItem{
	Q_OBJECT
public:
	explicit GraphView(QQuickItem *parent = 0);

///					<InfoProperties>
public:		// declare
	Q_PROPERTY(AbstractGraph* graph		READ getGraph		WRITE setGraph		NOTIFY graphChanged)
	Q_PROPERTY(int selectedVertex		READ getSelectedVertex					NOTIFY vertexSelected)
	Q_PROPERTY(QList<int> selectedRib	READ getSelectedRibList					NOTIFY ribSelected)

	Q_PROPERTY(bool editable			READ isEditable		WRITE setEditable	NOTIFY editableChanged)
	Q_PROPERTY(GVState state			READ getState							NOTIFY stateChanged)
	Q_PROPERTY(QColor backgroundColor	READ getBackColor	WRITE setBackColor	NOTIFY backColorChanged)
	Q_PROPERTY(QSize minimumSize		READ getMinimumSize						NOTIFY minimumSizeChanged)

	enum GVState{Normal, VertexMoving, RibAdd, RibRemove, RibSelect};
	Q_ENUM(GVState)

private:
	AbstractGraph *graph;
	int verSel; ABrib ribSel;

	bool    editable;
	GVState	currentState;
	QColor  backColor;
	QSize	minimumSize;

public:		// read
	inline AbstractGraph *getGraph();
	void setGraph(AbstractGraph *gr);

	inline int		  getSelectedVertex()	const;
	inline ABrib	  getSelectedRib()		const;
	inline QList<int> getSelectedRibList()	const;

	inline bool		isEditable()			const;
	void			setEditable(bool ie);
	inline GVState  getState()				const;
	inline QColor   getBackColor()			const;
	void			setBackColor(const QColor &col);
	inline QSize	getMinimumSize()		const;

private:	// write
	void setSelectedVertex(int v = 0);
	void setSelectedRib(const ABrib &r = ABrib());
	void setState(GVState st);
	void setMinimumSize(QSize sz);
///					</InfoProperties>

public: ///			<Controls/>
	void setStdVertex(const TvertexInfo &vi);
	void setStdRib(const TribInfo &ri);

												/// <Engine/>
private: ///		<Data/>
	TvertexInfo stdVertex;
	TribInfo    stdRib;

	QList <QQuickItem*> vertexItems;
	QList <QQuickItem*> ribItems;
	QQuickItem *addRibItem;

	QTimer moveTimer;		// Timer for vertex moving
	int curX, curY;			// Cursor coordinates
	int ribFromVer;

private slots: ///	<GraphControl/>
	void graphUpdate();
	void graphDestroyed();
	void graphVertexUpdate(int ver);
	void graphVertexRemove(int);
	void graphRibUpdate(ABrib r);
	void graphRibRemove(ABrib r);

private: ///		<Edit/>
	inline bool borderCross(int x, int y, int r);
	void setRVSelected(int px, int py);

private slots:
	void stopEditing();
	void moveStart();
	void moveEnd();

private: ///		<PaintEvent/>
	QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData*);
	void updateVertexes();
	void updateRibs();
	void graphRemove();

	void setVertexParams(QQuickItem *vi, int ver);
	void setRibParams(QQuickItem *ri, int ver, int rib);
	void setupAddRibParams();
	void updateAddRibParams();

private: ///		<EditEvents/>
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);

	void mouseMoveEvent(QMouseEvent *ev);
	void hoverMoveEvent(QHoverEvent *ev);

public slots: ///	<ContextMenu/>
	void cmAddVertex();
	void cmRemoveVertex();

	void cmAddRibBegin();
	void cmSelectRibBegin();
	void cmRemoveRibBegin();
	void cmRemoveRib();

private:
	void openContextMenu();
	void cmAddRibEnd();
	void cmSelectRibEnd();
	void cmRemoveRibEnd();

signals: ///		<Signals/>
	void contextMenu();
	void graphChanged();
	void vertexSelected(int=0);
	void ribSelected(ABrib=ABrib());

	void editableChanged();
	void stateChanged();
	void backColorChanged();
	void minimumSizeChanged();
};

				/// <Implement/>

AbstractGraph *GraphView::getGraph(){return graph;}
int GraphView::getSelectedVertex() const{return verSel;}
ABrib GraphView::getSelectedRib() const{return ribSel;}

QList<int> GraphView::getSelectedRibList() const{
	QList<int> rib;
	rib << ribSel.from << ribSel.to;
	return rib;
}

bool GraphView::isEditable() const{return editable;}
GraphView::GVState GraphView::getState() const{return currentState;}
QColor GraphView::getBackColor() const{return backColor;}
QSize GraphView::getMinimumSize() const{return minimumSize;}

#endif // GRAPHVIEW_H
