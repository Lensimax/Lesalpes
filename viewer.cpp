#include "viewer.h"

#include <math.h>
#include <iostream>
#include <QTime>

using namespace std;

/* taille d'un ligne de la grille */
const unsigned int GRID_SIZE = 1024;


Viewer::Viewer(const QGLFormat &format)
  : QGLWidget(format), _timer(new QTimer(this))
{

    setlocale(LC_ALL,"C");

    _noiseDebug = false;
    _normalDebug = false;

    _grid = new Grid(GRID_SIZE,-1.0f, 1.0f);

    char *filename = "models/sphere.off";
    _mesh = new Mesh(filename);

    _cam  = new Camera(_mesh->radius,glm::vec3(_mesh->center[0],_mesh->center[1],_mesh->center[2]));


    _timer->setInterval(10);
    connect(_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

Viewer::~Viewer() {
    deleteVAO();
    deleteShaders();
    deleteFBO();
    delete _timer;
    delete _mesh;
    delete _cam;
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



void Viewer::drawGrid(GLuint id){

    glUniformMatrix4fv(glGetUniformLocation(id,"mdvMat"),1,GL_FALSE,&(_cam->mdvMatrix()[0][0]));
    glUniformMatrix4fv(glGetUniformLocation(id,"projMat"),1,GL_FALSE,&(_cam->projMatrix()[0][0]));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_heightMap);
    glUniform1i(glGetUniformLocation(id, "heightmap"), 0);    

    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D,_normalMap);
    glUniform1i(glGetUniformLocation(id, "normalMap"), 1); 

    /* on dessine la grille */
    glBindVertexArray(_vaoTerrain);
    glDrawElements(GL_TRIANGLES,3*_grid->nbVertices(),GL_UNSIGNED_INT,(void *)0);
    glDrawElements(GL_TRIANGLES,3*_grid->nbFaces(),GL_UNSIGNED_INT,(void *)0);

    /* on desactive le vertex array */
    glBindVertexArray(0);

}


void Viewer::drawDebugMap(GLuint id, GLuint idTexture, char *shaderName){
    /* active la texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,idTexture);
    glUniform1i(glGetUniformLocation(id,shaderName),0);

    /* Dessine le carré */
    glBindVertexArray(_vaoQuad);
    glDrawArrays(GL_TRIANGLES,0,6);

    /* desactive le vertex array */
    glBindVertexArray(0);
}

void Viewer::drawQuad(){
    glBindVertexArray(_vaoQuad);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
}

void Viewer::computePerlinNoise(GLuint id){
    /* on active le FBO pour créer la texturea de bruit de Perlin et 
    la texture de normal */
    glBindFramebuffer(GL_FRAMEBUFFER,_fbo);

    /* on active le shader du bruit de Perlin */
    glUseProgram(id);

    /* on indique quelles textures on veut tracer */
    GLenum bufferlist [] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,bufferlist);

    /* on clear les buffers */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* on dessine le carré */
    drawGrid(_gridShader->id());

    /* desactive le FBO */
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Viewer::computeNormalMap(GLuint id){
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glUseProgram(id);

    GLenum bufferlist [] = {GL_COLOR_ATTACHMENT1};
    glDrawBuffers(1,bufferlist);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_heightMap);
    glUniform1i(glGetUniformLocation(id, "heightmap"), 0);

    drawQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Viewer::paintGL() {

    glViewport(0,0,width(),height());

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    computePerlinNoise(_noiseShader->id());

    computeNormalMap(_normalShader->id());

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    
    /* On active le shader pour afficher la grille */
    glUseProgram(_gridShader->id());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // drawGrid(_gridShader->id());
    drawGrid(_gridShader->id());


    /* affichage de la noise map */
    if(_noiseDebug){

        glUseProgram(_debugNoise->id());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawDebugMap(_debugNoise->id(), _heightMap, "noiseMap");
    }    

    /* affichage de la normal map */
    if(_normalDebug){

        glUseProgram(_debugNormal->id());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawDebugMap(_debugNormal->id(), _normalMap, "normalMap");
    }


    /* On desactive le shader actif */
    glUseProgram(0);
    glBindVertexArray(0);
}



void Viewer::initializeGL() {
    // make this window the current one
    makeCurrent();

    _grid = new Grid(8, 0.0, 5.0);

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

    /* Crée et initialize le FBO */
    createFBO();
    initFBO();

    /* creation des matrices: Modele, Vue, Projection */

    float fovy = 45.0;
    float aspect = width()/height();
    float near = 0.1;
    float far = 100;


    _modelMat = glm::mat4(1.0f);
    _viewMat = glm::mat4(1.0f);
    _projMat = glm::mat4(1.0f);
    _viewMat = glm::lookAt(glm::vec3(2.0,0.0,10.0) ,glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0)); 
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
    _debugNoise = new Shader();
    _debugNoise->load("shaders/debugNoise.vert","shaders/debugNoise.frag");
    _debugNormal = new Shader();
    _debugNormal->load("shaders/debugNormal.vert","shaders/debugNormal.frag");
    _normalShader = new Shader();
    _normalShader->load("shaders/normal.vert", "shaders/normal.frag");
}

/* Destruction shader */

void Viewer::deleteShaders() {
  delete _noiseShader; _noiseShader = NULL;
  delete _gridShader; _gridShader = NULL;
  delete _debugNoise; _debugNoise = NULL;
  delete _debugNormal; _debugNormal = NULL;
  delete _normalShader; _normalShader = NULL;
}

/* Create FBO */

void Viewer::createFBO(){
    int nbFBO = 1;

    glGenFramebuffers(nbFBO, &_fbo);
    glGenTextures(1,&_heightMap);
    glGenTextures(1,&_normalMap);

}

void Viewer::initFBO(){

    /* creation des textures */

    /* creation de la texture avec le bruit de Perlin */
    /* la taille est égale au nombre de cases de la grille */
    glBindTexture(GL_TEXTURE_2D,_heightMap);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F,GRID_SIZE,GRID_SIZE,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,_normalMap);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F,GRID_SIZE,GRID_SIZE,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* on active le frameBufferObject */
    /* pour associer les textures */
    glBindFramebuffer(GL_FRAMEBUFFER,_fbo);

    glBindTexture(GL_TEXTURE_2D,_heightMap);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,_heightMap,0);

    glBindTexture(GL_TEXTURE_2D,_normalMap);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,_normalMap,0);

    /* on desactive le buffer */
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Viewer::deleteFBO(){
  glDeleteFramebuffers(1,&_fbo);
  glDeleteTextures(1,&_heightMap);
  glDeleteTextures(1,&_normalMap);
}


/* Handler graphique */


void Viewer::resizeGL(int width,int height) {
    glViewport(0,0,width,height);
    initFBO();
    updateGL();
}

void Viewer::mousePressEvent(QMouseEvent *me) {
    const glm::vec2 p((float)me->x(),(float)(height()-me->y()));

  if(me->button()==Qt::LeftButton) {
    _cam->initRotation(p);
  } else if(me->button()==Qt::MidButton) {
    _cam->initMoveZ(p);
  } else if(me->button()==Qt::RightButton) {
  } 

  updateGL();
}

void Viewer::mouseMoveEvent(QMouseEvent *me) {
    const glm::vec2 p((float)me->x(),(float)(height()-me->y()));
 
    _cam->move(p);

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
    _gridShader->reload("shaders/grid.vert","shaders/grid.frag");
    _debugNoise->reload("shaders/debugNoise.vert","shaders/debugNoise.frag");
    _debugNormal->reload("shaders/debugNormal.vert","shaders/debugNormal.frag");
    _normalShader->reload("shaders/normal.vert", "shaders/normal.frag");

    }

    if(ke->key()==Qt::Key_P){
        _noiseDebug = !_noiseDebug;
    }

    if(ke->key()==Qt::Key_N){
        _normalDebug = !_normalDebug;
    }




    updateGL();
}

