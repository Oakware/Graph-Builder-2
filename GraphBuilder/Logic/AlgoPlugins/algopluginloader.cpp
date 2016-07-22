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

#include "algopluginloader.h"
#ifdef PLUGIN_TEST
#include "../../TN"
#endif

AlgoPluginLoader::AlgoPluginLoader(QObject *parent):
	QObject(parent)
{
	pluginsPath = QCoreApplication::applicationDirPath() + "/plugins/";
	QDir pdir(pluginsPath);
	if(!pdir.exists()) pdir.mkpath(pluginsPath);

	foreach(QString fp, pdir.entryList(QDir::Files)){
		fp = pluginsPath + fp;
		if(loadPlugin(QUrl::fromLocalFile(fp)).isEmpty())
			QFile::remove(fp);
	}

#ifdef PLUGIN_TEST
	testPlug = new TPLUG_NAME(this);
	testPlug->setParams(algoParams);
#endif
}

QStringList AlgoPluginLoader::getNames(){
#ifndef PLUGIN_TEST
	return plugins.keys();
#else
	return QString("Test");
#endif
}

AlgoPluginLoader::plugState AlgoPluginLoader::getPlugState(const QString &name){
	return plugins.value(name).state;
}

QObject *AlgoPluginLoader::getPlugin(const QString &name){
#ifndef PLUGIN_TEST
	QPluginLoader *pl = plugins.value(name).plugLoader;
	if(pl == NULL) return NULL;
	return pl->instance();
#else
	return testPlug;
#endif
}

AlgoInterface *AlgoPluginLoader::getInterface(const QString &name){
	return plugins.value(name).algoInterface();
}

QString AlgoPluginLoader::loadPlugin(const QUrl &path){
	QString fpath = path.toLocalFile();
	QFileInfo fi(fpath);
	if(!fi.exists()) return QString();

	plugItem pi(fi.fileName());
	QString npath = pluginsPath + pi.fileName;

	pi.plugLoader = new QPluginLoader(fpath, this);		// load
	if(!pi.algoInterface()){
		delete pi.plugLoader;
		return QString();
	}

	QString plugName = pi.algoInterface()->name();		// if exist
	if(plugins.contains(plugName) || pluginsToDel.contains(plugName)){
		pi.plugLoader->unload();
		delete pi.plugLoader;

		plugItem &opi = plugins[plugName];	// remove old
		if(opi.plugLoader) QFile::remove(pluginsPath + opi.fileName);
		opi.state = Reload;
		opi.fileName = pi.fileName;

		if(fpath != npath){				// copy to ./plugins
			QFile::remove(npath);
			QFile::copy(fpath, npath);
		}
		return plugName;
	}

	if(fpath != npath){			// copy to ./plugins
		QFile::remove(npath);
		QFile::copy(fpath, npath);
	}

	pi.plugLoader->instance()->setParent(this);
	pi.algoInterface()->setParams(algoParams);
	plugins[plugName] = pi;
	return plugName;
}

void AlgoPluginLoader::removePlugin(const QString &name){
	plugItem &pi = plugins[name];
	pi.state = Remove;
	pluginsToDel << name;
	QFile::remove(pluginsPath + pi.fileName);
}

int AlgoPluginLoader::getAlgoDelay(){
	return algoParams.sleepTime;
}

void AlgoPluginLoader::setAlgoDelay(int msec){
	algoParams.sleepTime = msec;
	foreach(const plugItem &pi, plugins)
		pi.algoInterface()->setParams(algoParams);
#ifdef PLUGIN_TEST
	testPlug->setParams(algoParams);
#endif
}

void AlgoPluginLoader::setStdVertex(const TvertexInfo &vi){
	algoParams.vi = vi;
	foreach(const plugItem &pi, plugins)
		pi.algoInterface()->setParams(algoParams);
#ifdef PLUGIN_TEST
	testPlug->setParams(algoParams);
#endif
}

void AlgoPluginLoader::setStdRib(const TribInfo &ri){
	algoParams.ri = ri;
	foreach(const plugItem &pi, plugins)
		pi.algoInterface()->setParams(algoParams);
#ifdef PLUGIN_TEST
	testPlug->setParams(algoParams);
#endif
}
