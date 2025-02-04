// Incluindo as bibliotecas.
#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hardware/timer.h" 
#include "hardware/pio.h"

// Definindo o pino do botão
#define BOTAO_PRIMEIRO 5  

// Variável para armazenar o status dos LEDs.
bool leds_ativados = false;    

// Constantes para os pinos dos LEDs
const uint LED_AZUL = 11;    
const uint LED_VERMELHO = 12;    
const uint LED_VERDE = 13;   

/* Função de callback para desativar o LED azul. */
int64_t desligar_led_azul(alarm_id_t id, void *user_data) {
    gpio_put(LED_AZUL, false); // Desliga o LED azul
    return 0; // Retorna 0 para impedir a repetição do alarme
}

/* Função de callback para desativar o LED vermelho. */
int64_t desligar_led_vermelho(alarm_id_t id, void *user_data) {
    gpio_put(LED_VERMELHO, false); // Desliga o LED vermelho
    return 0; // Retorna 0 para impedir a repetição do alarme
}

/* Função de callback para desativar o LED verde. */
int64_t desligar_led_verde(alarm_id_t id, void *user_data) {
    gpio_put(LED_VERDE, false); // Desliga o LED verde
    leds_ativados = false;       // Atualiza a variável para indicar que os LEDs estão apagados
    return 0;                    
}

/* Função para configurar os pinos e inicializar as portas. */
void configurar_pinos() {
    gpio_init(LED_VERMELHO);               
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);  
    gpio_init(LED_AZUL);                  
    gpio_set_dir(LED_AZUL, GPIO_OUT);    
    gpio_init(LED_VERDE);                 
    gpio_set_dir(LED_VERDE, GPIO_OUT);    
    gpio_init(BOTAO_PRIMEIRO);                    
    gpio_set_dir(BOTAO_PRIMEIRO, GPIO_IN);          
    gpio_pull_up(BOTAO_PRIMEIRO);
}

/* Função para acionar os LEDs e programar os alarmes para desativá-los após um tempo. */
void ativar_leds() {
    gpio_put(LED_VERMELHO, true);        
    gpio_put(LED_AZUL, true);       
    gpio_put(LED_VERDE, true);
    // Programando os alarmes para desligar os LEDs após 3s, 6s e 9s
    add_alarm_in_ms(3000, desligar_led_azul, NULL, false);
    add_alarm_in_ms(6000, desligar_led_vermelho, NULL, false);
    add_alarm_in_ms(9000, desligar_led_verde, NULL, false); 
}

int main() {
    // Inicializa os pinos e configura as portas
    configurar_pinos();

    while (true) {
        // Verifica se o botão foi pressionado e se os LEDs não estão ativados
        if ((!gpio_get(BOTAO_PRIMEIRO)) && (!leds_ativados)) {
            
            sleep_ms(50); // Atraso para evitar leituras múltiplas do botão

            if (!gpio_get(BOTAO_PRIMEIRO)) {
                ativar_leds();         // Aciona os 3 LEDs e configura os alarmes.
                leds_ativados = true;   // Marca a variável como 'verdadeira' para indicar que os LEDs estão ligados.
            }
        }
        sleep_ms(10);
    }
    return 0;
}
