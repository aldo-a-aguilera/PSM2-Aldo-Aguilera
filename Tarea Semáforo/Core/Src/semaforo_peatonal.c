/*
Tarea: Elaboración de un semáforo peatonal
Aldo Aguilera Diogenez Vera

 */

#include "semaforo_peatonal.h"
#include "timer.h"

// Funciones de condición para las transiciones
static int button_pressed(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    return (HAL_GPIO_ReadPin(semaforo->button_Port, semaforo->button_Pin) == GPIO_PIN_SET);
}

static int timer_expired(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    return timer_has_expired(&semaforo->timer);
}

// Funciones de acción para cada estado
void on_state_verde(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    HAL_GPIO_WritePin(semaforo->green_LED_Port, semaforo->green_LED_Pin, GPIO_PIN_RESET);  // LED verde encendido
    HAL_GPIO_WritePin(semaforo->red_LED_Port, semaforo->red_LED_Pin, GPIO_PIN_SET);        // LED rojo apagado
    timer_restart(&semaforo->timer);
}

void on_state_verde_parpadeo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    timer_update_duration(&semaforo->timer, 1000);  // Parpadeo durante 1 segundo
}

void on_state_rojo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    HAL_GPIO_WritePin(semaforo->green_LED_Port, semaforo->green_LED_Pin, GPIO_PIN_SET);    // LED verde apagado
    HAL_GPIO_WritePin(semaforo->red_LED_Port, semaforo->red_LED_Pin, GPIO_PIN_RESET);      // LED rojo encendido
    timer_update_duration(&semaforo->timer, 3000);  // Estado rojo durante 3 segundos
}

void on_state_rojo_parpadeo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    timer_update_duration(&semaforo->timer, 1000);  // Parpadeo durante 1 segundo
}

// Arreglos de transición para cada estado
static Transition verdeTransitions[] = {
    {button_pressed, VERDE_PARPADEO}  // Botón presionado: transición a VERDE_PARPADEO
};

static Transition verdeParpadeoTransitions[] = {
    {timer_expired, ROJO}  // Temporizador expira: transición a ROJO
};

static Transition rojoTransitions[] = {
    {timer_expired, ROJO_PARPADEO}  // Temporizador expira: transición a ROJO_PARPADEO
};

static Transition rojoParpadeoTransitions[] = {
    {timer_expired, VERDE}  // Temporizador expira: transición a VERDE
};

// Estados de la FSM
static FSMState semaforoStates[] = {
    {verdeTransitions, 1, on_state_verde},
    {verdeParpadeoTransitions, 1, on_state_verde_parpadeo},
    {rojoTransitions, 1, on_state_rojo},
    {rojoParpadeoTransitions, 1, on_state_rojo_parpadeo}
};

// Inicialización de la FSM del semáforo peatonal
void semaforo_peatonal_init(SemaforoPeatonal *semaforo, GPIO_TypeDef *green_LED_Port, uint16_t green_LED_Pin,
                            GPIO_TypeDef *red_LED_Port, uint16_t red_LED_Pin, GPIO_TypeDef *button_Port, uint16_t button_Pin) {
    fsm_init(&semaforo->fsm, semaforoStates, VERDE, semaforo);  // Comienza en el estado VERDE
    semaforo->green_LED_Port = green_LED_Port;
    semaforo->green_LED_Pin = green_LED_Pin;
    semaforo->red_LED_Port = red_LED_Port;
    semaforo->red_LED_Pin = red_LED_Pin;
    semaforo->button_Port = button_Port;
    semaforo->button_Pin = button_Pin;
    timer_start(&semaforo->timer, 0);  // Temporizador inicializado en 0
}

// Actualización de la FSM
void semaforo_peatonal_update(SemaforoPeatonal *semaforo) {
    fsm_update(&semaforo->fsm);
}
