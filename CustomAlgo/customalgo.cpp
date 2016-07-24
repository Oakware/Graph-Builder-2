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

#include "customalgo.h"
#include "AlgoPlugins/graphreset.h"

#define asleep() if(!sl->sleep(params.sleepTime)) return false;

CustomAlgo::CustomAlgo(QObject *par):
	QObject(par)
{
	sl = new Sleeper(this);
}

bool CustomAlgo::run(AbstractGraph *gr, int sver){
	resetGraph(gr, "#9CCC65", params.ri.color);
	GraphModel gm = gr->getGraphModel();

//	gm.toMatrix();
//	gm.toVectorList();
//	gm.toRibList();

	AG_CHANGE_VER_PARAM((*gr), sver, color, "#000000")

	asleep()
	result = tr("Finished.");
	return true;
}

void CustomAlgo::stop(){
	sl->stop();
}

QString CustomAlgo::name(){
	return tr("Custom");
}

QString CustomAlgo::qmlDialog(){
	return "qrc:/Custom/ResDialog.qml";
}

QString CustomAlgo::getResult(){
	return result;
}

void CustomAlgo::setParams(const plugParams &pp){
	params = pp;
}
