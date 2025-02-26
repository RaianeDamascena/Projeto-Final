## Simulador do Sistema de Gestão e Desempenho na BitDogLab

## Descrição

Este projeto implementa uma simulação de um sistema de gestão e desempenho (SGD) utilizando a placa BitDogLab. O sistema permite avaliar a performance de funcionários, verificando se atingiram as metas estabelecidas. O status de cada funcionário é exibido em um display OLED SSD1306, e LEDs RGB indicam o desempenho conforme as metas atingidas.

## Componentes Utilizados

Placa BitDogLab

Display OLED SSD1306 via I2C

Matriz de LEDs

Botões para interação

LEDs RGB para indicar o status de desempenho

## Configuração de Hardware

Componentes:

GPIO

SDA (I2C): 14

SCL (I2C): 15

Botão A: 5

Botão B: 6

LED Verde: 11

LED Azul: 12

LED Vermelho: 13

## Funcionalidades

Navegação entre os funcionários usando o Botão A

Exibição dos detalhes de um funcionário ao pressionar o Botão B

Indicação de desempenho com LEDs RGB:

Verde: Meta atingida

Azul: Quase na meta

Vermelho: Abaixo da meta

Exibição dos números de desempenho na matriz de LEDs

## Dependências

Este projeto utiliza as bibliotecas:

Pico SDK (para interação com GPIO e I2C)

ssd1306.h e ssd1306.c (para controle do display OLED)

font.h (para renderização de texto no display)

matriz_led.h (para controle da matriz de LEDs)

numeros.h (para exibição de números na matriz de LEDs)

## Compilação e Execução

Configurar o ambiente de desenvolvimento para o Raspberry Pi Pico SDK

Compilar o código utilizando o CMake

Transferir o arquivo .uf2 gerado para a placa BitDogLab

Reiniciar a placa para iniciar a execução

## Funcionamento do Código

O programa inicia configurando os periféricos, incluindo GPIOs, I2C e o display SSD1306. O loop principal verifica se um funcionário foi selecionado e exibe as informações correspondentes no display e na matriz de LEDs. Os botões permitem navegar entre os funcionários e exibir seus dados detalhados.

## LINK DE FUNCIONAMENTO NA PLACA BITDOGLAB

https://drive.google.com/drive/folders/1z5mS1UeKbmzO3Food0HfI18YGeQQWOtI?usp=sharing

## Autor

Desenvolvido por Raiane Damascena da Paixão para o curso do Embarcatech de Sistemas Embarcados, como parte do projeto final para conclusão da primeira fase.

Matricula: tic370100668

Email: rai.paixao2012@gmail.com

