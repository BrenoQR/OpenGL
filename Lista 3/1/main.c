// rubik.c
#include <GL/glut.h>

GLfloat rotX = 30.0f;
GLfloat rotY = -30.0f;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
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

    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);

    drawRubikCube();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f) {
        glOrtho(-2.0*aspect, 2.0*aspect, -2.0, 2.0, -10.0, 10.0);
    } else {
        glOrtho(-2.0, 2.0, -2.0/aspect, 2.0/aspect, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC para sair
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Cubo de Rubik - Ortogonal");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
