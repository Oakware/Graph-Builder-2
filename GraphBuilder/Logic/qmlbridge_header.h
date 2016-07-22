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

#ifndef QMLBRIDGE_HEADER
#define QMLBRIDGE_HEADER

#include <QObject>
#include "GraphView/graphview.h"

class RibInfoObj: public QObject{
	Q_OBJECT
private:
	int from, to, weight;
	QColor color; QString text; bool hidden;

public:
	RibInfoObj(){setRibInfo(TribInfo());}
	RibInfoObj(const TribInfo &ri){setRibInfo(ri);}

	void setRibInfo(const TribInfo &ri){
		from = ri.from; to = ri.to; weight = ri.weight;
		color = ri.color; text = ri.text; hidden = ri.hidden;
		emit changed();
	}

	TribInfo toRibInfo() const{
		TribInfo ri;
		ri.from = from; ri.to = to; ri.weight = weight;
		ri.color = color; ri.text = text; ri.hidden = hidden;
		return ri;
	}

public:
	Q_PROPERTY(int from		MEMBER from		NOTIFY changed)
	Q_PROPERTY(int to		MEMBER to		NOTIFY changed)
	Q_PROPERTY(int weight	MEMBER weight	NOTIFY changed)
	Q_PROPERTY(QColor color MEMBER color	NOTIFY changed)
	Q_PROPERTY(QString text MEMBER text		NOTIFY changed)
	Q_PROPERTY(bool hidden  MEMBER hidden	NOTIFY changed)

signals:
	void changed();
};

class VertexInfoObj: public QObject{
	Q_OBJECT
private:
	int x, y, radius, number, degree;
	QColor color; QString text; bool hidden;

public:
	VertexInfoObj(){setVertexInfo(TvertexInfo());}
	VertexInfoObj(const TvertexInfo &vi){setVertexInfo(vi);}

	void setVertexInfo(const TvertexInfo &vi){
		x = vi.x; y = vi.y; radius = vi.radius;
		number = vi.number; degree = vi.degree;
		color = vi.color; text = vi.text; hidden = vi.hidden;
		emit changed();
	}

	TvertexInfo toVertexInfo() const{
		TvertexInfo vi;
		vi.x = x; vi.y = y; vi.radius = radius;
		vi.number = number; vi.degree = degree;
		vi.color = color; vi.text = text; vi.hidden = hidden;
		return vi;
	}

public:
	Q_PROPERTY(int x		MEMBER x		NOTIFY changed)
	Q_PROPERTY(int y		MEMBER y		NOTIFY changed)
	Q_PROPERTY(int radius	MEMBER radius	NOTIFY changed)
	Q_PROPERTY(int number	MEMBER number	NOTIFY changed)
	Q_PROPERTY(int degree	MEMBER degree	NOTIFY changed)
	Q_PROPERTY(QColor color MEMBER color	NOTIFY changed)
	Q_PROPERTY(QString text MEMBER text		NOTIFY changed)
	Q_PROPERTY(bool hidden  MEMBER hidden	NOTIFY changed)

signals:
	void changed();
};

#endif // QMLBRIDGE_HEADER
