// g++ pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system
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

char mapa[ROWS][COLS+1] = {         //  MAPA E UMA CÓPIA PARA USAR COM O RESET
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

// IMPLEMENTAR O USO DESSAS VARIÁVEIS
/////////////////////////////////////
bool win = false;      // (atualmente apenas fecha quando morre, talvez uma mensagem)
bool gameover = false; // (atualmente apenas fecha quando morre, talvez uma mensagem)
/////////////////////////////////////

int vida = 3;
int pontua = 0;
int tempo;

// sprite pacman
Texture texture;
Sprite sprite;
// posicao do PacMan
int posx = ROWS / 2;
int posy = COLS / 2;

// a direcao de movimento do PacMan
bool cima = false;
bool baixo = false;
bool esq = false;
bool dir = false;

// a intecao do movimento do PacMan
bool movimentoup = false;
bool movimentodown = false;
bool movimentoleft = false;
bool movimentoright = false;

// posicao inimigos
// ghost 1
float ghost1x = 1;
float ghost1y = 1;
int ghost1_dir_x = 1; // 1: direita, -1: esquerda, 0: sem movimento horizontal
int ghost1_dir_y = 0; // 1: baixo, -1: cima, 0: sem movimento vertical
int ghost1_ant = 0;

// ghost 2
int ghost2x = 23;
int ghost2y = 1;

// ghost 3
int ghost3x = 1;
int ghost3y = 23;


// ghost 4
int ghost4x = 23;
int ghost4y = 23;


// sprite inimigos
// ghost 1
Texture texture1;
Sprite sprite1;

// ghost 2
Texture texture2;
Sprite sprite2;

// ghost 3
Texture texture3;
Sprite sprite3;

// ghost 4
Texture texture4;
Sprite sprite4;

// placar
Text placar;

// fonte
Font fonte;

// parar o moviento em caso de morte ou reset
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
    return;
}

// diminui vida, volta para posição inicial
void morrer() {
    vida--;
    posx = ROWS / 2;
    posy = COLS / 2;

    // Se vidas acabaram, reinicia jogo
    if (vida <= 0) {
        // Reinicia o jogo
        memcpy(mapa, map2, sizeof(mapa));
    }
    stop_move();
    return;
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
}

// Checa se é possível se mover para uma posição
// Se a posção estiver fora do mapa, inverte os indexes
bool check_boundaries(int y, int x) {
    int next_posy = posy + y;
    int next_posx = posx + x;

    if (posy == 11 || posy == 13) {  //tuneis verticais
        if (next_posx < 0) { 
            posx = COLS - 1; // recomeça no lado direito
            return true; 
        }
        if (next_posx >= COLS) { 
            posx = 0; // recomeça no lado esquerdo
            return true; 
        }
    }

    if (posx == 12) {  //tuneis verticais
        if (next_posy < 0) { 
            posy = COLS - 1; // recomeça no lado direito
            return true; 
        }
        if (next_posy >= COLS) { 
            posy = 0; // recomeça no lado esquerdo
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


    if (next_posgy < 0 or next_posgx >= COLS or next_posgy < 0 or next_posgy >= ROWS or mapa[next_posgy][next_posgx] == '1') {
        return false; 
    }

    return true;
}




int main() {
    // cria um relogio para medir o tempo do PacMan
    Clock clock;

    //cria um relogio pra medir o tempo - Fanstasmas
    Clock clock_ghosts;
    // cria a janela
    RenderWindow window(VideoMode(COLS * SIZE, (ROWS + 2) * SIZE), "Pac-Man");

    // shape da parede
    RectangleShape rectangle(Vector2f(SIZE, SIZE));
    rectangle.setFillColor(Color(22, 22, 165));
    rectangle.setOutlineThickness(-1);
    rectangle.setOutlineColor(Color(15, 15, 117));

    // sprite do PacMan
    if (!texture.loadFromFile("imagens_trab3/pacman.png")) {
        cout << "Erro lendo imagem imagens_trab3/pacman.png\n";
        return 0;
    }
    sprite.setTexture(texture);

    // sprite dos Fantasmas
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

    // setup da fonte
    if (!fonte.loadFromFile("ARCADEPI.TTF")) {
        cout << "Erro carregando fonte\n";
        return 0;
    }
    placar.setFont(fonte);
    placar.setCharacterSize(24);
    placar.setFillColor(Color::Yellow);
    placar.setPosition(20, ROWS * SIZE + 3); // parte inferior do mapa

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {
        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        Event event;
        while (window.pollEvent(event)) {

            // evento "fechar" acionado: fecha a janela
            if (event.type == Event::Closed)
                window.close();

            // tecla pressionada

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                }
                // reset do personagem/jogo
                else if (event.key.code == Keyboard::Enter)
                    reinicia();
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
                } else if (event.key.code == Keyboard::K) {
                    morrer();
                }
            }
        }

        // Muda a posição do PacMan a cada 0.2 segundos
        if (clock.getElapsedTime() > seconds(0.2)) { // tempo desde último restart > 0.2s?
            clock.restart();                         // recomeça contagem do tempo
            if (mapa[posy][posx] == '0') {
                pontua += 10;           // soma 10 pontos
                mapa[posy][posx] = '*'; // marca como comido
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
            // Move na direção atual
            if (cima && check_boundaries(-1, 0))
                posy--;
            else if (baixo && check_boundaries(1, 0))
                posy++;
            else if (esq && check_boundaries(0, -1))
                posx--;
            else if (dir && check_boundaries(0, 1))
                posx++;
            
            //Se o pacman bater num fantasma
            if ((posx == ghost1x && posy == ghost1y) ||
                (posx == ghost2x && posy == ghost2y) ||
                (posx == ghost3x && posy == ghost3y) ||
                (posx == ghost4x && posy == ghost4y)) {
                morrer();
                if (vida == 0) {
                    window.close();
                    return 0;
                }
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

      
        // limpa a janela com a cor preta
        window.clear(Color::Black);

        // desenhar tudo aqui...

        // desenha paredes

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (mapa[i][j] == '1') {
                    rectangle.setPosition(j * SIZE, i * SIZE);
                    window.draw(rectangle);
                } else if (mapa[i][j] == '0' || mapa[i][j] == '*') {
                    // Desenha ponto não comido (branco) ou comido (cinza)
                    CircleShape ponto(SIZE / 8.0f);
                    ponto.setFillColor(mapa[i][j] == '0' ? Color::White : Color(128, 128, 128));
                    ponto.setPosition(j * SIZE + SIZE / 2.5f, i * SIZE + SIZE / 2.5f);
                    window.draw(ponto);
                }
        // desenha PacMan
        sprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(sprite);

        // desenha os fanstamas
        sprite1.setPosition(ghost1x * SIZE, ghost1y * SIZE);
        sprite2.setPosition(ghost2x * SIZE, ghost2y * SIZE);
        sprite3.setPosition(1 * SIZE, 23 * SIZE);
        sprite4.setPosition(23 * SIZE, 23 * SIZE);
        // desenha o placar
        placar.setString("Pontos: " + to_string(pontua) + "   Vidas: " + to_string(vida));
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
        window.draw(sprite4);
        window.draw(placar);
        window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
