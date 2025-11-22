#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define textureWidth 8
#define textureHeight 8

GLubyte chessboardTexture[textureHeight][textureWidth][3];
GLuint texName;

void makeChessboardTexture() {
    int i, j, c;
    for (i = 0; i < textureHeight; i++) {
        for (j = 0; j < textureWidth; j++) {

            c = (((i & 1) ^ (j & 1))) * 255;
            
            chessboardTexture[i][j][0] = (GLubyte)c;
            chessboardTexture[i][j][1] = (GLubyte)c;
            chessboardTexture[i][j][2] = (GLubyte)c;
        }
    }
}


void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0); 
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);

    makeChessboardTexture();
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight,
                 0, GL_RGB, GL_UNSIGNED_BYTE, chessboardTexture);


    GLfloat s_plane[] = {0.5, 0.0, 0.0, 0.5}; 

    GLfloat t_plane[] = {0.0, 0.5, 0.0, 0.5}; 

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    
 
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s_plane);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t_plane);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat v0[2] = {-1.0, -0.5};
    GLfloat v1[2] = { 1.0, -0.5};
    GLfloat v2[2] = { 1.5,  0.5}; 
    GLfloat v3[2] = {-0.5,  0.5}; 

    glBindTexture(GL_TEXTURE_2D, texName);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glPushMatrix();
    glTranslatef(-1.5, 0.0, 0.0);
    glBegin(GL_QUADS);

        glVertex2fv(v0);
        glVertex2fv(v1);
        glVertex2fv(v2);
        glVertex2fv(v3);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glPushMatrix();
    glTranslatef(1.5, 0.0, 0.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2fv(v0);
        glTexCoord2f(1.0, 0.0); glVertex2fv(v1);
        glTexCoord2f(1.0, 1.0); glVertex2fv(v2);
        glTexCoord2f(0.0, 1.0); glVertex2fv(v3);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}


void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (h == 0) h = 1;
    gluOrtho2D(-3.0, 3.0, -2.0, 2.0); 
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 2");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}