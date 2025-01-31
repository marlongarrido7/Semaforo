# Utilização de Temporizador Periódico no Raspberry Pi Pico W 2040 - Semáforo

## Autor do Projeto  
Dr. Marlon da Silva Garrido  
Professor Associado IV (CENAMB - PPGEA)  
Universidade Federal do Vale do São Francisco (UNIVASF)  

Este projeto faz parte das atividades do EMBARCATECH 2024/25.  

---

## Índice  

1. Objetivos  
2. Descrição do Projeto  
3. Funcionalidades  
4. Requisitos do Projeto  
5. Diagrama Simplificado de Conexões  
6. Como Executar o Projeto  
7. Estrutura do Código  
8. Trecho de Código  
9. Explicação do Código  
10. Referências  
11. Demonstração em Vídeo  

---

## Objetivos  

- Implementar o uso do temporizador periódico no Raspberry Pi Pico W.  
- Criar um sistema de semáforo automatizado com controle de tempo.  
- Utilizar GPIOs para controlar LEDs e reforçar conceitos de eletrônica e programação embarcada.  
- Exibir informações no terminal serial para monitoramento do funcionamento do semáforo.  

---

## Descrição do Projeto  

O projeto implementa um sistema de semáforo automático utilizando a função `add_repeating_timer_ms()` da **Pico SDK**.  

O semáforo segue a seguinte lógica:  

1. O LED vermelho acende primeiro.  
2. Após 3 segundos, o LED amarelo acende.  
3. Após mais 3 segundos, o LED verde acende.  
4. O ciclo se repete continuamente.  

A mudança de estados é controlada por um **temporizador periódico**, que aciona uma função de callback responsável por alternar os LEDs. Além disso, o loop principal exibe mensagens no terminal serial a cada 1 segundo.  

---

## Funcionalidades  

- O temporizador muda automaticamente o estado do semáforo a cada 3 segundos.  
- Os LEDs alternam entre os estados: vermelho, amarelo e verde.  
- A porta serial exibe mensagens informando o estado atual do semáforo a cada 1 segundo.  
- O código pode ser testado na ferramenta BitDogLab utilizando um LED RGB nos GPIOs 11, 12 e 13.  

---

## Requisitos do Projeto  

- Utilizar o temporizador periódico `add_repeating_timer_ms()` para alternar os estados do semáforo.  
- O loop principal deve imprimir informações sobre o estado do semáforo na porta serial.  
- Os LEDs devem ser controlados pelos GPIOs 13 (vermelho), 12 (amarelo) e 11 (verde).  
- Possibilidade de testar o projeto na ferramenta BitDogLab utilizando um LED RGB.  

---

## Diagrama Simplificado de Conexões  

```
       Raspberry Pi Pico W
    +---------------------------+
    |                           |
    |  GPIO 13 -> LED Vermelho  |
    |  GPIO 12 -> LED Amarelo   |
    |  GPIO 11 -> LED Verde     |
    +---------------------------+
```

Caso esteja utilizando a ferramenta **BitDogLab**, conectar o LED RGB nos GPIOs 11, 12 e 13.  

---

## Como Executar o Projeto  

1. Clonar o repositório:  

```sh
git clone https://github.com/seu-usuario/semaforo-pico.git
cd semaforo-pico
```

2. Configurar o ambiente de desenvolvimento com o **Raspberry Pi Pico SDK**.  

3. Compilar o projeto:  

```sh
mkdir build
cd build
cmake ..
make
```

4. Transferir o arquivo `.uf2` para o **Raspberry Pi Pico W**.  

5. Abrir o terminal serial para visualizar as mensagens:  

```sh
screen /dev/ttyUSB0 115200
```

No Windows, utilizar **PuTTY** ou **Thonny** para monitoramento.  

---

## Estrutura do Código  

```
📁 semaforo-pico
│── 📄 CMakeLists.txt        # Configuração do build
│── 📄 main.c                # Código principal do semáforo
│── 📄 README.md             # Documentação do projeto
│── 📁 build/                # Diretório de compilação
```

---

## Trecho de Código  

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_VERMELHO 13
#define LED_AMARELO  12
#define LED_VERDE    11

// Estado atual do semáforo
volatile int estado_semaforo = 0;  // Começa no vermelho

// Callback do temporizador: alterna o estado do semáforo a cada 3 segundos
bool temporizador_callback(struct repeating_timer *t) {
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    switch (estado_semaforo) {
        case 0: gpio_put(LED_VERMELHO, 1); break;
        case 1: gpio_put(LED_AMARELO, 1); break;
        case 2: gpio_put(LED_VERDE, 1); break;
    }
    
    estado_semaforo = (estado_semaforo + 1) % 3;
    return true;
}

int main() {
    stdio_init_all();
    printf("Inicializando semáforo...\n");

    gpio_init(LED_VERMELHO); gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);  gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);    gpio_set_dir(LED_VERDE, GPIO_OUT);
    
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, temporizador_callback, NULL, &timer);
    
    while (true) {
        printf("Semáforo operando... Estado: %d\n", estado_semaforo);
        sleep_ms(1000);
    }
}
```

---

## Explicação do Código  

1. **Configuração dos pinos dos LEDs**  
   - Define os pinos **GPIO 13, 12 e 11** para os LEDs vermelho, amarelo e verde.  
   - Configura esses pinos como saída.  

2. **Função de callback do temporizador (`temporizador_callback`)**  
   - Desliga todos os LEDs antes de ativar o próximo estado.  
   - Alterna entre os estados do semáforo: vermelho → amarelo → verde → vermelho.  
   - Atualiza a variável `estado_semaforo` para determinar o próximo LED a ser aceso.  

3. **Configuração do temporizador**  
   - A função `add_repeating_timer_ms(3000, temporizador_callback, NULL, &timer);` cria um temporizador que executa a função `temporizador_callback` a cada **3 segundos**.  

4. **Loop principal (`while true`)**  
   - Exibe mensagens na **porta serial** informando o estado atual do semáforo.  
   - Utiliza `sleep_ms(1000)` para imprimir mensagens a cada **1 segundo**.  

---

## Referências  

- **Documentação Oficial do Raspberry Pi Pico SDK**  
  [https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)  

---

## Demonstração em Vídeo  

- Explicação e demonstração do funcionamento do semáforo:  
  [https://youtu.be/HGuRSortY4U](https://youtu.be/HGuRSortY4U)  

---

Se este projeto foi útil para você, não se esqueça de dar uma estrela no GitHub.  

**Contato:**  
Email: marlon.garrido@univasf.edu.br  
Site: [https://www.univasf.edu.br/](https://www.univasf.edu.br/)  
