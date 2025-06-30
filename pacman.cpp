// g++ pacman_copia.cpp -lsfml-graphics -lsfml-window -lsfml-system
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
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
const float PACMAN_SPEED = 0.2f;
const float PACMAN_FAST_SPEED = 0.15f;
const float ENERGY_DURATION = 5.0f;
const float GHOST_SPEED = 0.3f;
const int POINT_VALUE = 10;
const int FRUIT_VALUE = 50;
const int MAX_FRUITS = 5;

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

char mapa_original[ROWS][COLS+1] = {
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

int aux = 1;
int vida = 3;
int pontua = 0;
int total_pontos = 3340;
bool morto = 0;

int posx = ROWS / 2;
int posy = COLS / 2;

bool currentUp = false;
bool currentDown = false;
bool currentLeft = false;
bool currentRight = false;

bool intentionUp = false;
bool intentionDown = false;
bool intentionLeft = false;
bool intentionRight = false;

int ghost1x = 2;
int ghost1y = 1;

int ghost2x = 23;
int ghost2y = 1;

int ghost3x = 1;
int ghost3y = 23;

int ghost4x = 23;
int ghost4y = 23;

int ghostdirection[4][4];
int lastDirection[4] = {-1, -1, -1, -1};

Music music;
Music death_sound;
Music game_over;
Music choice;
Music win;

Texture texture;
Sprite sprite;
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
Texture energetico;
Sprite sprite_energetico;

bool efeitoEnergetico = false;
Clock relogioEnergetico;

bool tela_start = true;
Texture texture_tela_start;
Sprite sprite_tela_start;

bool tela_vitoria = false;
Texture texture_tela_vitoria;
Sprite sprite_tela_vitoria;

bool tela_morte = false;
Texture texture_tela_morte;
Sprite sprite_tela_morte;
Text text_restart;
Text text_exit;
Text placar;
Font fonte;

void stop_move() {
    currentUp = false;
    currentDown = false;
    currentLeft = false;
    currentRight = false;
    intentionUp = false;
    intentionDown = false;
    intentionLeft = false;
    intentionRight = false;
    if (!texture.loadFromFile("imagens/pacman.png")) {
        cout << "Erro lendo imagem imagens/pacman.png\n";
    }
}

void reposiciona(){
    posx = ROWS / 2;
    posy = COLS / 2;
    ghost1x = 1;
    ghost1y = 1;
    ghost2x = 23;
    ghost2y = 1;
    ghost3x = 1;
    ghost3y = 23;
    ghost4x = 23;
    ghost4y = 23;
    for (int i = 0; i < 4; i++) {
        lastDirection[i] = -1;
    }
}



void morrer() {
    death_sound.play();
    vida--;
    morto = 0;
    
    if (vida <= 0) {
        tela_morte = true;
        music.stop();
        game_over.play();
        placar.setString("Pontuacao: " + to_string(pontua));
    }
    
    reposiciona();
    stop_move();
    return;
}

void posiciona_frutas(char mapa[ROWS][COLS+1]) {
    srand(time(0));
    int fruitsPlaced = 0;

    while (fruitsPlaced < MAX_FRUITS) {
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
    music.play();
    vida = 3;
    pontua = 0;
    tela_morte = false;
    tela_vitoria = false;
    posiciona_frutas(mapa);
    posiciona_energetico(mapa);
    
    memcpy(mapa, mapa_original, sizeof(mapa)); 
    stop_move(); 
    reposiciona();
}

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




int calculata_distancia(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void move_ghost_persegue(int& x, int& y, int who, int targetX, int targetY) {
    int bestDirection = -1;
    int minDistance = 1000000000;

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    
    int oppositeDirection = -1;
    if (lastDirection[who] != -1) {
        oppositeDirection = (lastDirection[who] + 2) % 4;
    }
    
    for (int dir = 0; dir < 4; dir++) {
        if (dir == oppositeDirection) continue; 
        
        int newX = x + dx[dir];
        int newY = y + dy[dir];

        if (newX < 0) newX = COLS - 1;
        if (newX >= COLS) newX = 0;
        if (newY < 0) newY = ROWS - 1;
        if (newY >= ROWS) newY = 0;
        
        if (mapa[newY][newX] != '1') {
            int distance = calculata_distancia(newX, newY, targetX, targetY);
            if (distance < minDistance) {
                minDistance = distance;
                bestDirection = dir;
            }
        }
    }
    
    if (bestDirection != -1) {
        lastDirection[who] = bestDirection;
        x += dx[bestDirection];
        y += dy[bestDirection];
        
        // Trata túneis
        if (x < 0) x = COLS - 1;
        if (x >= COLS) x = 0;
        if (y < 0) y = ROWS - 1;
        if (y >= ROWS) y = 0;
    }
}

void move_ghost(int& x, int& y, int who){
    for (int i = 0; i < 4; i++) {
        ghostdirection[who][i] = 0;
    }
    int oppositeDirection = -1;
    if (lastDirection[who] != -1) {
        oppositeDirection = (lastDirection[who] + 2) % 4;
    }
    int validDirections[4];
    int validCount = 0;
    
    if (mapa[y][x + 1] != '1' && oppositeDirection != 0) {
        validDirections[validCount++] = 0;
    } 
    
    if (mapa[y + 1][x] != '1' && oppositeDirection != 1) {
        validDirections[validCount++] = 1;
    } 
    
    if (mapa[y][x - 1] != '1' && oppositeDirection != 2) {
        validDirections[validCount++] = 2;
    } 
    
    
    if (mapa[y - 1][x] != '1' && oppositeDirection != 3) {
        validDirections[validCount++] = 3;
    } 
    
    int chosenDirection = validDirections[rand() % validCount];
    ghostdirection[who][chosenDirection] = 1;

    lastDirection[who] = chosenDirection;
    
    if (ghostdirection[who][0] == 1) {
        x++;
        if (x >= COLS) {
            x = 0;
        }
    }
    else if (ghostdirection[who][1] == 1) {
        y++;
        if (y >= ROWS) {
            y = 0;
        }
    }
    else if (ghostdirection[who][2] == 1) {
        x--;
        if (x < 0) {
            x = COLS - 1;
        }
    }
    else if (ghostdirection[who][3] == 1) {
        y--;
        if (y < 0) {
            y = ROWS - 1;
        }
    }
}

void verificar_vitoria() {
    if (pontua >= total_pontos) {
        tela_vitoria = true;
        music.stop();
        win.play();
    }
}

bool verifica_morte(){
    return (posx == ghost1x && posy == ghost1y) || (posx == ghost2x && posy == ghost2y) ||
            (posx == ghost3x && posy == ghost3y) || (posx == ghost4x && posy == ghost4y);
}

int main() {
    music.openFromFile("sons/music.mp3");
    death_sound.openFromFile("sons/death.mp3");
    game_over.openFromFile("sons/game_over.mp3");
    choice.openFromFile("sons/choice.mp3");
    win.openFromFile("sons/win.wav");

    music.setVolume(40);
    game_over.setVolume(50);
    music.setLoop(true);

    posiciona_frutas(mapa);
    posiciona_energetico(mapa);

    Clock clock;
    Clock clock_ghosts;
    RenderWindow window(VideoMode(COLS * SIZE, (ROWS + 2) * SIZE), "Pac-Man");

    RectangleShape rectangle(Vector2f(SIZE, SIZE));
    rectangle.setFillColor(Color(22, 22, 165));
    rectangle.setOutlineThickness(-1);
    rectangle.setOutlineColor(Color(15, 15, 117));

    texture_tela_start.loadFromFile("imagens/start.png");
    sprite_tela_start.setTexture(texture_tela_start);

    if (!texture.loadFromFile("imagens/pacman.png")) {
        cout << "Erro lendo imagem imagens/pacman.png\n";
        return 0;
    }
    sprite.setTexture(texture);

    if (!texture1.loadFromFile("imagens/ghostbd.png")) {
        cout << "Erro lendo imagem imagens/ghostbd.png\n";
        return 0;
    }
    sprite1.setTexture(texture1);

    if (!texture2.loadFromFile("imagens/ghostge.png")) {
        cout << "Erro lendo imagem imagens/ghostge.png\n";
        return 0;
    }
    sprite2.setTexture(texture2);

    if (!texture3.loadFromFile("imagens/ghostrd.png")) {
        cout << "Erro lendo imagem imagens/ghostrd.png\n";
        return 0;
    }
    sprite3.setTexture(texture3);

    if (!texture4.loadFromFile("imagens/ghostye.png")) {
        cout << "Erro lendo imagem imagens/ghostye.png\n";
        return 0;
    }
    sprite4.setTexture(texture4);

    if (!energetico.loadFromFile("imagens/energy-drink.png")) {
        cout << "Erro lendo imagem imagensenergey-drink.png\n";
        return 0;
    }
    sprite_energetico.setTexture(energetico);

    if (!fruta.loadFromFile("imagens/pizza.png")) {
        cout << "Erro lendo imagem imagens/pizza.png\n";
        return 0;
    }
    sprite_fruta.setTexture(fruta);

    if (!texture_tela_morte.loadFromFile("imagens/tela_demorte.png")) {
        cout << "Erro ao carregar da tela_demorte.png\n";
        return 0;
    }
    sprite_tela_morte.setTexture(texture_tela_morte);

    if (!texture_tela_vitoria.loadFromFile("imagens/win.png")) {
        cout << "Erro ao carregar da win.png\n";
    }
    sprite_tela_vitoria.setTexture(texture_tela_vitoria);

    if (!fonte.loadFromFile("ARCADEPI.TTF")) {
        cout << "Erro carregando fonte\n";
        return 0;
    }
    placar.setFont(fonte);
    placar.setCharacterSize(24);
    placar.setFillColor(Color::Yellow);
    placar.setPosition(20, ROWS * SIZE + 3);

    text_restart.setFont(fonte);
    text_restart.setCharacterSize(32);
    text_restart.setFillColor(Color::White);
    text_restart.setPosition(100, 400);
    text_exit.setFont(fonte);
    text_exit.setCharacterSize(32);
    text_exit.setFillColor(Color::White);
    text_exit.setPosition(120, 450);

    while (window.isOpen()) {
        Event event;

        if (tela_start) {
            while (window.pollEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        choice.play();

                        texture_tela_start.loadFromFile("imagens/start.png");
                        aux = 1;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        choice.play();

                        texture_tela_start.loadFromFile("imagens/start_2.png");
                        aux = 2;
                    } else if (event.key.code == Keyboard::Enter) {
                        if (aux == 1) {
                            choice.play();
                            music.play();
                            tela_start = false;
                        } else {
                            window.close();
                            return 0;
                        }
                    }
                }
            }

        } else if (tela_morte || tela_vitoria)
            while (window.pollEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::R) {
                        choice.play();
                        reinicia();
                    } else if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                }
            }

        else {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                    // reset do personagem/jogo
                    else if (event.key.code == Keyboard::R)
                        reinicia();
                    else if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A) {
                        intentionLeft = true;
                        intentionRight = intentionUp = intentionDown = false;
                    } else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {
                        intentionRight = true;
                        intentionLeft = intentionUp = intentionDown = false;
                    } else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        intentionUp = true;
                        intentionLeft = intentionRight = intentionDown = false;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        intentionDown = true;
                        intentionLeft = intentionRight = intentionUp = false;
                    } // else if (event.key.code == Keyboard::K)
                    //     morrer();
                }
            }
            if (!tela_morte || !tela_start) {

                float intervalo = efeitoEnergetico ? PACMAN_FAST_SPEED : PACMAN_SPEED;
                if (clock.getElapsedTime() > seconds(intervalo)) {
                    if (efeitoEnergetico && relogioEnergetico.getElapsedTime() > seconds(ENERGY_DURATION)) {
                        efeitoEnergetico = false;
                    }

                    if (mapa[posy][posx] == '0') {
                        pontua += POINT_VALUE;
                        mapa[posy][posx] = '*';
                    } else if (mapa[posy][posx] == 'f') {
                        pontua += FRUIT_VALUE;
                        mapa[posy][posx] = '*';
                    } else if (mapa[posy][posx] == 'e') {
                        efeitoEnergetico = true;
                        relogioEnergetico.restart();
                        mapa[posy][posx] = '*';
                    }

                    verificar_vitoria();

                    if (intentionUp && check_boundaries(-1, 0)) {
                        intentionUp = false;
                        currentUp = true;
                        currentDown = currentLeft = currentRight = false;
                        texture.loadFromFile("imagens/pacman-up.png");
                    } else if (intentionDown && check_boundaries(1, 0)) {
                        intentionDown = false;
                        currentDown = true;
                        currentUp = currentLeft = currentRight = false;
                        texture.loadFromFile("imagens/pacman-down.png");
                    } else if (intentionLeft && check_boundaries(0, -1)) {
                        intentionLeft = false;
                        currentLeft = true;
                        currentUp = currentDown = currentRight = false;
                        texture.loadFromFile("imagens/pacman-esq.png");
                    } else if (intentionRight && check_boundaries(0, 1)) {
                        intentionRight = false;
                        currentRight = true;
                        currentUp = currentDown = currentLeft = false;
                        texture.loadFromFile("imagens/pacman.png");
                    }

                    if (currentUp && check_boundaries(-1, 0))
                        posy--;
                    else if (currentDown && check_boundaries(1, 0))
                        posy++;
                    else if (currentLeft && check_boundaries(0, -1))
                        posx--;
                    else if (currentRight && check_boundaries(0, 1))
                        posx++;

                    if (verifica_morte()) {
                        morto = 1;
                    }
                    clock.restart();
                }

                if (clock_ghosts.getElapsedTime() > seconds(GHOST_SPEED)) {
                    move_ghost(ghost1x, ghost1y, 0);
                    move_ghost(ghost2x, ghost2y, 1);
                    move_ghost_persegue(ghost3x, ghost3y, 3, posx, posy);
                    move_ghost_persegue(ghost4x, ghost4y, 3, posx, posy);
                    clock_ghosts.restart();
                    if (verifica_morte()) {
                        morto = 1;
                    }
                }
            }
        }

        window.clear(Color::Black);

        // Desenha o jogo
        if (tela_start) {
            window.draw(sprite_tela_start);
        } else {

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
                    } else if (mapa[i][j] == 'f') {
                        sprite_fruta.setPosition(j * SIZE, i * SIZE);
                        window.draw(sprite_fruta);
                    } else if (mapa[i][j] == 'e') {
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

            if (tela_morte || tela_vitoria) {

                // Fundo escuro semi-transparente
                RectangleShape overlay(Vector2f(COLS * SIZE, (ROWS + 2) * SIZE));
                overlay.setFillColor(Color(0, 0, 0, 180));
                window.draw(overlay);

                if (tela_morte) {

                    sprite_tela_morte.setPosition(140, 100);

                    text_restart.setString("Continuar? (Aperte R)");

                    window.draw(sprite_tela_morte);

                } else {

                    text_restart.setString("Reiniciar? (Aperte R)");

                    window.draw(sprite_tela_vitoria);
                }

                text_exit.setString("Sair? (Aperte ESC)");

                // Tela de morte
                window.draw(text_restart);
                window.draw(text_exit);
            }
        }
        window.display();
        if (morto) {
            morrer();
        }
    }

    return 0;
}
