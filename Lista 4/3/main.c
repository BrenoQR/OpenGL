#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

void init() {

    glClearColor(0.2, 0.2, 0.2, 1.0);
    
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_DEPTH_TEST);

    GLfloat mat_ambient[] = { 0.3, 0.3, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.0, 1.0 };
 
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; 
    
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.0,  
              0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    GLUquadric* quad = gluNewQuadric();

    gluSphere(quad, 1.5, 50, 50);
    gluDeleteQuadric(quad);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (h == 0) h = 1;

    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 3");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}