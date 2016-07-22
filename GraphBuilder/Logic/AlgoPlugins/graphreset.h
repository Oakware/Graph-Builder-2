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

#ifndef RESET_GRAPH_H
#define RESET_GRAPH_H

#include "../AbstractGraph/abstractgraph.h"

inline void resetGraph(AbstractGraph *gr, const QColor &vc, const QColor &rc){
	GraphModel gm = gr->getGraphModel();

	for(int i=0; i < gm.vertexes.count(); ++i){
		gm.vertexes[i].color = vc;
		gr->updateVertex(i+1, gm.vertexes[i]);
	}

	for(int i=0; i < gm.ribs.count(); ++i){
		gm.ribs[i].color = rc;
		gr->updateRib(ABrib(gm.ribs[i].from, gm.ribs[i].to), gm.ribs[i]);
	}
}

#endif // RESET_GRAPH_H
