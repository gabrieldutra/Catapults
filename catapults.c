#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include "model/props/props.h"
#include "model/mapa/mapa.h"
#include "model/balista/balista.h"
#include "model/tiro/tiro.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define KEYBOARD_CONTROL 1
#define MOUSE_CONTROL 0

int controleDoJogo = MOUSE_CONTROL;
int keyState[256];

// Objetos
Mapa mapa;
Balista balista;
ListaTiro *tiros = NULL;
Tiro tiro;

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(-balista.posicao.x, -balista.posicao.y, 0);
        mapa_desenhaMapa(&mapa);
        ListaTiro *_tiros = tiros;
        while(_tiros != NULL){
            glPushMatrix();
                glTranslatef(_tiros->tiro.posicao.x, _tiros->tiro.posicao.y, 0); 
                glRotatef(_tiros->tiro.inclinacao, 0, 0, 1);  
                tiro_desenhaTiro(&(_tiros->tiro));    
            glPopMatrix();
            _tiros = _tiros->proximo;
        }                 
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
    int i;
    for(i=0;i<256;i++) keyState[i]=0;
    mapa = mapa_criaMapa();
    balista = balista_criaBalista();
    tiro = tiro_criaTiro(balista.posicao, 5, balista.inclinacao);
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto
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
    // Verificações de teclas
    if(keyState['w'] && balista.velocidade < 5) balista.velocidade+=0.2;
    
    if(keyState['s'] && balista.velocidade < 5) {
        if(balista.velocidade > 0) balista.velocidade -= 0.2;
    }

    if(keyState['a'] && controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao+=5;

    if(keyState['d'] && controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao-=5;


    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos

    // Movimento da Balista
    
    balista.posicao.x+=balista.velocidade*cos(grausParaRadianos(balista.inclinacao));
    balista.posicao.y+=balista.velocidade*sin(grausParaRadianos(balista.inclinacao));
    if(balista.velocidade > 0) balista.velocidade -=0.05;
    if(balista.velocidade < 0) balista.velocidade = 0;

    if(balista.podeAtirar < BALISTA_TEMPO_RECARGA) balista.podeAtirar++;

    // Movimento dos tiros
    
    ListaTiro *_tiros = tiros;
    while(_tiros != NULL){
        _tiros->tiro.posicao.x+=_tiros->tiro.velocidade*cos(grausParaRadianos(_tiros->tiro.inclinacao));
        _tiros->tiro.posicao.y+=_tiros->tiro.velocidade*sin(grausParaRadianos(_tiros->tiro.inclinacao));
        if(_tiros->tiro.velocidade > 0) _tiros->tiro.velocidade -=0.08;
        if(_tiros->tiro.velocidade < 5) {
            tiros = listatiro_deletaTiro(tiros, &(_tiros->tiro));
        }
        _tiros = _tiros->proximo;
    }

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
    keyState[key] = 1;
    Tiro _novoTiro;
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case 'c':
            controleDoJogo = !controleDoJogo; // inverte o controle do jogo
            break;
        case ' ':
            if(balista.podeAtirar == BALISTA_TEMPO_RECARGA){
                _novoTiro = tiro_criaTiro(balista.posicao, 10, balista.inclinacao);
                tiros = listatiro_adicionaTiro(tiros, _novoTiro);
                balista.podeAtirar = 0;
            }
            // tiro_atualizaPosicao(&tiro, balista.posicao);
            // tiro.inclinacao = balista.inclinacao;
            // tiro.velocidade = 10;
            break;
        default:
            break;
    }
}

// Callback de evento de teclado liberado
void tecladoUp(unsigned char key, int x, int y)
{
    keyState[key] = 0;
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
    glutKeyboardUpFunc(tecladoUp);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
