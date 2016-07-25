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

#include "graph_save_struct.h"

VectorListGS GraphModel::toVectorList(){
	VectorListGS gr(vertexes.count()+1);

	for(int i=0; i<ribs.count(); ++i){
		if(ribs[i].from > 0 && ribs[i].from <= vertexes.count() &&
		   ribs[i].to > 0   && ribs[i].to   <= vertexes.count())
		{
			gr[ribs[i].from] << VLGraphRib(ribs[i].to, ribs[i].weight);
		}
	}

	return gr;
}

RibListGS GraphModel::toRibList(){
	RibListGS gr(ribs.count());

	for(int i=0; i<ribs.count(); ++i){
		RLGraphItem gi;
		gi.from   = ribs[i].from;
		gi.to     = ribs[i].to;
		gi.weight = ribs[i].weight;
		gr[i] = gi;
	}

	return gr;
}

MatrixGS GraphModel::toMatrix(){
	MatrixGS gr(vertexes.count()+1);

	for(int i=0; i<=vertexes.count(); ++i)
		gr[i].resize(vertexes.count()+1);

	for(int i=0; i<ribs.count(); ++i){
		if(ribs[i].from > 0 && ribs[i].from <= vertexes.count() &&
		   ribs[i].to   > 0 && ribs[i].to   <= vertexes.count())
		{
			gr[ribs[i].from][ribs[i].to] = ribs[i].weight;
		}
	}

	return gr;
}
