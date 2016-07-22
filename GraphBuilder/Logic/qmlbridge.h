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

#ifndef QMLBRIDGE_H
#define QMLBRIDGE_H

#include "qmlbridge_header.h"
#include "qmlbridgebasic.h"
#include "AlgoPlugins/algopluginloader.h"

class QmlBridge: public QmlBridgeBasic{
	Q_OBJECT
public:
	explicit QmlBridge(QObject *parent = 0);

private:
	AlgoPluginLoader *plugLoader;
	int languageIndex;
	QString saveFilePath;

public:
	static void installTranslation(QTranslator *trs, int ind);

public slots:
	bool isSaveFileExist() const;			// File controls
	QString getSaveFileName() const;
	QUrl getSaveFileFolder() const;
	QUrl getSaveFilePath() const;
	void setSaveFile(const QUrl &url);
	bool openFile(AbstractGraph *ag);
	bool saveFile(AbstractGraph *ag);

	AlgoPluginLoader *getPluginLoader();
	int  getTranslation() const;
	void setTranslation(int ind);

	void getRibInfo(AbstractGraph *gr, RibInfoObj *ri, int a, int b);	// Graph controls
	void setRibInfo(AbstractGraph *gr, RibInfoObj *ri, int a, int b);
	void setStdRib(GraphView *gv, RibInfoObj *ri);

	void getVertexInfo(AbstractGraph *gr, VertexInfoObj *vi, int ver);
	void setVertexInfo(AbstractGraph *gr, VertexInfoObj *vi, int ver);
	void setStdVertex(GraphView *gv, VertexInfoObj *vi);

	virtual void saveProperties();
};

#endif // QMLBRIDGE_H
