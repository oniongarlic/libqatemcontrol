QT += network

TARGET = qatemcontrol
TEMPLATE = lib

DEFINES += LIBQATEMCONTROL_LIBRARY

SOURCES += qatemconnection.cpp \
    qatemfairlight.cpp \
    qatemmixeffect.cpp \
    qatemcameracontrol.cpp \
    qatemdownstreamkey.cpp

HEADERS += qatemconnection.h \
        libqatemcontrol_global.h \
    qatemfairlight.h \
    qupstreamkeysettings.h \
    qatemmixeffect.h \
    qatemtypes.h \
    qatemcameracontrol.h \
    qatemdownstreamkey.h

macx {
    target.path = /usr/local/lib
    header_files.path = /usr/local/include
}
unix:!macx {
    target.path = /usr/lib
    header_files.path = /usr/include
}
unix {
    INSTALLS += target

    header_files.files = $$HEADERS
    INSTALLS += header_files
}

windows {
    headers.path= $$[QT_INSTALL_PREFIX]/include/qatemcontrol
    headers.files=$$HEADERS
    target.path=$$[QT_INSTALL_PREFIX]/lib/qatemcontrol
    INSTALLS += headers target
}

!isEmpty(target.path): INSTALLS += target
