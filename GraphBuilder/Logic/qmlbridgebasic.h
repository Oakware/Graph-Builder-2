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

#ifndef QMLBRIDGEBASIC_H
#define QMLBRIDGEBASIC_H

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QSettings>

#define REGISTER_QML_BRIDGE(MN) qmlRegisterType<QmlBridge>((MN), 1, 0, "CBridge");

class QmlBridgeBasic: public QObject{
	Q_OBJECT
public:
	explicit QmlBridgeBasic(QObject *parent = 0);
	virtual ~QmlBridgeBasic();

protected:		/// <Data/>
	QSettings *config;

private slots:	/// <OnExit/>
	void appStateChanged(Qt::ApplicationState as);
	void appExit();

public slots:	/// <Controls/>
	static QString getPlatform();

	virtual void saveProperties();
	QVariant getProperty(const QString &k, const QVariant &sv=QVariant());
	void setProperty(const QString &k, const QVariant &v);

signals:
	void aboutToExit();
};

#endif // QMLBRIDGEBASIC_H
