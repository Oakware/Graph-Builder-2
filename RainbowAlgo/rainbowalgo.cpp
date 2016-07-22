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

#include "rainbowalgo.h"

#define asleep() if(!sl->sleep(params.sleepTime/40)) break;

RainbowAlgo::RainbowAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool RainbowAlgo::run(AbstractGraph *gr, int){
	double beta; QColor col;	// initialize
	alpha = 0;
	int rcnt = 8;
	int cpos = 130;
	char text[] = "Rainbow!";
	TvertexInfo vi; vi.text = ' ';
	TribInfo ri;	ri.text = ' ';

	gr->clear();			// fill graph
	gr->setOriented(false);
	vi.radius = 10;
	gr->addVertex(cpos, cpos, vi);
	vi.radius = 20;
	for(int i=2; i<=rcnt+1; ++i){
		vi.text = text[i-2];
		gr->addVertex(cpos, cpos, vi);
		gr->addRib(ABrib(i, 1),   ri);
		gr->addRib(ABrib(i, i-1), ri);
	}
	gr->addRib(ABrib(2, rcnt+1), ri);

	while(true){
		col.setHsv(180*alpha/M_PI, 255, 255);
		AG_CHANGE_VER_PARAM((*gr), 1, color, col)

		for(int i=2; i<=rcnt+1; ++i){		// rotate
			beta = 2*M_PI*i/rcnt + alpha;
			gr->moveVertex(i, cpos + 80*cos(beta), cpos + 80*sin(beta));
			col.setHsv(180*beta/M_PI, 255, 255);
			AG_CHANGE_VER_PARAM((*gr), i, color, col)

			alpha += 0.005;
			if(alpha > 2*M_PI) alpha -= 2*M_PI;
		}
		asleep()
	}

	return false;
}

void RainbowAlgo::stop(){
	sl->stop();
}

QString RainbowAlgo::name(){
	return tr("Rainbow");
}

QString RainbowAlgo::qmlDialog(){
	return "qrc:/Rainbow/ResDialog.qml";
}

QString RainbowAlgo::getResult(){
	return tr("Finished.");
}

void RainbowAlgo::setParams(const plugParams &pp){
	params = pp;
}
