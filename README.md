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
- **IA Inteligente**: Ghost2 implementa algoritmo de perseguição ao Pac-Man
- **IA Normal**: Ghost1, Ghost3 e Ghost4 com movimento aleatório inteligente
- **Prevenção de reversão**: Todos os fantasmas evitam voltar na direção anterior
- **Suporte a túneis**: Fantasmas podem usar túneis horizontais e verticais
- **Colisão**: Detecção de colisão com Pac-Man causa morte
- **Sprites diferenciados**: Cada fantasma tem cor e sprite únicos
- **Posicionamento inicial**: Ghost1(1,1), Ghost2(23,1), Ghost3(1,23), Ghost4(23,23)

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

#### Sistema de Túneis
- **Túneis horizontais**: Nas linhas 11 e 13 do mapa
- **Túnel vertical**: Na coluna central (coluna 12)
- **Teleportação**: Pac-Man e fantasmas podem atravessar as bordas da tela

#### IA dos Fantasmas
- **Fantasma Perseguidor (Ghost2)**: Usa algoritmo de distância Manhattan para perseguir o Pac-Man
- **Fantasmas Aleatórios**: Movimento aleatório inteligente com prevenção de reversão
- **Detecção de colisão**: Sistema robusto de detecção de paredes
- **Movimento sincronizado**: Todos os fantasmas se movem independentemente

#### Sistema de Estados
- **Estado inicial**: Tela de menu
- **Estado de jogo**: Gameplay principal
- **Estado de morte**: Tela de game over
- **Estado de vitória**: Tela de vitória
- **Transições**: Navegação fluida entre estados

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
- **Fantasma Azul (Ghost1)**: Movimento aleatório inteligente - posição inicial (1,1)
- **Fantasma Verde (Ghost2)**: **IA PERSEGUIDORA** que usa algoritmo de pathfinding para perseguir o Pac-Man - posição inicial (23,1)
- **Fantasma Vermelho (Ghost3)**: Movimento aleatório inteligente - posição inicial (1,23)
- **Fantasma Amarelo (Ghost4)**: Movimento aleatório inteligente - posição inicial (23,23)

## Tecnologias Utilizadas

- **Linguagem**: C++17
- **Biblioteca gráfica**: SFML 2.6.2 (Graphics, Audio, System, Window)
- **Compilador**: GCC/G++
- **Sistema**: Linux/Windows
- **Recursos**: Sprites PNG, fontes TTF, áudio MP3

## Como compilar

- Abra o terminal (Prompt de Comando ou PowerShell) na pasta do projeto.
- Use o comando abaixo:
    ```sh
    g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o pacman.out
    ```

**Observação:** As bibliotecas da SFML devem estar instaladas no sistema para que o jogo funcione corretamente.

## Como jogar

1. Compile o jogo usando o comando acima
2. Execute o arquivo resultante:
   ```sh
   ./pacman.out
   ```
3. Use as setas direcionais ou WASD para mover o Pac-Man
4. Colete todos os pontos, frutas e energéticos para vencer
5. Evite os fantasmas (especialmente o verde que persegue!)
6. Colete 3340 pontos para completar o jogo

## Algoritmos Implementados

### IA do Fantasma Perseguidor
O Ghost2 (fantasma verde) implementa um algoritmo de perseguição baseado em:
- **Distância Manhattan**: Calcula a distância `|x1-x2| + |y1-y2|` até o Pac-Man
- **Pathfinding simples**: Escolhe sempre a direção que diminui a distância
- **Prevenção de loops**: Evita voltar na direção anterior
- **Tratamento de túneis**: Considera teleportação nas bordas do mapa

### Sistema de Vitória Inteligente
- **Contador automático**: Sistema conta automaticamente pontos coletados
- **Verificação em tempo real**: Checa vitória após cada collectible
- **Pontuação fixa**: 3340 pontos totais calculados do mapa

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
- **Detecção de Colisão**: Robusta para paredes, túneis e fantasmas
- **Gerenciamento de Recursos**: Texturas, sons e fontes organizados
- **Pontuação Dinâmica**: Sistema de contagem automática e precisa

**🎮 O jogo está pronto para ser jogado e avaliado!**

---

*Projeto desenvolvido para a disciplina INF110 - Programação 1*
