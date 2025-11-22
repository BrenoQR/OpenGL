#include <GL/glut.h>
#include <stdlib.h>

GLuint tex;
int texSize = 512;

unsigned char *checker(int size,int n){
    unsigned char *img=malloc(4*size*size);
    int s=size/n;
    for(int y=0;y<size;y++)for(int x=0;x<size;x++){
        int c=((x/s)+(y/s))%2?255:0;
        int i=4*(y*size+x);
        img[i]=img[i+1]=img[i+2]=c; img[i+3]=255;
    }
    return img;
}

void buildTex(){
    unsigned char *img=checker(texSize,8);
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texSize,texSize,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
    free(img);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex);
    glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(-1,-1);
      glTexCoord2f(1,0); glVertex2f(1,-1);
      glTexCoord2f(1,1); glVertex2f(1,1);
      glTexCoord2f(0,1); glVertex2f(-1,1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

int main(int c,char**v){
    glutInit(&c,v);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(600,600);
    glutCreateWindow("Ex1 - Tabuleiro");
    buildTex();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
