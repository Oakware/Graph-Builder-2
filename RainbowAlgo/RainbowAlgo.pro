TARGET = Rainbow
TEMPLATE = lib
INCLUDEPATH += ../GraphBuilder/Logic/

DEFINES += RAINBOWALGO_LIBRARY

SOURCES += rainbowalgo.cpp\
	../GraphBuilder/Logic/AbstractGraph/abstractgraph.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_save_struct.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.cpp

HEADERS += rainbowalgo.h\
		rainbowalgo_global.h\
	../GraphBuilder/Logic/AbstractGraph/abstractgraph.h \
	../GraphBuilder/Logic/AbstractGraph/graph_save_struct.h \
	../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.h \
	../GraphBuilder/Logic/AlgoPlugins/graphreset.h\
	../GraphBuilder/Logic/AlgoPlugins/sleep.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
	resources.qrc
