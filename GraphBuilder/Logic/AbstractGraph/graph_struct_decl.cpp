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

#include "graph_struct_decl.h"

Trib::Trib(int w, const QColor &col):
	to(NULL), weight(w),
	color(col), isHidden(false){}

Trib::Trib(const TribInfo &ri):
	to(NULL)
{
	setProperties(ri);
}

void Trib::setProperties(const TribInfo &ri){
	weight   = ri.weight;
	color    = ri.color;
	text     = ri.text;
	isHidden = ri.hidden;
}


Tvertex::Tvertex(int r, const QColor &col):
	x(0), y(0), radius(r),
	color(col), isHidden(false){}

Tvertex::Tvertex(const TvertexInfo &vi){
	setProperties(vi);
}

void Tvertex::setProperties(const TvertexInfo &vi){
	radius   = vi.radius;
	color    = vi.color;
	text     = vi.text;
	isHidden = vi.hidden;
}


TribInfo::TribInfo(int w, const QColor &col):
	from(0), to(0), weight(w),
	color(col), hidden(false){}

TribInfo::TribInfo(const Trib &rib, int from, int to):
	from(from), to(to)
{
	getProperties(rib);
}

void TribInfo::getProperties(const Trib &rib){
	weight = rib.weight;
	color  = rib.color;
	text   = rib.text;
	hidden = rib.isHidden;
}


TvertexInfo::TvertexInfo(int r, const QColor &col):
	x(0), y(0), radius(r), number(0), degree(0),
	color(col), hidden(false){}

TvertexInfo::TvertexInfo(const Tvertex &ver, int n):
	number(n)
{
	getProperties(ver);
}

void TvertexInfo::getProperties(const Tvertex &ver){
	x = ver.x; y = ver.y;
	radius = ver.radius;
	color  = ver.color;
	text   = ver.text;
	degree = ver.ribs.count();
	hidden = ver.isHidden;
}
