#include <GL/glut.h>

void display_rewritten(void) {
    glLoadIdentity();
    
    //Primeiro quadrado
    glColor3f(1.0, 0.0, 0.0);
    
    glTranslatef(0.5, 0.5, 0.0);

    glRectf(-0.25, -0.25, 0.25, 0.25);
    
    glTranslatef(-0.5, -0.5, 0.0);

    //Segundo quadrado
    glColor3f(0.0, 0.0, 1.0);

    glTranslatef(0.5, 0.5, 0.0);

    glRotatef(45.0, 0.0, 0.0, 1.0);

    // Desenha o segundo quadrado na posição transformada
    glRectf(-0.25, -0.25, 0.25, 0.25);

    glRotatef(-45.0, 0.0, 0.0, 1.0);

    glTranslatef(-0.5, -0.5, 0.0);
}