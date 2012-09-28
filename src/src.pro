QT += opengl

SOURCES +=  lvp.cpp \
            main.cpp \
            confEq.cpp \
            baseDnmImageviewer.cpp \
            basePnmImageviewer.cpp \
            baseImageViewer.cpp \
            dbmImageViewer.cpp \
            dgmImageViewer.cpp \
            pbmImageviewer.cpp \
            pgmImageviewer.cpp \
            glwidget.cpp \
            reconstruction.cpp \
            ploter.cpp \
            reconstructionES.cpp \
            textEditor.cpp \
            import.cpp

HEADERS +=  lvp.h \
            confEq.h \
            baseDnmImageviewer.h \
            basePnmImageviewer.h \
            baseImageViewer.h \
            dbmImageViewer.h \
            dgmImageViewer.h \
            pbmImageviewer.h \
            pgmImageviewer.h \
            glwidget.h \
            reconstruction.h \
            ploter.h \
            reconstructionES.h \
            textEditor.h \
            import.h

FORMS +=    lvpinterface.ui \
            dialogImport.ui \
            dialogConfEq.ui \
            dialogReconstruction.ui \
            dialogReconstructionES.ui

TEMPLATE =  app

CONFIG +=   thread \
            qt \
            rtti \
            opengl \
            precompile_header

DESTDIR =   ../bin

RESOURCES = application.qrc

MOC_DIR =   ./tmp

OBJECTS_DIR =  ./tmp

TRANSLATIONS = lvp_en.ts \
               lvp_pt.ts

INCLUDEPATH += ../../qwt/include \
               ../../libldsc/src/

LIBS +=  -L../../qwt/lib \
         -L../../libldsc/src \
         -lqwt \
         -lldsc

TARGET = lvp
