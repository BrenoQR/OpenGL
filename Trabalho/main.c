#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

// --- Estruturas de Dados ---

// Representa um vertice 3D com coordenadas x, y, z
typedef struct {
    float x, y, z;
} Vertex;

// Representa uma face triangular armazenando 3 indices de vertices
typedef struct {
    int vIndices[3];
} Face;

// Estrutura principal que armazena uma malha 3D completa
typedef struct {
    Vertex* vertices;      // Array de todos os vertices
    int numVertices;       // Quantidade de vertices
    Face* faces;           // Array de todas as faces (triangulos)
    int numFaces;          // Quantidade de faces
    
    // Dados para normalizacao (centralizar e escalar a malha)
    Vertex minBounds;      // Ponto minimo da caixa delimitadora
    Vertex maxBounds;      // Ponto maximo da caixa delimitadora
    Vertex center;         // Centro da malha
    float scale;           // Fator de escala para padronizar tamanho
} Mesh;

// --- Variaveis Globais ---

Mesh meshes[3];           // Array armazena Dragao, Coelho, Buddha (indices 0, 1, 2)
int currentMeshIndex = 0; // Indice da malha atualmente exibida (0, 1 ou 2)

// Controle do Mouse (Rotacao)
float rotX = 0.0f, rotY = 0.0f;  // Angulos de rotacao em graus
int lastMouseX, lastMouseY;       // Posicao anterior do mouse
int isDragging = 0;               // Flag indicando se esta arrastando o mouse

// Controle de Zoom
float zoom = 3.0f;  // Distancia da camera (quanto maior, mais afastado)

// --- Funcoes Auxiliares de Vetor ---

// Calcula a normal (vetor perpendicular) de uma face triangular
// Utilizada para iluminacao basica (Flat Shading)
// Parâmetros: v1, v2, v3 = vertices do triângulo; nx, ny, nz = componentes da normal resultante
void calculateNormal(Vertex v1, Vertex v2, Vertex v3, float* nx, float* ny, float* nz) {
    // Calcula dois vetores das arestas do triangulo
    float u[3] = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    float v[3] = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

    // Produto vetorial para obter a normal (u × v)
    *nx = u[1] * v[2] - u[2] * v[1];
    *ny = u[2] * v[0] - u[0] * v[2];
    *nz = u[0] * v[1] - u[1] * v[0];

    // Normaliza o vetor resultante (reduz magnitude para 1.0)
    float len = sqrt((*nx)*(*nx) + (*ny)*(*ny) + (*nz)*(*nz));
    if (len > 0.0f) {
        *nx /= len; *ny /= len; *nz /= len;
    }
}

// --- Carregador de OBJ ---

// Carrega um arquivo OBJ e armazena os dados na malha especificada
// Parametros: filename = caminho do arquivo; meshIndex = indice no array meshes (0, 1 ou 2)
void loadOBJ(const char* filename, int meshIndex) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro: Nao foi possivel abrir %s\n", filename);
        exit(1);
    }

    printf("Carregando %s...\n", filename);

    // Aloca memoria inicial para vertices e faces (sera realocada conforme necessario)
    int maxV = 100000, maxF = 200000;
    meshes[meshIndex].vertices = (Vertex*)malloc(sizeof(Vertex) * maxV);
    meshes[meshIndex].faces = (Face*)malloc(sizeof(Face) * maxF);
    meshes[meshIndex].numVertices = 0;
    meshes[meshIndex].numFaces = 0;

    // Inicializa os limites da caixa delimitadora com valores extremos
    meshes[meshIndex].minBounds = (Vertex){FLT_MAX, FLT_MAX, FLT_MAX};
    meshes[meshIndex].maxBounds = (Vertex){-FLT_MAX, -FLT_MAX, -FLT_MAX};

    // Le o arquivo linha por linha
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Processa vertices (linhas começam com 'v ')
        if (line[0] == 'v' && line[1] == ' ') {
            Vertex v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            
            // Realoca memoria se necessário (dobra o tamanho)
            if (meshes[meshIndex].numVertices >= maxV) {
                maxV *= 2;
                meshes[meshIndex].vertices = (Vertex*)realloc(meshes[meshIndex].vertices, sizeof(Vertex) * maxV);
            }

            meshes[meshIndex].vertices[meshes[meshIndex].numVertices++] = v;

            // Atualiza os limites da caixa delimitadora
            if (v.x < meshes[meshIndex].minBounds.x) meshes[meshIndex].minBounds.x = v.x;
            if (v.y < meshes[meshIndex].minBounds.y) meshes[meshIndex].minBounds.y = v.y;
            if (v.z < meshes[meshIndex].minBounds.z) meshes[meshIndex].minBounds.z = v.z;
            if (v.x > meshes[meshIndex].maxBounds.x) meshes[meshIndex].maxBounds.x = v.x;
            if (v.y > meshes[meshIndex].maxBounds.y) meshes[meshIndex].maxBounds.y = v.y;
            if (v.z > meshes[meshIndex].maxBounds.z) meshes[meshIndex].maxBounds.z = v.z;
        }
        // Processa faces (linhas começam com 'f ')
        else if (line[0] == 'f' && line[1] == ' ') {
            Face f;
            int vIdx[3];
            char* token = strtok(line + 2, " ");  // Extrai numeros apos "f "
            int count = 0;
            
            // Le ate 3 indices de vertices para formar um triangulo
            while (token != NULL && count < 3) {
                vIdx[count] = atoi(token);
                
                // TRATAMENTO DE INDICES (OBJ pode usar indices positivos ou negativos)
                if (vIdx[count] < 0) {
                    // Indice negativo: referencia relativa ao vertice atual
                    vIdx[count] = meshes[meshIndex].numVertices + vIdx[count]; 
                } else {
                    // Indice positivo: converte de 1-based (OBJ) para 0-based (C)
                    vIdx[count] -= 1; 
                }
                
                f.vIndices[count] = vIdx[count];
                count++;
                token = strtok(NULL, " ");
            }
            
            // Armazena a face se conseguiu ler 3 vertices validos
            if (count == 3) { 
                if (meshes[meshIndex].numFaces >= maxF) {
                    maxF *= 2;
                    meshes[meshIndex].faces = (Face*)realloc(meshes[meshIndex].faces, sizeof(Face) * maxF);
                }
                meshes[meshIndex].faces[meshes[meshIndex].numFaces++] = f;
            }
        }
    }
    fclose(file);

    // Calcula o centro e fator de escala da malha
    Mesh* m = &meshes[meshIndex];
    m->center.x = (m->minBounds.x + m->maxBounds.x) / 2.0f;
    m->center.y = (m->minBounds.y + m->maxBounds.y) / 2.0f;
    m->center.z = (m->minBounds.z + m->maxBounds.z) / 2.0f;

    // Calcula as dimensoes da caixa delimitadora
    float dx = m->maxBounds.x - m->minBounds.x;
    float dy = m->maxBounds.y - m->minBounds.y;
    float dz = m->maxBounds.z - m->minBounds.z;
    
    // Encontra a dimensao maxima
    float maxDim = dx;
    if (dy > maxDim) maxDim = dy;
    if (dz > maxDim) maxDim = dz;

    // Define escala para caber em um cubo de -1 a 1 (factor 2.0)
    m->scale = 2.0f / maxDim;

    printf("Concluido: %d vertices, %d faces.\n", m->numVertices, m->numFaces);
}

// Funcao de renderizacao - chamada a cada frame
void display() {
    // Limpa o buffer de cor e profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posiciona a camera usando a variável 'zoom' (afastamento)
    gluLookAt(0.0, 0.0, zoom,  // Posicao da camera (afastada no eixo Z)
              0.0, 0.0, 0.0,   // Ponto para onde a camera olha (origem)
              0.0, 1.0, 0.0);  // Vetor "para cima" (eixo Y)

    // Aplica as rotacoes controladas pelo mouse
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);  // Rotacao em torno do eixo X
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);  // Rotacao em torno do eixo Y

    Mesh* m = &meshes[currentMeshIndex];

    // Salva o estado da matriz antes de transformar
    glPushMatrix();
    
    // Aplica escala e translacao para normalizar a malha
    glScalef(m->scale, m->scale, m->scale);
    glTranslatef(-m->center.x, -m->center.y, -m->center.z);

    // Desenha todos os triangulos da malha
    glBegin(GL_TRIANGLES);
    glColor3f(0.8f, 0.8f, 0.8f);  // Cor cinza claro
    
    for (int i = 0; i < m->numFaces; i++) {
        // Obtem os 3 vertices do triangulo
        Vertex v1 = m->vertices[m->faces[i].vIndices[0]];
        Vertex v2 = m->vertices[m->faces[i].vIndices[1]];
        Vertex v3 = m->vertices[m->faces[i].vIndices[2]];

        // Calcula a normal do triangulo para iluminacao
        float nx, ny, nz;
        calculateNormal(v1, v2, v3, &nx, &ny, &nz);
        glNormal3f(nx, ny, nz);

        // Especifica os 3 vertices do triangulo
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();

    // Restaura o estado da matriz
    glPopMatrix();
    
    // Troca os buffers (front e back) para exibir a imagem renderizada
    glutSwapBuffers();
}

// Funcao chamada quando a janela e redimensionada
void reshape(int w, int h) {
    // Evita divisao por zero
    if (h == 0) h = 1;
    
    // Calcula a proporcao (aspect ratio) da janela
    float aspect = (float)w / (float)h;
    
    // Define a area de desenho (viewport)
    glViewport(0, 0, w, h);
    
    // Configura a matriz de projecao (perspectiva)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);  // Campo de visao 45°, planos de recorte
    
    // Volta para a matriz de modelo-visao
    glMatrixMode(GL_MODELVIEW);
}

// Funcao chamada quando uma tecla e pressionada
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);  // ESC para sair
    
    // Teclas 1, 2, 3 para trocar entre os modelos
    if (key == '1') currentMeshIndex = 0;  // Coelho
    if (key == '2') currentMeshIndex = 1;  // Dragao
    if (key == '3') currentMeshIndex = 2;  // Buddha
    
    char* names[] = {"Coelho", "Dragao", "Buddha"};
    printf("Exibindo: %s\n", names[currentMeshIndex]);
    
    // Solicita um novo frame de renderizacao
    glutPostRedisplay();
}

// Funcao para controlar o zoom com a roda do mouse
void updateZoom(int direction) {
    if (direction > 0) {
        zoom -= 0.2f;  // Aproximar
    } else {
        zoom += 0.2f;  // Afastar
    }
    
    // Limita o zoom para manter a camera em posicao valida
    if (zoom < 0.5f) zoom = 0.5f;
    if (zoom > 10.0f) zoom = 10.0f;

    glutPostRedisplay();
}

// Funcao chamada quando o mouse clica ou usa scroll
void mouse(int button, int state, int x, int y) {
    // Suporte para zoom com scroll do mouse
    if (state == GLUT_UP) {
        if (button == 3) updateZoom(1);   // Aproximar
        if (button == 4) updateZoom(-1);  // Afastar
    }

    // Detecta clique do botao esquerdo para rotacao
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;        // Iniciou o arrasto
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = 0;        // Terminou o arrasto
        }
    }
}

// Funcao chamada quando o mouse e movido (enquanto arrastando)
void motion(int x, int y) {
    if (isDragging) {
        // Calcula a diferenca em relacao a posicao anterior
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // Atualiza as rotacoes baseado no movimento do mouse
        rotY += dx * 0.5f;  // Rotacao horizontal
        rotX += dy * 0.5f;  // Rotacao vertical

        // Atualiza a posicao do mouse
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// Inicializacao do OpenGL
void initGL() {
    // Habilita teste de profundidade (depth buffer) para desenho correto de oclusao
    glEnable(GL_DEPTH_TEST); 
    
    // Habilita iluminacao
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Primeira luz
    glEnable(GL_COLOR_MATERIAL);  // Permite colorir materiais

    // Define a posicao e cor da luz
    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};  // Luz direcional (última 0.0)
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Branca
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    
    // Define cor de fundo (cinza escuro)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
}

// Funcao principal - ponto de entrada do programa
int main(int argc, char** argv) {
    // Inicializa GLUT com argumentos da linha de comando
    glutInit(&argc, argv);
    
    // Configura modo de exibicao (double buffer, RGB, depth)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Define tamanho inicial da janela
    glutInitWindowSize(800, 600);
    
    // Cria a janela
    glutCreateWindow("Visualizador OBJ");

    // Inicializa OpenGL
    initGL();

    // Carrega os 3 modelos 3D
    loadOBJ("bunny.obj", 0);
    loadOBJ("dragon.obj", 1);
    loadOBJ("buddha.obj", 2);

    // Registra as funções callback (chamadas quando certos eventos ocorrem)
    glutDisplayFunc(display);      // Renderizacao
    glutReshapeFunc(reshape);      // Redimensionamento de janela
    glutKeyboardFunc(keyboard);    // Teclado
    glutMouseFunc(mouse);          // Mouse (clique e scroll)
    glutMotionFunc(motion);        // Movimento do mouse

    // Exibe instrucoes de uso
    printf("\nInstrucoes:\n");
    printf("- Segure botao esquerdo do mouse para rotacionar o objeto.\n");
    printf("- Use o SCROLL do mouse para dar Zoom.\n");
    printf("- Pressione '1' para ver o Dragao.\n");
    printf("- Pressione '2' para ver o Coelho.\n");
    printf("- Pressione '3' para ver o Buddha.\n");

    // Inicia o loop principal de eventos
    glutMainLoop();
    
    return 0;
}