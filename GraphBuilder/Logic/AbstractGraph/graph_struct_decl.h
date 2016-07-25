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

#ifndef GRAPH_STRUCT_DECL_H
#define GRAPH_STRUCT_DECL_H

/*
 * Graph engine structures declaration:
 *  - Items(Tvertex, Trib)
 *  - Info items(TvertexInfo, TribInfo)
*/

#include <QColor>
#include <QVector>

///			<Defines/>
#define VER_DEFAULT_RADIUS 20
#define VER_DEFAULT_COLOR QColor("#F13746")
#define RIB_DEFAULT_WEIGHT 10
#define RIB_DEFAULT_COLOR QColor(Qt::black)

struct Trib;
struct Tvertex;
struct TribInfo;
struct TvertexInfo;

///				<GraphItems>
struct Trib{
	Trib(int w = RIB_DEFAULT_WEIGHT,
		 const QColor &col = RIB_DEFAULT_COLOR);

	Trib(const TribInfo &ri);
	void setProperties(const TribInfo &ri);

	Tvertex *to;		// To vertex
	int weight;
	QColor color;
	QString text;
	bool isHidden;
};

struct Tvertex{
	Tvertex(int r = VER_DEFAULT_RADIUS,
			const QColor &col = VER_DEFAULT_COLOR);

	Tvertex(const TvertexInfo &vi);
	void setProperties(const TvertexInfo &vi);

	QVector <Trib> ribs;	// Ribs vector
	int x, y, radius;		// Center position
	QColor color;
	QString text;
	bool isHidden;

					// Returns i rib
	inline const Trib &operator[](int i) const
		{return ribs.at(i);}
	inline Trib &operator[](int i)
		{return ribs[i];}
};
///				</GraphItems>

///				<GtaphItemsInfo>
struct TribInfo{
	TribInfo(int w = RIB_DEFAULT_WEIGHT,
			 const QColor &col = RIB_DEFAULT_COLOR);

	TribInfo(const Trib &rib, int from = 0, int to = 0);
	void getProperties(const Trib &rib);

	int from, to, weight;
	QColor color;
	QString text;
	bool hidden;
};

struct TvertexInfo{
	TvertexInfo(int r = VER_DEFAULT_RADIUS,
				const QColor &col = VER_DEFAULT_COLOR);

	TvertexInfo(const Tvertex &ver, int n = 0);
	void getProperties(const Tvertex &ver);

	int x, y, radius;
	int number, degree;
	QColor color;
	QString text;
	bool hidden;
};
///				</GtaphItemsInfo>

///			<IO/>
QDataStream &operator<<(QDataStream &, const TribInfo &);
QDataStream &operator>>(QDataStream &,		 TribInfo &);

QDataStream &operator<<(QDataStream &, const TvertexInfo &);
QDataStream &operator>>(QDataStream &,		 TvertexInfo &);

#endif // GRAPH_STRUCT_DECL_H
