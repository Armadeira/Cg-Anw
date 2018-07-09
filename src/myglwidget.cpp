// myglwidget.cpp

#include "GL/glew.h"

#include "myglwidget.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <QtWidgets/QInputDialog>

using namespace cg;
using namespace glm;
using namespace std;


MyGLWidget::MyGLWidget(QWidget *parent, QGLFormat format)
        : QGLWidget(format, parent) {


    camera = new Camera();

    QWidget::setFocusPolicy(Qt::ClickFocus);

    backgroundColor = vec3(0, 0, 0);

}

MyGLWidget::~MyGLWidget() {
}

QSize MyGLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const {
    return QSize(400, 400);
}

void MyGLWidget::onOpenClicked(){
    std::cout << "onOpen" << std::endl;
}




void MyGLWidget::initializeGL() {


    char *vertexshader_path = (char *) "../shader/simpleShader.vert";
    char *fragmentshader_path = (char *) "../shader/simpleShader.frag";

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);


    glewExperimental = GL_TRUE;
    glewInit();


    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);


    //enable point size in shader
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


    //// create shader
    shader.Init(vertexshader_path, fragmentshader_path);
    openMesh(nullptr);



    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 4, 7));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);

    //camera->Update();



}

void MyGLWidget::openMesh(char *model_path) {
    //prepare data for OpenGL
    renderer.initRenderer(shader, model_path);
}


void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //camera->Update();
    camera->GetMatricies(projectionMatrix, viewMatrix, modelMatrix);

    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;    //Compute the mvp matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    glMatrixMode(GL_MODELVIEW);


    shader.Bind();

    shader.passUniformToShader(modelMatrix, viewMatrix, projectionMatrix, normalMatrix, camera->getCamera_position());

    renderer.render();

    shader.Unbind();


}


void MyGLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);
    camera->Update();

}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->buttons() & Qt::LeftButton) {

        renderer.select(getWorldCoordinates(event->pos().x(), event->pos().y()));

        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        glm::vec3 worldPos = getWorldCoordinates(event->pos().x(), event->pos().y());
        renderer.getMeshWrapper().addVertex(worldPos);

        renderer.getMeshWrapper().deselectAll();

        renderer.recreateMesh();

        renderer.select(worldPos);
        updateGL();
    }


}

void MyGLWidget::saveOBJ(char *file){
    renderer.getMeshWrapper().writeMesh(file);

}


void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    float moveStepSize = 0.01;

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_Z) {
        renderer.getMeshWrapper().undo();
        renderer.recreateMesh();
        updateGL();
    }

    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            camera->Move(FORWARD);
            updateGL();
            break;
        case Qt::Key_A:
            camera->Move(LEFT);
            updateGL();
            break;
        case Qt::Key_S:
            camera->Move(BACK);
            updateGL();
            break;
        case Qt::Key_D:
            camera->Move(RIGHT);
            updateGL();
            break;
        case Qt::Key_Q:
            camera->Move(DOWN);
            updateGL();
            break;
        case Qt::Key_E:
            camera->Move(UP);
            updateGL();
            break;
        case Qt::Key_G:
            renderer.setRenderGrid(!renderer.isRenderGrid());
            updateGL();
            break;
        case Qt::Key_P:
            renderer.setRenderPoints(!renderer.isRenderPoints());
            updateGL();
            break;
        case Qt::Key_R:
            renderer.getMeshWrapper().deleteSelectedVertices();
            renderer.updateMesh(true);
            updateGL();
            break;
        case Qt::Key_F:
            renderer.getMeshWrapper().makeSelectedFace();
            renderer.getMeshWrapper().deselectAll();
            renderer.recreateMesh();
            updateGL();
            break;
        case Qt::Key_C:
            renderer.getMeshWrapper().subdivision();
            renderer.getMeshWrapper().deselectAll();
            renderer.recreateMesh();
            updateGL();
            break;
        case Qt::Key_Left:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(-moveStepSize, 0.0, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Right:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(moveStepSize, 0.0, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Up:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, 0.0, -moveStepSize));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Down:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, 0.0, moveStepSize));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Minus:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, -moveStepSize, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Plus:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, moveStepSize, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        default:
            break;
    }


}

#define NORMALMATRIX {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}
vec3 MyGLWidget::getWorldCoordinates(int x, int y) {

    GLdouble obj[3];
    GLint viewport[4];
    GLdouble model[16] = NORMALMATRIX;
    GLdouble *viewProjection = this->camera->viewProjection;
    GLfloat winX, winY, winZ;
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, model, viewProjection, viewport, obj, obj + 1, obj + 2);

    return vec3(obj[0], obj[1], obj[2]);


}



