
# Projeto: Controle de Matriz de LEDs 5x5 com Botões

Este projeto utiliza uma matriz de LEDs WS2812 5x5 para exibir números de 0 a 9, controlados por dois botões. O número é incrementado ou decrementado ao pressionar os botões. Além disso, um LED RGB na placa pisca 5 vezes por segundo e um LED no pino 13 também pisca.

## Hardware Utilizado

- **Placa de Desenvolvimento**: Raspberry Pi Pico (ou compatível)
- **Matriz de LEDs WS2812**: 5x5 LEDs conectados ao pino GPIO 7
- **Botões**:
  - Botão A conectado ao pino GPIO 5 (incrementa número)
  - Botão B conectado ao pino GPIO 6 (decrementa número)
- **LEDs**:
  - LED RGB (conectado aos pinos GPIO 11, 12 e 13)
  - LED simples no pino GPIO 13 que pisca 5 vezes por segundo

## Funcionalidade

### Incrementar/Decrementar Número
- O número exibido na matriz de LEDs varia de 0 a 9.
- **Botão A**: Incrementa o número exibido na matriz.
- **Botão B**: Decrementa o número exibido na matriz.
- O número é exibido em uma matriz 5x5 de LEDs, representando o número de forma binária.

### Piscar LED
- O LED conectado ao pino GPIO 13 pisca 5 vezes por segundo.
- O LED RGB também pisca alternando entre as cores configuradas.

### Debounce
- Ambos os botões possuem debounce implementado, o que impede múltiplas leituras rápidas durante uma única pressão do botão.

## Estrutura do Código

- **Matriz de LEDs**: A matriz é representada como um array de 5x5, onde cada número de 0 a 9 tem sua representação na matriz.
- **Função de Atualização de LED**: A função `atualizar_led_buffer` é responsável por atualizar o buffer da matriz com base no número atual.
- **Exibição do Número**: A função `mostrar_numero` exibe o número atual na matriz de LEDs, utilizando as cores configuradas.
- **Interrupções para os Botões**: As interrupções são usadas para detectar a pressão dos botões A e B. O número é incrementado ou decrementado com base na pressão do botão, respeitando o limite de 0 a 9.

## Requisitos

- **Raspberry Pi Pico** (ou compatível)
- **Biblioteca de GPIO e PIO do Raspberry Pi Pico**
- **Biblioteca WS2812 para controle de LEDs**
  # interrupcoes
