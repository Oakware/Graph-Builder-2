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

#ifndef POINT_TO_LINE_H
#define POINT_TO_LINE_H

#include <QPoint>
#include <QLine>
#include <cmath>

#define sqr(A) ((A)*(A))

inline double dist(const QPoint &p1, const QPoint &p2){
	return sqrt(sqr(p1.x()-p2.x()) + sqr(p1.y()-p2.y()));
}

double pointToLine(const QPoint &m, const QLine &l){
	double r0, r1, r2, r12;
	double a, b, c, t;

	r1  = dist(m, l.p1());
	r2  = dist(m, l.p2());
	r12 = dist(l.p1(), l.p2());

			// Right to B
	if(r1 >= dist(QPoint(0,0), QPoint(r2,r12)))
		return r2;

			// Left to A
	if(r2 >= dist(QPoint(0,0), QPoint(r1,r12)))
		return r1;

			// Get line (Ax+By+C=0)
	a = l.p2().y()-l.p1().y();
	b = l.p1().x()-l.p2().x();
	c = l.p1().y() * (l.p2().x()-l.p1().x()) -
		l.p1().x() * (l.p2().y()-l.p1().y());

	t = dist(QPoint(0,0), QPoint(a,b));

	if(c > 0){a*=-1; b*=-1; c*=-1;}
	r0 = (a*m.x()+b*m.y()+c)/t;

	return fabs(r0);
//	r0 < 0 => under line;
}

#endif // POINT_TO_LINE_H
