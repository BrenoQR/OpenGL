#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int viewIndex = 0;
float fov = 60.0f;             // campo de visão vertical em graus

// Ângulos (X, Y) para cada ponto de vista
GLfloat views[8][2] = {
    { 30.0f, -30.0f},
    { 30.0f,  30.0f},
    { 30.0f, 150.0f},
    { 30.0f,-150.0f},
    {-30.0f, -30.0f},
    {-30.0f,  30.0f},
    {-30.0f, 150.0f},
    {-30.0f,-150.0f}
};

void setProjection(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, aspect, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
}

void init(void) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
}

void drawRubikCube(void) {
    // Frente (vermelho)
    glBegin(GL_QUADS);
      glColor3f(1,0,0);
      glVertex3f(-1,-1, 1);
      glVertex3f( 1,-1, 1);
      glVertex3f( 1, 1, 1);
      glVertex3f(-1, 1, 1);
    glEnd();

    // Trás (laranja)
    glBegin(GL_QUADS);
      glColor3f(1,0.5,0);
      glVertex3f(-1,-1,-1);
      glVertex3f(-1, 1,-1);
      glVertex3f( 1, 1,-1);
      glVertex3f( 1,-1,-1);
    glEnd();

    // Direita (azul)
    glBegin(GL_QUADS);
      glColor3f(0,0,1);
      glVertex3f( 1,-1,-1);
      glVertex3f( 1, 1,-1);
      glVertex3f( 1, 1, 1);
      glVertex3f( 1,-1, 1);
    glEnd();

    // Esquerda (verde)
    glBegin(GL_QUADS);
      glColor3f(0,1,0);
      glVertex3f(-1,-1,-1);
      glVertex3f(-1,-1, 1);
      glVertex3f(-1, 1, 1);
      glVertex3f(-1, 1,-1);
    glEnd();

    // Topo (branco)
    glBegin(GL_QUADS);
      glColor3f(1,1,1);
      glVertex3f(-1, 1,-1);
      glVertex3f(-1, 1, 1);
      glVertex3f( 1, 1, 1);
      glVertex3f( 1, 1,-1);
    glEnd();

    // Baixo (amarelo)
    glBegin(GL_QUADS);
      glColor3f(1,1,0);
      glVertex3f(-1,-1,-1);
      glVertex3f( 1,-1,-1);
      glVertex3f( 1,-1, 1);
      glVertex3f(-1,-1, 1);
    glEnd();
}

void display(void) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    setProjection(w, h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Afasta a câmera para ver o cubo inteiro
    glTranslatef(0.0f, 0.0f, -6.0f);

    // Rotaciona conforme vista
    glRotatef(views[viewIndex][0], 1.0, 0.0, 0.0);
    glRotatef(views[viewIndex][1], 0.0, 1.0, 0.0);

    drawRubikCube();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    setProjection(w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);

    if (key == 'v' || key == 'V') {
        viewIndex = (viewIndex + 1) % 8;
    }
    else if (key >= '1' && key <= '8') {
        viewIndex = key - '1';
    }
    else if (key == 'i' || key == 'I') {       // zoom in
        fov -= 5.0f;
        if (fov < 15.0f) fov = 15.0f;          // limite mínimo
        printf("Zoom IN → FOV: %.1f\n", fov);
    }
    else if (key == 'o' || key == 'O') {       // zoom out
        fov += 5.0f;
        if (fov > 120.0f) fov = 120.0f;        // limite máximo
        printf("Zoom OUT → FOV: %.1f\n", fov);
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Cubo de Rubik - Perspectiva");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
