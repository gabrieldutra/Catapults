#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <string.h>
#include "../props/props.h"
#include "menu.h"

/** Opcao - Adicionar opção
* @param lista Lista que vai receber a opção
* @param opcao String a ser adicionada como opção
**/
Opcao *opcao_adicionaOpcao(Opcao *lista, char *opcao){
    Opcao *_novaLista = (Opcao *) malloc(sizeof(Opcao));
    _novaLista->opcao = opcao;
    _novaLista->proximo = lista;
    return _novaLista;
}

/** Menu - Construtor do Menu
* @return Menu com valores iniciais
**/
Menu menu_criaMenu(Vetor posicao, char *titulo, Opcao *opcoes, int n_opcoes){
    Menu _novoMenu;
    
    _novoMenu.posicao = posicao;
    _novoMenu.titulo = titulo;
    _novoMenu.opcoes = opcoes;
    _novoMenu.n_opcoes = n_opcoes;
    _novoMenu.opcaoAtual = 0;
    _novoMenu.estaAberto = 1;

    return _novoMenu;
}

/** Menu - Desenha Menu
* @param menu a ser desenhada
**/
void menu_desenhaMenu(Menu *menu){

    double _menuWidth = 150;
    double _menuHeight = 18 + 36 + 36*(menu->n_opcoes);

    // Cor preta
    glColor3f(0, 0, 0);

    // Desenha a parte preta da menu
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-(_menuWidth/2), -(_menuHeight/2));
        glVertex2f( -(_menuWidth/2),  (_menuHeight/2));
        glVertex2f((_menuWidth/2),  (_menuHeight/2));
        glVertex2f((_menuWidth/2),  -(_menuHeight/2));
    glEnd();

    // // Pontuação
    // char _scoreMessage[10];
    // sprintf(_scoreMessage, "Score");
    glColor3f(1, 1, 1);
    int _linhaAtual=(_menuHeight/2)-20;
    escreveTexto(GLUT_BITMAP_HELVETICA_18, menu->titulo, -4.5* (int) strlen(menu->titulo), _linhaAtual, 0);
    Opcao *_opcao = menu->opcoes;
    int _count=0;
    while(_opcao != NULL){
        _linhaAtual-=40;
        if(_count == menu->opcaoAtual) {
            glColor3f(1, 1, 1);
        } else glColor3f(.6, .6, .6);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, _opcao->opcao, -4.5* (int) strlen(_opcao->opcao), _linhaAtual, 0);
        _opcao = _opcao->proximo;
        _count++;
    }
}