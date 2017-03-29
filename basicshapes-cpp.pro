
QT += 3dcore 3drender 3dinput 3dextras
QT += widgets

INCLUDE_DIR = .
NET_DIR = $${INCLUDE_DIR}/nets
SCENE_DIR = $${INCLUDE_DIR}/scene

INCLUDEPATH += $${NET_DIR} \
               $${INCLUDE_DIR} \
               $${SCENE_DIR} \

DEPENDPATH += $$INCLUDEPATH

SOURCES += main.cpp \
    $${NET_DIR}/net.cpp \
    $${NET_DIR}/trianglenet.cpp \
    $${NET_DIR}/triangle3dnet.cpp \
    $${NET_DIR}/quadnet.cpp \
    $${NET_DIR}/cubenet.cpp \
    $${NET_DIR}/netfactory.cpp \
    $${SCENE_DIR}/connection.cpp \
    $${SCENE_DIR}/node.cpp \
    core.cpp \
    debug.cpp \
    $${SCENE_DIR}/scene.cpp \
    application.cpp

HEADERS += \
    settings.h \
    $${NET_DIR}/net.h \
    $${NET_DIR}/trianglenet.h \
    $${NET_DIR}/triangle3dnet.h \
    $${NET_DIR}/quadnet.h \
    $${NET_DIR}/cubenet.h \
    $${NET_DIR}/netfactory.h \
    core.h \
    $${SCENE_DIR}/connection.h \
    $${SCENE_DIR}/node.h \
    debug.h \
    $${SCENE_DIR}/scene.h \
    application.h




