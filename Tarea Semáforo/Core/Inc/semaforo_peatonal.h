#ifndef INC_SEMAFORO_PEATONAL_H_
#define INC_SEMAFORO_PEATONAL_H_

#include "fsm.h"
#include "timer.h"
#include "main.h"

// Estados del semáforo peatonal
typedef enum {
    VERDE,
    VERDE_PARPADEO,
    ROJO,
    ROJO_PARPADEO
} SemaforoPeatonalState;

// Estructura de control para el semáforo peatonal
typedef struct {
    FSM fsm;                    // FSM para el control del semáforo
    Timer timer;                // Temporizador para transiciones de estado
    GPIO_TypeDef *green_LED_Port;
    uint16_t green_LED_Pin;
    GPIO_TypeDef *red_LED_Port;
    uint16_t red_LED_Pin;
    GPIO_TypeDef *button_Port;
    uint16_t button_Pin;
} SemaforoPeatonal;

// Funciones públicas
void semaforo_peatonal_init(SemaforoPeatonal *semaforo, GPIO_TypeDef *green_LED_Port, uint16_t green_LED_Pin,
                            GPIO_TypeDef *red_LED_Port, uint16_t red_LED_Pin, GPIO_TypeDef *button_Port, uint16_t button_Pin);
void semaforo_peatonal_update(SemaforoPeatonal *semaforo);

#endif /* INC_SEMAFORO_PEATONAL_H_ */
