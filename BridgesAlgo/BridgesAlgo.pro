TARGET = Bridges
TEMPLATE = lib
INCLUDEPATH += ../GraphBuilder/Logic/

DEFINES += BRIDGESALGO_LIBRARY

SOURCES += bridgesalgo.cpp\
	../GraphBuilder/Logic/AbstractGraph/abstractgraph.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_save_struct.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.cpp

HEADERS += bridgesalgo.h\
		bridgesalgo_global.h\
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
