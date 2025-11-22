#include <GL/glut.h>
#include <math.h>

void display_parallelogram(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat shearMatrix_col_major[16] = {
        1.0, 1.732, 0.0, 0.0,
        0.0,   1.0, 0.0, 0.0,
        0.0,   0.0, 1.0, 0.0,
        0.0,   0.0, 0.0, 1.0
    };

    glColor3f(1.0, 0.5, 0.0);

    glPushMatrix();
    glMultMatrixf(shearMatrix_col_major);
    glRectf(0.0, 0.0, 0.5, 0.5);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 2.0, -1.0, 2.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Paralelogramo");

    glutDisplayFunc(display_parallelogram);
    glutReshapeFunc(reshape);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutMainLoop();
    return 0;
}
