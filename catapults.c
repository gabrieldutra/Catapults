#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include "model/props.h"
#include "model/mapa.h"
#include "model/balista.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define KEYBOARD_CONTROL 1
#define MOUSE_CONTROL 0

int controleDoJogo = MOUSE_CONTROL;

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
   glOrtho(-(WINDOW_WIDTH/2), (WINDOW_WIDTH/2), -(WINDOW_HEIGHT/2), (WINDOW_HEIGHT/2), -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void atualiza(int idx) {
    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos
    
    balista.posicao.x+=balista.velocidade*cos(grausParaRadianos(balista.inclinacao));
    balista.posicao.y+=balista.velocidade*sin(grausParaRadianos(balista.inclinacao));

    if(balista.velocidade > 0) balista.velocidade -=0.1;
    if(balista.velocidade < 0) balista.velocidade = 0;

    orientacaoEmGraus += velocidadeAngular;

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

// Callback de movimento do mouse
void movimentoMouse(int x, int y) {
    /* Ponto central da tela: ((WINDOW_WIDTH/2), (WINDOW_HEIGHT/2))
       Ponto do mouse na tela:  (x, y)
       Vetor direção da balista: Ponto do mouse - Ponto central
       Ângulo da nave: ângulo entre o vetor direção e o vetor i
    */
    Vetor _vetorDirecao;
    _vetorDirecao.x = x-(WINDOW_WIDTH/2);
    _vetorDirecao.y = -1*(y-(WINDOW_HEIGHT/2)); // -1 para inverter o sentido de crescimento
    
    Vetor _i;
    _i.x = 1;
    _i.y = 0;
    
    // Define a inclinação da balista
    if(controleDoJogo == MOUSE_CONTROL) balista.inclinacao = vetor_calculaAngulo(_vetorDirecao,_i);
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
            if(balista.velocidade < 5) balista.velocidade+=1;
            break;
        case 's':
            if(balista.velocidade > 0) balista.velocidade-=1;
            if(balista.velocidade < 0) balista.velocidade = 0;
            break;
        case 'a':
            if(controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao+=10;
            break;
        case 'd':
            if(controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao-=10;
            break;
        case 'c':
            controleDoJogo = !controleDoJogo; // inverte o controle do jogo
            break;
        case '+':
        case '=':
            velocidadeAngular += 1;
            break;
        case '-':
        case '_':
            velocidadeAngular -= 1;
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
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_WIDTH)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-WINDOW_HEIGHT)/2);

    // Abre a janela
    glutCreateWindow("Catapults - Medieval war where you actually control a Ballista");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutPassiveMotionFunc(movimentoMouse);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
