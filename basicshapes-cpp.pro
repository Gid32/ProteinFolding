
QT += 3dcore 3drender 3dinput 3dextras
QT += widgets

INCLUDE_DIR = .
NET_DIR = $${INCLUDE_DIR}/nets
SCENE_DIR = $${INCLUDE_DIR}/scene
CORE_DIR = $${INCLUDE_DIR}/core

INCLUDEPATH += $${NET_DIR} \
               $${INCLUDE_DIR} \
               $${SCENE_DIR} \
               $${CORE_DIR} \

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
    $${CORE_DIR}/core.cpp \
    application.cpp \
    $${CORE_DIR}/convolution.cpp \
    $${CORE_DIR}/ant.cpp \
    $${CORE_DIR}/convolutionfactory.cpp \
    $${SCENE_DIR}/settingsform.cpp \
    $${SCENE_DIR}/mainwindow.cpp \
    $${SCENE_DIR}/proteinForm.cpp \
    core/result.cpp \
    scene/resultform.cpp

HEADERS += \
    settings.h \
    $${NET_DIR}/net.h \
    $${NET_DIR}/trianglenet.h \
    $${NET_DIR}/triangle3dnet.h \
    $${NET_DIR}/quadnet.h \
    $${NET_DIR}/cubenet.h \
    $${NET_DIR}/netfactory.h \
    $${CORE_DIR}/core.h \
    $${SCENE_DIR}/connection.h \
    $${SCENE_DIR}/node.h \
    application.h \
    $${CORE_DIR}/convolution.h \
    $${CORE_DIR}/ant.h \
    $${CORE_DIR}/convolutionfactory.h \
    $${SCENE_DIR}/settingsform.h \
    $${SCENE_DIR}/mainwindow.h \
    $${SCENE_DIR}/proteinForm.h \
    core/result.h \
    scene/resultform.h

FORMS += \
    $${SCENE_DIR}/settingsform.ui \
    $${SCENE_DIR}/mainwindow.ui \
    $${SCENE_DIR}/proteinForm.ui \
    scene/resultform.ui

RESOURCES += \
    scene/resources.qrc




