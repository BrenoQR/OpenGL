#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

// Ângulos para a rotação da Terra e da Lua
static float earthAngle = 0.0;
static float moonAngle = 0.0;

// Objeto Quadric para desenhar o disco
GLUquadric* quad;

// Função para inicializar o ambiente OpenGL
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-200, 200, -200, 200); 

    quad = gluNewQuadric();
}

// Função de callback para desenhar na tela
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    gluDisk(quad, 0, 30.0, 50, 1);
    glPopMatrix();


    // Calcula a posição e desenha a Terra (azul)
    glPushMatrix();
        glRotatef(earthAngle, 0.0, 0.0, 1.0); 
        glTranslatef(120.0, 0.0, 0.0);     
        glColor3f(0.0, 0.0, 1.0);
        gluDisk(quad, 0, 15.0, 30, 1);      

        glPushMatrix();
            glRotatef(moonAngle, 0.0, 0.0, 1.0); 
            glTranslatef(30.0, 0.0, 0.0);     
            glColor3f(0.8, 0.8, 0.8);
            gluDisk(quad, 0, 5.0, 20, 1); 
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

// Função para animar a cena
void animate(int value) {
    earthAngle += 1.0;
    moonAngle += 5.0;

    if (earthAngle > 360.0) {
        earthAngle -= 360.0;
    }
    if (moonAngle > 360.0) {
        moonAngle -= 360.0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}

// Função de limpeza ao fechar o programa
void cleanup() {
    gluDeleteQuadric(quad);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sistema Solar");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(25, animate, 0);
    
    
    atexit(cleanup);
    
    glutMainLoop();
    return 0;
}