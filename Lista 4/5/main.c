#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float intensidade = 1.0f;   
int aumentando = 0;         
float angulo = 30.0f;       
float angMin = 20.0f;       
float angMax = 80.0f;       

void configurarIluminacao() {
    float rad = angulo * (M_PI / 180.0f);
    GLfloat posLuz[] = { 0.0f, -cos(rad), -sin(rad), 0.0f };
    GLfloat luzAmbiente[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat luzDifusa[]   = { intensidade, intensidade, intensidade, 1.0f };
    GLfloat luzSpecular[] = { intensidade, intensidade, intensidade, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzSpecular);

    GLfloat matAmb[]  = { 0.2f, 0.18f, 0.05f, 1.0f };
    GLfloat matDif[]  = { 0.9f, 0.85f, 0.1f, 1.0f };
    GLfloat matSpec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat brilho    = 64.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 3.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    configurarIluminacao();

    GLUquadric *q = gluNewQuadric();
    gluSphere(q, 1.0, 64, 64);
    gluDeleteQuadric(q);

    glutSwapBuffers();
}

void atualizarLuz(int v) {
    if (aumentando) intensidade += 0.02f;
    else intensidade -= 0.02f;

    if (intensidade >= 1.0f) { intensidade = 1.0f; aumentando = 0; }
    if (intensidade <= 0.1f) { intensidade = 0.1f; aumentando = 1; }

    float t = (intensidade - 0.1f) / (1.0f - 0.1f);
    if (t < 0) t = 0; if (t > 1) t = 1;
    angulo = angMax - t * (angMax - angMin);

    glutPostRedisplay();
    glutTimerFunc(30, atualizarLuz, 0);
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Ex5 - Variação de intensidade e direção da luz");

    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(30, atualizarLuz, 0);

    glutMainLoop();
    return 0;
}
