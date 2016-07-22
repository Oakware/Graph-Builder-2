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

#ifndef ALGOINTERFACE_H
#define ALGOINTERFACE_H

#include <QtPlugin>
#include "../AbstractGraph/abstractgraph.h"

struct plugParams;

class AlgoInterface{
public:
	virtual ~AlgoInterface(){}

	virtual bool run(AbstractGraph *gr, int sver) = 0;
	virtual void stop() = 0;

	virtual QString name() = 0;
	virtual QString qmlDialog() = 0;
	virtual void setParams(const plugParams &pp) = 0;
};

#define AlgoInterface_iid "org.ioproject.AlgoInterface"
Q_DECLARE_INTERFACE(AlgoInterface, AlgoInterface_iid)

#endif // ALGOINTERFACE_H
