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

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include <QObject>
#include <QRect>

#include "abstractgraph_header.h"
#include "graph_struct_decl.h"
#include "graph_save_struct.h"

class AbstractGraph: public QObject{
	Q_OBJECT
	friend class GraphView;

public:
	explicit AbstractGraph(QObject *parrent = 0);
	~AbstractGraph();

public: ///		<InfoProperties/>
	Q_PROPERTY(bool oriented	READ isOriented WRITE setOriented NOTIFY orientChanged)
	Q_PROPERTY(int vertexCount	READ vertexCount)
	Q_PROPERTY(int ribCount		READ ribCount)

	inline bool isOriented()	const;
	void		setOriented(bool io);
	inline int  vertexCount()	const;
	int			ribCount()		const;

private: ///	<Data/>
	QList <Tvertex*> vertexList;	// < Vertexes <Ribs> >
	bool oriented;					// Oriented flag
	int changesDeepFlag;

///				<Controls>
public:								// Vertex
	bool addVertex(int px, int py, const TvertexInfo &vi = TvertexInfo());
	bool updateVertex(int v, const TvertexInfo &vi);
	bool moveVertex(int v, int px, int py);
	bool removeVertex(int v);
									// Rib
	bool addRib(const ABrib r, const TribInfo &ri = TribInfo());
	bool updateRib(const ABrib r, const TribInfo &ri);
	bool removeRib(const ABrib r);

public slots:
	void clear();
///				</Controls>

public: ///		<Convert/>
	Q_INVOKABLE AbstractGraph *clone()	const;
	GraphModel	getGraphModel()			const;
	void		setGraphModel(const GraphModel &gr);

	TvertexInfo getVertexInfo(int v)		const;
	TribInfo	getRibInfo(const ABrib &r)	const;

	QRect vertexToRound(int v)		const;
	QLine ribToLine(const ABrib &r)	const;

public: ///		<Search/>
	int		findVertex(int px, int py, int r = 0, int exept = -1) const;
	ABrib	findRib(int px, int py, int r = 0) const;
	inline bool isRibExist(const ABrib &r) const;

private: ///	<Engine/>
	int findABrib(const ABrib &r) const;
	int findABrib(const Tvertex &va, const Tvertex &vb) const;

///				<Signals>
signals:		// Vertex changes
	void atVertexAdd(int=0);    void vertexAdded(int=0);
	void atVertexUpdate(int=0); void vertexUpdated(int=0);
	void atVertexMove(int=0);   void vertexMoved(int=0);
	void atVertexRemove(int=0); void vertexRemoved(int=0);

				// Rib changes
	void atRibAdd(ABrib=ABrib());    void ribAdded(ABrib=ABrib());
	void atRibUpdate(ABrib=ABrib()); void ribUpdated(ABrib=ABrib());
	void atRibRemove(ABrib=ABrib()); void ribRemoved(ABrib=ABrib());

				// Other changes
	void atOrientChanged();	void orientChanged();
	void atClear();			void cleared();
	void aboutToChange();	void changed();
///				</Signals>
};

bool AbstractGraph::isOriented() const{return oriented;}
int AbstractGraph::vertexCount() const{return vertexList.count();}
bool AbstractGraph::isRibExist(const ABrib &r) const{return findABrib(r) >= 0;}

#endif // ABSTRACTGRAPH_H
