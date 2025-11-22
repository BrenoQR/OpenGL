// rubik_zoom_fix.c
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int viewIndex = 0;
float zoom = 1.0f; // 1.0 = padrão

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
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    float base = 2.0f;           // tamanho de referência do volume ortogonal
    float s = base * zoom;       // s aumenta com zoom (quanto maior s, mais "afastado")

    if (aspect >= 1.0f) {
        glOrtho(-s * aspect, s * aspect, -s, s, -10.0f, 10.0f);
    } else {
        glOrtho(-s, s, -s / aspect, s / aspect, -10.0f, 10.0f);
    }

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(views[viewIndex][0], 1.0, 0.0, 0.0);
    glRotatef(views[viewIndex][1], 0.0, 1.0, 0.0);

    drawRubikCube();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    setProjection(w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC

    if (key == 'v' || key == 'V') {
        viewIndex = (viewIndex + 1) % 8;
    } else if (key >= '1' && key <= '8') {
        viewIndex = key - '1';
    } else if (key == 'i' || key == 'I') { // zoom in
        zoom *= 0.9f;                // aproxima
        if (zoom < 0.02f) zoom = 0.02f;
        setProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        printf("zoom IN -> %f\n", zoom);
    } else if (key == 'o' || key == 'O') { // zoom out
        zoom *= 1.1f;                // afasta
        if (zoom > 50.0f) zoom = 50.0f;
        setProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        printf("zoom OUT -> %f\n", zoom);
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int winSize = 700;
    glutInitWindowSize(winSize, winSize);
    glutCreateWindow("Cubo de Rubik - Zoom Corrigido");

    init();
    setProjection(winSize, winSize); // projecção inicial

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
