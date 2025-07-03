// g++ pacman_copia.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.out && ./pacman.out
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>


// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   implementado
// Intenção de movimento: implementado

#define Pair pair<int,int>
#define pPair pair<double,pair<int,int>> 

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
const int TOTAL_POINTS = 3340;

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

char mapa[ROWS][COLS + 1];

struct Pacman {
    int x = ROWS / 2;
    int y = COLS / 2;
    bool currentUp = false;
    bool currentDown = false;
    bool currentLeft = false;
    bool currentRight = false;

    bool intentionUp = false;
    bool intentionDown = false;
    bool intentionLeft = false;
    bool intentionRight = false;

    Texture texture;
    Sprite sprite;
};

struct Ghost {
    Texture texture;
    Sprite sprite;
    int x, y;
    int opposite_direction = -1;
};

struct Game_State {
    int life = 3;
    int points = 0;
    bool start = true;
    bool win = false;
    bool lose = false;
    bool boost = false;
};
Pacman pacman;
Ghost ghost[4];
Game_State game_state;

Music music;
Music death_sound;
Music game_over;
Music choice;
Music win;
Texture texture_fruta;
Sprite sprite_fruta;
Texture texture_boost;
Sprite sprite_boost;
Texture texture_start;
Sprite sprite_start;
Texture texture_win;
Sprite sprite_win;
Texture texture_lose;
Sprite sprite_lose;
Text text_restart;
Text text_exit;
Text placar;
Font fonte;

Clock clock_boost;
Clock relo;
Clock clock_ghosts;

void stop_move() {
    pacman.currentUp = pacman.currentDown = pacman.currentLeft = pacman.currentRight = false;
    pacman.intentionUp = pacman.intentionDown = pacman.intentionLeft = pacman.intentionRight = false;
    if (!pacman.texture.loadFromFile("imagens/pacman.png")) {
        cout << "Erro lendo imagem imagens/pacman.png\n";
    }
}

void reposiciona() {
    pacman.x = ROWS / 2;
    pacman.y = COLS / 2;
    ghost[0].x = 1; ghost[0].y = 1;
    ghost[1].x = 1; ghost[1].y = 23;
    ghost[2].x = 23; ghost[2].y = 1;
    ghost[3].x = 23; ghost[3].y = 23;

    for (int i = 0; i < 4; i++) {
        ghost[i].opposite_direction = -1;
    }
}

void morrer() {
    death_sound.play();
    game_state.life--;
    game_state.boost = false;
    if (game_state.life <= 0) {
        game_state.lose = true;
        music.stop();
        game_over.play();
    }

    reposiciona();
    stop_move();
    return;
}

void posiciona_frutas(char mapa[ROWS][COLS + 1]) {
    srand(time(0));
    int fruits_placed = 0;

    while (fruits_placed < MAX_FRUITS) {
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (mapa[row][col] == '0' || mapa[row][col] == '*') {
            mapa[row][col] = 'f';
            fruits_placed++;
        }
    }
}

void posiciona_boost(char mapa[ROWS][COLS + 1]) {
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
    game_state.life = 3;
    game_state.points = 0;
    game_state.lose = false;
    game_state.win = false;
    posiciona_frutas(mapa);
    posiciona_boost(mapa);

    memcpy(mapa, mapa_original, sizeof(mapa));
    stop_move();
    reposiciona();
}

bool check_boundaries(int y, int x) {
    if (pacman.y + y < 0) {
        pacman.y = ROWS;
        return 1;
    }
    if (pacman.y + y >= ROWS) {
        pacman.y = -1;
        return 1;
    }
    if (pacman.x + x < 0) {
        pacman.x = COLS;
        return 1;
    }
    if (pacman.x + x >= COLS) {
        pacman.x = -1;
        return 1;
    }
    return (mapa[pacman.y + y][pacman.x + x] != '1');
}

int calcula_distancia(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }

void move_ghost_persegue(Ghost temp, int target_x, int target_y) {
    int direction_chose = -1;
    int min_dist = 1000000000;

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    for (int dir = 0; dir < 4; dir++) {
        if (dir == temp.opposite_direction)
            continue;

        int new_x = temp.x + dx[dir];
        int new_y = temp.y + dy[dir];

        if (new_x < 0)
            new_x = COLS - 1;
        if (new_x >= COLS)
            new_x = 0;
        if (new_y < 0)
            new_y = ROWS - 1;
        if (new_y >= ROWS)
            new_y = 0;

        if (mapa[new_y][new_x] != '1') {
            int distance = calcula_distancia(new_x, new_y, target_x, target_y);
            if (distance < min_dist) {
                min_dist = distance;
                direction_chose = dir;
            }
        }
    }

    if (direction_chose != -1) {
        temp.opposite_direction = (direction_chose + 2) % 4;
        temp.x += dx[direction_chose];
        temp.y += dy[direction_chose];

        if (temp.x < 0)
            temp.x = COLS - 1;
        if (temp.x >= COLS)
            temp.x = 0;
        if (temp.y < 0)
            temp.y = ROWS - 1;
        if (temp.y >= ROWS)
            temp.y = 0;
    }
}

void move_ghost(Ghost& temp) {

    int valid_directions[4];
    int valid_count = 0;

    if (mapa[temp.y][temp.x + 1] != '1' && temp.opposite_direction != 0) {
        valid_directions[valid_count++] = 0;
    }

    if (mapa[temp.y + 1][temp.x] != '1' && temp.opposite_direction != 1) {
        valid_directions[valid_count++] = 1;
    }

    if (mapa[temp.y][temp.x - 1] != '1' && temp.opposite_direction != 2) {
        valid_directions[valid_count++] = 2;
    }

    if (mapa[temp.y - 1][temp.x] != '1' && temp.opposite_direction != 3) {
        valid_directions[valid_count++] = 3;
    }

    int direction_chose = valid_directions[rand() % valid_count];

    temp.opposite_direction = (direction_chose + 2) % 4;

    if (direction_chose == 0) {
        temp.x++;
    } else if (direction_chose == 1) {
        temp.y++;
    } else if (direction_chose == 2) {
        temp.x--;
    } else if (direction_chose == 3) {
        temp.y--;
    }

    if (temp.x < 0)
        temp.x = COLS - 1;
    if (temp.x >= COLS)
        temp.x = 0;
    if (temp.y < 0)
        temp.y = ROWS - 1;
    if (temp.y >= ROWS)
        temp.y = 0;
}

void verificar_vitoria() {
    if (game_state.points >= TOTAL_POINTS) {
        game_state.win = true;
        music.stop();
        win.play();
    }
}

bool verifica_morte() {
    for (int i = 0; i < 4; i++) {
        if (pacman.x == ghost[i].x && pacman.y == ghost[i].y) {
            return 1;
        }
    }
    return 0;
}

int main() {
    reposiciona();
    // Copia mapa
    memcpy(mapa, mapa_original, sizeof(mapa_original));

    //////////////////////////////////////
    // SETUP SONS
    music.openFromFile("sons/music.mp3");
    death_sound.openFromFile("sons/death.mp3");
    game_over.openFromFile("sons/game_over.mp3");
    choice.openFromFile("sons/choice.mp3");
    win.openFromFile("sons/win.wav");
    music.setVolume(40);
    game_over.setVolume(50);
    music.setLoop(true);

    // Use para testar sons
    //  if (!************.openFromFile("sons/**********")) {
    //      cout << "Erro ao carregar o som\n";
    //  }

    ////////////////////////////////////;

    // posiciona frutas e boost
    posiciona_frutas(mapa);
    posiciona_boost(mapa);

    // Cria window
    RenderWindow window(VideoMode(COLS * SIZE, (ROWS + 2) * SIZE), "Pac-Man");

    // Setup blocos matriz
    RectangleShape rectangle(Vector2f(SIZE, SIZE));
    rectangle.setFillColor(Color(22, 22, 165));
    rectangle.setOutlineThickness(-1);
    rectangle.setOutlineColor(Color(15, 15, 117));

    ///////////////////////////////////////////////////
    // SETUP TEXTURA E SPRITE GHOSTS, PACMAN, TELAS, BOOST
    texture_win.loadFromFile("imagens/start.png");
    sprite_win.setTexture(texture_win);

    pacman.texture.loadFromFile("imagens/pacman.png");
    pacman.sprite.setTexture(pacman.texture);

    ghost[0].texture.loadFromFile("imagens/ghostbd.png");
    ghost[0].sprite.setTexture(ghost[0].texture);

    ghost[1].texture.loadFromFile("imagens/ghostge.png");
    ghost[1].sprite.setTexture(ghost[1].texture);

    ghost[2].texture.loadFromFile("imagens/ghostrd.png");
    ghost[2].sprite.setTexture(ghost[2].texture);

    ghost[3].texture.loadFromFile("imagens/ghostye.png");
    ghost[3].sprite.setTexture(ghost[3].texture);

    texture_boost.loadFromFile("imagens/energy-drink.png");
    sprite_boost.setTexture(texture_boost);

    texture_fruta.loadFromFile("imagens/pizza.png");
    sprite_fruta.setTexture(texture_fruta);

    texture_lose.loadFromFile("imagens/tela_demorte.png");
    sprite_lose.setTexture(texture_lose);

    texture_start.loadFromFile("imagens/start.png");
    sprite_start.setTexture(texture_start);

    // Use para testar imagens
    //  if (!************.loadFromFile("imagens/********** */.png")) {
    //      cout << "Erro ao carregar o png\n";
    //  }

    /////////////////////////////////////////////////

    // setup fonte
    fonte.loadFromFile("ARCADEPI.TTF");

    // setup placar
    placar.setFont(fonte);
    placar.setCharacterSize(24);
    placar.setFillColor(Color::Yellow);
    placar.setPosition(20, ROWS * SIZE + 3);

    // setup texto quando ganhar ou perder
    text_restart.setFont(fonte);
    text_restart.setCharacterSize(32);
    text_restart.setFillColor(Color::White);
    text_restart.setPosition(100, 400);
    text_exit.setFont(fonte);
    text_exit.setCharacterSize(32);
    text_exit.setFillColor(Color::White);
    text_exit.setPosition(120, 450);
    int aux = 1;
    while (window.isOpen()) {
        
        Event event;

        if (game_state.start) {
            
            while (window.pollEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        choice.play();
                        texture_start.loadFromFile("imagens/start.png");
                        aux = 1;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        choice.play();
                        texture_start.loadFromFile("imagens/start_2.png");
                        aux = 2;
                    } else if (event.key.code == Keyboard::Enter) {
                        if (aux == 1) {
                            choice.play();
                            music.play();
                            game_state.start = false;
                        } else {
                            window.close();
                            return 0;
                        }
                    }
                }
            }

        } else if (game_state.lose || game_state.win)
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
                        pacman.intentionLeft = true;
                        pacman.intentionRight = pacman.intentionUp = pacman.intentionDown = false;
                    } else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {
                        pacman.intentionRight = true;
                        pacman.intentionLeft = pacman.intentionUp = pacman.intentionDown = false;
                    } else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        pacman.intentionUp = true;
                        pacman.intentionLeft = pacman.intentionRight = pacman.intentionDown = false;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        pacman.intentionDown = true;
                        pacman.intentionLeft = pacman.intentionRight = pacman.intentionUp = false;
                    } // else if (event.key.code == Keyboard::K)
                    //     morrer();
                }
            }
            if (!game_state.lose || !game_state.win) {

                float intervalo = game_state.boost ? PACMAN_FAST_SPEED : PACMAN_SPEED;
                if (relo.getElapsedTime() > seconds(intervalo)) {
                    if (game_state.boost && clock_boost.getElapsedTime() > seconds(ENERGY_DURATION)) {
                        game_state.boost = false;
                    }

                    if (mapa[pacman.y][pacman.x] == '0') {
                        game_state.points += POINT_VALUE;
                        mapa[pacman.y][pacman.x] = '*';
                    } else if (mapa[pacman.y][pacman.x] == 'f') {
                        game_state.points += FRUIT_VALUE;
                        mapa[pacman.y][pacman.x] = '*';
                    } else if (mapa[pacman.y][pacman.x] == 'e') {
                        game_state.boost = true;
                        clock_boost.restart();
                        mapa[pacman.y][pacman.x] = '*';
                    }

                    verificar_vitoria();

                    if (pacman.intentionUp && check_boundaries(-1, 0)) {
                        pacman.intentionUp = false;
                        pacman.currentUp = true;
                        pacman.currentDown = pacman.currentLeft = pacman.currentRight = false;
                        pacman.texture.loadFromFile("imagens/pacman-up.png");
                    } else if (pacman.intentionDown && check_boundaries(1, 0)) {
                        pacman.intentionDown = false;
                        pacman.currentDown = true;
                        pacman.currentUp = pacman.currentLeft = pacman.currentRight = false;
                        pacman.texture.loadFromFile("imagens/pacman-down.png");
                    } else if (pacman.intentionLeft && check_boundaries(0, -1)) {
                        pacman.intentionLeft = false;
                        pacman.currentLeft = true;
                        pacman.currentUp = pacman.currentDown = pacman.currentRight = false;
                        pacman.texture.loadFromFile("imagens/pacman-esq.png");
                    } else if (pacman.intentionRight && check_boundaries(0, 1)) {
                        pacman.intentionRight = false;
                        pacman.currentRight = true;
                        pacman.currentUp = pacman.currentDown = pacman.currentLeft = false;
                        pacman.texture.loadFromFile("imagens/pacman.png");
                    }

                    if (pacman.currentUp && check_boundaries(-1, 0))
                        pacman.y--;
                    else if (pacman.currentDown && check_boundaries(1, 0))
                        pacman.y++;
                    else if (pacman.currentLeft && check_boundaries(0, -1))
                        pacman.x--;
                    else if (pacman.currentRight && check_boundaries(0, 1))
                        pacman.x++;

                    if (verifica_morte()) {
                        morrer();
                    }
                    relo.restart();
                }

                if (clock_ghosts.getElapsedTime() > seconds(GHOST_SPEED)) {
                    move_ghost(ghost[0]);
                    move_ghost(ghost[1]);
                    move_ghost(ghost[2]);
                    move_ghost(ghost[3]);
                   
                    clock_ghosts.restart();
                    if (verifica_morte()) {
                        morrer();
                    }
                }
            }
        }

        window.clear(Color::Black);

        // Desenha o jogo
        if (game_state.start) {
            sprite_start.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(sprite_start);
        } else {

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (mapa[i][j] == '1') {
                        rectangle.setPosition(j * SIZE, i * SIZE);
                        rectangle.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(rectangle);
                    } else if (mapa[i][j] == '0' || mapa[i][j] == '*') {
                        CircleShape ponto(SIZE / 8.0f);
                        ponto.setFillColor(mapa[i][j] == '0' ? Color::White : Color(128, 128, 128));
                        ponto.setPosition(j * SIZE + SIZE / 2.5f, i * SIZE + SIZE / 2.5f);
                        ponto.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(ponto);
                    } else if (mapa[i][j] == 'f') {
                        sprite_fruta.setPosition(j * SIZE, i * SIZE);
                        sprite_fruta.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(sprite_fruta);
                    } else if (mapa[i][j] == 'e') {
                        sprite_boost.setPosition(j * SIZE, i * SIZE);
                        sprite_boost.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(sprite_boost);
                    }
                }
            }

            pacman.sprite.setPosition(pacman.x * SIZE, pacman.y * SIZE);
            pacman.sprite.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(pacman.sprite);

            for (int i = 0; i < 4; i++) {
                ghost[i].sprite.setPosition(ghost[i].x * SIZE, ghost[i].y * SIZE);
            }

            ghost[0].sprite.setScale(SIZE / 25.0, SIZE / 25.0);
            ghost[1].sprite.setScale(SIZE / 25.0, SIZE / 25.0);
            ghost[2].sprite.setScale(SIZE / 25.0, SIZE / 25.0);
            ghost[3].sprite.setScale(SIZE / 25.0, SIZE / 25.0);

            window.draw(ghost[0].sprite);
            window.draw(ghost[1].sprite);
            window.draw(ghost[2].sprite);
            window.draw(ghost[3].sprite);

            placar.setString("Pontos: " + to_string(game_state.points) + "   Vidas: " + to_string(game_state.life));
            placar.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(placar);

            if (game_state.lose || game_state.win) {

                // Fundo escuro semi-transparente
                RectangleShape overlay(Vector2f(COLS * SIZE, (ROWS + 2) * SIZE));
                overlay.setFillColor(Color(0, 0, 0, 180));
                overlay.setScale(SIZE / 25.0, SIZE / 25.0);
                window.draw(overlay);

                if (game_state.lose) {

                    sprite_lose.setPosition(140, 100);

                    text_restart.setString("Continuar? (Aperte R)");
                    sprite_lose.setScale(SIZE / 25.0, SIZE / 25.0);

                    window.draw(sprite_lose);

                } else {

                    text_restart.setString("Reiniciar? (Aperte R)");
                    sprite_win.setScale(SIZE / 25.0, SIZE / 25.0);
                    window.draw(sprite_win);
                }

                text_exit.setString("Sair? (Aperte ESC)");

                // Tela de morte
                text_restart.setScale(SIZE / 25.0, SIZE / 25.0);
                text_exit.setScale(SIZE / 25.0, SIZE / 25.0);

                window.draw(text_restart);
                window.draw(text_exit);
            }
        }
        window.display();
    }

    return 0;
}
