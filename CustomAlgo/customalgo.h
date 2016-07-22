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

#ifndef CUSTOMALGO_H
#define CUSTOMALGO_H

#include <QObject>



#include "customalgo_global.h"
#include "AlgoPlugins/algopluginloader.h"
#include "AlgoPlugins/sleep.h"

class CUSTOMALGOSHARED_EXPORT CustomAlgo: public QObject, AlgoInterface{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AlgoInterface_iid)
	Q_INTERFACES(AlgoInterface)

public:
	CustomAlgo(QObject *par = 0);

private:
	Sleeper *sl;
	plugParams params;
	QString result;

public slots:
	virtual bool run(AbstractGraph *gr, int sver);
	virtual void stop();

	virtual QString name();
	virtual QString qmlDialog();

	QString getResult();

public:
	virtual void setParams(const plugParams &pp);
};

#endif // CUSTOMALGO_H
