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



    void createFBOComputing();
    void initFBOComputing();
    void deleteFBOComputing();

 private:

    void loadTexture(GLuint id,const char *filename);
    void createTextures();
    void deleteTextures();

    void drawVAO(GLuint id);
    void drawGrid(GLuint id);
    void drawDebugMap(GLuint id, GLuint idTexture, char *shaderName);
    void drawQuad();
    void computePerlinNoise(GLuint id);
    void computeNormalMap(GLuint id);


    

    Grid *_grid; // notre grille avec le terrain

    GLuint _vaoTerrain;
    GLuint _vaoQuad;
    GLuint _terrain[2];
    GLuint _quad;

    GLuint _fboComputing;

    GLuint _mountainText;

    /* Texture created */
    GLuint _heightMap;
    GLuint _normalMap;
    GLuint _renderedGridMap;

    Shader *_noiseShader;
    Shader *_gridShader;
    Shader *_normalShader;
    Shader *_postProcessShader;

    glm::mat4 _modelMat;
    glm::mat4 _viewMat;
    glm::mat4 _projMat;

    /* Debug shader */
    Shader *_debugNoise;
    Shader *_debugNormal;

    bool _noiseDebug;
    bool _normalDebug;

    Camera *_cam;




    QTimer *_timer;    // timer that controls the animation
  
};

#endif // VIEWER_H
