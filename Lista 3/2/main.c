// rubik_view.c
#include <GL/glut.h>
#include <stdlib.h>

int viewIndex = 0;

// Tabelas de ângulos (X, Y) para cada ponto de vista
GLfloat views[8][2] = {
    { 30.0f, -30.0f},  // Vista 1 (padrão)
    { 30.0f,  30.0f},  // Vista 2
    { 30.0f, 150.0f},  // Vista 3
    { 30.0f, -150.0f}, // Vista 4
    {-30.0f, -30.0f},  // Vista 5
    {-30.0f,  30.0f},  // Vista 6
    {-30.0f, 150.0f},  // Vista 7
    {-30.0f, -150.0f}  // Vista 8
};

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
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f)
        glOrtho(-2.0*aspect, 2.0*aspect, -2.0, 2.0, -10.0, 10.0);
    else
        glOrtho(-2.0, 2.0, -2.0/aspect, 2.0/aspect, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'v': // troca de ponto de vista
        case 'V':
            viewIndex = (viewIndex + 1) % 8;
            glutPostRedisplay();
            break;
        case '1'...'8': // vai direto a uma vista
            viewIndex = key - '1';
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Cubo de Rubik - Vistas");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
