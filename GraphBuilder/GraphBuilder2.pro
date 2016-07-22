TEMPLATE = app

TARGET = GraphBuilder2
ICON = app.icns
RC_FILE = app.rc

QT += qml quick widgets
CONFIG += c++11

TRANSLATIONS += Translations/Ukrainian.ts\
                Translations/Russian.ts

SOURCES += main.cpp \
    Logic/qmlbridgebasic.cpp \
    Logic/qmlbridge.cpp \
    Logic/AbstractGraph/abstractgraph.cpp \
    Logic/AbstractGraph/graph_save_struct.cpp \
    Logic/AbstractGraph/graph_struct_decl.cpp \
    Logic/GraphView/graphview.cpp \
    Logic/AlgoPlugins/algopluginloader.cpp

RESOURCES += UI/qml.qrc \
    Resources/resources.qrc

#DEFINES += PLUGIN_TEST			# TESTING
#SOURCES += ../
#HEADERS += ../

lupdate_only{
	SOURCES += UI/*.qml\
			../BfsAlgo/*.cpp			../BfsAlgo/*.qml\
			../DfsAlgo/*.cpp			../DfsAlgo/*.qml\
			../BridgesAlgo/*.cpp		../BridgesAlgo/*.qml\
			../MinOstKruskalAlgo/*.cpp	../MinOstKruskalAlgo/*.qml\
			../MinOstPrimAlgo/*.cpp		../MinOstPrimAlgo/*.qml\
            ../DijkstraAlgo/*.cpp		../DijkstraAlgo/*.qml\
            ../RainbowAlgo/*.cpp		../RainbowAlgo/*.qml\
            ../CustomAlgo/*.cpp			../CustomAlgo/*.qml
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += Logic/
HEADERS += \
    Logic/qmlbridgebasic.h \
    Logic/qmlbridge.h \
    Logic/AbstractGraph/abstractgraph.h \
    Logic/AbstractGraph/abstractgraph_header.h \
    Logic/AbstractGraph/graph_save_struct.h \
    Logic/AbstractGraph/graph_struct_decl.h \
    Logic/GraphView/graphview.h \
    Logic/AbstractGraph/point_to_line.h \
    Logic/qmlbridge_header.h \
    Logic/AlgoPlugins/algopluginloader.h \
    Logic/AlgoPlugins/algointerface.h \
    Logic/AlgoPlugins/sleep.h \
    Logic/AbstractGraph/graphio.h \
    Logic/AlgoPlugins/graphreset.h
