QT += quick

# To make threaded gl check...
QT += core-private gui-private

HEADERS += threadrenderer.h \
    logorenderer.h \
    openglcontexttool.h
SOURCES += threadrenderer.cpp main.cpp \
    logorenderer.cpp \
    openglcontexttool.cpp

RESOURCES += \
    qml.qrc \
    shader.qrc

OTHER_FILES +=

DISTFILES +=
