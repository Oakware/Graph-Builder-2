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

#include "qmlbridge.h"
#include "AbstractGraph/graphio.h"

QmlBridge::QmlBridge(QObject *parent):
	QmlBridgeBasic(parent)
{
	plugLoader = new AlgoPluginLoader(this);
	plugLoader->setAlgoDelay(config->value("algoDelay", 250).toInt());
	languageIndex = config->value("language", 0).toInt();
	saveFilePath  = config->value("saveFile").toString();
}

void QmlBridge::installTranslation(QTranslator *trs, int ind){
	if(ind == 1){
		trs->load("Ukrainian", ":/ts");
		QApplication::installTranslator(trs);
	}else if(ind == 2){
		trs->load("Russian", ":/ts");
		QApplication::installTranslator(trs);
	}else
		QApplication::removeTranslator(trs);
}

bool QmlBridge::isSaveFileExist() const{
	return !saveFilePath.isEmpty();
}

QString QmlBridge::getSaveFileName() const{
	if(saveFilePath.isEmpty()) return saveFilePath;
	QFileInfo fi(saveFilePath);
	return fi.completeBaseName();
}

QUrl QmlBridge::getSaveFileFolder() const{
	if(saveFilePath.isEmpty())
		return QUrl::fromLocalFile(QStandardPaths::
				  writableLocation(QStandardPaths::DocumentsLocation));

	QFileInfo fi(saveFilePath);
	return QUrl::fromLocalFile(fi.absolutePath());
}

QUrl QmlBridge::getSaveFilePath() const{
	return QUrl::fromLocalFile(saveFilePath);
}

void QmlBridge::setSaveFile(const QUrl &url){
	saveFilePath = url.toLocalFile();
	if(saveFilePath.isEmpty())
		return;

	QFileInfo fi(saveFilePath);
	if(fi.suffix() != "graph" && fi.suffix() != "grm")
		saveFilePath += ".grm";
}

bool QmlBridge::openFile(AbstractGraph *ag){
	QFile f(saveFilePath);
	QDataStream io(&f);
	if(!f.open(QFile::ReadOnly))
		return false;

	GraphModel gm;
	QFileInfo fi(saveFilePath);
	if(fi.suffix() == "grm")
		io >> gm;
	else
		GraphFile::readGraph(io, gm);
	ag->setGraphModel(gm);
	f.close();
	return true;
}

bool QmlBridge::saveFile(AbstractGraph *ag){
	QSaveFile f(saveFilePath);
	QDataStream io(&f);
	if(!f.open(QFile::WriteOnly))
		return false;

	QFileInfo fi(saveFilePath);
	if(fi.suffix() == "grm") io << ag->getGraphModel();
	else GraphFile::writeGraph(io, ag->getGraphModel());
	return f.commit();
}

AlgoPluginLoader *QmlBridge::getPluginLoader(){
	return plugLoader;
}

int QmlBridge::getTranslation() const{
	return languageIndex;
}

void QmlBridge::setTranslation(int ind){
	languageIndex = ind;
}

void QmlBridge::getRibInfo(AbstractGraph *gr, RibInfoObj *ri, int a, int b){
	ri->setRibInfo(gr->getRibInfo(ABrib(a, b)));
}

void QmlBridge::setRibInfo(AbstractGraph *gr, RibInfoObj *ri, int a, int b){
	gr->updateRib(ABrib(a, b), ri->toRibInfo());
}

void QmlBridge::setStdRib(GraphView *gv, RibInfoObj *ri){
	TribInfo r = ri->toRibInfo();
	gv->setStdRib(r);
	plugLoader->setStdRib(r);
}

void QmlBridge::getVertexInfo(AbstractGraph *gr, VertexInfoObj *vi, int ver){
	vi->setVertexInfo(gr->getVertexInfo(ver));
}

void QmlBridge::setVertexInfo(AbstractGraph *gr, VertexInfoObj *vi, int ver){
	gr->updateVertex(ver, vi->toVertexInfo());
}

void QmlBridge::setStdVertex(GraphView *gv, VertexInfoObj *vi){
	TvertexInfo v = vi->toVertexInfo();
	gv->setStdVertex(v);
	plugLoader->setStdVertex(v);
}

void QmlBridge::saveProperties(){
	config->setValue("language", languageIndex);
	config->setValue("saveFile", saveFilePath);
	config->setValue("algoDelay", plugLoader->getAlgoDelay());
	config->sync();
}
