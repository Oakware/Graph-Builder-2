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

#include "dfsalgo.h"
#include "AlgoPlugins/graphreset.h"

#define asleep() if(!sl->sleep(params.sleepTime)) return false;

DfsAlgo::DfsAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool DfsAlgo::run(AbstractGraph *gr, int sver){
	VectorListGS sgr = gr->getGraphModel().toVectorList();	// Return if
	if(sgr.count() <= 1) return false;						//  no vertexes

	QBitArray vis(sgr.count());			// Variables
	QStack <int> st;
	int conComp = 1;

	st << sver; vis[sver] = true;
	resetGraph(gr, "#03A9F4", params.ri.color);
	AG_CHANGE_VER_PARAM((*gr), sver, color, "#7E57C2")

	while(!st.empty()){
		asleep();

		int i, v = st.top();
		for(i=0; i<sgr[v].count(); ++i)
			if(!vis[sgr[v][i].to]) break;

		if(i < sgr[v].count()){
			i = sgr[v][i].to;
			st << i;
			vis[i] = true;

			AG_CHANGE_VER_PARAM((*gr), i, color, "#7E57C2")
			AG_CHANGE_VER_PARAM((*gr), v, color, "#bdc3c7")
			AG_CHANGE_RIB_PARAM((*gr), ABrib(v,i), color, "#E64A19")
		}else{
			st.pop();
			AG_CHANGE_VER_PARAM((*gr), v, color, "#2c3e50")
			if(!st.empty()){
				AG_CHANGE_VER_PARAM((*gr), st.top(), color, "#7E57C2")
			}
		}

		if(st.empty()){
			for(int i=1; i<sgr.count(); ++i){
				if(!vis[i]){
					st << i; vis[i] = true;
					AG_CHANGE_VER_PARAM((*gr), i, color, "#7E57C2")
					++conComp; break;
				}
			}
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

void DfsAlgo::stop(){
	sl->stop();
}

QString DfsAlgo::name(){
	return tr("DFS");
}

QString DfsAlgo::qmlDialog(){
	return "qrc:/DFS/ResDialog.qml";
}

QString DfsAlgo::getResult(){
	return result;
}

void DfsAlgo::setParams(const plugParams &pp){
	params = pp;
}
