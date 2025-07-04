# Jogo Pac-Man em C++ com SFML

## Descrição do Projeto

Este é um projeto de implementação do clássico jogo Pac-Man utilizando C++ e a biblioteca SFML (Simple and Fast Multimedia Library). O projeto foi desenvolvido como trabalho prático da disciplina INF110.

## Estado Atual do Projeto - JOGO COMPLETO ✅

### Funcionalidades Implementadas ✅

#### Sistema de Jogo Completo
- **Janela do jogo**: Criada com SFML com dimensões 625x675 pixels
- **Mapa do labirinto**: Implementado com matriz 25x25 usando caracteres ('1' para paredes, '0' para pontos)
- **Sistema de coordenadas**: Cada célula tem 25x25 pixels
- **Tela de início**: Menu inicial com opções de navegação
- **Tela de game over**: Tela de morte com opções de reinício
- **Tela de vitória**: Tela de vitória quando todos os pontos são coletados

#### Controles do Pac-Man
- **Movimentação**: Setas direcionais ou teclas WASD
- **Sprites direcionais**: Diferentes imagens para cada direção (cima, baixo, esquerda, direita)
- **Movimento contínuo**: Sistema de intenção de movimento implementado
- **Colisão com paredes**: Verificação de limites e obstáculos
- **Túneis**: Suporte para túneis horizontais e verticais

#### Sistema de Pontuação e Collectibles
- **Pontos básicos**: +10 pontos por cada ponto coletado
- **Frutas bônus**: +50 pontos por fruta (5 frutas espalhadas aleatoriamente)
- **Power pellets**: Sistema de energético implementado
- **Visualização**: Placar exibido na parte inferior da tela
- **Persistência**: Pontuação mantida durante o jogo
- **Condição de vitória**: 3340 pontos totais para completar o jogo

#### Sistema de Vidas
- **Vidas iniciais**: 3 vidas
- **Morte**: Pac-Man retorna à posição inicial quando morre
- **Game Over**: Tela de morte quando vidas acabam
- **Exibição**: Contador de vidas no placar
- **Reset**: Fantasmas retornam às posições iniciais após morte

#### Sistema de Fantasmas Avançado
- **4 fantasmas ativos**: Todos os quatro fantasmas implementados e funcionais
- **IA Inteligente com A* Pathfinding**: Ghost[2] implementa algoritmo A* para perseguição inteligente ao Pac-Man
- **IA Normal**: Ghost[0], Ghost[1] e Ghost[3] com movimento aleatório inteligente
- **Prevenção de reversão**: Todos os fantasmas evitam voltar na direção anterior
- **Algoritmo A* Otimizado**: Sistema de pathfinding que encontra o caminho mais curto até o Pac-Man
- **Cache de caminho**: Recalcula rota apenas quando o Pac-Man muda de posição ou caminho termina
- **Fallback inteligente**: Se A* falhar, usa movimento aleatório como backup
- **Suporte a túneis**: Fantasmas podem usar túneis horizontais e verticais
- **Colisão**: Detecção de colisão com Pac-Man causa morte via função `verifica_morte()`
- **Sprites diferenciados**: Cada fantasma tem cor e sprite únicos
- **Posicionamento inicial**: Ghost[0](1,1), Ghost[1](1,23), Ghost[2](23,1), Ghost[3](23,23)
- **Verificação otimizada**: Sistema de colisão executado a cada frame

#### Sistema de Power Pellets
- **Energético**: Item especial que acelera o Pac-Man
- **Efeito temporário**: Dura 5 segundos
- **Velocidade aumentada**: Pac-Man se move mais rápido durante o efeito
- **Posicionamento aleatório**: Energético colocado em posição aleatória no mapa

#### Sistema de Áudio Completo
- **Música de fundo**: Música ambiente com loop
- **Som de vitória**: Som especial quando o jogo é completado
- **Efeitos sonoros**: Sons de morte, game over e navegação no menu
- **Controle de volume**: Volumes ajustados para cada tipo de som

#### Interface Visual Avançada
- **Fonte personalizada**: ARCADEPI.TTF carregada
- **Cores**: Paredes azuis com bordas, pontos brancos/cinzas
- **Animações**: Sprites mudam conforme direção do movimento
- **Telas de menu**: Tela inicial, tela de game over e tela de vitória
- **Overlay**: Fundo semi-transparente nas telas de transição

#### Controles do Sistema
- **ESC**: Fecha o jogo
- **Enter**: Confirma seleções no menu
- **Setas/WASD**: Navegação no menu e movimento do Pac-Man
- **R**: Reinicia o jogo na tela de game over ou vitória

### Funcionalidades Técnicas Avançadas

#### Constantes e Configurações do Jogo
- **SIZE**: 25 pixels (tamanho de cada célula do mapa)
- **ROWS/COLS**: 25x25 (dimensões do mapa)
- **PACMAN_SPEED**: 0.2f (velocidade normal do Pac-Man)
- **PACMAN_FAST_SPEED**: 0.15f (velocidade com energético)
- **GHOST_SPEED**: 0.3f (velocidade dos fantasmas)
- **ENERGY_DURATION**: 5.0f segundos (duração do energético)
- **POINT_VALUE**: 10 pontos (valor de cada ponto)
- **FRUIT_VALUE**: 50 pontos (valor de cada fruta)
- **MAX_FRUITS**: 5 frutas (total de frutas no mapa)
- **TOTAL_POINTS**: 3340 pontos (total para vitória)

#### Sistema de A* Pathfinding
- **Algoritmo A* Completo**: Implementação do algoritmo de busca heurística para encontrar o caminho mais curto
- **Estruturas Especializadas**: `Node` para representar células com custos f, g, h e `Compare` para priority queue
- **Heurística Manhattan**: Usa distância Manhattan como função heurística para estimar custos
- **Priority Queue Otimizada**: Min-heap baseado no custo f para processamento eficiente
- **Prevenção de Movimento Reverso**: Evita que fantasmas voltem na direção anterior durante o pathfinding
- **Cache Inteligente**: Recalcula caminho apenas quando necessário (alvo mudou ou caminho terminou)
- **Validação de Células**: Verifica limites do mapa e colisões com paredes usando `is_valid_map_cell()`
- **Fallback Robusto**: Sistema de backup com movimento aleatório se A* falhar

#### Sistema de Túneis
- **Túneis horizontais**: Nas linhas 11 e 13 do mapa (índices do array)
- **Túnel vertical**: Na coluna central (coluna 12)
- **Teleportação**: Pac-Man e fantasmas podem atravessar as bordas da tela
- **Funcionalidade completa**: Suporte para túneis tanto para Pac-Man quanto para todos os fantasmas
- **Detecção automática**: Sistema verifica automaticamente limites e aplica teleportação

#### IA dos Fantasmas
- **Fantasma Perseguidor Inteligente (Ghost[2])**: Usa algoritmo A* para perseguição otimizada do Pac-Man
- **Fantasmas Aleatórios**: Ghost[0], Ghost[1] e Ghost[3] com movimento aleatório inteligente com prevenção de reversão
- **Detecção de colisão**: Sistema robusto de detecção de paredes
- **Movimento sincronizado**: Todos os fantasmas se movem independentemente
- **Rastreamento de direção**: Cada fantasma mantém histórico de `last_direction` e `opposite_direction`
- **Função `verifica_morte()`**: Sistema otimizado que verifica colisão com todos os 4 fantasmas

#### Sistema de Estados
- **Estado inicial**: Tela de menu
- **Estado de jogo**: Gameplay principal
- **Estado de morte**: Tela de game over
- **Estado de vitória**: Tela de vitória
- **Transições**: Navegação fluida entre estados

### Sistema de Reinicialização Inteligente
- **Função `reinicia()`**: Sistema completo de reset do jogo
- **Ordem correta**: Copia mapa original ANTES de posicionar itens
- **Reset completo**: Vida, pontuação, estados de tela e posições
- **Gerenciamento de memória**: Uso correto de `memcpy()` para restaurar mapa
- **Reposicionamento**: Pac-Man e fantasmas retornam às posições iniciais

### Sistema de Escala Responsiva
- **Adaptação automática**: Sprites se adaptam ao tamanho das células do mapa
- **Escalamento proporcional**: Todos os elementos visuais mantêm proporção
- **Fórmula de escala**: `SIZE/25.0` para compatibilidade com diferentes resoluções

#### Sistema de Vitória Completo
- **Contador de pontos**: Sistema que verifica pontuação total (3340 pontos)
- **Detecção automática**: Vitória detectada quando todos os collectibles são coletados
- **Tela de vitória**: Interface específica para celebrar a vitória
- **Som de vitória**: Áudio especial para o momento da vitória

## Estrutura de Arquivos

```
trabalho3/
├── pacman.cpp           # Código principal do jogo (versão final)
├── pacman.out           # Executável compilado
├── ARCADEPI.TTF         # Fonte do jogo
├── README.md            # Este arquivo
├── .gitignore           # Configuração do Git
├── .clang-format        # Formatação de código
├── .vscode/             # Configurações do VS Code
├── sons/                # Pasta com todos os áudios
│   ├── music.mp3        # Música de fundo
│   ├── death.mp3        # Som de morte
│   ├── game_over.mp3    # Som de game over
│   ├── choice.mp3       # Som de navegação no menu
│   └── win.wav          # Som de vitória
└── imagens/             # Pasta com todos os sprites
    ├── pacman*.png      # Sprites do Pac-Man (todas direções)
    ├── ghost*.png       # Sprites dos 4 fantasmas
    ├── pizza.png        # Sprite da fruta
    ├── energy-drink.png # Sprite do energético
    ├── start*.png       # Sprites do menu inicial
    ├── tela_demorte.png # Sprite da tela de game over
    ├── win.png          # Sprite da tela de vitória
    └── pacmanvida.png   # Sprite para vidas
```

## Controles

### No Menu
- **Setas cima/baixo ou W/S**: Navegação entre opções
- **Enter**: Confirmar seleção

### No Jogo
- **Setas direcionais ou WASD**: Movimento do Pac-Man
- **ESC**: Voltar ao menu/sair

### Na Tela de Game Over ou Vitória
- **R**: Reiniciar o jogo
- **ESC**: Sair do jogo

## Elementos do Jogo

### Collectibles
- **Pontos pequenos (.)**: +10 pontos cada
- **Frutas (pizza)**: +50 pontos cada (5 no total)
- **Energético**: Acelera o Pac-Man por 5 segundos

### Personagens
- **Pac-Man**: Controlado pelo jogador
- **Fantasma Azul (Ghost[0])**: Movimento aleatório inteligente - posição inicial (1,1)
- **Fantasma Verde (Ghost[1])**: Movimento aleatório inteligente - posição inicial (1,23)
- **Fantasma Vermelho (Ghost[2])**: **IA PERSEGUIDORA A*** que usa algoritmo A* pathfinding para perseguir o Pac-Man inteligentemente - posição inicial (23,1)
- **Fantasma Amarelo (Ghost[3])**: Movimento aleatório inteligente - posição inicial (23,23)

## Tecnologias Utilizadas

- **Linguagem**: C++17
- **Biblioteca gráfica**: SFML 2.6.2 (Graphics, Audio, System, Window)
- **Compilador**: GCC/G++
- **Sistema**: Linux/Windows
- **Recursos**: Sprites PNG, fontes TTF, áudio MP3

## Como compilar

- Abra o terminal na pasta do projeto.
- Use o comando abaixo:
    ```sh
    g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o pacman.out
    ```

## Funcionalidades Avançadas Implementadas

### Sistema A* Pathfinding
- **Algoritmo A* Completo**: Ghost[2] implementa algoritmo A* para perseguição inteligente
- **Estruturas Especializadas**: 
  - `Node`: Representa células com custos f, g, h e coordenadas dos pais
  - `Compare`: Operador de comparação para priority queue min-heap
- **Heurística Euclidiana**: Usa função `calculate_distance()` para estimativa de custo
- **Prevenção de Reversão**: Evita que fantasmas voltem na direção anterior
- **Cache de Caminho**: Recalcula apenas quando Pac-Man muda de posição ou caminho termina
- **Fallback Inteligente**: Sistema de backup com movimento aleatório se A* falhar

### Código Snake_Case
- **Padronização**: Todas as variáveis e funções seguem padrão `snake_case`
- **Structs Atualizadas**: 
  - `Pacman`: `current_up`, `intention_left`, etc.
  - `Ghost`: `last_direction`, `opposite_direction`
  - `Game_State`: `life`, `points`, etc.
- **Funções Utilitárias**: `is_valid_map_cell()`, `calculate_distance()`, `move_ghost_astar()`

### Algoritmo A* - Funções Principais
```cpp
// Validação de células (limites do mapa e paredes)
bool is_valid_map_cell(int x, int y);

// Cálculo de distância Manhattan (heurística)
double calculate_distance(int x1, int y1, int x2, int y2);

// Implementação completa do algoritmo A*
vector<pair<int, int>> findPath(int start_x, int start_y, int target_x, int target_y, Ghost ghost_ref);

// Movimento inteligente com cache de caminho
void move_ghost_astar(Ghost& ghost_ref, int target_x, int target_y, bool force_recalc = false);
```

### Performance e Otimizações
- **Priority Queue**: Min-heap baseado no custo f para processamento eficiente
- **Cache Inteligente**: Evita recálculos desnecessários do caminho A*
- **Validação Otimizada**: Verificação rápida de células válidas
- **Fallback Robusto**: Movimento aleatório como backup se pathfinding falhar

**Observação:** As bibliotecas da SFML devem estar instaladas no sistema para que o jogo funcione corretamente.

### Solucionando Problemas de Compilação

Se você receber erros de "undefined reference" relacionados ao áudio:

1. **Certifique-se de incluir todas as bibliotecas necessárias:**
   ```sh
   g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o pacman.out
   ```

2. **Instale as bibliotecas de desenvolvimento SFML:**
   ```sh
   # Ubuntu/Debian
   sudo apt-get install libsfml-dev
   
   # Arch Linux
   sudo pacman -S sfml
   
   # Fedora/CentOS
   sudo dnf install SFML-devel
   ```

3. **Compilação sem áudio (se necessário):**
   Se você não conseguir instalar as bibliotecas de áudio, pode comentar as linhas relacionadas ao som no código e compilar apenas com:
   ```sh
   g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.out
   ```sh
   g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.out
   ```

## Como jogar

1. Compile o jogo usando o comando acima
2. Execute o arquivo resultante:
   ```sh
   ./pacman.out
   ```
3. Use as setas direcionais ou WASD para mover o Pac-Man
4. Colete todos os pontos, frutas e energéticos para vencer
5. Evite os fantasmas (especialmente o vermelho que persegue!)
6. Colete 3340 pontos para completar o jogo

## Funcionalidades Técnicas Detalhadas

### Sistema de Colisão Avançado
- **Detecção precisa**: Verifica colisões entre Pac-Man e fantasmas a cada frame
- **Função `verifica_morte()`**: Sistema otimizado de verificação de colisão
- **Verificação por coordenadas**: Compara posições exatas `(posx, posy)` com todos os fantasmas
- **Túneis seguros**: Colisões funcionam corretamente mesmo nos túneis
- **Performance otimizada**: Uma única função verifica todos os 4 fantasmas

### Sistema de Escala Responsiva
- **Adaptação automática**: Todos os sprites se ajustam dinamicamente
- **Escalamento consistente**: Usa `SIZE/25.0` para manter proporções
- **Interface adaptável**: Textos, overlay e elementos UI também escalam

### Algoritmos Implementados

#### IA do Fantasma Perseguidor
O Ghost2 (fantasma verde) implementa um algoritmo de perseguição baseado em:
- **Distância Manhattan**: Calcula a distância `|x1-x2| + |y1-y2|` até o Pac-Man
- **Pathfinding simples**: Escolhe sempre a direção que diminui a distância
- **Prevenção de loops**: Evita voltar na direção anterior
- **Tratamento de túneis**: Considera teleportação nas bordas do mapa

#### Sistema de Vitória Inteligente
- **Contador automático**: Sistema conta automaticamente pontos coletados
- **Verificação em tempo real**: Checa vitória após cada collectible
- **Pontuação fixa**: 3340 pontos totais calculados do mapa

#### Sistema de Escalamento Visual
- **Escala dinâmica**: Fórmula `SIZE/25.0` para todos os sprites
- **Consistência visual**: Mantém proporções independente do tamanho da célula
- **Adaptabilidade**: Permite fácil modificação do tamanho do jogo
- **Elementos escaláveis**: Sprites, textos, overlay e elementos UI se ajustam automaticamente

## Status de Desenvolvimento

✅ **JOGO 100% COMPLETO E FUNCIONAL**

### Implementações Concluídas
✅ **Sistema de Fantasmas Avançado**: Todos os 4 fantasmas com IA diferenciada
✅ **IA Perseguidora**: Ghost2 implementa algoritmo de pathfinding inteligente  
✅ **Sistema de Vitória**: Detecção automática quando todos os pontos são coletados (3340 pontos)
✅ **Sistema de Áudio Completo**: Música, efeitos sonoros e som de vitória
✅ **Interface Completa**: Telas de início, game over e vitória
✅ **Collectibles**: Pontos, frutas e power pellets funcionais
✅ **Túneis**: Sistema completo de teleportação
✅ **Controles**: Movimento fluido e responsivo
✅ **Estados do Jogo**: Transições perfeitas entre telas

### Recursos Técnicos Avançados
- **Algoritmo de Perseguição**: Distância Manhattan + prevenção de loops
- **Sistema de Estados**: Máquina de estados bem definida  
- **Detecção de Colisão**: Robusta para paredes, túneis e fantasmas via `verifica_morte()`
- **Gerenciamento de Recursos**: Texturas, sons e fontes organizados
- **Pontuação Dinâmica**: Sistema de contagem automática e precisa
- **Escalamento Visual**: Sistema responsivo que adapta todos os elementos visuais
- **Otimização de Performance**: Verificações de colisão otimizadas por frame
- **Reinicialização Inteligente**: Sistema robusto de reset com ordem correta de operações
- **Gerenciamento de Memória**: Uso eficiente de `memcpy()` e arrays estáticos

**🎮 O jogo está pronto para ser jogado e avaliado!**

---

*Projeto desenvolvido para a disciplina INF110 - Programação 1*

## Melhorias Técnicas Implementadas

### 1. Algoritmo A* Pathfinding
- **Implementação Completa**: Sistema de busca heurística para encontrar caminho ótimo
- **Estruturas de Dados Otimizadas**: Priority queue com min-heap para eficiência
- **Heurística Inteligente**: Distância euclidiana para estimativa de custo
- **Prevenção de Reversão**: Evita que fantasmas voltem na direção anterior
- **Cache de Caminho**: Recalcula apenas quando Pac-Man muda de posição ou caminho termina
- **Fallback Inteligente**: Sistema de backup com movimento aleatório se A* falhar

### 2. Refatoração para Snake_Case
- **Padronização**: Todas as variáveis seguem convenção `snake_case`
- **Melhor Legibilidade**: Código mais profissional e consistente
- **Manutenibilidade**: Facilita futuras modificações e debugging

### 3. Otimizações de Performance
- **Cache de Caminho**: Recalcula A* apenas quando necessário
- **Validação Eficiente**: Função `is_valid_cell()` otimizada
- **Fallback Inteligente**: Sistema robusto de backup para movimento

### 4. Arquitetura Modular
- **Separação de Responsabilidades**: Cada função tem propósito específico
- **Reutilização de Código**: Funções utilitárias compartilhadas
- **Escalabilidade**: Fácil adição de novos tipos de IA

---

**Nota**: Este projeto demonstra implementação avançada de algoritmos de IA em jogos, com foco em pathfinding inteligente e código limpo seguindo boas práticas de programação.

#### Estruturas de Dados Principais

##### Struct Pacman
```cpp
struct Pacman {
    int x, y;                    // Posição atual no mapa
    bool current_up/down/left/right;    // Direção atual de movimento
    bool intention_up/down/left/right;  // Intenção de movimento (buffer)
    Texture texture;             // Textura do sprite
    Sprite sprite;              // Sprite para renderização
};
```

##### Struct Ghost
```cpp
struct Ghost {
    Texture texture;            // Textura do sprite
    Sprite sprite;             // Sprite para renderização
    int x, y;                  // Posição atual no mapa
    int opposite_direction;    // Direção oposta (para evitar reversão)
    int last_direction;        // Última direção tomada
};
```

##### Struct Game_State
```cpp
struct Game_State {
    int life;                  // Vidas restantes (inicia com 3)
    int points;                // Pontuação atual
    bool start;                // Estado da tela inicial
    bool win;                  // Estado de vitória
    bool lose;                 // Estado de game over
    bool boost;                // Estado do energético ativo
};
```

##### Struct Node (para A*)
```cpp
struct Node {
    int x, y;                  // Coordenadas da célula
    double f, g, h;            // Custos do algoritmo A*
    int parent_x, parent_y;    // Coordenadas do nó pai (para reconstrução do caminho)
};
```

#### Sistema de Mapa
- **Mapa Original**: Array 2D `char mapa_original[25][26]` (inclui terminador de string)
- **Mapa de Jogo**: Array 2D `char mapa[25][26]` (cópia trabalhável do mapa original)
- **Células do Mapa**:
  - `'1'`: Paredes
  - `'0'`: Pontos normais (+10 pontos)
  - `'f'`: Frutas (+50 pontos)
  - `'e'`: Energético (velocidade aumentada)
  - `'*'`: Célula vazia (coletado)

#### Sistema de Controles e Eventos

##### Controles de Movimento
- **Setas Direcionais**: `Keyboard::Up/Down/Left/Right`
- **Teclas WASD**: `Keyboard::W/A/S/D`
- **Sistema de Intenção**: O jogo registra a intenção de movimento e executa quando possível
- **Prevenção de Movimento Inválido**: Verifica colisões antes de executar movimento

##### Controles do Sistema
- **ESC**: `Event::Closed` - Fecha o jogo em qualquer estado
- **Enter**: Confirma seleções no menu inicial
- **R**: Reinicia o jogo nas telas de game over ou vitória

##### Estados do Jogo
1. **Estado Inicial** (`game_state.start = true`):
   - Exibe tela de menu com navegação
   - Música de fundo ativa
   - Aguarda confirmação para iniciar

2. **Estado de Jogo** (`!game_state.start && !game_state.lose && !game_state.win`):
   - Gameplay principal ativo
   - Movimento de personagens
   - Coleta de itens e verificação de colisões

3. **Estado de Game Over** (`game_state.lose = true`):
   - Exibe tela de morte
   - Para música de fundo
   - Reproduz som de game over
   - Aguarda reinício ou saída

4. **Estado de Vitória** (`game_state.win = true`):
   - Exibe tela de vitória
   - Para música de fundo
   - Reproduz som de vitória
   - Aguarda reinício ou saída

## Notas Técnicas e Implementação

### Decisões de Design
- **Snake_Case**: Toda nomenclatura segue padrão `snake_case` para consistência
- **Arrays de Char**: Mapa representado como array de strings para facilidade de edição
- **Sprites Direcionais**: Pac-Man tem sprites diferentes para cada direção
- **Sistema de Estados**: Máquina de estados clara para transições de tela
- **Cache Inteligente**: A* usa cache para evitar recálculos desnecessários

### Limitações Conhecidas
- **Mapa Fixo**: Labirinto não é gerado proceduralmente
- **IA Simples**: Apenas um fantasma usa pathfinding avançado
- **Sem Power Pellets Tradicionais**: Energético não torna fantasmas vulneráveis
- **Resolução Fixa**: Interface não é totalmente responsiva

### Possíveis Melhorias
- **Múltiplos Níveis**: Sistema de progressão com diferentes mapas
- **IA Melhorada**: Mais fantasmas com comportamentos únicos
- **Power Pellets**: Implementar vulnerabilidade temporária dos fantasmas
- **Sistema de High Score**: Persistência de pontuação máxima
- **Efeitos Sonoros**: Mais feedback auditivo para ações do jogador

### Compatibilidade
- **Sistema Operacional**: Linux (primário), Windows (com SFML instalado)
- **Compilador**: GCC/G++ com suporte a C++17
- **Dependências**: SFML 2.5+ (Graphics, Window, System, Audio)
- **Resolução**: Janela fixa 625x675 pixels

### Créditos e Histórico de Desenvolvimento
```cpp
// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23  
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   implementado
// Intenção de movimento: implementado
// Sistema A* Pathfinding: implementado
// Sistema completo:      finalizado
```

**Projeto desenvolvido como trabalho prático da disciplina INF110.**

---

**Status Final: ✅ JOGO 100% COMPLETO E FUNCIONAL**

Este README documenta todas as funcionalidades implementadas no jogo Pac-Man. O projeto está completo e totalmente jogável, incluindo IA avançada, sistema de áudio, interface gráfica completa e todos os recursos esperados de um jogo Pac-Man moderno.
