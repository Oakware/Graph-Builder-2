TARGET = MinOstPrim
TEMPLATE = lib
INCLUDEPATH += ../GraphBuilder/Logic/

DEFINES += MINOSTPRIM_LIBRARY

SOURCES +=\
	../GraphBuilder/Logic/AbstractGraph/abstractgraph.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_save_struct.cpp \
	../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.cpp \
    minostprim.cpp

HEADERS +=\
	../GraphBuilder/Logic/AbstractGraph/abstractgraph.h \
	../GraphBuilder/Logic/AbstractGraph/graph_save_struct.h \
	../GraphBuilder/Logic/AbstractGraph/graph_struct_decl.h \
	../GraphBuilder/Logic/AlgoPlugins/graphreset.h\
	../GraphBuilder/Logic/AlgoPlugins/sleep.h \
    minostprim.h \
    minostprim_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resources.qrc
