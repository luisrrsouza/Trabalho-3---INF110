// g++ pacman_copia.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.out && ./pacman.out
// https://github.com/luisrrsouza/Trabalho-3---INF110
// https://docs.google.com/presentation/d/1cCzdtlDNhkRqIwMMGLtdGYEfC60Vs4DZoHIObHdFGsk/edit?usp=sharing
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

// Função para validar se uma célula está dentro dos limites do mapa e não é parede
bool is_valid_map_cell(int x, int y) { return (x >= 0 && x < COLS && y >= 0 && y < ROWS && mapa[y][x] != '1'); }

// Função para calcular distância euclidiana entre dois pontos (usada como heurística no A*)
double calculate_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs (y1 - y2);
}

// Para todas as direções do movimento do Pac-Man e resetar sprite para padrão
void stop_move() {
    pacman.current_up = pacman.current_down = pacman.current_left = pacman.current_right = false;
    pacman.intention_up = pacman.intention_down = pacman.intention_left = pacman.intention_right = false;
    if (!pacman.texture.loadFromFile("imagens/pacman.png")) {
        cout << "Erro lendo imagem imagens/pacman.png\n";
    }
}

// Reposiciona Pac-Man e fantasmas para posições iniciais
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

// Executa quando Pac-Man morre: toca som, remove vida, verifica game over
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

// Posiciona frutas aleatoriamente no mapa em células vazias
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

// Posiciona power pellet (energético) aleatoriamente no mapa
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

// Verifica limites do mapa e permite teleporte através das bordas
bool check_pacman_move(int y, int x) {
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

// Verifica se o jogador coletou todos os pontos para ganhar
void verificar_vitoria() {
    if (game_state.points >= TOTAL_POINTS) {
        game_state.win = true;
        music.stop();
        win.play();
    }
}

// Verifica se algum fantasma colidiu com o Pac-Man
bool verifica_morte() {
    for (int i = 0; i < 4; i++) {
        if (pacman.x == ghost[i].x && pacman.y == ghost[i].y) {
            return 1;
        }
    }
    return 0;
}

// Estrutura para representar um nó no algoritmo A*
struct Node {
    int x, y;
    double f, g, h;
    int parent_x, parent_y;

    Node() : x(0), y(0), f(0), g(0), h(0), parent_x(-1), parent_y(-1) {}
    Node(int _x, int _y) : x(_x), y(_y), f(0), g(0), h(0), parent_x(-1), parent_y(-1) {}
};

// Comparador para priority queue (min-heap baseado no custo f)
struct Compare {
    bool operator()(const Node &a, const Node &b) {
        return a.f > b.f; // compara o valor de f (g+h)
    }
};

// Movimento aleatório inteligente para fantasmas normais (evita reversão)
void move_ghost(Ghost &ghost_ref) {
    // decla variáveis necessárias
    int valid_directions[4];
    int valid_count = 0;

    // Verifica direções válidas (não bloqueadas e não opostas)
    if (mapa[ghost_ref.y][ghost_ref.x + 1] != '1' && ghost_ref.opposite_direction != 0) {
        valid_directions[valid_count++] = 0; // Direita
    }

    if (mapa[ghost_ref.y + 1][ghost_ref.x] != '1' && ghost_ref.opposite_direction != 1) {
        valid_directions[valid_count++] = 1; // Baixo
    }

    if (mapa[ghost_ref.y][ghost_ref.x - 1] != '1' && ghost_ref.opposite_direction != 2) {
        valid_directions[valid_count++] = 2; // Esquerda
    }

    if (mapa[ghost_ref.y - 1][ghost_ref.x] != '1' && ghost_ref.opposite_direction != 3) {
        valid_directions[valid_count++] = 3; // Cima
    }

    // Escolhe direção aleatória entre as válidas
    int direction_chosen = valid_directions[rand() % valid_count];

    // Atualiza direções para próximo movimento
    ghost_ref.opposite_direction = (direction_chosen + 2) % 4;
    ghost_ref.last_direction = direction_chosen;

    // Move fantasma na direção escolhida
    if (direction_chosen == 0) {
        ghost_ref.x++;
    } else if (direction_chosen == 1) {
        ghost_ref.y++;
    } else if (direction_chosen == 2) {
        ghost_ref.x--;
    } else if (direction_chosen == 3) {
        ghost_ref.y--;
    }

    // Teleporte através das bordas (túneis)
    if (ghost_ref.x < 0)
        ghost_ref.x = COLS - 1;
    if (ghost_ref.x >= COLS)
        ghost_ref.x = 0;
    if (ghost_ref.y < 0)
        ghost_ref.y = ROWS - 1;
    if (ghost_ref.y >= ROWS)
        ghost_ref.y = 0;
}

// Implementação do algoritmo A* para encontrar o caminho mais curto
vector<pair<int, int>> finda_path(int start_x, int start_y, int target_x, int target_y, Ghost ghost_ref) {
    vector<pair<int, int>> path;

    // Verifica se o alvo é válido ou se já está na posição
    if (!is_valid_map_cell(target_x, target_y) || (start_x == target_x && start_y == target_y)) {
        return path;
    }

    // Lista de células já processadas
    bool closed_list[ROWS][COLS];
    memset(closed_list, 0, sizeof(closed_list));

    // Array para armazenar detalhes de cada célula
    Node cell_details[ROWS][COLS];

    // Inicializa todas as células com valores infinitos
    // exceto a posição de ghost que é 0 na prática 
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cell_details[i][j] = Node(j, i);
            cell_details[i][j].f = FLT_MAX;
            cell_details[i][j].g = FLT_MAX;
            cell_details[i][j].h = FLT_MAX;
        }
    }

    // Inicializa a célula de início
    cell_details[start_y][start_x].f = 0.0;
    cell_details[start_y][start_x].g = 0.0;
    cell_details[start_y][start_x].h = 0.0;
    cell_details[start_y][start_x].parent_x = start_x;
    cell_details[start_y][start_x].parent_y = start_y;

    // Priority queue (min-heap) para células a serem processadas
    priority_queue<Node, vector<Node>, Compare> open_list;
    // adiciona a célula da posição iniciaçl
    open_list.push(cell_details[start_y][start_x]);

    bool found_dest = false;

    // Direções: direita, baixo, esquerda, cima
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    // Loop principal do algoritmo A*
    while (!open_list.empty() && !found_dest) {
        Node current = open_list.top();
        open_list.pop();

        int x = current.x;
        int y = current.y;
        // adiciona a célula atual a lista de células calculadas/visitadas
        closed_list[y][x] = true;

        int opposite_direciton = ghost_ref.opposite_direction;

        // Verifica todas as 4 direções
        for (int dir = 0; dir < 4; dir++) {
            // Evita movimento reverso na posição inicial
            if (x == start_x && y == start_y && dir == opposite_direciton) {
                continue;
            }

            // nex_x e new_y para testar se aquela posição é valida e dentro loop testar se ela é "prioritária"
            int new_x = x + dx[dir];
            int new_y = y + dy[dir];

            if (is_valid_map_cell(new_x, new_y)) {
                // Verifica se chegou ao destino
                if (new_x == target_x && new_y == target_y) {
                    cell_details[new_y][new_x].parent_x = x;
                    cell_details[new_y][new_x].parent_y = y;
                    found_dest = true;

                    // Reconstrói o caminho seguindo os pais (retorna a função a* para fazer a mduança de posição)
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
                    // Calcula custos g, h e f
                    // Como não tem peso no gráfico cada andada é dada como 1
                    double g_new = cell_details[y][x].g + 1.0;
                    // dada pela distância manhattan
                    double h_new = calculate_distance(new_x, new_y, target_x, target_y);
                    // "Prioridade" final do caminho
                    double f_new = g_new + h_new;

                    // Atualiza se encontrou um caminho melhor
                    if (cell_details[new_y][new_x].f == FLT_MAX || cell_details[new_y][new_x].f > f_new) {

                        cell_details[new_y][new_x].f = f_new;
                        cell_details[new_y][new_x].g = g_new;
                        cell_details[new_y][new_x].h = h_new;
                        cell_details[new_y][new_x].parent_x = x;
                        cell_details[new_y][new_x].parent_y = y;

                        open_list.push(cell_details[new_y][new_x]);
                        //adiciona à lista de celulas a serem exploradas                    
                    }
                }
            }
        }
    }

    return path;
}

// Movimento inteligente usando A* pathfinding para Ghost[2]
void move_ghost_astar(Ghost &ghost_ref, int target_x, int target_y, bool force_recalc = false) {
    // Variáveis estáticas mantêm estado entre chamadas
    static vector<pair<int, int>> current_path;
    static int path_index = 0;
    static int last_target_x = -1, last_target_y = -1;

    int last_direction = ghost_ref.last_direction;

    // Recalcula caminho se: forçado(quando reinicia), alvo mudou(pacman moveu), ou caminho terminou(chegou em pacman)
    if (force_recalc || target_x != last_target_x || target_y != last_target_y || path_index >= current_path.size()) {
        current_path = finda_path(ghost_ref.x, ghost_ref.y, target_x, target_y, ghost_ref);
        path_index = 0;
        last_target_x = target_x;
        last_target_y = target_y;
    }

    // Segue o caminho calculado pelo A*
    if (!current_path.empty() && path_index < current_path.size()) {
        // Pula primeira posição se for a posição atual
        if (path_index == 0 && current_path[0].first == ghost_ref.x && current_path[0].second == ghost_ref.y) {
            path_index++;
        }

        if (path_index < current_path.size()) {
            // Calcula direção do movimento
            // diferença entre a posição atual e a proxima posição eficiente
            int delta_x = current_path[path_index].first - ghost_ref.x;
            int delta_y = current_path[path_index].second - ghost_ref.y;

            // Move para próxima posição do caminho
            ghost_ref.x = current_path[path_index].first;
            ghost_ref.y = current_path[path_index].second;

            // Atualiza direções para próximo movimento
            if (delta_x == 1 && delta_y == 0)
                ghost_ref.last_direction = 0; // Direita
            else if (delta_x == 0 && delta_y == 1)
                ghost_ref.last_direction = 1; // Baixo
            else if (delta_x == -1 && delta_y == 0)
                ghost_ref.last_direction = 2; // Esquerda
            else if (delta_x == 0 && delta_y == -1)
                ghost_ref.last_direction = 3; // Cima
            ghost_ref.opposite_direction = ((ghost_ref.last_direction + 2) % 4);
            path_index++;
        }
    } else {
        // usa movimento aleatório se A* falhar
        move_ghost(ghost_ref);
    }
}

// Reinicia o jogo: reseta variáveis, reposiciona elementos, força recálculo A*
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
    // Força recálculo do caminho A* após reposicionamento
    move_ghost_astar(ghost[2], pacman.x, pacman.y, true);
}

int main() {
    // Inicialização: posiciona elementos e copia mapa original
    reposiciona();
    memcpy(mapa, mapa_original, sizeof(mapa_original));

    //////////////////////////////////////
    // SETUP SONS: carrega arquivos de áudio e configura volume
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

    ////////////////////////////////////
    // Posiciona elementos aleatórios no mapa
    posiciona_frutas(mapa);
    posiciona_boost(mapa);

    // Cria janela do jogo
    RenderWindow window(VideoMode(COLS * SIZE, (ROWS + 2) * SIZE), "Pac-Man");

    // Setup blocos das paredes
    RectangleShape rectangle(Vector2f(SIZE, SIZE));
    rectangle.setFillColor(Color(22, 22, 165));
    rectangle.setOutlineThickness(-1);
    rectangle.setOutlineColor(Color(15, 15, 117));

    ///////////////////////////////////////////////////
    // SETUP TEXTURAS E SPRITES: carrega imagens para todos os elementos
    texture_win.loadFromFile("imagens/start.png");
    sprite_win.setTexture(texture_win);

    pacman.texture.loadFromFile("imagens/pacman.png");
    pacman.sprite.setTexture(pacman.texture);

    // Carrega sprites dos fantasmas com cores diferentes
    ghost[0].texture.loadFromFile("imagens/ghostbd.png");
    ghost[0].sprite.setTexture(ghost[0].texture);

    ghost[1].texture.loadFromFile("imagens/ghostge.png");
    ghost[1].sprite.setTexture(ghost[1].texture);

    ghost[2].texture.loadFromFile("imagens/ghostrd.png"); // Fantasma A* (vermelho)
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
    // Setup fonte e textos da interface
    fonte.loadFromFile("ARCADEPI.TTF");

    // Setup placar (pontos e vidas)
    placar.setFont(fonte);
    placar.setCharacterSize(24);
    placar.setFillColor(Color::Yellow);
    placar.setPosition(20, ROWS * SIZE + 3);

    // Setup texto das telas de game over e vitória
    text_restart.setFont(fonte);
    text_restart.setCharacterSize(32);
    text_restart.setFillColor(Color::White);
    text_restart.setPosition(100, 400);
    text_exit.setFont(fonte);
    text_exit.setCharacterSize(32);
    text_exit.setFillColor(Color::White);
    text_exit.setPosition(120, 450);
    
    // Variável para controle do menu
    int aux = 1;
    // Loop principal do jogo
    while (window.isOpen()) {

        Event event;

        // Estado: Tela inicial (menu)
        if (game_state.start) {

            while (window.pollEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    // Navegação no menu
                    if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                        choice.play();
                        texture_start.loadFromFile("imagens/start.png");
                        aux = 1; // Opção "Jogar"
                    } else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                        choice.play();
                        texture_start.loadFromFile("imagens/start_2.png");
                        aux = 2; // Opção "Sair"
                    } else if (event.key.code == Keyboard::Enter) {
                        if (aux == 1) {
                            choice.play();
                            music.play();
                            game_state.start = false; // Inicia o jogo
                        } else {
                            window.close();
                            return 0;
                        }
                    }
                }
            }

        } 
        // Estado: Tela de game over ou vitória
        else if (game_state.lose || game_state.win)
            while (window.pollEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::R) {
                        choice.play();
                        reinicia(); // Reinicia o jogo
                    } else if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                }
            }
        // Estado: Jogo ativo
        else {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        window.close();
                    }
                    // Reset manual do jogo
                    else if (event.key.code == Keyboard::R)
                        reinicia();
                    // Controles de movimento do Pac-Man (definição de intenções)
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
            // Lógica de jogo ativa (movimento e colisões)
            if (!game_state.lose || !game_state.win) {

                // Define velocidade baseada no boost
                float intervalo = game_state.boost ? PACMAN_FAST_SPEED : PACMAN_SPEED;
                bool morte = 0;
                
                // Timer de movimento do Pac-Man
                if (relo.getElapsedTime() > seconds(intervalo)) {
                    // Verifica se o boost expirou
                    if (game_state.boost && clock_boost.getElapsedTime() > seconds(ENERGY_DURATION)) {
                        game_state.boost = false;
                    }

                    // Sistema de coleta de itens
                    if (mapa[pacman.y][pacman.x] == '0') {
                        game_state.points += POINT_VALUE; // Ponto normal (+10)
                        mapa[pacman.y][pacman.x] = '*';
                    } else if (mapa[pacman.y][pacman.x] == 'f') {
                        game_state.points += FRUIT_VALUE; // Fruta (+50)
                        mapa[pacman.y][pacman.x] = '*';
                    } else if (mapa[pacman.y][pacman.x] == 'e') {
                        game_state.boost = true; // Energético (velocidade)
                        clock_boost.restart();
                        mapa[pacman.y][pacman.x] = '*';
                    }

                    verificar_vitoria();

                    // Sistema de intenção de movimento: verifica se pode mover na direção desejada
                    if (pacman.intention_up && check_pacman_move(-1, 0)) {
                        pacman.intention_up = false;
                        pacman.current_up = true;
                        pacman.current_down = pacman.current_left = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-up.png");
                    } else if (pacman.intention_down && check_pacman_move(1, 0)) {
                        pacman.intention_down = false;
                        pacman.current_down = true;
                        pacman.current_up = pacman.current_left = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-down.png");
                    } else if (pacman.intention_left && check_pacman_move(0, -1)) {
                        pacman.intention_left = false;
                        pacman.current_left = true;
                        pacman.current_up = pacman.current_down = pacman.current_right = false;
                        pacman.texture.loadFromFile("imagens/pacman-esq.png");
                    } else if (pacman.intention_right && check_pacman_move(0, 1)) {
                        pacman.intention_right = false;
                        pacman.current_right = true;
                        pacman.current_up = pacman.current_down = pacman.current_left = false;
                        pacman.texture.loadFromFile("imagens/pacman.png");
                    }

                    // Movimento contínuo: continua movendo na direção atual se possível
                    if (pacman.current_up && check_pacman_move(-1, 0))
                        pacman.y--;
                    else if (pacman.current_down && check_pacman_move(1, 0))
                        pacman.y++;
                    else if (pacman.current_left && check_pacman_move(0, -1))
                        pacman.x--;
                    else if (pacman.current_right && check_pacman_move(0, 1))
                        pacman.x++;

                    if (verifica_morte()) {
                        morrer();
                        morte = 1;
                    }
                    relo.restart();
                }

                // Timer de movimento dos fantasmas
                if (clock_ghosts.getElapsedTime() > seconds(GHOST_SPEED)) {
                    // Move fantasmas: [0,1,3] aleatório, [2] usa A* pathfinding
                    move_ghost(ghost[0]);        // Fantasma azul - movimento aleatório
                    move_ghost(ghost[1]);        // Fantasma verde - movimento aleatório  
                    move_ghost_astar(ghost[2], pacman.x, pacman.y); // Fantasma vermelho - A* inteligente
                    move_ghost(ghost[3]);        // Fantasma amarelo - movimento aleatório

                    clock_ghosts.restart();
                    // Verifica morte após movimento dos fantasmas
                    if (!morte && verifica_morte()) {
                        morrer();
                    }
                }
                morte = 0;
            }
        }

        // Limpa tela e inicia renderização
        window.clear(Color::Black);

        // Renderização baseada no estado do jogo
        if (game_state.start) {
            // Desenha tela inicial (menu)
            sprite_start.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(sprite_start);
        } else {

            // Desenha o mapa: paredes, pontos, frutas e energéticos
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (mapa[i][j] == '1') {
                        // Desenha parede
                        rectangle.setPosition(j * SIZE, i * SIZE);
                        rectangle.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(rectangle);
                    } else if (mapa[i][j] == '0' || mapa[i][j] == '*') {
                        // Desenha pontos (brancos = coletáveis, cinzas = coletados)
                        CircleShape ponto(SIZE / 8.0f);
                        ponto.setFillColor(mapa[i][j] == '0' ? Color::White : Color(128, 128, 128));
                        ponto.setPosition(j * SIZE + SIZE / 2.5f, i * SIZE + SIZE / 2.5f);
                        ponto.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(ponto);
                    } else if (mapa[i][j] == 'f') {
                        // Desenha fruta
                        sprite_fruta.setPosition(j * SIZE, i * SIZE);
                        sprite_fruta.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(sprite_fruta);
                    } else if (mapa[i][j] == 'e') {
                        // Desenha energético
                        sprite_boost.setPosition(j * SIZE, i * SIZE);
                        sprite_boost.setScale(SIZE / 25.0, SIZE / 25.0);
                        window.draw(sprite_boost);
                    }
                }
            }

            // Desenha Pac-Man
            pacman.sprite.setPosition(pacman.x * SIZE, pacman.y * SIZE);
            pacman.sprite.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(pacman.sprite);

            // Posiciona e desenha todos os fantasmas
            for (int i = 0; i < 4; i++) {
                ghost[i].sprite.setPosition(ghost[i].x * SIZE, ghost[i].y * SIZE);
            }

            // Escala e desenha sprites dos fantasmas
            ghost[0].sprite.setScale(SIZE / 25.0, SIZE / 25.0); // Azul
            ghost[1].sprite.setScale(SIZE / 25.0, SIZE / 25.0); // Verde
            ghost[2].sprite.setScale(SIZE / 25.0, SIZE / 25.0); // Vermelho (A*)
            ghost[3].sprite.setScale(SIZE / 25.0, SIZE / 25.0); // Amarelo

            window.draw(ghost[0].sprite);
            window.draw(ghost[1].sprite);
            window.draw(ghost[2].sprite);
            window.draw(ghost[3].sprite);

            // Desenha placar (pontos e vidas)
            placar.setString("Pontos: " + to_string(game_state.points) + "   Vidas: " + to_string(game_state.life));
            placar.setScale(SIZE / 25.0, SIZE / 25.0);
            window.draw(placar);

            // Desenha telas de game over ou vitória
            if (game_state.lose || game_state.win) {

                // Fundo escuro semi-transparente
                RectangleShape overlay(Vector2f(COLS * SIZE, (ROWS + 2) * SIZE));
                overlay.setFillColor(Color(0, 0, 0, 180));
                overlay.setScale(SIZE / 25.0, SIZE / 25.0);
                window.draw(overlay);

                if (game_state.lose) {
                    // Tela de game over
                    sprite_lose.setPosition(140, 100);
                    text_restart.setString("Continuar? (Aperte R)");
                    sprite_lose.setScale(SIZE / 25.0, SIZE / 25.0);
                    window.draw(sprite_lose);
                } else {
                    // Tela de vitória
                    text_restart.setString("Reiniciar? (Aperte R)");
                    sprite_win.setScale(SIZE / 25.0, SIZE / 25.0);
                    window.draw(sprite_win);
                }

                text_exit.setString("Sair? (Aperte ESC)");

                // Desenha textos de controle
                text_restart.setScale(SIZE / 25.0, SIZE / 25.0);
                text_exit.setScale(SIZE / 25.0, SIZE / 25.0);
                window.draw(text_restart);
                window.draw(text_exit);
            }
        }
        // Atualiza a tela
        window.display();
    }

    return 0;
}
