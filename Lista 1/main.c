#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Cor amarela para o "sol"
    glColor3f(1.0, 1.0, 0.0);

    int numPontas = 16;       // número de "raios"
    float raioExterno = 0.8;  // raio maior
    float raioInterno = 0.4;  // raio menor

    float angInicial = PI / 2; // começa no topo (90°)

    glBegin(GL_POLYGON);
    for (int i = 0; i < numPontas * 2; i++) {
        float ang = angInicial + i * PI / numPontas;
        float r = (i % 2 == 0) ? raioInterno : raioExterno;
        float x = r * cos(ang);
        float y = r * sin(ang);
        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}

void init(void) {
    // Cor de fundo cinza-escuro
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void reshape(int w, int h) {
    // mantém a figura sem deformar, ancorada no canto inferior esquerdo
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // mantém escala constante, independente do tamanho da janela
    // área visível fixa em [-1,1] x [-1,1]
    if (w <= h) {
        // largura limita a janela
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 
                             1.0 * (GLfloat)h / (GLfloat)w);
    } else {
        // altura limita a janela
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 
                    1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("/tmp/estrela");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);  // <- função que controla redimensionamento
    glutMainLoop();
    return 0;
}
