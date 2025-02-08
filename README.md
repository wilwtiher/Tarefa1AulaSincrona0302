# Aluno: Wilton Lacerda Silva Júnior
## Grupo 6 Subgrupo 5
# Video explicativo: https://youtu.be/rsGUFUMvx80
# Atividade 1: Introdução às Interfaces de Comunicação Serial com RP2040 UART, SPI e I2C

Este projeto utiliza do display ssd1306 e das funcionalidades UART para fortalecer o conhecimento obtido na aula do dia 03/02/25.

## Funcionalidades

- **Display:**  
  - O display funciona-rá como um retorno visual para as informações passadas para o microcontrolador
  - Ele se altera e mostra o feedback do que foi passado.
- **Comunicação USB:**
  - A comunicação usb permite passar valores do computador para o microcontrolador, além de receber mensagens de volta.
- **Matriz de led 5x5:**
  - A matriz de led permite a visualizacao quando um numero e passado pela serial.
- **Botões com interrupção:**
  - Os botões permitem interação com o conjunto de maneira instantanea, sem precisar passar no codigo main. Eles interrompem o código para ligar/desligar um led específico e dar retorno por meio da porta USB e do display.

## Hardware Utilizado

- **Placa:** BitDogLab (ou Raspberry Pi Pico adaptado)


## Instruções de uso

  - **Placa BitDogLab:**
    O programa deve ser importado no Visual Studio Code com a extensão do Raspberry Pi Pico, conectar a placa no computador no modo de bootloader, e clicar na opção "Run". Para verificar a saída serial, deve iniciar o Serial Monitor. Caso nao conectado de primeira, apertar o botao reset da placa e esperar alguns instantes. Depois, pode-se inserir informações por meio do monitor serial do proprio visual studio code.
