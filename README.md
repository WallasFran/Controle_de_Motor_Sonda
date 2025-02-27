# Projeto de Controle de Combustível e Mistura de Ar

Este projeto foi desenvolvido para controlar a mistura de combustível com base nos valores de um sensor de sonda lambda e de aceleração, utilizando um microcontrolador **Raspberry Pi Pico**. O sistema ajusta a proporção de combustível e ar com base nas medições de dois sensores: a sonda lambda e o potenciômetro de aceleração. As informações são exibidas em um display OLED e os LEDs RGB indicam o estado da mistura. Um buzzer também é utilizado para alertar o usuário sobre a condição da mistura de combustível.

## Objetivo

O principal objetivo deste projeto é monitorar a mistura de combustível e ar em um motor de combustão interna. A mistura ideal é crucial para o bom desempenho do motor e para a redução das emissões de poluentes. O sistema é capaz de ajustar dinamicamente o combustível fornecido com base nas condições atuais do motor, que são lidas por sensores de sonda lambda e aceleração.

## Componentes Utilizados

- **Raspberry Pi Pico**: Microcontrolador responsável pelo controle e processamento dos dados.
- **Sonda Lambda**: Sensor responsável por medir a quantidade de oxigênio nos gases de escape do motor.
- **Potenciômetro de Aceleração**: Sensor que mede a aceleração do veículo, representando a demanda de combustível com base na aceleração.
- **Display OLED (SSD1306)**: Display utilizado para exibir as informações da sonda lambda, oxigênio, combustível e aceleração.
- **LEDs RGB**: LEDs utilizados para indicar o estado da mistura de combustível:
  - **Verde**: Mistura rica (excesso de combustível).
  - **Azul**: Mistura estequiométrica (mistura ideal).
  - **Vermelho**: Mistura pobre (falta de combustível).
- **Buzzer**: Emite um sinal sonoro quando a mistura está ideal (próxima do valor estequiométrico).

## Como Funciona o Sistema

O sistema faz leituras contínuas da sonda lambda e do potenciômetro de aceleração, e com base nesses dados, ele ajusta o valor do PWM (Pulse Width Modulation) para controlar a quantidade de combustível fornecida ao motor. 

1. **Sonda Lambda**: O valor da tensão fornecido pela sonda lambda indica a concentração de oxigênio nos gases de escape. Um valor ideal de tensão representa uma mistura de ar e combustível perfeita (mistura estequiométrica). Tensão mais baixa indica mistura rica (excesso de combustível), e tensão mais alta indica mistura pobre (falta de combustível).

2. **Aceleração**: O valor do potenciômetro de aceleração indica a demanda de combustível. Quando o veículo está acelerando, mais combustível é necessário, o que é ajustado pelo sistema.

3. **Controle de PWM**: O sistema ajusta o valor do PWM para controlar a quantidade de combustível. Se a mistura estiver rica (excesso de combustível), o PWM será reduzido, e se a mistura estiver pobre (falta de combustível), o PWM será aumentado. Em uma mistura ideal, o PWM permanece em um valor balanceado.

4. **Indicadores LED RGB**: O sistema utiliza LEDs RGB para indicar a condição da mistura de combustível:
   - **LED Verde**: Mistura rica (quando a sonda lambda indica excesso de combustível).
   - **LED Azul**: Mistura estequiométrica (quando a sonda lambda indica uma mistura ideal).
   - **LED Vermelho**: Mistura pobre (quando a sonda lambda indica falta de combustível).

5. **Buzzer**: O buzzer emite um sinal sonoro quando a mistura está ideal (fator lambda ~ 1.0), indicando ao motorista que a mistura está dentro do intervalo desejado.

6. **Display OLED**: As informações sobre a sonda lambda, a porcentagem de oxigênio, o valor do PWM (combustível) e o valor da aceleração são exibidas em um display OLED. Isso permite ao motorista monitorar as condições do motor em tempo real.

## Como Configurar o Projeto

### Materiais Necessários:
- **Raspberry Pi Pico** ou outro microcontrolador compatível com a linguagem de programação C.
- **Sensor de Sonda Lambda** para medir a quantidade de oxigênio nos gases de escape.
- **Potenciômetro de Aceleração** para medir a aceleração do veículo.
- **Display OLED SSD1306** para exibir informações.
- **LEDs RGB** para indicar o estado da mistura de combustível.
- **Buzzer** para alertas sonoros.
- Fios de conexão e protoboard para montagem do circuito.

### Passos para Montar o Circuito:
1. **Conectar o Sensor de Sonda Lambda** ao pino de entrada analógica do microcontrolador.
2. **Conectar o Potenciômetro de Aceleração** também a um pino de entrada analógica do microcontrolador.
3. **Conectar os LEDs RGB** aos pinos de saída digitais do microcontrolador.
4. **Conectar o Display OLED** via I2C (SDA e SCL) ao microcontrolador.
5. **Conectar o Buzzer** a um pino de saída digital do microcontrolador.
6. **Programar o Microcontrolador** utilizando a linguagem de programação C e a biblioteca SDK do Raspberry Pi Pico.

## Funcionamento do Sistema

O sistema realiza leituras contínuas dos sensores de sonda lambda e de aceleração. O valor lido pela sonda lambda é convertido em um fator lambda, que indica se a mistura de combustível é rica, pobre ou ideal. Com esse valor e a aceleração do veículo, o sistema ajusta o PWM, que controla a quantidade de combustível injetado no motor.

Além disso, o sistema fornece feedback visual e sonoro:
- **LEDs RGB** indicam o status da mistura de combustível.
- **Buzzer** emite um sinal sonoro quando a mistura está ideal.
- **Display OLED** exibe informações detalhadas sobre a sonda lambda, oxigênio, combustível e aceleração.

## Link para Vídeo Demonstrativo

Confira o vídeo demonstrativo do projeto para entender melhor o funcionamento do sistema:

https://drive.google.com/file/d/1xQSruLmB8KA3azRRzZTCIbTQx18PtYUU/view?usp=sharing


## Conclusão

Este projeto visa melhorar a eficiência de combustão de motores, garantindo que a mistura de combustível e ar esteja sempre otimizada.


