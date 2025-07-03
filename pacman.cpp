// g++ pacman_copia.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.out && ./pacman.out
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   implementado
// Intenção de movimento: implementado

#define Pair pair<int, int>
#define pPair pair<double, pair<int, int>>

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
    bool current_up = false;
    bool current_down = false;
    bool current_left = false;
    bool current_right = false;

    bool intention_up = false;
    bool intention_down = false;
    bool intention_left = false;
    bool intention_right = false;

    Texture texture;
    Sprite sprite;
};

struct Ghost {
    Texture texture;
    Sprite sprite;
    int x, y;
    int opposite_direction = -1;
    int last_direction = -1;
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

bool is_valid_cell(int x, int y) { return (x >= 0 && x < COLS && y >= 0 && y < ROWS && mapa[y][x] != '1'); }

double calculate_distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void stop_move() {
    pacman.current_up = pacman.current_down = pacman.current_left = pacman.current_right = false;
    pacman.intention_up = pacman.intention_down = pacman.intention_left = pacman.intention_right = false;
    if (!pacman.texture.loadFromFile("imagens/pacman.png")) {
        cout << "Erro lendo imagem imagens/pacman.png\n";
    }
}

void reposiciona() {
    pacman.x = ROWS / 2;
    pacman.y = COLS / 2;
    ghost[0].x = 1;
    ghost[0].y = 1;
    ghost[1].x = 1;
    ghost[1].y = 23;
    ghost[2].x = 23;
    ghost[2].y = 1;
    ghost[3].x = 23;
    ghost[3].y = 23;

    for (int i = 0; i < 4; i++) {
        ghost[i].opposite_direction = -1;
        ghost[i].last_direction = -1;
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

struct Node {
    int x, y;
    double f, g, h;
    int parent_x, parent_y;

    Node() : x(0), y(0), f(0), g(0), h(0), parent_x(-1), parent_y(-1) {}
    Node(int _x, int _y) : x(_x), y(_y), f(0), g(0), h(0), parent_x(-1), parent_y(-1) {}
};

struct Compare {
    bool operator()(const Node &a, const Node &b) {
        return a.f > b.f; // Min heap based on f value
    }
};

void move_ghost(Ghost &ghost_ref) {
    int valid_directions[4];
    int valid_count = 0;

    if (mapa[ghost_ref.y][ghost_ref.x + 1] != '1' && ghost_ref.opposite_direction != 0) {
        valid_directions[valid_count++] = 0;
    }

    if (mapa[ghost_ref.y + 1][ghost_ref.x] != '1' && ghost_ref.opposite_direction != 1) {
        valid_directions[valid_count++] = 1;
    }

    if (mapa[ghost_ref.y][ghost_ref.x - 1] != '1' && ghost_ref.opposite_direction != 2) {
        valid_directions[valid_count++] = 2;
    }

    if (mapa[ghost_ref.y - 1][ghost_ref.x] != '1' && ghost_ref.opposite_direction != 3) {
        valid_directions[valid_count++] = 3;
    }

    int direction_chosen = valid_directions[rand() % valid_count];

    ghost_ref.opposite_direction = (direction_chosen + 2) % 4;
    ghost_ref.last_direction = direction_chosen;

    if (direction_chosen == 0) {
        ghost_ref.x++;
    } else if (direction_chosen == 1) {
        ghost_ref.y++;
    } else if (direction_chosen == 2) {
        ghost_ref.x--;
    } else if (direction_chosen == 3) {
        ghost_ref.y--;
    }

    if (ghost_ref.x < 0)
        ghost_ref.x = COLS - 1;
    if (ghost_ref.x >= COLS)
        ghost_ref.x = 0;
    if (ghost_ref.y < 0)
        ghost_ref.y = ROWS - 1;
    if (ghost_ref.y >= ROWS)
        ghost_ref.y = 0;
}

vector<pair<int, int>> findPath(int start_x, int start_y, int target_x, int target_y, Ghost ghost_ref) {
    vector<pair<int, int>> path;

    if (!is_valid_cell(target_x, target_y) || (start_x == target_x && start_y == target_y)) {
        return path;
    }

    bool closed_list[ROWS][COLS];
    memset(closed_list, 0, sizeof(closed_list));

    Node cell_details[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cell_details[i][j] = Node(j, i);
            cell_details[i][j].f = FLT_MAX;
            cell_details[i][j].g = FLT_MAX;
            cell_details[i][j].h = FLT_MAX;
        }
    }

    cell_details[start_y][start_x].f = 0.0;
    cell_details[start_y][start_x].g = 0.0;
    cell_details[start_y][start_x].h = 0.0;
    cell_details[start_y][start_x].parent_x = start_x;
    cell_details[start_y][start_x].parent_y = start_y;

    priority_queue<Node, vector<Node>, Compare> open_list;
    open_list.push(cell_details[start_y][start_x]);

    bool found_dest = false;

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!open_list.empty() && !found_dest) {
        Node current = open_list.top();
        open_list.pop();

        int x = current.x;
        int y = current.y;
        closed_list[y][x] = true;

        int opposite_direciton = ghost_ref.opposite_direction;

        for (int dir = 0; dir < 4; dir++) {
            if (x == start_x && y == start_y && dir == opposite_direciton) {
                continue;
            }

            int new_x = x + dx[dir];
            int new_y = y + dy[dir];

            if (is_valid_cell(new_x, new_y)) {
                if (new_x == target_x && new_y == target_y) {
                    cell_details[new_y][new_x].parent_x = x;
                    cell_details[new_y][new_x].parent_y = y;
                    found_dest = true;

                    int path_x = target_x, path_y = target_y;
                    while (!(cell_details[path_y][path_x].parent_x == path_x &&
                             cell_details[path_y][path_x].parent_y == path_y)) {
                        path.push_back({path_x, path_y});
                        int temp_x = cell_details[path_y][path_x].parent_x;
                        int temp_y = cell_details[path_y][path_x].parent_y;
                        path_x = temp_x;
                        path_y = temp_y;
                    }
                    path.push_back({path_x, path_y});
                    reverse(path.begin(), path.end());
                    break;
                } else if (!closed_list[new_y][new_x]) {
                    double g_new = cell_details[y][x].g + 1.0;
                    double h_new = calculate_distance(new_x, new_y, target_x, target_y);
                    double f_new = g_new + h_new;

                    if (cell_details[new_y][new_x].f == FLT_MAX || cell_details[new_y][new_x].f > f_new) {

                        cell_details[new_y][new_x].f = f_new;
                        cell_details[new_y][new_x].g = g_new;
                        cell_details[new_y][new_x].h = h_new;
                        cell_details[new_y][new_x].parent_x = x;
                        cell_details[new_y][new_x].parent_y = y;

                        open_list.push(cell_details[new_y][new_x]);
                    }
                }
            }
        }
    }

    return path;
}

void move_ghost_astar(Ghost &ghost_ref, int target_x, int target_y, bool force_recalc = false) {
    static vector<pair<int, int>> current_path;
    static int path_index = 0;
    static int last_target_x = -1, last_target_y = -1;

    int last_direction = ghost_ref.last_direction;

    if (force_recalc || target_x != last_target_x || target_y != last_target_y || path_index >= current_path.size()) {
        current_path = findPath(ghost_ref.x, ghost_ref.y, target_x, target_y, ghost_ref);
        path_index = 0;
        last_target_x = target_x;
        last_target_y = target_y;
    }

    if (!current_path.empty() && path_index < current_path.size()) {
        if (path_index == 0 && current_path[0].first == ghost_ref.x && current_path[0].second == ghost_ref.y) {
            path_index++;
        }

        if (path_index < current_path.size()) {
            int delta_x = current_path[path_index].first - ghost_ref.x;
            int delta_y = current_path[path_index].second - ghost_ref.y;

            ghost_ref.x = current_path[path_index].first;
            ghost_ref.y = current_path[path_index].second;

            if (delta_x == 1 && delta_y == 0)
                ghost_ref.last_direction = 0;
            else if (delta_x == 0 && delta_y == 1)
                ghost_ref.last_direction = 1;
            else if (delta_x == -1 && delta_y == 0)
                ghost_ref.last_direction = 2;
            else if (delta_x == 0 && delta_y == -1)
                ghost_ref.last_direction = 3;
            ghost_ref.opposite_direction = ((ghost_ref.last_direction + 2) % 4);
            path_index++;
        }
    } else {
        move_ghost(ghost_ref);
    }
}

void reinicia() {
    music.play();
    game_state.life = 3;
    game_state.points = 0;
    game_state.lose = false;
    game_state.win = false;
    memcpy(mapa, mapa_original, sizeof(mapa));
    posiciona_frutas(mapa);
    posiciona_boost(mapa);
    stop_move();
    reposiciona();
    move_ghost_astar(ghost[2], pacman.x, pacman.y, true);
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
                        pacman.intention_left = true;
                        pacman.intention_right = pacman.intention_up = pacman.intention_down = false;
                    } else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {
                        pacman.intention_right = true;
                        pacman.intention_left = pacman.intention_up = pacman.intention_down = false;
                    } else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        pacman.intention_up = true;
                        pacman.intention_left = pacman.intention_right = pacman.intention_down = false;
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        pacman.intention_down = true;
                        pacman.intention_left = pacman.intention_right = pacman.intention_up = false;
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

                    if (pacman.intention_up && check_boundaries(-1, 0)) {
                        pacman.intention_up = false;
                        pacman.current_up = true;
                        pacman.current_down = pacman.current_left = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-up.png");
                    } else if (pacman.intention_down && check_boundaries(1, 0)) {
                        pacman.intention_down = false;
                        pacman.current_down = true;
                        pacman.current_up = pacman.current_left = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-down.png");
                    } else if (pacman.intention_left && check_boundaries(0, -1)) {
                        pacman.intention_left = false;
                        pacman.current_left = true;
                        pacman.current_up = pacman.current_down = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-esq.png");
                    } else if (pacman.intention_right && check_boundaries(0, 1)) {
                        pacman.intention_right = false;
                        pacman.current_right = true;
                        pacman.current_up = pacman.current_down = pacman.current_left = false;
                        pacman.texture.loadFromFile("imagens/pacman.png");
                    }

                    if (pacman.current_up && check_boundaries(-1, 0))
                        pacman.y--;
                    else if (pacman.current_down && check_boundaries(1, 0))
                        pacman.y++;
                    else if (pacman.current_left && check_boundaries(0, -1))
                        pacman.x--;
                    else if (pacman.current_right && check_boundaries(0, 1))
                        pacman.x++;

                    if (verifica_morte()) {
                        morrer();
                    }
                    relo.restart();
                }

                if (clock_ghosts.getElapsedTime() > seconds(GHOST_SPEED)) {
                    move_ghost(ghost[0]);
                    move_ghost(ghost[1]);
                    move_ghost_astar(ghost[2], pacman.x, pacman.y);
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
