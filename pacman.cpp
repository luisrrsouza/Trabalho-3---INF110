// g++ pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstring>
#include <iostream>
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
int ghost1x = 1;
int ghost1y = 1;

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
    // aqui reinicia se a  pessoa apertar enter ele volta para o começo do jogo
    //  redesenhando o mapa
    memcpy(mapa, map2, sizeof(mapa));
    posx = COLS / 2;
    posy = ROWS / 2;
    pontua = 0;
    vida = 3;
    stop_move();

    return;
}

// Checa se é possível se mover para uma posição
// Se a posção estiver fora do mapa, inverte os indexes
bool check_boundaries(int y, int x) {
    if (posy + y < 0) {
        posy = ROWS;
        return 1;
    }
    if (posy + y >= ROWS) {
        posy = -1;
        return 1;
    }
    if (posx + x < 0) {
        posx = COLS;
        return 1;
    }
    if (posx + x >= COLS) {
        posx = -1;
        return 1;
    }
    return (mapa[posy + y][posx + x] != '1');
}

int main() {
    // cria um relogio para medir o tempo do PacMan
    Clock clock;

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
                movimentoup = false;
                cima = true;
                baixo = esq = dir = false;
                texture.loadFromFile("imagens_trab3/pacman-up.png");
            } else if (movimentodown && check_boundaries(1, 0)) {
                movimentodown = false;
                baixo = true;
                cima = esq = dir = false;
                texture.loadFromFile("imagens_trab3/pacman-down.png");
            } else if (movimentoleft && check_boundaries(0, -1)) {
                movimentoleft = false;
                esq = true;
                cima = baixo = dir = false;
                texture.loadFromFile("imagens_trab3/pacman-esq.png");
            } else if (movimentoright && check_boundaries(0, 1)) {
                movimentoright = false;
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
            if ((posx == ghost1x && posy == ghost1y) || (posx == ghost2x && posy == ghost2y) ||(posx == ghost3x && posy == ghost3y) ||(posx == ghost4x && posy == ghost4y) ) {
                //ainda falta implementar o que acontece quando o fantas e o pacman vão em direções contrárias
                morrer();
                if (vida == 0) {
                    window.close();
                    return 0;
                }
            }
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
        sprite3.setPosition(ghost3x * SIZE, ghost3y * SIZE);
        sprite4.setPosition(ghost4x * SIZE, ghost4y * SIZE);
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
