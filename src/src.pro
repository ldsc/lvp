QT += opengl

SOURCES +=  lvp.cpp \
            main.cpp \
            confEq.cpp \
            baseDnmImageviewer.cpp \
            basePnmImageviewer.cpp \
            baseImageViewer.cpp \
            dbmImageViewer.cpp \
            #dbmImageViewerInt.cpp \
            dgmImageViewer.cpp \
            pbmImageviewer.cpp \
            pgmImageviewer.cpp \
            glwidget.cpp \
            reconstruction.cpp \
            ploter.cpp \
            reconstructionES.cpp \
            textEditor.cpp \
            import.cpp\
            hexEditor.cpp \
            optionsdialog.cpp \
            #searchdialog.cpp \
            qHexEdit/qhexedit.cpp \
            qHexEdit/qhexedit_p.cpp \
            qHexEdit/xbytearray.cpp \
            qHexEdit/commands.cpp \
            crop3D.cpp

HEADERS +=  lvp.h \
            confEq.h \
            baseDnmImageviewer.h \
            basePnmImageviewer.h \
            baseImageViewer.h \
            dbmImageViewer.h \
            #dbmImageViewerInt.h \
            dgmImageViewer.h \
            pbmImageviewer.h \
            pgmImageviewer.h \
            glwidget.h \
            reconstruction.h \
            ploter.h \
            reconstructionES.h \
            textEditor.h \
            import.h \
            hexEditor.h \
            optionsdialog.h \
            #searchdialog.h \
            qHexEdit/qhexedit.h \
            qHexEdit/qhexedit_p.h \
            qHexEdit/xbytearray.h \
            qHexEdit/commands.h \
            crop3D.h

FORMS +=    lvpinterface.ui \
            dialogImport.ui \
            dialogConfEq.ui \
            dialogReconstruction.ui \
            dialogReconstructionES.ui \
            optionsdialog.ui \
            dialogCrop3D.ui
            #searchdialog.ui

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

OTHER_FILES += \
    ../Doxyfile
