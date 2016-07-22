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

#include <QDataStream>
#include "abstractgraph.h"

QDataStream &operator<<(QDataStream &io, const TribInfo &r){
	io << r.from  << r.to   << r.weight
	   << r.color << r.text << r.hidden;
	return io;
}

QDataStream &operator>>(QDataStream &io, TribInfo &r){
	io >> r.from  >> r.to   >> r.weight
	   >> r.color >> r.text >> r.hidden;
	return io;
}

QDataStream &operator<<(QDataStream &io, const TvertexInfo &v){
	io << v.number << v.x     << v.y    << v.radius
	   << v.degree << v.color << v.text << v.hidden;
	return io;
}

QDataStream &operator>>(QDataStream &io, TvertexInfo &v){
	io >> v.number >> v.x     >> v.y    >> v.radius
	   >> v.degree >> v.color >> v.text >> v.hidden;
	return io;
}

QDataStream &operator<<(QDataStream &io, const GraphModel &gr){
	io << gr.oriented;
	io << gr.vertexes.count();
	for(int i=0; i<gr.vertexes.count(); ++i)
		io << gr.vertexes[i];

	io << gr.ribs.count();
	for(int i=0; i<gr.ribs.count(); ++i)
		io << gr.ribs[i];

	return io;
}

QDataStream &operator>>(QDataStream &io, GraphModel &gr){
	int n;
	io >> gr.oriented;

	io >> n;
	gr.vertexes.resize(n);
	for(int i=0; i<n; ++i)
		io >> gr.vertexes[i];

	io >> n;
	gr.ribs.resize(n);
	for(int i=0; i<n; ++i)
		io >> gr.ribs[i];

	return io;
}

namespace GraphFile{
	void readRib(QDataStream &io, TribInfo &r){
		io >> r.from  >> r.to >> r.weight
		   >> r.color >> r.hidden;
	}

	void writeRib(QDataStream &io, const TribInfo &r){
		io << r.from  << r.to << r.weight
		   << r.color << r.hidden;
	}

	void readVertex(QDataStream &io, TvertexInfo &v){
		io >> v.number >> v.x >> v.y >> v.radius
		   >> v.degree >> v.color >> v.hidden;
	}

	void writeVertex(QDataStream &io, const TvertexInfo &v){
		io << v.number << v.x << v.y << v.radius
		   << v.degree << v.color << v.hidden;
	}

	void readGraph(QDataStream &io, GraphModel &gr){
		int n;
		io >> gr.oriented;

		io >> n;
		gr.vertexes.resize(n);
		for(int i=0; i<n; ++i)
			readVertex(io, gr.vertexes[i]);

		io >> n;
		gr.ribs.resize(n);
		for(int i=0; i<n; ++i)
			readRib(io, gr.ribs[i]);
	}

	void writeGraph(QDataStream &io, const GraphModel &gr){
		io << gr.oriented;
		io << gr.vertexes.count();
		for(int i=0; i<gr.vertexes.count(); ++i)
			writeVertex(io, gr.vertexes[i]);

		io << gr.ribs.count();
		for(int i=0; i<gr.ribs.count(); ++i)
			writeRib(io, gr.ribs[i]);
	}
}
