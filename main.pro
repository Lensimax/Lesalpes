GLM_PATH  = ../ext/glm-0.9.4.1

TEMPLATE  = app
TARGET    = lesalpes

LIBS     += -lGLEW -lGL -lGLU -lm
INCLUDEPATH  +=  $${GLM_PATH}

SOURCES   = main.cpp viewer.cpp grid.cpp shader.cpp camera.cpp meshLoader.cpp trackball.cpp
HEADERS   = grid.h viewer.h shader.h camera.h meshLoader.h trackball.h

CONFIG   += qt opengl warn_on thread uic4 release
QT       *= xml opengl core
