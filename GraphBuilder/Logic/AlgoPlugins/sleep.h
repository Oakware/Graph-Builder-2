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

#ifndef SLEEP_H
#define SLEEP_H

#include <QCoreApplication>
#include <QObject>
#include <QBasicTimer>

class Sleeper: public QObject{
	Q_OBJECT
public:
	Sleeper(QObject *par = 0): QObject(par){}

public slots:
	inline bool sleep(int msec);
	inline void stop();

private:
	inline void timerEvent(QTimerEvent *ev);
	QBasicTimer timer;
	bool stopped;
};

			/// <Implement/>

bool Sleeper::sleep(int msec){
	if(timer.isActive()) return false;
	stopped = false;

	timer.start(msec, this);
	while(timer.isActive())
		QCoreApplication::processEvents();

	return !stopped;
}

void Sleeper::stop(){
	stopped = true;
	timer.stop();
}

void Sleeper::timerEvent(QTimerEvent *ev){
	if(ev->timerId() == timer.timerId())
		timer.stop();
}

#endif // SLEEP_H
