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

#include "minostkruskal.h"
#include "AlgoPlugins/graphreset.h"
#include <algorithm>
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

MinOstKruskal::MinOstKruskal(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool MinOstKruskal::run(AbstractGraph *gr, int){
	if(gr->isOriented()){
		result = tr("Graph must not be oriented.");
		return true;
	}

	RibListGS sgr = gr->getGraphModel().toRibList();		// Return if
	if(!sgr.count()) return false;							//  no vertexes

				// Hide ribs
	resetGraph(gr, "#26A69A", params.ri.color);
	for(int i=0; i<sgr.count(); ++i)
		AG_CHANGE_RIB_PARAM((*gr), ABrib(sgr[i].from, sgr[i].to), hidden, true)

	int n = sgr.count()-1;			// Variables
	vector<int> tree_id(n+1);

	sort(sgr.begin(), sgr.end(), cmp);
	for(int i=1; i<=n; ++i)
		tree_id[i] = i;

	for(int i=0; i<sgr.count(); ++i){
		int a = sgr[i].from,  b = sgr[i].to;
		if(tree_id[a] != tree_id[b]){
			asleep()
			int old_id = tree_id[b],  new_id = tree_id[a];
			AG_CHANGE_RIB_PARAM((*gr), ABrib(a,b), hidden, false)
			AG_CHANGE_VER_PARAM((*gr), a, color, "#2196F3")
			AG_CHANGE_VER_PARAM((*gr), b, color, "#2196F3")
			for(int j=1; j<=n; ++j){
				if(tree_id[j] == old_id)
					tree_id[j] = new_id;
			}
		}
	}

	result = tr("Finished.");
	return true;
}

void MinOstKruskal::stop(){
	sl->stop();
}

QString MinOstKruskal::name(){
	return tr("Minimal spanning tree (Kruskal)");
}

QString MinOstKruskal::qmlDialog(){
	return "qrc:/MinOstKruskal/ResDialog.qml";
}

QString MinOstKruskal::getResult(){
	return result;
}

void MinOstKruskal::setParams(const plugParams &pp){
	params = pp;
}
