# Jogo Pac-Man em C++ com SFML

## Descrição do Projeto

Este é um projeto de implementação do clássico jogo Pac-Man utilizando C++ e a biblioteca SFML (Simple and Fast Multimedia Library). O projeto foi desenvolvido como trabalho prático da disciplina INF110.

## Estado Atual do Projeto

### Partes Implementadas

#### Funcionalidades Básicas
- **Janela do jogo**: Criada com SFML com dimensões 625x675 pixels
- **Mapa do labirinto**: Implementado com matriz 25x25 usando caracteres ('1' para paredes, '0' para pontos)
- **Sistema de coordenadas**: Cada célula tem 25x25 pixels

#### Controles do Pac-Man
- **Movimentação**: Setas direcionais ou teclas WASD
- **Sprites direcionais**: Diferentes imagens para cada direção (cima, baixo, esquerda, direita)
- **Movimento contínuo**: Sistema de intenção de movimento implementado
- **Colisão com paredes**: Verificação de limites e obstáculos

#### Sistema de Pontuação
- **Pontos**: +10 pontos por cada ponto coletado
- **Visualização**: Placar exibido na parte inferior da tela
- **Persistência**: Pontuação mantida durante o jogo

#### Sistema de Vidas
- **Vidas iniciais**: 3 vidas
- **Morte**: Pac-Man retorna à posição inicial quando morre
- **Game Over**: Jogo fecha quando vidas acabam
- **Exibição**: Contador de vidas no placar

#### Fantasmas (Parcial)
- **Posicionamento**: Ghost1 posicionado em (1,1)
- **Sprite**: Carregamento da imagem do fantasma azul
- **Colisão**: Detecção de colisão com Pac-Man causa morte

#### Interface Visual
- **Fonte personalizada**: ARCADEPI.TTF carregada
- **Cores**: Paredes azuis com bordas, pontos brancos/cinzas
- **Animações**: Sprites mudam conforme direção do movimento

#### Controles do Sistema
- **ESC**: Fecha o jogo
- **Enter**: Reinicia o jogo (reset completo)
- **K**: Comando de debug para morrer

### Partes para terminar

#### Sistema de Fantasmas
- **Movimento automático**: Fantasmas devem se mover independentemente
- **IA diferenciada**: Cada fantasma com comportamento único
- **Múltiplos fantasmas**: Implementar os outros 3 fantasmas declarados

#### Condição de Vitória
- **Completar o labirinto**: Detectar quando todos os pontos foram coletados

### Tarefas Finais

- Comentar o código
- Formatar o código
- Refatorar o código (melhorar a estrutura e organização do código sem alterar seu comportamento)

### Funcionalidades Extras (Opcionais)

As seguintes funcionalidades não fazem parte do escopo obrigatório do trabalho, mas podem ser implementadas como melhorias:

- **Power pellets e modo assustado**
- **Sistema de áudio (música e efeitos sonoros)**
- **Telas de menu e game over**
- **Frutas bonus e sistema de níveis**
- **Animações mais elaboradas**
- **Sistema de recordes**

## Estrutura de Arquivos

```
trabalho3/
├── pacman.cpp          # Código principal do jogo
├── ARCADEPI.TTF         # Fonte do jogo
├── imagens_trab3/       # Pasta com todos os sprites
│   ├── pacman*.png      # Sprites do Pac-Man
│   ├── ghost*.png       # Sprites dos fantasmas
│   └── pacmanvida.png   # Sprite para vidas
└── README.md           # Este arquivo
```

## Controles

- **Movimento**: Setas direcionais ou WASD
- **Reiniciar**: Enter
- **Sair**: ESC
- **Debug (morrer)**: K

## Tecnologias Utilizadas

- **Linguagem**: C++17
- **Biblioteca gráfica**: SFML 2.6.2
- **Compilador**: GCC/G++
- **Sistema**: Linux/Windows

## Como compilar

- Abra o terminal (Prompt de Comando ou PowerShell) na pasta do projeto.
- Use o comando abaixo (ajuste os caminhos conforme necessário):
    ```sh
    g++ -std=c++17 pacman.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pacman.exe (ou .out)
    ```

**Observação:** As DLLs da SFML devem estar no mesmo diretório do executável para que o jogo funcione corretamente.

---

*Projeto desenvolvido para a disciplina INF110 - Programação 1*
