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

#include "bfsalgo.h"
#include "AlgoPlugins/graphreset.h"

#define asleep() if(!sl->sleep(params.sleepTime)) return false;

BfsAlgo::BfsAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool BfsAlgo::run(AbstractGraph *gr, int sver){
	VectorListGS sgr = gr->getGraphModel().toVectorList();	// Return if
	if(sgr.count() <= 1) return false;						//  no vertexes

	QBitArray vis(sgr.count());		// Variables
	QQueue <int> qe;
	int conComp = 1;

	qe << sver; vis[sver] = true;
	resetGraph(gr, "#9CCC65", params.ri.color);
	AG_CHANGE_VER_PARAM((*gr), sver, color, "#f44336")

	while(!qe.empty()){
		asleep()

		int v = qe.first();
		for(int i=0; i<sgr[v].count(); ++i){
			int to = sgr[v][i].to;
			if(!vis[to]){
				vis[to] = true;
				qe << to;
				AG_CHANGE_VER_PARAM((*gr), to, color, "#FF9800")
				AG_CHANGE_RIB_PARAM((*gr), ABrib(v,to), color, "#E64A19")
			}
		}

		AG_CHANGE_VER_PARAM((*gr), v, color, "#78909C")
		qe.dequeue();

		if(qe.empty()){
			for(int i=1; i<sgr.count(); ++i){
				if(!vis[i]){
					qe << i; vis[i] = true;
					AG_CHANGE_VER_PARAM((*gr), i, color, "#FF9800")
					++conComp; break;
				}
			}
		}else{
			AG_CHANGE_VER_PARAM((*gr), qe.head(), color, "#f44336")
		}
	}

	result = tr("Finished.");
	if(!gr->isOriented()){
		result += '\n';
		result += tr("Connected components count: ")
				+ QString::number(conComp);
	}
	return true;
}

void BfsAlgo::stop(){
	sl->stop();
}

QString BfsAlgo::name(){
	return tr("BFS");
}

QString BfsAlgo::qmlDialog(){
	return "qrc:/BFS/ResDialog.qml";
}

QString BfsAlgo::getResult(){
	return result;
}

void BfsAlgo::setParams(const plugParams &pp){
	params = pp;
}
