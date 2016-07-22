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

#ifndef ALGOPLUGINLOADER_H
#define ALGOPLUGINLOADER_H

#include <QCoreApplication>
#include <QPluginLoader>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QMap>
#include <QSet>
#include "algointerface.h"

#ifdef PLUGIN_TEST
#define TPLUG_NAME BfsAlgo
class TPLUG_NAME;
#endif

struct plugParams{
	TvertexInfo vi;
	TribInfo ri;
	int sleepTime;
};

class AlgoPluginLoader: public QObject{				/// <Class/>
	Q_OBJECT
public:
	explicit AlgoPluginLoader(QObject *parent = 0);

public:
	Q_PROPERTY(int algoDelay READ getAlgoDelay WRITE setAlgoDelay)
	enum plugState{Normal, Reload, Remove};
	Q_ENUM(plugState)

private:
	struct plugItem{
		plugItem(const QString &f = QString()):
			state(Normal), fileName(f), plugLoader(NULL){}

		plugState state;
		QString fileName;
		QPluginLoader *plugLoader;
		inline AlgoInterface *algoInterface() const{
			return (plugLoader)
					? qobject_cast<AlgoInterface*>(plugLoader->instance())
					: NULL;
		}
	};

private: ///		<Data/>
	QMap <QString, plugItem> plugins;	// <name,plug>
	QSet <QString> pluginsToDel;
	QString pluginsPath;
	plugParams algoParams;

#ifdef PLUGIN_TEST
	TPLUG_NAME *testPlug;
#endif

public:	///			<Controls/>
	Q_INVOKABLE QStringList getNames();
	Q_INVOKABLE plugState getPlugState(const QString &name);
	Q_INVOKABLE QObject *getPlugin(const QString &name);
	AlgoInterface    *getInterface(const QString &name);

public slots:
	QString loadPlugin(const QUrl &path);	// return name
	void removePlugin(const QString &name);

public:
	int  getAlgoDelay();
	void setAlgoDelay(int msec);

	void setStdVertex(const TvertexInfo &vi);
	void setStdRib(const TribInfo &ri);
};

#endif // ALGOPLUGINLOADER_H
