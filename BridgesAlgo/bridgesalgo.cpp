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

#include "bridgesalgo.h"
#include "AlgoPlugins/graphreset.h"

#define asleep() if(!sl->sleep(params.sleepTime)) return false;

BridgesAlgo::BridgesAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool BridgesAlgo::run(AbstractGraph *gr, int sver){
	if(gr->isOriented()){
		result = tr("Graph must not be oriented.");
		return true;
	}

	VectorListGS sgr = gr->getGraphModel().toVectorList();	// Return if
	if(sgr.count() <= 1) return false;						//  no vertexes

	QBitArray vis(sgr.count());			// Variables
	QStack  <int> st;
	QVector <int> tin(sgr.count());
	QVector <int> tout(sgr.count());
	int time = 1;

	resetGraph(gr, "#03A9F4", params.ri.color);
	st << sver; vis[sver] = true;
	tin[sver] = tout[sver] = time++;
	AG_CHANGE_VER_PARAM((*gr), sver, color, "#7E57C2")
	AG_CHANGE_VER_PARAM((*gr), sver, text,  QString::number(tin[sver]) + '/' +
											QString::number(tout[sver]))

	while(!st.empty()){
		asleep();

		int i, to, p = 0, v = st.top();
		if(st.count() >= 2) p = st.at(st.count()-2);

		for(i=0; i < sgr[v].count(); ++i){
			to = sgr[v][i].to;
			if(to == p) continue;
			if(vis[to]){
				tout[v] = qMin(tout[v], tout[to]);
				AG_CHANGE_VER_PARAM((*gr), v, text, QString::number(tin[v]) + '/' +
													QString::number(tout[v]))
			}else{
				to = sgr[v][i].to;
				st << to;
				vis[to] = true;
				tin[to] = tout[to] = time++;

				AG_CHANGE_VER_PARAM((*gr), v,  color, "#607D8B")
				AG_CHANGE_VER_PARAM((*gr), to, color, "#7E57C2")
				AG_CHANGE_VER_PARAM((*gr), to, text, QString::number(tin[to]) + '/' +
													 QString::number(tout[to]))
				break;
			}
		}

		if(i >= sgr[v].count()){
			to = st.pop();
			AG_CHANGE_VER_PARAM((*gr), v, color, "#4CAF50")

			if(!st.empty()){
				v = st.top();
				tout[v] = qMin(tout[v], tout[to]);
				AG_CHANGE_VER_PARAM((*gr), v, color, "#7E57C2")
				AG_CHANGE_VER_PARAM((*gr), v, text, QString::number(tin[v]) + '/' +
													QString::number(tout[v]))
				if(tout[to] > tin[v])
					AG_CHANGE_RIB_PARAM((*gr), ABrib(v,to), color, "#E64A19")
			}
		}

		if(st.empty()){
			for(int i=1; i<sgr.count(); ++i){
				if(!vis[i]){
					st << i; vis[i] = true;
					tin[i] = tout[i] = time++;
					AG_CHANGE_VER_PARAM((*gr), i, color, "#7E57C2")
					AG_CHANGE_VER_PARAM((*gr), i, text, QString::number(tin[i]) + '/' +
														QString::number(tout[i]))
					break;
				}
			}
		}
	}

	result = tr("Finished.");
	return true;
}

void BridgesAlgo::stop(){
	sl->stop();
}

QString BridgesAlgo::name(){
	return tr("Bridges");
}

QString BridgesAlgo::qmlDialog(){
	return "qrc:/Bridges/ResDialog.qml";
}

QString BridgesAlgo::getResult(){
	return result;
}

void BridgesAlgo::setParams(const plugParams &pp){
	params = pp;
}
