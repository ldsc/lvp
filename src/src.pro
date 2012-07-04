QT += opengl

SOURCES += lvp.cpp \
           main.cpp \
           imageviewer.cpp \
           confEq.cpp \
           imageViewer3D.cpp \
           baseImageViewer.cpp \
	       glwidget.cpp \
	       reconstruction.cpp \
	       ploter.cpp \
	       reconstructionES.cpp \
	       textEditor.cpp

HEADERS += lvp.h \
           imageviewer.h \
           confEq.h \
           imageViewer3D.h \
           baseImageViewer.h \
	       glwidget.h \
	       reconstruction.h \
	       ploter.h \
	       reconstructionES.h \
	       textEditor.h

FORMS +=   lvpinterface.ui \
           dialogConfEq.ui \
	       dialogReconstruction.ui \
	       dialogReconstructionES.ui

TEMPLATE = app

CONFIG +=  thread \
           qt \
           rtti \
	       opengl \
	       precompile_header

DESTDIR =  ../bin

RESOURCES= application.qrc

MOC_DIR =  ./tmp

OBJECTS_DIR = ./tmp

TRANSLATIONS = lvp_en.ts \
               lvp_pt.ts

INCLUDEPATH += ../../qwt/include \
			   ../../libldsc/src/

LIBS += -L../../qwt/lib \
                -L../../libldsc/src \
		-lqwt \
        -lldsc

TARGET = lvp
