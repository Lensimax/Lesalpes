#ifndef VIEWER_H
#define VIEWER_H

// GLEW lib: needs to be included first!!
#include <GL/glew.h> 

// OpenGL library 
#include <GL/gl.h>

// OpenGL Utility library
#include <GL/glu.h>

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QGLFormat>
#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <stack>

#include "grid.h"
#include "shader.h"
#include "camera.h"

class Viewer : public QGLWidget {
 public:
    Viewer(const QGLFormat &format=QGLFormat::defaultFormat());
    ~Viewer();
  
 protected :
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL(int width,int height);
    virtual void keyPressEvent(QKeyEvent *ke);
    virtual void mousePressEvent(QMouseEvent *me);
    virtual void mouseMoveEvent(QMouseEvent *me);

    void createVAO();
    void deleteVAO();

    void createShaders();
    void deleteShaders();




 private:

    Shader *_noiseShader;
    

    Grid *_grid; // notre grille avec le terrain

    GLuint _vaoTerrain;
    GLuint _vaoQuad;
    GLuint _terrain[2];
    GLuint _quad;




    Camera *_cam;
    glm::vec3 _light;
    bool _mode;



    QTimer *_timer;    // timer that controls the animation
  
};

#endif // VIEWER_H
