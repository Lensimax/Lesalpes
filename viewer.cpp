#include "viewer.h"

#include <math.h>
#include <iostream>
#include <QTime>

using namespace std;

/* taille d'un ligne de la grille */
const unsigned int GRID_SIZE = 512;


Viewer::Viewer(const QGLFormat &format)
  : QGLWidget(format), _timer(new QTimer(this))
{

    setlocale(LC_ALL,"C");


    _grid = new Grid(GRID_SIZE,-1.0f, 1.0f);


    _cam  = new Camera(1, glm::vec3(0,0,0), 0);



    _timer->setInterval(10);
    connect(_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

Viewer::~Viewer() {
    deleteVAO();
    deleteShaders();
    delete _timer;
    delete _cam;
}


void Viewer::createVAO() {

  const GLfloat quadData[] = {-1.0f,-1.0f,0.0f, 1.0f,-1.0f,0.0f, -1.0f,1.0f,0.0f, -1.0f,1.0f,0.0f, 1.0f,-1.0f,0.0f, 1.0f,1.0f,0.0f };

  glGenBuffers(2,_terrain);
  glGenBuffers(1,&_quad);
  glGenVertexArrays(1,&_vaoTerrain);
  glGenVertexArrays(1,&_vaoQuad);

  // create the VBO associated with the grid (the terrain)
  glBindVertexArray(_vaoTerrain);
  glBindBuffer(GL_ARRAY_BUFFER,_terrain[0]); // vertices
  glBufferData(GL_ARRAY_BUFFER,_grid->nbVertices()*3*sizeof(float),_grid->vertices(),GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_terrain[1]); // indices
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,_grid->nbFaces()*3*sizeof(int),_grid->faces(),GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);

  // create the VBO associated with the screen quad
  glBindVertexArray(_vaoQuad);
  glBindBuffer(GL_ARRAY_BUFFER,_quad); // vertices
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadData),quadData,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *)0);
  glEnableVertexAttribArray(0);
}

void Viewer::deleteVAO() {
    glDeleteBuffers(2,_terrain);
    glDeleteBuffers(1,&_quad);
    glDeleteVertexArrays(1,&_vaoTerrain);
    glDeleteVertexArrays(1,&_vaoQuad);
}

void Viewer::createShaders(){
    _noiseShader = new Shader();
    _noiseShader->load("shaders/noise.vert","shaders/noise.frag");
}

void Viewer::deleteShaders(){
    delete _noiseShader; _noiseShader = NULL;
}

void Viewer::createFBOfirstPass(){
    glGenFramebuffers(1, &_fbofirstPass);
    glGenTextures(1, &_heightMap);
    glGenTextures(1, &_normalMap);
}

void Viewer::deleteFBOfirstPass(){
    glDeleteFramebuffers(1, &_fbofirstPass);
    glDeleteTextures(1, &_normalMap);
    glDeleteTextures(1, &_heightMap);

}

void Viewer::drawQuad(){
    glBindVertexArray(_vaoQuad);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
}

void Viewer::computeHeightMap(GLuint id){
    glUseProgram(id);

    glClear(GL_COLOR_BUFFER_BIT);

    drawQuad();
}

void Viewer::paintGL() {
    glViewport(0,0,GRID_SIZE,GRID_SIZE);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);


    computeHeightMap(_noiseShader->id());
    
    /* on desactive le shader */
    glUseProgram(0);



}



void Viewer::initializeGL() {
    // make this window the current one
    makeCurrent();

    // init and chack glew
    glewExperimental = GL_TRUE;

    if(glewInit()!=GLEW_OK) {
        cerr << "Warning: glewInit failed!" << endl;
    }

    // init OpenGL settings
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glViewport(0,0,width(),height());

    _cam->initialize(width(),height(),true);

    createShaders();
    createVAO();


    // starts the timer 
    _timer->start();
}





/* Handler graphique */


void Viewer::resizeGL(int width,int height) {
    glViewport(0,0,width,height);
    updateGL();
}

void Viewer::mousePressEvent(QMouseEvent *me) {
    const glm::vec2 p((float)me->x(),(float)(height()-me->y()));

    if(me->button()==Qt::LeftButton) {
        _cam->initRotation(p);
        _mode = false;
    } else if(me->button()==Qt::MidButton) {
        _cam->initMoveZ(p);
        _mode = false;
    } else if(me->button()==Qt::RightButton) {
        _light[0] = (p[0]-(float)(width()/2))/((float)(width()/2));
        _light[1] = (p[1]-(float)(height()/2))/((float)(height()/2));
        _light[2] = 1.0f-std::max(fabs(_light[0]),fabs(_light[1]));
        _light = glm::normalize(_light);
        _mode = true;
    } 

    updateGL();
}

void Viewer::mouseMoveEvent(QMouseEvent *me) {
    const glm::vec2 p((float)me->x(),(float)(height()-me->y()));

    if(_mode) {
        // light mode
        _light[0] = (p[0]-(float)(width()/2))/((float)(width()/2));
        _light[1] = (p[1]-(float)(height()/2))/((float)(height()/2));
        _light[2] = 1.0f-std::max(fabs(_light[0]),fabs(_light[1]));
        _light = glm::normalize(_light);
    } else {
    // camera mode
        _cam->move(p);
    }

    updateGL();
}

void Viewer::keyPressEvent(QKeyEvent *ke) {
  
    // key a: play/stop animation
    if(ke->key()==Qt::Key_A) {
    if(_timer->isActive()) 
      _timer->stop();
    else 
      _timer->start();
    }

    // key r: reload shaders 
    if(ke->key()==Qt::Key_R) {
        _noiseShader->reload("shaders/noise.vert","shaders/noise.frag");

    }



    // key i: init camera
    if(ke->key()==Qt::Key_I) {
        _cam->initialize(width(),height(),true);
    }



    updateGL();
}

