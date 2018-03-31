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

    void createFBO();
    void initFBO();
    void deleteFBO();

 private:

    void drawGrid(GLuint id);
    void drawDebugMap(GLuint id, GLuint idTexture, char *shaderName);
    void drawQuad();
    void computePerlinNoise(GLuint id);


    

    Grid *_grid; // notre grille avec le terrain

    GLuint _vaoTerrain;
    GLuint _vaoQuad;
    GLuint _terrain[2];
    GLuint _quad;

    GLuint _fbo;

    /* Texture created */
    GLuint _perlinTexture;

    Shader *_noiseShader;
    Shader *_gridShader;

    glm::mat4 _modelMat;
    glm::mat4 _viewMat;
    glm::mat4 _projMat;

    /* Debug shader */
    Shader *_debugNoise;
    Shader *_debugNormal;

    bool _noiseDebug;
    bool _normalDebug;


    QTimer *_timer;    // timer that controls the animation
  
};

#endif // VIEWER_H
