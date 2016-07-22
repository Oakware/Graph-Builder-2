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

#ifndef ABSTRACTGRAPH_HEADER_H
#define ABSTRACTGRAPH_HEADER_H

///			<Defines/>
#define AG_CHANGE_VER_PARAM(AG, ver, param, val) {\
	TvertexInfo vi = (AG).getVertexInfo(ver);\
	if(vi.number){\
		vi.param = (val);\
		(AG).updateVertex((ver), vi);\
	}\
}

#define AG_CHANGE_RIB_PARAM(AG, rib, param, val) {\
	TribInfo ri = (AG).getRibInfo(rib);\
	if(ri.from){\
		ri.param = (val);\
		(AG).updateRib((rib), ri);\
	}\
}

///			<Structs/>
struct ABrib{
	ABrib(): from(0), to(0){}
	ABrib(int a, int b):
		from(a), to(b){}

	int from, to;

	inline bool operator==(const ABrib &other){
		return ((from == other.from) && (to == other.to));
	}

	inline bool notOrientEqual(const ABrib &other){		// Equal at not oriented graph
		return ((*this)==other) || ((to == other.from) && (from == other.to));
	}

	inline bool operator!=(const ABrib &other){
		return !((*this)==other);
	}
};

#endif // ABSTRACTGRAPH_HEADER_H
