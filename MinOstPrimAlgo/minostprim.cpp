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

#include "minostprim.h"
#include "AlgoPlugins/graphreset.h"
#include <set>
using namespace std;

#define asleep() if(!sl->sleep(params.sleepTime)) return false;

bool cmp(RLGraphItem a, RLGraphItem b){
	if(a.weight == b.weight){
		if(a.from == b.from)
			{return a.to < b.to;}
		return a.from < b.from;
	}
	return a.weight < b.weight;
}

MinOstPrim::MinOstPrim(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool MinOstPrim::run(AbstractGraph *gr, int sver){
	if(gr->isOriented()){
		result = tr("Graph must not be oriented.");
		return true;
	}

	VectorListGS sgr = gr->getGraphModel().toVectorList();	// Return if
	if(sgr.count() <= 1) return false;						//  no vertexes

	int n = sgr.count()-1;
	const int INF = 1000000000;

				// Hide ribs
	resetGraph(gr, "#26A69A", params.ri.color);
	for(int i=1; i<=n; ++i){
		for(int j=0; j<sgr[i].count(); ++j)
			AG_CHANGE_RIB_PARAM((*gr), ABrib(i, sgr[i][j].to), hidden, true)
	}

	vector <int> min_e(n+1, INF), sel_e(n+1, -1);		// Variables
	min_e[sver] = 0;
	set < pair<int,int> > q;
	int v = 0;
	q.insert(make_pair (0, sver));

	while(!q.empty()){
		if(v){AG_CHANGE_VER_PARAM((*gr), v, color, "#2196F3")}

		v = q.begin()->second;
		q.erase (q.begin());
		AG_CHANGE_VER_PARAM((*gr), v, color, "#FF5722")

		if(sel_e[v] != -1){
			AG_CHANGE_RIB_PARAM((*gr), ABrib(v, sel_e[v]), hidden, false)
		}

		for(int j=0; j<sgr[v].size(); ++j){
			int to = sgr[v][j].to,
				cost = sgr[v][j].weight;
			if(cost < min_e[to]){
				AG_CHANGE_VER_PARAM((*gr), to, color, "#9C27B0")
				q.erase(make_pair(min_e[to], to));
				min_e[to] = cost;
				sel_e[to] = v;
				q.insert(make_pair(min_e[to], to));
			}
		}

		asleep()
	}
	AG_CHANGE_VER_PARAM((*gr), v, color, "#2196F3")

	result = tr("Finished.");
	return true;
}

void MinOstPrim::stop(){
	sl->stop();
}

QString MinOstPrim::name(){
	return tr("Minimal spanning tree (Prim)");
}

QString MinOstPrim::qmlDialog(){
	return "qrc:/MinOstPrim/ResDialog.qml";
}

QString MinOstPrim::getResult(){
	return result;
}

void MinOstPrim::setParams(const plugParams &pp){
	params = pp;
}
