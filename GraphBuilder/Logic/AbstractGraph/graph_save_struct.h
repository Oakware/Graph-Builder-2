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

#ifndef GRAPH_SAVE_STRUCT_H
#define GRAPH_SAVE_STRUCT_H

/*
 * Graph save structures:
 *  - Graph model      (GraphModel)
 *  - Vector list      (VectorListGS)
 *  - Rib list         (RibListGS)
 *  - Adjacency matrix (MatrixGS)
*/

#include <QVector>
#include "graph_struct_decl.h"

///			<VectorListGS/>
struct VLGraphRib{
	VLGraphRib(int Vto = 0, int w = 0):
		to(Vto),weight(w){}
	int to, weight;
};

typedef QVector < QVector <VLGraphRib> > VectorListGS;

///			<RibList/>
struct RLGraphItem{int from, to, weight;};
typedef QVector <RLGraphItem> RibListGS;

///			<MatrixGS/>
typedef QVector< QVector<int> > MatrixGS;

///			<GraphModel/>
class GraphModel{
public:
	GraphModel(): oriented(false){}

	bool oriented;
	QVector <TvertexInfo>	vertexes;
	QVector <TribInfo>		ribs;

	VectorListGS	toVectorList();
	RibListGS		toRibList	();
	MatrixGS		toMatrix	();
};

///			<IO/>
QDataStream &operator<<(QDataStream &, const GraphModel &);
QDataStream &operator>>(QDataStream &,		 GraphModel &);

#endif // GRAPH_SAVE_STRUCT_H
