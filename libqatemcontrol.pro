QT += network

TARGET = qatemcontrol
TEMPLATE = lib

DEFINES += LIBQATEMCONTROL_LIBRARY

SOURCES += qatemconnection.cpp \
    qatemfairlight.cpp \
    qatemmixeffect.cpp \
    qatemcameracontrol.cpp \
    qatemdownstreamkey.cpp \
    qatemsubsystembase.cpp

HEADERS += qatemconnection.h \
        libqatemcontrol_global.h \
    qatemfairlight.h \
    qatemsubsystembase.h \
    qupstreamkeysettings.h \
    qatemmixeffect.h \
    qatemtypes.h \
    qatemcameracontrol.h \
    qatemdownstreamkey.h

isEmpty(QAC_INSTALL_LIBS):QAC_INSTALL_LIBS = $$[QT_INSTALL_LIBS]
isEmpty(QAC_INSTALL_BINS):QAC_INSTALL_BINS = $$[QT_INSTALL_BINS]
isEmpty(QAC_INSTALL_HEADERS):QAC_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]/QAtemControl

unix {
    target.path = $$QAC_INSTALL_LIBS
    headers.path = $$QAC_INSTALL_HEADERS
    headers.files = $$HEADERS

    INSTALLS += headers target
}

windows {
    target.path = $$QAC_INSTALL_LIBS
    headers.path = $$QAC_INSTALL_HEADERS
    headers.files=$$HEADERS

    INSTALLS += headers target
}

