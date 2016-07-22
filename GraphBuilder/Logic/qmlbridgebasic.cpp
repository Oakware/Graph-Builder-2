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

#include "qmlbridgebasic.h"

QmlBridgeBasic::QmlBridgeBasic(QObject *parent):
	QObject(parent)
{
	qDebug() << "QmlBridge :: start";
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(appExit()));
	connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)),
			this, SLOT(appStateChanged(Qt::ApplicationState)));

	config = new QSettings(this);
}

QmlBridgeBasic::~QmlBridgeBasic(){
	qDebug() << "QmlBridge :: end";
}

void QmlBridgeBasic::appStateChanged(Qt::ApplicationState as){
	if(as == Qt::ApplicationSuspended)
		appExit();
}

void QmlBridgeBasic::appExit(){
	emit aboutToExit();
	saveProperties();
}

QString QmlBridgeBasic::getPlatform(){
#if   defined(Q_OS_WIN)
	return "windows";
#elif defined(Q_OS_OSX)
	return "osx";
#elif defined(Q_OS_ANDROID)
	return "android";
#elif defined(Q_OS_IOS)
	return "ios";
#else
	return "linux";
#endif
}

void QmlBridgeBasic::saveProperties(){
	config->sync();
}

QVariant QmlBridgeBasic::getProperty(const QString &k, const QVariant &sv){
	return config->value("UiData/"+k, sv);
}

void QmlBridgeBasic::setProperty(const QString &k, const QVariant &v){
	config->setValue("UiData/"+k, v);
}
