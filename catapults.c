#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "model/props/props.h"
#include "model/mapa/mapa.h"
#include "model/balista/balista.h"
#include "model/tiro/tiro.h"
#include "model/asteroide/asteroide.h"
#include "model/barra/barra.h"
#include "model/menu/menu.h"

int windowWidth = 800;
int windowHeight = 600;

#define KEYBOARD_CONTROL 1
#define MOUSE_CONTROL 0
#define GAME_RADIUS 1000
#define NUMERO_ASTEROIDES_BASE 15

int controleDoJogo = MOUSE_CONTROL;
int jogoRodando = 0;
int jogoRecemAberto = 1;
int telaInicial;
int keyState[256];
int numeroAsteroides = NUMERO_ASTEROIDES_BASE;
double velocidadeTiro = TIRO_VELOCIDADE_MINIMA;

int texturaTiro, texturaAsteroide;

// Objetos
Mapa mapa;
Balista balista;
ListaTiro *tiros = NULL;
ListaAsteroide *asteroides = NULL;
Menu menuPause;
Menu menuGameOver;

// Barras do HUD
Barra barraTiro;
Barra barraPoderTiro;

// Informativos
FILE *registraPontuacaoMaxima;
int pontuacaoUsuario = 0;
int pontuacaoMaxima;

// Variáveis de som
Mix_Chunk *somBackground = NULL;
Mix_Chunk *somTiro = NULL;
Mix_Chunk *somAsteroideDestruido = NULL;
Mix_Chunk *somGameOver = NULL;


#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

void desenhaInicio(){
    // Começa a usar a cor branca
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, telaInicial);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex2f(-(mapa.dimensoes.width/1.3), -(mapa.dimensoes.height/2));
        glTexCoord2f(1, 0); glVertex2f((mapa.dimensoes.width/1.3),  -(mapa.dimensoes.height/2));
        glTexCoord2f(1, 1); glVertex2f((mapa.dimensoes.width/1.3),  (mapa.dimensoes.height/2));
        glTexCoord2f(0, 1); glVertex2f(-(mapa.dimensoes.width/1.3), (mapa.dimensoes.height/2));
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        /* Move o sistema de coordenadas para a posição onde deseja-se desenhar
        *  Desenha o mapa a partir da posição -BalistaX módulo com mapaWidth e -BalistaY módulo com mapaHeight
        *  Desse jeito, o mapa repete facilmente, pois na tela a posição só varia entre (0 ~ mapaWidth, 0 ~ mapaHeight)
        *  Varia exatamente a quantidade que o usuário andou em relação àquele ponto
        */
        glTranslatef(-fmod(balista.posicao.x, mapa.dimensoes.width), -fmod(balista.posicao.y, mapa.dimensoes.height), 0);
        int i,j;
        for(i=-3; i<=3; i++){
            for(j=-3; j<=3; j++){
                glPushMatrix();
                    glTranslatef(i*mapa.dimensoes.width,j*mapa.dimensoes.height,0);
                    mapa_desenhaMapa(&mapa);
                glPopMatrix();
            }
        }
    glPopMatrix();

    // Desenha tiros
    glPushMatrix();
        glTranslatef(-balista.posicao.x, -balista.posicao.y, 0);
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

    // Desenha Asteróides
    glPushMatrix();
        glTranslatef(-balista.posicao.x, -balista.posicao.y, 0);
        ListaAsteroide *_asteroides = asteroides;
        while(_asteroides != NULL){
            glPushMatrix();
                glTranslatef(_asteroides->asteroide.posicao.x, _asteroides->asteroide.posicao.y, 0);
                int _rotacaoAsteroideExtra = (int)(_asteroides->asteroide.posicao.x+_asteroides->asteroide.posicao.y)%360;
                glRotatef(_asteroides->asteroide.inclinacao+_rotacaoAsteroideExtra, 0, 0, 1);
                asteroide_desenhaAsteroide(&(_asteroides->asteroide));
            glPopMatrix();
            _asteroides = _asteroides->proximo;
        }
    glPopMatrix();

    glPushMatrix();
        glRotatef(balista.inclinacao, 0, 0, 1);
        // Desenha a Balista na origem
        balista_desenhaBalista(&balista);
    glPopMatrix();

    glPushMatrix();
        // Desenha a barra de poder do tiro
        glTranslatef(barraPoderTiro.posicao.x, barraPoderTiro.posicao.y, 0);
        if(keyState[' ']) barra_desenhaBarra(&barraPoderTiro);
    glPopMatrix();

    // HUD

    glPushMatrix();
        glTranslatef(0, -(windowHeight/2), 0);
        // Barra do tiro
        glPushMatrix();
            glTranslatef(barraTiro.posicao.x, barraTiro.posicao.y, 0);
            barra_desenhaBarra(&barraTiro);
            // Pontuação
            char _scoreMessage[10];
            sprintf(_scoreMessage, "Score: %d",pontuacaoUsuario);
            glColor3f(1, 1, 1);
            escreveTexto(GLUT_BITMAP_HELVETICA_18, _scoreMessage, -4.5* (int) strlen(_scoreMessage), 20, 0);
        glPopMatrix();

    glPopMatrix();

    // Menu de pause
    if(menuPause.estaAberto){
        glPushMatrix();
            glTranslatef(menuPause.posicao.x, menuPause.posicao.y, 0);
            menu_desenhaMenu(&menuPause);
        glPopMatrix();
    }

    // Menu de game over
    if(menuGameOver.estaAberto){
        glPushMatrix();
            glTranslatef(menuGameOver.posicao.x, menuGameOver.posicao.y, 0);
            menu_desenhaMenu(&menuGameOver);
        glPopMatrix();
    }

    if(jogoRecemAberto){
        glPushMatrix();
            desenhaInicio();
        glPopMatrix();
    }

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    telaInicial = SOIL_load_OGL_texture(
        "textures/telaInicial.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    //abre o arquivo Pontuação máxima
    registraPontuacaoMaxima = fopen("highscore.ctp","r");
    if (registraPontuacaoMaxima == NULL) pontuacaoMaxima=0; //se o arquivo não existir, salva a pontuação como 0
    else{ //se não, salva na variavel pontuacaoMaxima
            fscanf(registraPontuacaoMaxima,"%d",&pontuacaoMaxima);
            fclose(registraPontuacaoMaxima); //fecha o arquivo
    }


    int i;
    for(i=0;i<256;i++) keyState[i]=0;
    mapa = mapa_criaMapa();
    balista = balista_criaBalista();

    Vetor _posicaoBarraTiro;
    _posicaoBarraTiro.x = 0;
    _posicaoBarraTiro.y = 40;

    Dimensoes _dimensoesBarraTiro;
    _dimensoesBarraTiro.width = 140;
    _dimensoesBarraTiro.height = 30;

    barraTiro = barra_criaBarra(_posicaoBarraTiro, _dimensoesBarraTiro, 100, 1, .5, 0);

    Vetor _posicaoBarraPoderTiro;
    _posicaoBarraPoderTiro.x = 0;
    _posicaoBarraPoderTiro.y = -40;

    Dimensoes _dimensoesBarraPoderTiro;
    _dimensoesBarraPoderTiro.width = 40;
    _dimensoesBarraPoderTiro.height = 10;

    barraPoderTiro = barra_criaBarra(_posicaoBarraPoderTiro, _dimensoesBarraPoderTiro, 100, 1, 0, 0);

    Vetor _posicaoMenus;
    _posicaoMenus.x = 0;
    _posicaoMenus.y = 0;

    char *_tituloMenuPause = strdup("Pause");
    char *_tituloMenuGameOver = strdup("Game Over");

    Opcao *_opcoesMenuPause = NULL;
    Opcao *_opcoesMenuGameOver = NULL;

    char *_opcaoRetomar = strdup("Retomar");
    char *_opcaoReiniciar = strdup("Reiniciar");
    char *_opcaoControle = strdup("Mouse: Sim");
    char *_opcaoSair = strdup("Sair");

    _opcoesMenuPause = opcao_adicionaOpcao(_opcoesMenuPause, _opcaoRetomar);
    _opcoesMenuPause = opcao_adicionaOpcao(_opcoesMenuPause, _opcaoReiniciar);
    _opcoesMenuPause = opcao_adicionaOpcao(_opcoesMenuPause, _opcaoControle);
    _opcoesMenuPause = opcao_adicionaOpcao(_opcoesMenuPause, _opcaoSair);

    _opcoesMenuGameOver = opcao_adicionaOpcao(_opcoesMenuGameOver, _opcaoReiniciar);
    _opcoesMenuGameOver = opcao_adicionaOpcao(_opcoesMenuGameOver, _opcaoSair);

    menuPause = menu_criaMenu(_posicaoMenus, _tituloMenuPause, _opcoesMenuPause, 4);
    menuGameOver = menu_criaMenu(_posicaoMenus, _tituloMenuGameOver, _opcoesMenuGameOver, 2);

    // Texturas
    texturaTiro = SOIL_load_OGL_texture(
        "textures/arrow.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if(texturaTiro == 0) printf("Erro do SOIL: '%s'\n", SOIL_last_result());

    texturaAsteroide = SOIL_load_OGL_texture(
        "textures/rock.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
    );

    if(texturaAsteroide == 0) printf("Erro do SOIL: '%s'\n", SOIL_last_result());

    // Implementação do som
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    somBackground = Mix_LoadWAV( "soundfx/somBackground.wav" );
    if( somBackground == NULL ){
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    somTiro = Mix_LoadWAV( "soundfx/somTiro.wav" );
    if( somTiro == NULL ){
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    somAsteroideDestruido = Mix_LoadWAV( "soundfx/somAsteroideDestruido.wav" );
    if( somAsteroideDestruido == NULL ){
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    somGameOver = Mix_LoadWAV( "soundfx/somGameOver.wav" );
    if( somGameOver == NULL ){
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    Mix_PlayChannel( -1, somBackground, 0 );


    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   windowWidth = w;
   windowHeight = h;
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-(w/2), (w/2), -(h/2), (h/2), -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void atualizaJogo(){
    int _balistaAtirando = (keyState[' '] && balista.podeAtirar == BALISTA_TEMPO_RECARGA);
    // Verificações de teclas
    if((keyState['w']||keyState['W']) && !_balistaAtirando && balista.velocidade < 5) balista.velocidade+=0.2;

    if((keyState['s']||keyState['S'] || _balistaAtirando) && balista.velocidade < 5) {
        if(balista.velocidade > 0) balista.velocidade -= 0.2;
    }

    if((keyState['a']||keyState['A']) && controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao+=5;

    if((keyState['d']||keyState['D']) && controleDoJogo == KEYBOARD_CONTROL) balista.inclinacao-=5;

    if(_balistaAtirando){
        if(velocidadeTiro < TIRO_VELOCIDADE_MAXIMA){

            velocidadeTiro+=0.25;
            double _valorBarraPoderTiro = 100*(float) (velocidadeTiro-TIRO_VELOCIDADE_MINIMA)/(TIRO_VELOCIDADE_MAXIMA-TIRO_VELOCIDADE_MINIMA);
            barraPoderTiro.valor = _valorBarraPoderTiro;
        }
    }


    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos

    // Movimento da Balista

    balista.posicao.x+=balista.velocidade*cos(grausParaRadianos(balista.inclinacao));
    balista.posicao.y+=balista.velocidade*sin(grausParaRadianos(balista.inclinacao));
    if(balista.velocidade > 0) balista.velocidade -= 0.05;
    if(balista.velocidade < 0) balista.velocidade = 0;

    if(balista.podeAtirar < BALISTA_TEMPO_RECARGA) balista.podeAtirar++;
    barraTiro.valor = (float) (balista.podeAtirar*100)/BALISTA_TEMPO_RECARGA;

    // Textura da Balista
    if(_balistaAtirando) balista.atualTextura = 2;
    else
    if(balista.podeAtirar == BALISTA_TEMPO_RECARGA) balista.atualTextura = 0;
    else balista.atualTextura = 1;

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

    // Gerenciamento dos asteróides

    // Deleta os asteróides distantes
    ListaAsteroide *_asteroides = asteroides;
    while(_asteroides != NULL){
        Vetor _asteroideBalista; // Vetor do asteróide até a balista
        _asteroideBalista.x = balista.posicao.x-_asteroides->asteroide.posicao.x;
        _asteroideBalista.y = balista.posicao.y-_asteroides->asteroide.posicao.y;

        if(vetor_calculaModulo(_asteroideBalista) > GAME_RADIUS+10){
            asteroides = listaasteroide_deletaAsteroide(asteroides, &(_asteroides->asteroide));
        }
        _asteroides = _asteroides->proximo;
    }

    // Verifica se o número de asteróides no mapa é menor do que o definido
    if(listaasteroide_contaAsteroides(asteroides) < numeroAsteroides){
        // Cria um asteróide no raio do jogo
        double _anguloNoRaioDoJogo = rand()%360; // o asteróide vem de algum ponto no raio do jogo

        Vetor _posicaoAsteroide;
        _posicaoAsteroide.x = balista.posicao.x+(GAME_RADIUS*cos(grausParaRadianos(_anguloNoRaioDoJogo)));
        _posicaoAsteroide.y = balista.posicao.y+(GAME_RADIUS*sin(grausParaRadianos(_anguloNoRaioDoJogo)));

        Vetor _direcaoAsteroide; // O asteróide começa em direção à balista
        _direcaoAsteroide.x = balista.posicao.x-_posicaoAsteroide.x;
        _direcaoAsteroide.y = balista.posicao.y-_posicaoAsteroide.y;

        Vetor _i;
        _i.x = 1;
        _i.y = 0;

        double _anguloAsteroide = vetor_calculaAngulo(_direcaoAsteroide, _i); // O ângulo do asteróide é de 0 a 360

        // A velocidade dos asteróides varia entre ASTEROIDE_VELOCIDADE_MINIMA e ASTEROIDE_VELOCIDADE_MAXIMA
        double _velocidadeAsteroide = ASTEROIDE_VELOCIDADE_MINIMA + (float)( rand()%(ASTEROIDE_VELOCIDADE_MAXIMA-ASTEROIDE_VELOCIDADE_MINIMA+1)*100)/100;

        Dimensoes _dimensoesAsteroide;
        _dimensoesAsteroide.width=ASTEROIDE_TAMANHO_MAXIMO;
        _dimensoesAsteroide.height=ASTEROIDE_TAMANHO_MAXIMO;

        Asteroide _novoAsteroide = asteroide_criaAsteroide(_posicaoAsteroide, _velocidadeAsteroide, _dimensoesAsteroide, _anguloAsteroide, &texturaAsteroide);
        asteroides = listaasteroide_adicionaAsteroide(asteroides, _novoAsteroide);
    }

    // Movimento dos asteroides
    _asteroides = asteroides;
    while(_asteroides != NULL){
        _asteroides->asteroide.posicao.x+=_asteroides->asteroide.velocidade*cos(grausParaRadianos(_asteroides->asteroide.inclinacao));
        _asteroides->asteroide.posicao.y+=_asteroides->asteroide.velocidade*sin(grausParaRadianos(_asteroides->asteroide.inclinacao));
        _asteroides = _asteroides->proximo;
    }

    // Verificação de colisões asteróide com tiro
    _asteroides = asteroides;
    while(_asteroides != NULL){
        _tiros = tiros;
        while(_tiros != NULL){
            if(asteroide_checaColisaoComTiro(_asteroides->asteroide, _tiros->tiro)){ //se houve colisão
                int _i, _numAsteroide;
                for(_i=0,_numAsteroide=rand()%3 + 2;_i<_numAsteroide;_i++){ //cria numAsteroide asteróides

                    Dimensoes _dimensaoAtual; //variavel para tratar as dimensões dos asteroides a serem criados
                    _dimensaoAtual.width = _asteroides->asteroide.dimensoes.width/1.5 + rand()%3; //diminui o tamanho em um valor aleatorio menor que o original
                    _dimensaoAtual.height = _dimensaoAtual.width;
                    if(_dimensaoAtual.width<=12) break; //limita o numero de divisões a 2

                    double _anguloAsteroide = rand()%360; // O ângulo do asteróide é de 0 a 360

                    //Cria o asteróide e o adiciona a lista
                    Asteroide _novoAsteroide = asteroide_criaAsteroide(_asteroides->asteroide.posicao, _asteroides->asteroide.velocidade+rand()%3, _dimensaoAtual, _anguloAsteroide, &texturaAsteroide);
                    asteroides = listaasteroide_adicionaAsteroide(asteroides,_novoAsteroide);
                }
                asteroides = listaasteroide_deletaAsteroide(asteroides, &(_asteroides->asteroide));
                tiros = listatiro_deletaTiro(tiros, &(_tiros->tiro));
                pontuacaoUsuario++;
                numeroAsteroides = NUMERO_ASTEROIDES_BASE + pontuacaoUsuario;
                Mix_PlayChannel( -1, somAsteroideDestruido, 0 );
            }
            _tiros = _tiros->proximo;
        }
        _asteroides = _asteroides->proximo;
    }

    // Verificação de colisões asteróide com balista
    _asteroides = asteroides;
    while(_asteroides != NULL){
        if(asteroide_checaColisaoComBalista(_asteroides->asteroide, balista)){
            if(pontuacaoUsuario > pontuacaoMaxima){
                pontuacaoMaxima=pontuacaoUsuario;
                registraPontuacaoMaxima = fopen("highscore.ctp","w");
                fprintf(registraPontuacaoMaxima, "%d\n", pontuacaoMaxima);
                fclose(registraPontuacaoMaxima);
            }
            jogoRodando = 0;
            menuPause.estaAberto = 0;
            char _tituloMenuGameOver[50];
            sprintf(_tituloMenuGameOver, "Highscore: %d", pontuacaoMaxima);
            menuGameOver.titulo = strdup(_tituloMenuGameOver);
            menuGameOver.estaAberto = 1;
            Mix_PlayChannel( -1, somGameOver, 0 );
        }
        _asteroides = _asteroides->proximo;
    }
}

void atualiza(int idx) {

    if(jogoRodando) atualizaJogo(); // Caso o jogo esteja rodando chama a atualizaJogo

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

// Callback de movimento do mouse
void movimentoMouse(int x, int y) {
    /* Ponto central da tela: ((windowWidth/2), (windowHeight/2))
       Ponto do mouse na tela:  (x, y)
       Vetor direção da balista: Ponto do mouse - Ponto central
       Ângulo da nave: ângulo entre o vetor direção e o vetor i
    */
    Vetor _vetorDirecao;
    _vetorDirecao.x = x-(windowWidth/2);
    _vetorDirecao.y = -1*(y-(windowHeight/2)); // -1 para inverter o sentido de crescimento

    Vetor _i;
    _i.x = 1;
    _i.y = 0;

    // Define a inclinação da balista
    if(jogoRodando && controleDoJogo == MOUSE_CONTROL) balista.inclinacao = vetor_calculaAngulo(_vetorDirecao,_i);
}

// Callback do glutSpecialFunc
void tecladoEspecial(int key, int x, int y){
    if(key == GLUT_KEY_DOWN){
        if(menuPause.estaAberto) menu_selecionaAbaixo(&menuPause);
        if(menuGameOver.estaAberto) menu_selecionaAbaixo(&menuGameOver);
    }

    if(key == GLUT_KEY_UP){
        if(menuPause.estaAberto) menu_selecionaAcima(&menuPause);
        if(menuGameOver.estaAberto) menu_selecionaAcima(&menuGameOver);
    }
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    keyState[key] = 1;
    switch(key)
    {
        case 'w':
        case 'W':
            if(menuPause.estaAberto) menu_selecionaAcima(&menuPause);
            if(menuGameOver.estaAberto) menu_selecionaAcima(&menuGameOver);
            break;
        case 's':
        case 'S':
            if(menuPause.estaAberto) menu_selecionaAbaixo(&menuPause);
            if(menuGameOver.estaAberto) menu_selecionaAbaixo(&menuGameOver);
            break;
        // Tecla ESC
        case 27:
            if(!menuGameOver.estaAberto && !jogoRecemAberto) if(menuPause.estaAberto){
                menuPause.estaAberto = 0;
                jogoRodando = 1;
            } else {
                menuPause.estaAberto = 1;
                jogoRodando = 0;
            }
            break;
        // Tecla ENTER
        case 13:
            if(jogoRecemAberto){
                balista.posicao.x = 0;
                balista.posicao.y = 0;
                balista.inclinacao = 0;
                balista.velocidade = 0;
                balista.podeAtirar = BALISTA_TEMPO_RECARGA;
                ListaAsteroide *_asteroides = asteroides;
                // Deleta todos os asteróides
                while(_asteroides != NULL){
                    asteroides = listaasteroide_deletaAsteroide(asteroides, &_asteroides->asteroide);
                    _asteroides = _asteroides->proximo;
                }
                menuGameOver.estaAberto = 0;
                menuPause.estaAberto = 0;
                pontuacaoUsuario = 0;
                jogoRodando = 1;
                jogoRecemAberto=0;
            }
            if(menuGameOver.estaAberto){
                if(menuGameOver.opcaoAtual == 0){ // Retomar
                    balista.posicao.x = 0;
                    balista.posicao.y = 0;
                    balista.inclinacao = 0;
                    balista.velocidade = 0;
                    balista.podeAtirar = BALISTA_TEMPO_RECARGA;
                    ListaAsteroide *_asteroides = asteroides;
                    // Deleta todos os asteróides
                    while(_asteroides != NULL){
                        asteroides = listaasteroide_deletaAsteroide(asteroides, &_asteroides->asteroide);
                        _asteroides = _asteroides->proximo;
                    }
                    menuGameOver.estaAberto = 0;
                    menuPause.estaAberto = 0;
                    pontuacaoUsuario = 0;
                    jogoRodando = 1;
                }

                if(menuGameOver.opcaoAtual == 1){ // Sair
                    exit(0);
                }
            }

            if(menuPause.estaAberto){
                if(menuPause.opcaoAtual == 0){ // Retomar
                    menuPause.estaAberto = 0;
                    jogoRodando = 1;
                }

                if(menuPause.opcaoAtual == 1){ // Reiniciar
                    balista.posicao.x = 0;
                    balista.posicao.y = 0;
                    balista.inclinacao = 0;
                    balista.velocidade = 0;
                    balista.podeAtirar = BALISTA_TEMPO_RECARGA;
                    ListaAsteroide *_asteroides = asteroides;
                    // Deleta todos os asteróides
                    while(_asteroides != NULL){
                        asteroides = listaasteroide_deletaAsteroide(asteroides, &_asteroides->asteroide);
                        _asteroides = _asteroides->proximo;
                    }
                    menuPause.estaAberto = 0;
                    pontuacaoUsuario = 0;
                    jogoRodando = 1;
                }

                if(menuPause.opcaoAtual == 2){ // Mouse: Sim/não
                    if(controleDoJogo == MOUSE_CONTROL){
                        controleDoJogo = KEYBOARD_CONTROL;
                        menuPause.opcoes->proximo->proximo->opcao = strdup("Mouse: Nao");
                    } else {
                        controleDoJogo = MOUSE_CONTROL;
                        menuPause.opcoes->proximo->proximo->opcao = strdup("Mouse: Sim");
                    }
                }

                if(menuPause.opcaoAtual == 3){ // Sair
                    exit(0);
                }
            }
            break;
        default:
            break;
    }
}

// Callback de evento de teclado liberado
void tecladoUp(unsigned char key, int x, int y)
{
    Tiro _novoTiro;
    if(key == ' ' && keyState[' ']){
        if(balista.podeAtirar == BALISTA_TEMPO_RECARGA){
            Mix_PlayChannel( -1, somTiro, 0 );
            _novoTiro = tiro_criaTiro(balista.posicao, velocidadeTiro, balista.inclinacao, &texturaTiro);
            tiros = listatiro_adicionaTiro(tiros, _novoTiro);
            balista.podeAtirar = 0;
            velocidadeTiro = TIRO_VELOCIDADE_MINIMA;
        }
    }

    keyState[key] = 0;
}

// Rotina principal
int main(int argc, char **argv)
{
    srand(time(NULL));
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-windowWidth)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-windowHeight)/2);

    // Abre a janela
    glutCreateWindow("Catapults - Medieval war where you actually control a Ballista");

    glutIgnoreKeyRepeat(1);

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutPassiveMotionFunc(movimentoMouse);
    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(tecladoUp);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
