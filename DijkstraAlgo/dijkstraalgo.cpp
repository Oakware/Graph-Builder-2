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

#include "dijkstraalgo.h"
#include "AlgoPlugins/graphreset.h"
#include <QBitArray>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

#define RIB_INF_WEIGHT 1000000
#define asleep() if(!sl->sleep(params.sleepTime)) return false;

DijkstraAlgo::DijkstraAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool DijkstraAlgo::run(AbstractGraph *gr, int sver){
	VectorListGS sgr = gr->getGraphModel().toVectorList();	// Return if
	if(sgr.count() <= 1) return false;						//  no vertexes

					// Variables
	const int vertex_number = sgr.count()-1;
	const unsigned MAX_INT = RIB_INF_WEIGHT;

	graph = gr;
	result.fill(DijkstraPath(MAX_INT), vertex_number+1);
	QBitArray is_closed(vertex_number+1);
	priority_queue< DistanceToVertex, vector< DistanceToVertex > > opened;

	int current_vertex = sver;
	result[current_vertex] = 0;
	resetGraph(gr, "#4CAF50", params.ri.color);
	AG_CHANGE_VER_PARAM((*gr), current_vertex, color, "#f44336")

	for(int i=1; i <= vertex_number; ++i){
		if(i != sver)
			opened.push(DistanceToVertex(MAX_INT, i));
	}
	opened.push(DistanceToVertex(0, sver));

	int num_closed = 0;
	while(num_closed < vertex_number){
		asleep();

		do{
			AG_CHANGE_VER_PARAM((*gr), current_vertex, color, "#78909C")
			current_vertex = opened.top().vertex;
			opened.pop();
		}while(is_closed[current_vertex]);

		AG_CHANGE_VER_PARAM((*gr), current_vertex, color, "#f44336")

		++num_closed;
		is_closed[current_vertex] = true;

		const QVector <VLGraphRib> &edge_list = sgr[current_vertex];
		const unsigned current_dist = result[current_vertex].distance;
		for (int i=0; i<edge_list.count(); ++i){
			if(!is_closed[edge_list[i].to]){
				unsigned dist_through_current = current_dist + edge_list[i].weight;
				AG_CHANGE_VER_PARAM((*gr), edge_list[i].to, color, "#FF9800")

				if(result[edge_list[i].to].distance > dist_through_current){
					AG_CHANGE_RIB_PARAM((*gr), ABrib(current_vertex, edge_list[i].to),
										color, "#FF5722")
					result[edge_list[i].to].distance = dist_through_current;
					result[edge_list[i].to].from = current_vertex;
					opened.push(DistanceToVertex(dist_through_current, edge_list[i].to));
				}
			}
		}
	}
	AG_CHANGE_VER_PARAM((*gr), current_vertex, color, "#78909C")

	asleep()
	resetGraph(gr, "#4CAF50", params.ri.color);
	verSelected = 0;
	return true;
}

void DijkstraAlgo::stop(){
	sl->stop();
}

QString DijkstraAlgo::name(){
	return tr("Dijkstra");
}

QString DijkstraAlgo::qmlDialog(){
	return "qrc:/Dijkstra/ResDialog.qml";
}

int DijkstraAlgo::getResultCount(){
	return result.count();
}

QVariant DijkstraAlgo::getResultVertex(int ver){
	TvertexInfo vi = graph->getVertexInfo(ver);
	if(!vi.text.isEmpty())
		return vi.text;
	return ver;
}

QVariant DijkstraAlgo::getResultDistance(int ver){
	if(result[ver].distance < RIB_INF_WEIGHT)
		return result[ver].distance;
	return tr("No path");
}

void DijkstraAlgo::resetPath(int ver){
	if(verSelected) resetVerPath(verSelected, "#4CAF50", params.ri.color);
	verSelected = ver;
	if(verSelected) resetVerPath(verSelected, "#3F51B5", "#FF5722");
}

void DijkstraAlgo::setParams(const plugParams &pp){
	params = pp;
}

void DijkstraAlgo::resetVerPath(int ver, QColor cver, QColor crib){
	int i = ver;
	AG_CHANGE_VER_PARAM((*graph), i, color, cver);

	while(result[i].from){
		AG_CHANGE_VER_PARAM((*graph), result[i].from, color, cver);
		AG_CHANGE_RIB_PARAM((*graph), ABrib(result[i].from, i), color, crib);
		i = result[i].from;
	}
}
