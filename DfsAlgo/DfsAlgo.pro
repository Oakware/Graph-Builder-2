TARGET = DFS
TEMPLATE = lib
INCLUDEPATH += ../GraphBuilder/Logic/

DEFINES += DFSALGO_LIBRARY

SOURCES +=\
    ../GraphBuilder/Logic/AbstractGraph/abstractgraph.cpp \
    ../GraphBuilder/Logic/AbstractGraph/graph_save_struct.cpp \
    ../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.cpp \
    dfsalgo.cpp

HEADERS +=\
    ../GraphBuilder/Logic/AbstractGraph/abstractgraph.h \
    ../GraphBuilder/Logic/AbstractGraph/graph_save_struct.h \
    ../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.h \
    ../GraphBuilder/Logic/AlgoPlugins/graphreset.h\
    ../GraphBuilder/Logic/AlgoPlugins/sleep.h \
    dfsalgo.h \
    dfsalgo_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resources.qrc
