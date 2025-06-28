// g++ pacman_copia.cpp -lsfml-graphics -lsfml-window -lsfml-system
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstring>
#include <iostream>
#include <random>
#include <ctime>





// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   implementado
// Intenção de movimento: implementado


using namespace sf;
using namespace std;

const int SIZE = 25; // Tamanho de cada célula do mapa
const int ROWS = 25; // Tamanho do mapa
const int COLS = 25;

char mapa[ROWS][COLS+1] = {
  "1111111111110111111111111",
  "1000000000000000000000001",
  "1011011110110110111101101",
  "1010000000100010000000101",
  "1010111111101011111110101",
  "1010100000000000000010101",
  "1010101111111111111010101",
  "1010100000000000000010101",
  "1010111110110110111110101",
  "1010000010100010100000101",
  "1011111010101010101111101",
  "0000000000101010000000000",
  "1111111110000000111111111",
  "0000000000101010000000000",
  "1011111010101010101111101",
  "1010000010100010100000101",
  "1010111110110110111110101",
  "1010100000000000000010101",
  "1010101111111111111010101",
  "1010100000000000000010101",
  "1010111111101011111110101",
  "1010000000100010000000101",
  "1011011110110110111101101",
  "1000000000000000000000001",
  "1111111111110111111111111"
};

char map2[ROWS][COLS+1] = {
  "1111111111110111111111111",
  "1000000000000000000000001",
  "1011011110110110111101101",
  "1010000000100010000000101",
  "1010111111101011111110101",
  "1010100000000000000010101",
  "1010101111111111111010101",
  "1010100000000000000010101",
  "1010111110110110111110101",
  "1010000010100010100000101",
  "1011111010101010101111101",
  "0000000000101010000000000",
  "1111111110000000111111111",
  "0000000000101010000000000",
  "1011111010101010101111101",
  "1010000010100010100000101",
  "1010111110110110111110101",
  "1010100000000000000010101",
  "1010101111111111111010101",
  "1010100000000000000010101",
  "1010111111101011111110101",
  "1010000000100010000000101",
  "1011011110110110111101101",
  "1000000000000000000000001",
  "1111111111110111111111111"
};

bool win = false;
bool gameover = false;
int vida = 3;
int pontua = 0;
int tempo;

Texture texture;
Sprite sprite;
int posx = ROWS / 2;
int posy = COLS / 2;

bool cima = false;
bool baixo = false;
bool esq = false;
bool dir = false;

bool movimentoup = false;
bool movimentodown = false;
bool movimentoleft = false;
bool movimentoright = false;

float ghost1x = 1;
float ghost1y = 1;
int ghost1_dir_x = 1;
int ghost1_dir_y = 0;
int ghost1_ant = 0;

int ghost2x = 23;
int ghost2y = 1;

int ghost3x = 1;
int ghost3y = 23;

int ghost4x = 23;
int ghost4y = 23;

Texture texture1;
Sprite sprite1;

Texture texture2;
Sprite sprite2;

Texture texture3;
Sprite sprite3;

Texture texture4;
Sprite sprite4;

Texture fruta;
Sprite sprite_fruta;
int numfrutas = 5;

Texture energetico;
Sprite sprite_energetico;
bool temEnergetico = true;
bool efeitoEnergetico = false;
Clock relogioEnergetico;

bool tela_morte = false;
Texture texture_tela_morte;
Sprite sprite_tela_morte;
Text text_restart;
Text text_exit;
Text text_pontuacao;  

Text placar;

Font fonte;

void stop_move() {
    cima = false;
    baixo = false;
    esq = false;
    dir = false;
    movimentoup = false;
    movimentodown = false;
    movimentoleft = false;
    movimentoright = false;
    if (!texture.loadFromFile("imagens_trab3/pacman.png")) {
        cout << "Erro lendo imagem imagens_trab3/pacman.png\n";
    }
}



void morrer() {
    if (vida > 0) {  
        vida--;
        posx = ROWS / 2;
        posy = COLS / 2;
    }
    
    if (vida <= 0) {
        tela_morte = true;
        stop_move();
        text_pontuacao.setString("Pontuacao: " + to_string(pontua));
        FloatRect limite_telapontuacao = text_pontuacao.getLocalBounds();
        text_pontuacao.setOrigin(limite_telapontuacao.width/2, limite_telapontuacao.height/2);
    }
}

bool check_boundaries(int y, int x) {
    int next_posy = posy + y;
    int next_posx = posx + x;

    if (posy == 11 || posy == 13) {
        if (next_posx < 0) { 
            posx = COLS - 1;
            return true; 
        }
        if (next_posx >= COLS) { 
            posx = 0;
            return true; 
        }
    }

    if (posx == 12) {
        if (next_posy < 0) { 
            posy = COLS - 1;
            return true; 
        }
        if (next_posy >= COLS) { 
            posy = 0;
            return true; 
        }
    }

    if (mapa[next_posy][next_posx] == '1') {
        return false; 
    }

    return true;
}

bool check_boundaries_ghosts(int ghost_y, int ghost_x, int y, int x) {
    int next_posgy = ghost_y + y;
    int next_posgx = ghost_x + x;

    if (next_posgy < 0 || next_posgx >= COLS || next_posgy < 0 || next_posgy >= ROWS || mapa[next_posgy][next_posgx] == '1') {
        return false; 
    }

    return true;
}

void posiciona_frutas(char mapa[ROWS][COLS+1], int numFrutas) {
    srand(time(0));
    int fruitsPlaced = 0;

    while (fruitsPlaced < numFrutas) {
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (mapa[row][col] == '0' || mapa[row][col] == '*') {
            mapa[row][col] = 'f';
            fruitsPlaced++;
        }
    }
}

void posiciona_energetico(char mapa[ROWS][COLS + 1]) {
    srand(time(0));

    while (true) {
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (mapa[row][col] == '0') {
            mapa[row][col] = 'e';
            break;
        }
    }
}

void reinicia() {
    vida = 3;
    pontua = 0;
    posx = ROWS / 2; 
    posy = COLS / 2; 
    memcpy(mapa, map2, sizeof(mapa)); 
    stop_move(); 
    ghost1x = 1;
    ghost1y = 1;
    ghost2x = 23;
    ghost2y = 1;
    ghost3x = 1;
    ghost3y = 23;
    ghost4x = 23;
    ghost4y = 23;
    win = false;
    gameover = false;
    tela_morte = false;
    posiciona_frutas(mapa, 5);
    posiciona_energetico(mapa);
}

int main() {
    posiciona_frutas(mapa, numfrutas);
    posiciona_energetico(mapa);

    Clock clock;
    Clock clock_ghosts;
    RenderWindow window(VideoMode(COLS * SIZE, (ROWS + 2) * SIZE), "Pac-Man");

    RectangleShape rectangle(Vector2f(SIZE, SIZE));
    rectangle.setFillColor(Color(22, 22, 165));
    rectangle.setOutlineThickness(-1);
    rectangle.setOutlineColor(Color(15, 15, 117));

    if (!texture.loadFromFile("imagens_trab3/pacman.png")) {
        cout << "Erro lendo imagem imagens_trab3/pacman.png\n";
        return 0;
    }
    sprite.setTexture(texture);

    if (!texture1.loadFromFile("imagens_trab3/ghostbd.png")) {
        cout << "Erro lendo imagem imagens_trab3/ghostbd.png\n";
        return 0;
    }
    sprite1.setTexture(texture1);

    if (!texture2.loadFromFile("imagens_trab3/ghostge.png")) {
        cout << "Erro lendo imagem imagens_trab3/ghostbd.png\n";
        return 0;
    }
    sprite2.setTexture(texture2);

    if (!texture3.loadFromFile("imagens_trab3/ghostrd.png")) {
        cout << "Erro lendo imagem imagens_trab3/ghostbd.png\n";
        return 0;
    }
    sprite3.setTexture(texture3);

    if (!texture4.loadFromFile("imagens_trab3/ghostye.png")) {
        cout << "Erro lendo imagem imagens_trab3/ghostbd.png\n";
        return 0;
    }
    sprite4.setTexture(texture4);
    
    if (!energetico.loadFromFile("imagens_trab3/energy-drink.png")) {
        cout << "Erro lendo imagem imagens_trab3/energetico.png\n";
        return 0;
    }
    sprite_energetico.setTexture(energetico);

    if (!fruta.loadFromFile("imagens_trab3/pizza.png")) {
        cout << "Erro lendo imagem imagens_trab3/pizza.png\n";
        return 0;
    }
    sprite_fruta.setTexture(fruta);
    
    if (!texture_tela_morte.loadFromFile("imagens_trab3/tela_demorte.png")) {
        cout << "Erro ao carregar da tela de morte.png\n";
        return 0;
    }
    sprite_tela_morte.setTexture(texture_tela_morte);
    sprite_tela_morte.setPosition(COLS*SIZE/2 - texture_tela_morte.getSize().x/2, 
                                ROWS*SIZE/2 - texture_tela_morte.getSize().y/2);

    if (!fonte.loadFromFile("ARCADEPI.TTF")) {
        cout << "Erro carregando fonte\n";
        return 0;
    }

    text_restart.setFont(fonte);
    text_restart.setString("Continuar? (Aperte R)");
    text_restart.setCharacterSize(32);
    text_restart.setFillColor(Color::White);
    text_restart.setPosition(COLS*SIZE/2 - text_restart.getLocalBounds().width/2, 
                            ROWS*SIZE/2 + 100);

    text_exit.setFont(fonte);
    text_exit.setString("Sair? (Aperte ESC)");
    text_exit.setCharacterSize(32);
    text_exit.setFillColor(Color::White);
    text_exit.setPosition(COLS*SIZE/2 - text_exit.getLocalBounds().width/2, 
                         ROWS*SIZE/2 + 150);
                         
    
    placar.setFont(fonte);
    placar.setCharacterSize(24);
    placar.setFillColor(Color::Yellow);
    placar.setPosition(20, ROWS * SIZE + 3);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (tela_morte) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::R) {
                        reinicia();
                    } else if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                }
            } else {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                    else if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A) {
                        movimentoleft = true;
                        movimentoright = movimentoup = movimentodown = false;
                    } else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {
                        movimentoright = true;
                        movimentoleft = movimentoup = movimentodown = false;
                    } else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        movimentoup = true;
                        movimentoleft = movimentoright = movimentodown = false;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        movimentodown = true;
                        movimentoleft = movimentoright = movimentoup = false;
                    }
                }
            }
        }

        if (!tela_morte) {
            float intervalo = efeitoEnergetico ? 0.1f : 0.2f;
            if (clock.getElapsedTime() > seconds(intervalo)) {
                if (efeitoEnergetico && relogioEnergetico.getElapsedTime() > seconds(5)) {
                    efeitoEnergetico = false;
                }

                clock.restart();
                if (mapa[posy][posx] == '0') {
                    pontua += 10;
                    mapa[posy][posx] = '*';
                }
                else if (mapa[posy][posx] == 'f') {
                    pontua += 50;
                    mapa[posy][posx] = '*';
                    numfrutas--;
                }
                else if (mapa[posy][posx] == 'e') {
                    efeitoEnergetico = true;
                    relogioEnergetico.restart();
                    mapa[posy][posx] = '*';
                }

                if (movimentoup && check_boundaries(-1, 0)) {
                    cima = true;
                    baixo = esq = dir = false;
                    texture.loadFromFile("imagens_trab3/pacman-up.png");
                } else if (movimentodown && check_boundaries(1, 0)) {
                    baixo = true;
                    cima = esq = dir = false;
                    texture.loadFromFile("imagens_trab3/pacman-down.png");
                } else if (movimentoleft && check_boundaries(0, -1)) {
                    esq = true;
                    cima = baixo = dir = false;
                    texture.loadFromFile("imagens_trab3/pacman-esq.png");
                } else if (movimentoright && check_boundaries(0, 1)) {
                    dir = true;
                    cima = baixo = esq = false;
                    texture.loadFromFile("imagens_trab3/pacman.png");
                }

                if (cima && check_boundaries(-1, 0))
                    posy--;
                else if (baixo && check_boundaries(1, 0))
                    posy++;
                else if (esq && check_boundaries(0, -1))
                    posx--;
                else if (dir && check_boundaries(0, 1))
                    posx++;
                
                if ((posx == ghost1x && posy == ghost1y) ||
                    (posx == ghost2x && posy == ghost2y) ||
                    (posx == ghost3x && posy == ghost3y) ||
                    (posx == ghost4x && posy == ghost4y)) {
                    morrer();
                }
            }
            
            if (clock_ghosts.getElapsedTime() > seconds(0.3)) { 
                clock_ghosts.restart();

                if (check_boundaries_ghosts(ghost1x, ghost1y, 1, 0)) {
                    ghost1x++;
                }
                if (check_boundaries_ghosts(ghost2x,ghost2y,0,-1))
                    ghost2x++;
            }
        }

        window.clear(Color::Black);

        // Desenha o jogo
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (mapa[i][j] == '1') {
                    rectangle.setPosition(j * SIZE, i * SIZE);
                    window.draw(rectangle);
                } else if (mapa[i][j] == '0' || mapa[i][j] == '*') {
                    CircleShape ponto(SIZE / 8.0f);
                    ponto.setFillColor(mapa[i][j] == '0' ? Color::White : Color(128, 128, 128));
                    ponto.setPosition(j * SIZE + SIZE / 2.5f, i * SIZE + SIZE / 2.5f);
                    window.draw(ponto);
                }
                else if (mapa[i][j] == 'f') {
                    sprite_fruta.setPosition(j * SIZE, i * SIZE);
                    window.draw(sprite_fruta);
                }
                else if (mapa[i][j] == 'e') {
                    sprite_energetico.setPosition(j * SIZE, i * SIZE);
                    window.draw(sprite_energetico);
                }
            }
        }
        
        sprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(sprite);

        sprite1.setPosition(ghost1x * SIZE, ghost1y * SIZE);
        sprite2.setPosition(ghost2x * SIZE, ghost2y * SIZE);
        sprite3.setPosition(ghost3x * SIZE, ghost3y * SIZE);
        sprite4.setPosition(ghost4x * SIZE, ghost4y * SIZE);
        
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
        window.draw(sprite4);

        placar.setString("Pontos: " + to_string(pontua) + "   Vidas: " + to_string(vida));
        window.draw(placar);
        
        if (tela_morte) {
            // Fundo escuro semi-transparente
            RectangleShape overlay(Vector2f(COLS*SIZE, (ROWS+2)*SIZE));
            overlay.setFillColor(Color(0, 0, 0, 180));
            window.draw(overlay);
            
            // Tela de morte
            window.draw(sprite_tela_morte);
            window.draw(text_pontuacao);

            window.draw(text_restart);
            window.draw(text_exit);
        }
        text_pontuacao.setFont(fonte);
        text_pontuacao.setString("Score : " + to_string(pontua)); //pontuação tem acento!
        text_pontuacao.setCharacterSize(28);
        text_pontuacao.setFillColor(Color::Yellow);
        text_pontuacao.setStyle(Text::Bold);

        FloatRect tamanho_telapontuacao = text_pontuacao.getLocalBounds();
        text_pontuacao.setOrigin(tamanho_telapontuacao.width/2, tamanho_telapontuacao.height/2);
        text_pontuacao.setPosition(COLS*SIZE/2, ROWS*SIZE/2 + 50);  // p ficar entre imagem e os botões
        window.display();
    }

    return 0;
}
