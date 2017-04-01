#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include "model/props.h"
#include "model/mapa.h"
#include "model/balista.h"

GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
const GLfloat velocidadeTangencial = 0.5;

// Objetos
Mapa mapa;
Balista balista;

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(-balista.posicao.x, -balista.posicao.y, 0);
        mapa_desenhaMapa(&mapa);
    glPopMatrix();

    glPushMatrix();    
        glRotatef(balista.inclinacao, 0, 0, 1);        
        // Desenha a Balista na origem
        balista_desenhaBalista(&balista);
        glEnd();
    glPopMatrix();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    mapa = mapa_criaMapa();
    balista = balista_criaBalista();
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto

    // imprime instruções
    printf("Instrucoes:\n");
    printf("\t+: gira no sentido horario\n");
    printf("\t-: gira no sentido anti-horario\n\n");
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-320, 320, -240, 240, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void atualiza(int idx) {
    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos
    GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);
    x += cos(orientacaoEmRadianos) * velocidadeTangencial;
    y += sin(orientacaoEmRadianos) * velocidadeTangencial;

    orientacaoEmGraus += velocidadeAngular;

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case 'w':
            balista.posicao.y+=5;
            break;
        case 's':
            balista.posicao.y-=5;
            break;
        case 'a':
            balista.posicao.x-=5;
            break;
        case 'd':
            balista.posicao.x+=5;
            break;
        case '+':
        case '=':
            velocidadeAngular += 0.5;
            break;
        case '-':
        case '_':
            velocidadeAngular -= 0.5;
            break;
        default:
            break;
    }
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-480)/2);

    // Abre a janela
    glutCreateWindow("Catapults - Medieval war where you actually control a Ballista");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
