QT += opengl
#QT += 3dcore 3drenderer 3dinput
QT += printsupport

CONFIG += static

SOURCES +=  lvp.cpp \
            main.cpp \
            confEq.cpp \
            baseDnmImageviewer.cpp \
            basePnmImageviewer.cpp \
            baseImageViewer.cpp \
            dbmImageViewer.cpp \
            #dbmImageViewerInt.cpp \
            dgmImageViewer.cpp \
            neuralNetworkConfig.cpp \
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
            percolationNetwork.cpp \
            #searchdialog.cpp \
            qHexEdit/qhexedit.cpp \
            qHexEdit/qhexedit_p.cpp \
            qHexEdit/xbytearray.cpp \
            qHexEdit/commands.cpp \
            crop3D.cpp \
            properties.cpp \
            poresThroats.cpp \
            intrinsicPermeabilityByNetwork.cpp \
            dialogGrayTons.cpp

HEADERS +=  lvp.h \
            confEq.h \
            baseDnmImageviewer.h \
            basePnmImageviewer.h \
            baseImageViewer.h \
            dbmImageViewer.h \
            #dbmImageViewerInt.h \
            dgmImageViewer.h \
            neuralNetworkConfig.h \
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
            percolationNetwork.h \
            #searchdialog.h \
            qHexEdit/qhexedit.h \
            qHexEdit/qhexedit_p.h \
            qHexEdit/xbytearray.h \
            qHexEdit/commands.h \
            crop3D.h \
            properties.h \
            qdebugstream.h \
            poresThroats.h \
            intrinsicPermeabilityByNetwork.h \
            dialogGrayTons.h


FORMS +=    lvpinterface.ui \
            dialogImport.ui \
            dialogConfEq.ui \
            dialogProperties.ui \
            dialogReconstruction.ui \
            dialogReconstructionES.ui \
            neuralNetworkConfig.ui \
            optionsdialog.ui \
            dialogCrop3D.ui \
            dialogPoresThroats.ui \
            dialogIntrinsicPermeabilityByNetwork.ui \
            dialogPercolationNetwork.ui \
            dialogGrayTons.ui
            #searchdialog.ui

TEMPLATE =  app

CONFIG +=   thread \
            qt \
            rtti \
            opengl \
            precompile_header

DESTDIR =   ../bin

RESOURCES = application.qrc

RC_FILE = dialogo.rc

MOC_DIR =   ./tmp

OBJECTS_DIR =  ./tmp

#TRANSLATIONS = lvp_en.ts \
#               lvp_pt.ts

#CODECFORSRC = UTF-8

INCLUDEPATH += /usr/local/qwt-6.2.0/include \
               /usr/local/libldsc/src/ \
                /usr/include/qwt/ \
                /usr/local/libldsc/src/

LIBS +=  -L/usr/local/qwt-6.2.0/lib \
         -L/usr/include/qwt \
         -L/usr/local/libldsc/src \
         -lqwt \
         -lldsc \
         -lgomp

unix {
    LIBS +=  -lGLU
}
win32 {
    LIBS +=  -lglu32\
             -lopengl32
}

TARGET = lvp

OTHER_FILES += \
    ../Doxyfile

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -std=c++11
#debug {}
QMAKE_CXXFLAGS_DEBUG += -ggdb
#release {}
QMAKE_LFLAGS += -fopenmp
