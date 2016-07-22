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

#include <QtQml>
#include "Logic/qmlbridge.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	app.setOrganizationName("Oakware");
	app.setApplicationName("Graph builder");
#ifndef Q_OS_OSX
	app.setWindowIcon(QIcon(":/AppIcon.png"));
#endif

	REGISTER_QML_BRIDGE("GraphEngine")
	qmlRegisterType<RibInfoObj>       ("GraphEngine", 1, 0, "RibInfo"     );
	qmlRegisterType<VertexInfoObj>    ("GraphEngine", 1, 0, "VertexInfo"  );
	qmlRegisterType<AbstractGraph>    ("GraphEngine", 1, 0, "GraphModel"  );
	qmlRegisterType<GraphView>        ("GraphEngine", 1, 0, "GraphView"   );
	qmlRegisterType<AlgoPluginLoader> ("GraphEngine", 1, 0, "PluginLoader");

	QSettings *config = new QSettings();	// Install translation
	QTranslator ts;
	int ind = config->value("language", 0).toInt();
	QmlBridge::installTranslation(&ts, ind);
	delete config;

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
