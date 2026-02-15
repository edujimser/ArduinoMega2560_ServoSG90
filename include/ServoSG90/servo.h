#ifndef SERVO_H
#define SERVO_H

#include "System/pinout/pinout.h"
#include "System/msg/msg.h"
#include "ServoSG90/timmer.h"
#include "System/msg/msg.h"


#define DEBUG_SERVO_SG90  1

constexpr int PINES_VALIDOS_SERVO[] = { 2, 3, 5, 6, 7, 10, 11, 12 };

class ServoMotor {

public :
    //PinInfo pin;
    volatile uint8_t  mask;
    volatile uint8_t* port;
    volatile uint8_t* ddr;

    int angle = 0;
    int ms = 0;
    uint8_t preEscalar = 0;
    int ticks = 0;

    //Estados
    char PinInicializado = 0; 
    char ServoInicializado = 0;

    //Timmer asociado al servo
    Timmer timmerServo;
public :
    // Constructor
    ServoMotor(const PinInfo& pin);
    // Metodo para visualizar el estado del pinout del servo
    void printServoPinOut(const PinInfo& pin);
    // Metodo para mover el servo a un angulo especifico
    bool movimientoAngulo(uint8_t angulo);
    // Metodo para verificar si el pin es compatible con servo
    bool pinesNoDisponibles(const PinInfo& pin);
    // Metodo para imprimir mensaje de pin no disponible
    void printNopinDisponibleParaServo(const PinInfo& pin);
    // Metodo para imprimir texto con formato fijo
    void printFijo(const char* text, uint8_t width); 
};

#endif /* Servo.h */