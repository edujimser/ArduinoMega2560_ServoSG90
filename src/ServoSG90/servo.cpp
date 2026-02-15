#include "ServoSG90/servo.h"


// Constructor
ServoMotor::ServoMotor(const PinInfo& pin) 
    : timmerServo(pin) // Inicializar el timmer asociado al pin
{
    standardMessage("Configurando servo motor SG90", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    if (pinesNoDisponibles(pin)){printNopinDisponibleParaServo(pin); return;}
    

    mask = digitalPinToBitMask(pin.number);
    port = portOutputRegister(digitalPinToPort(pin.number));
    ddr  = portModeRegister(digitalPinToPort(pin.number));
    
    //Indicamos al ddr que el pin es de salida
    //Levantamos el bit correspondiente en el registro ddr
    *ddr |= mask;

    //Revisamos la inicialización y ponemos a 0 el pin
    if (!PinInicializado) {
        *port &= ~mask;  //Poner a 0 el pin
         PinInicializado = 1;
    }


    //Constructor timmer y configuración
    this-> ServoInicializado = this->timmerServo.initTimmer();



    #if DEBUG_SERVO_SG90
        printServoPinOut(pin);
        this->timmerServo.printTimmerConfig();
        Serial.print("TCCR3B en loop: "); Serial.println(TCCR3B, BIN);
    #endif
};

bool ServoMotor::movimientoAngulo(uint8_t angulo) {
    this->angle = angulo;

    // Mapear el ángulo (0-180) a un valor OCR (ej. 1000-2000 para 1 µs - 2 µs)
    this->ms = map(angulo, 0, 180, 544, 2400);
    this->preEscalar = 8; //mejorar
    this->ticks = this->ms * 2; // Con prescaler de 8 y tick de 0.5 µs

    switch (this->timmerServo.pin.number) {
    case  2: // OC3B
         OCR3B = this->ticks; // Asignar el valor al registro OCR3B
         this->timmerServo.registroOCRData = OCR3B;
         break;       
    case  3: // OC3C
        OCR3C = this->ticks; // Asignar el valor al registro OCR3C
        this->timmerServo.registroOCRData = OCR3C;
        break; 
    case  5: // OC3A
        OCR3A = this->ticks; // Asignar el valor al registro OCR3A
        this->timmerServo.registroOCRData = OCR3A;
        break; 
    case 6: // OC4A
        OCR4A = this->ticks; // Asignar el valor al registro OCR4A
        this->timmerServo.registroOCRData = OCR4A;
        break; 
    case 7: // OC4B
        OCR4B = this->ticks; // Asignar el valor al registro OCR4B
        this->timmerServo.registroOCRData = OCR4B;
        break; 
    case 8: // OC4C
        OCR4C = this->ticks; // Asignar el valor al registro OCR4
        this->timmerServo.registroOCRData = OCR4C;
        break;
    case 11: // OC1A
        OCR1A = this->ticks; // Asignar el valor al registro OCR1A
        this->timmerServo.registroOCRData = OCR1A;
        break;
    case 12: // OC1B
        OCR1B = this->ticks; // Asignar el valor al registro OCR1
        this->timmerServo.registroOCRData = OCR1B;
        break;
    case 4: // OC0B
        OCR0B = this->ticks; // Asignar el valor al registro OCR0B
        this->timmerServo.registroOCRData = OCR0B;
        break;
    case 13: // OC0A
        OCR0A = this->ticks; // Asignar el valor al registro OCR0A
        this->timmerServo.registroOCRData = OCR0A;
        break;
     case 9: // OC2B
        OCR2B = this->ticks; // Asignar el valor al registro OCR2B
        this->timmerServo.registroOCRData = OCR2B;
        break; 
    default:
        break;
    }
    return true;

};

bool ServoMotor::pinesNoDisponibles(const PinInfo& pin) {
    for (int p : PINES_VALIDOS_SERVO) {
        if (pin.number == p)
            return false;   
    };
     return true;          
};


void ServoMotor::printServoPinOut(const PinInfo& pin) {

    standardMessage("🧪 Configuración PinOut Servo", __FILE__, __FUNCTION__, __DATE__, __TIME__);
    
    Serial.println(F("+----------------------+----------------------+"));
    Serial.println(F("| Campo                | Valor                |"));
    Serial.println(F("+----------------------+----------------------+"));

    Serial.print  (F("| Pin number           | "));
    Serial.print  (pin.number);
    Serial.println(F("                    |")); 

    Serial.print  (F("| Port address         | 0x"));
    Serial.print  ((uint16_t)port, HEX);
    Serial.println(F("                 |"));

    Serial.print  (F("| Mask                 | "));
    Serial.print  (mask, BIN);
    Serial.println(F("              |"));

    Serial.println(F("+----------------------+----------------------+"));

}

void ServoMotor::printNopinDisponibleParaServo(const PinInfo& pin) {

    Serial.println(F("+-------------------+---------------+----------------------------+"));
    Serial.println(F("| PIN NO DISPONIBLE PARA SERVO                                   |"));
    Serial.println(F("+-------------------+---------------+----------------------------+"));
    Serial.println(F("| Nombre            | Número        | Familia                    |"));
    Serial.println(F("+-------------------+---------------+----------------------------+"));

    Serial.print(F("| "));
    printFijo(pin.name, 18);

    Serial.print(F("| "));
    char numStr[8];
    itoa(pin.number, numStr, 10);
    printFijo(numStr, 14);

    Serial.print(F("| "));
    printFijo(pin.family, 27);
    Serial.println(F("|"));

    Serial.println(F("+-------------------+---------------+----------------------------+"));
    Serial.println(F("  Este pin no está permitido para controlar un servo SG90."));
    Serial.println();
}


void ServoMotor::printFijo(const char* text, uint8_t width) {
    uint8_t len = strlen(text);

    for (uint8_t i = 0; i < width; i++) {
        if (i < len)
            Serial.print(text[i]);
        else
            Serial.print(' ');
    }
}
