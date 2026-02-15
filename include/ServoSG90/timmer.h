#ifndef TIMMER_H
#define TIMMER_H

#include "System/pinout/pinout.h"
#include "System/msg/msg.h"                                          

#define DEBUG_SERVO_SG90  1

enum class E_CANAL_OC {
    OC3B = 0,
    OC3C = 1,
    OC0A = 2,
    OC0B = 3,
    OC3A = 4,
    OC4A = 5,
    OC4B = 6,
    OC4C = 7,
    OC2B = 8,
    OC2A = 9,
    OC1A = 10,
    OC1B = 11,
    OC1C = 12,
   
};

enum class E_TIMMER_ASCIOADO {
    TIMMER0 = 0,
    TIMMER1 = 1,
    TIMMER2 = 2,
    TIMMER3 = 3,

};

enum class E_REGISTRO_OCR {
    OCR_3B = 0,
    OCR_3C = 1,
    OCR_0B = 2,
    OCR_3A = 3,
    OCR_4A = 4,
    OCR_4B = 5,
    OCR_4C = 6,
    OCR_2B = 7,
    OCR_2A = 8,
    OCR_1A = 9,
    OCR_1B = 10,
    OCR_1C = 11,
    OCR_0A = 12,
};

enum class E_REGISTRO_ICR {
    NA    = 0,
    ICR_0 = 1,
    ICR_1 = 2,
    ICR_2 = 3,
    ICR_3 = 4,
    ICR_4 = 5,
    ICR_5 = 6,
};

class Timmer {
    public:
        // Información del pin asociado al timer
        PinInfo pin;
        char pinData;
        E_CANAL_OC canalOC;
        uint8_t registroTCCRA;
        uint8_t registroTCCRB;
        E_REGISTRO_OCR registroOCR;
        int registroOCRData;
        E_REGISTRO_ICR registroICR;
        uint16_t registroICRData;

        // Status del timer
        bool isInitialized = false;

    public:
    // Constructor
    Timmer(PinInfo pin) : pin(pin) {
        this->pinData = pin.number;
    };

    public:
        // Método para inicializar el timer asociado al pin
        bool initTimmer();
        // Metodo para visualizar configuracion
        void printTimmerConfig();
        // Metodo para visualizar los bits con ceros a la izquierda
        String bin8(uint8_t val);
};



#endif /* TIMMER_H */
