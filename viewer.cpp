#include "viewer.h"

#include <math.h>
#include <iostream>
#include <QTime>

using namespace std;


Viewer::Viewer(const QGLFormat &format)
  : QGLWidget(format), _timer(new QTimer(this))
    {

    setlocale(LC_ALL,"C");

    _noiseDebug = false;

    _grid = new Grid(1024,-1.0f, 1.0f);


    _timer->setInterval(10);
    connect(_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

Viewer::~Viewer() {
    deleteVAO();
    deleteShaders();
}


void Viewer::createVAO() {

    const GLfloat quadData[] = {
    -1.0f,-1.0f,0.0f, 1.0f,-1.0f,0.0f, -1.0f,1.0f,0.0f, -1.0f,1.0f,0.0f, 1.0f,-1.0f,0.0f, 1.0f,1.0f,0.0f };

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

void enableNoiseShader(){

}

void Viewer::drawGrid(GLuint id){

    glUniformMatrix4fv(glGetUniformLocation(id,"modelMat"),1,GL_FALSE,&(_modelMat[0][0]));
    glUniformMatrix4fv(glGetUniformLocation(id,"viewMat"),1,GL_FALSE,&(_viewMat[0][0]));
    glUniformMatrix4fv(glGetUniformLocation(id,"projMat"),1,GL_FALSE,&(_projMat[0][0]));

    /* on dessine la grille */
    glBindVertexArray(_vaoTerrain);
    glDrawElements(GL_TRIANGLES,3*_grid->nbVertices(),GL_UNSIGNED_INT,(void *)0);

}


void Viewer::paintGL() {

    glViewport(0,0,width(),height());

    /* On active le shader pour générer le bruit */
    glUseProgram(_gridShader->id());

    /* on clear les buffers */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGrid(_gridShader->id());

    


    /* On desactive le shader actif */
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

    createShaders();
    createVAO();

    /* creation des matrices: Modele, Vue, Projection */

    float fovy = 45.0;
    float aspect = width()/height();
    float near = 0.1;
    float far = 100;


    _modelMat = glm::mat4(1.0f);
    _viewMat = glm::mat4(1.0f);
    _projMat = glm::mat4(1.0f);
    _viewMat = glm::lookAt(glm::vec3(0.0,0.0,5.0) ,glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,1.0)); 
    _projMat = glm::perspective(fovy, aspect, near, far);

    // starts the timer 
    _timer->start();
}

/* Creation shader */

void Viewer::createShaders(){
    _noiseShader = new Shader();
    _noiseShader->load("shaders/noise.vert","shaders/noise.frag");
    _gridShader = new Shader();
    _gridShader->load("shaders/grid.vert","shaders/grid.frag");
}

/* Destruction shader */

void Viewer::deleteShaders() {
  delete _noiseShader; _noiseShader = NULL;
  delete _gridShader; _gridShader = NULL;
}


/* Handler graphique */


void Viewer::resizeGL(int width,int height) {
    glViewport(0,0,width,height);
    updateGL();
}

void Viewer::mousePressEvent(QMouseEvent *me) {


  updateGL();
}

void Viewer::mouseMoveEvent(QMouseEvent *me) {
    const glm::vec2 p((float)me->x(),(float)(height()-me->y()));


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
        _gridShader->reload("shaders/grid.vert","shaders/grid.frag");
        _noiseShader->reload("shaders/noise.vert","shaders/noise.frag");

    }


    updateGL();
}

