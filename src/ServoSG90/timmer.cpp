#include "ServoSG90/timmer.h"

bool Timmer::initTimmer() {
/*
    Mapeo completo de timers, canales OC y pines en Arduino Mega 2560
    -----------------------------------------------------------------------------------------------
    Pin Arduino | Canal OC | Timer asociado | Puerto/Bit | Registro OCR | Registro ICR | Registro DDR | Tipo Timer | Uso típico
    -----------------------------------------------------------------------------------------------
    2           | OC3B     | Timer3         | PE4        | OCR3B        | ICR3         | DDRE (bit 4) | 16 bits    | Servo / PWM preciso
    3           | OC3C     | Timer3         | PE5        | OCR3C        | ICR3         | DDRE (bit 5) | 16 bits    | Servo / PWM preciso
    4           | OC0B     | Timer0         | PG5        | OCR0B        | ---          | DDRG (bit 5) | 8 bits     | PWM rápido (~976 Hz)
    5           | OC3A     | Timer3         | PE3        | OCR3A        | ICR3         | DDRE (bit 3) | 16 bits    | Servo / PWM preciso
    6           | OC4A     | Timer4         | PH3        | OCR4A        | ICR4         | DDRH (bit 3) | 16 bits    | Servo / PWM preciso
    7           | OC4B     | Timer4         | PH4        | OCR4B        | ICR4         | DDRH (bit 4) | 16 bits    | Servo / PWM preciso
    8           | OC4C     | Timer4         | PH5        | OCR4C        | ICR4         | DDRH (bit 5) | 16 bits    | Servo / PWM preciso
    9           | OC2B     | Timer2         | PB3        | OCR2B        | ---          | DDRB (bit 3) | 8 bits     | PWM rápido (~976 Hz)
    10          | OC2A     | Timer2         | PB4        | OCR2A        | ---          | DDRB (bit 4) | 8 bits     | PWM rápido (~976 Hz)
    11          | OC1A     | Timer1         | PB5        | OCR1A        | ICR1         | DDRB (bit 5) | 16 bits    | Servo / PWM preciso
    12          | OC1B     | Timer1         | PB6        | OCR1B        | ICR1         | DDRB (bit 6) | 16 bits    | Servo / PWM preciso
    13          | OC0A     | Timer0         | PB7        | OCR0A        | ---          | DDRB (bit 7) | 8 bits     | PWM rápido (~976 Hz)

    Explicación de columnas:
    - Pin Arduino: número de pin digital en la placa Mega 2560.
    - Canal OC: salida de comparación del timer (Output Compare). Cada timer tiene varios canales (A, B, C).
    - Timer asociado: indica qué temporizador controla ese pin (Timer0–Timer5).
    - Puerto/Bit: nombre del puerto interno del ATmega2560 y el bit específico que corresponde al pin.
    - Registro OCR: registro de comparación que define el ancho del pulso (duty cycle).
    - Registro ICR: registro que define el valor TOP (solo en timers de 16 bits). Permite fijar el periodo de la señal.
    - Registro DDR: registro de dirección de datos. Se debe activar el bit correspondiente para configurar el pin como salida.
    - Tipo Timer: tamaño del temporizador (8 bits o 16 bits). Los de 16 bits permiten periodos largos y mayor resolución.
    - Uso típico: aplicaciones comunes. Los timers de 16 bits se usan para servos o PWM preciso; los de 8 bits para PWM rápido en LEDs, ventiladores, etc.

    Notas:
    - Los timers de 16 bits (Timer1, Timer3, Timer4, Timer5) permiten configurar periodos largos (ej. 20 ms para servos).
    - Los timers de 8 bits (Timer0 y Timer2) tienen resolución limitada (0–255 ticks), útiles para PWM rápido en LEDs o ventiladores.
    - El registro OCRnx controla el ancho del pulso (duty cycle).
    - El registro ICRn (solo en timers de 16 bits) define el TOP → periodo de la señal.
    - Para habilitar el pin como salida, se activa el bit correspondiente en su DDR (ej. pin 2 → DDRE |= (1 << PE4)).
    - Los pines 2–13 son los PWM más comunes en Arduino Mega, pero existen más en otros puertos (ej. Timer5 en pines 44–46).
*/


    switch (pin.number){
        case 2: // OC3B → Timer3 canal B. Configuración del Timer3 para Fast PWM con ICR3 como TOP
        case 3: // OC3C → Timer3 canal C. Configuración del Timer3 para Fast PWM con ICR3 como TOP
        case 5: // OC3A → Timer3 canal A. Configuración del Timer3 para Fast PWM con ICR3 como TOP
        /*
        Tabla de combinaciones de WGM30–WGM33 para Timer3 (ATmega2560) Waveform Generation Mode (Modo de Generación de Ondas)
        --------------------------------------------------------------
        Estos 4 bits (WGM33, WGM32, WGM31, WGM30) configuran el modo de operación del Timer3.
        El modo define cómo cuenta el timer y cómo genera la señal PWM o interrupciones.

        WGM33 | WGM32 | WGM31 | WGM30 | Modo seleccionado                  | Descripción / Uso típico
        --------------------------------------------------------------------------------------------
        0       0       0       0    Normal                              Contador libre, desborda en 0xFFFF
        0       1       0       0    CTC (TOP = OCRnA)                   Temporizador, reinicia al llegar a OCRnA
        0       0       1       0    Phase Correct PWM (TOP = OCRnA)     PWM simétrico, periodo definido por OCRnA
        0       1       1       0    Fast PWM (TOP = OCRnA)              PWM rápido, periodo definido por OCRnA
        1       0       1       0    Phase Correct PWM (TOP = ICRn)      PWM simétrico, periodo definido por ICRn
        1       1       0       0    CTC (TOP = ICRn)                    Temporizador, reinicia al llegar a ICRn
        1       1       1       0    Fast PWM (TOP = ICRn)               PWM rápido, periodo definido por ICRn (ideal para servos)
        0       0       0       1    Phase Correct PWM (TOP = 0xFF)      PWM simétrico de 8 bits
        0       0       1       1    Phase Correct PWM (TOP = 0x1FF)     PWM simétrico de 9 bits
        0       1       0       1    Phase Correct PWM (TOP = 0x3FF)     PWM simétrico de 10 bits
        0       0       1       1    Fast PWM (TOP = 0xFF)               PWM rápido de 8 bits
        0       1       1       1    Fast PWM (TOP = 0x1FF)              PWM rápido de 9 bits
        1       0       0       1    Fast PWM (TOP = 0x3FF)              PWM rápido de 10 bits

        Notas:
        - Para servos se usa normalmente: WGM33=1, WGM32=1, WGM31=1, WGM30=0 → Fast PWM con TOP = ICRn.
        - En ese modo, ICRn define el periodo (ej. 20 ms) y OCRnx define el ancho del pulso (ej. 1–2 ms).
        - Los modos con TOP fijo (0xFF, 0x1FF, 0x3FF) son útiles para PWM rápido en LEDs o ventiladores.
        - Los modos Phase Correct generan una señal más simétrica, pero con frecuencia más baja.
        */

        //Selección del canal OC basado en el pin
        if     (pin.number == 2)this->canalOC = E_CANAL_OC::OC3B;
        else if(pin.number == 3)this->canalOC = E_CANAL_OC::OC3C;
        else if(pin.number == 5)this->canalOC = E_CANAL_OC::OC3A;

        /*
        Diferencia entre TCCRnA y TCCRnB:

        - TCCRnA:
            * Bits WGMn0 y WGMn1 → seleccionan parte del modo de generación de onda.
            * Bits COMnx1 y COMnx0 → definen cómo se comporta la salida OCnx (PWM normal, invertido, toggle).
            * En resumen: controla la "forma" de la señal.

        - TCCRnB:
            * Bits WGMn2 y WGMn3 → completan la selección del modo de operación.
            * Bits CSn2, CSn1, CSn0 → seleccionan el prescaler (divisor de frecuencia del reloj).
            * En resumen: controla la "velocidad" del conteo del timer.

        Ejemplo:
            TCCR3A = (1 << WGM31) | (1 << COM3B1);   // Configura modo PWM y salida OC3B
            TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); // Configura Fast PWM con TOP=ICR3 y prescaler=8
        */
        
        //Formato Fast PWM con ICR3 como TOP
        TCCR3A &= ~((1 << WGM30) | (1 << WGM31));
        TCCR3B &= ~((1 << WGM32) | (1 << WGM33));
        //Formato prescaler: limpiar bits CS32, CS31, CS30 
        TCCR3B &= ~((1 << CS30) | (1 << CS31) | (1 << CS32));

        //Configurar modo Fast PWM con ICR3 como TOP
        TCCR3A |= (1 << WGM31);
        TCCR3B |= (1 << WGM33) | (1 << WGM32);
        
        
        /*
        Tabla de modos COMnx1:COMnx0 (Compare Output Mode)
        --------------------------------------------------
        Estos bits (COMnx1 y COMnx0) definen cómo se comporta la salida OCnx
        (ejemplo: OC3B → pin 2 en Arduino Mega) cuando el contador del timer
        alcanza el valor de comparación (OCRnx).

        COMnx1 | COMnx0 | Modo de salida en OCnx                  | Descripción detallada / Uso típico
        -----------------------------------------------------------------------------------------------
        0        0     Desconectado                             El pin no cambia, salida desconectada del timer.
                                                                Útil si solo quieres usar el timer para interrupciones.

        0        1     Toggle                                   El pin cambia de estado (HIGH ↔ LOW) cada vez que
                                                                el contador coincide con OCRnx. Útil para generar
                                                                señales cuadradas o medir frecuencia.

        1        0     Clear on Compare, Set at TOP             El pin se pone en LOW cuando el contador alcanza OCRnx
                                                                y se pone en HIGH cuando el contador reinicia en TOP.
                                                                Este es el modo típico para generar PWM normal.

        1        1     Set on Compare, Clear at TOP             El pin se pone en HIGH cuando el contador alcanza OCRnx
                                                                y se pone en LOW cuando el contador reinicia en TOP.
                                                                Es el modo PWM invertido (la lógica se invierte).

        Notas importantes:
        - "TOP" depende del modo WGM seleccionado:
            * En timers de 8 bits (Timer0, Timer2) → TOP = 0xFF (255).
            * En timers de 16 bits (Timer1, Timer3, Timer4, Timer5) → TOP puede ser OCRnA o ICRn.
        - El valor OCRnx define el instante de comparación → controla el ancho del pulso.
        - Para PWM normal se usa COMnx1=1 y COMnx0=0.
        - Para PWM invertido se usa COMnx1=1 y COMnx0=1.
        - El modo Toggle (0,1) es útil para generar señales de prueba o medir frecuencia con un osciloscopio.
        */


        if (pin.number == 2){
            //Formatear Compare Output Mode para OC3B (pin 2)
            TCCR3A &= ~((1 << COM3B0) | (1 << COM3B1));
            // Configurar modo Clear on Compare, Set at TOP para OC3B
            TCCR3A |= (1 << COM3B1);
            this->registroTCCRA = TCCR3A;
        }else if(pin.number == 3){
            //Formatear Compare Output Mode para OC3C (pin 3)
            TCCR3A &= ~((1 << COM3C0) | (1 << COM3C1));
            // Configurar modo Clear on Compare, Set at TOP para OC3C
            TCCR3A |= (1 << COM3C1);
            this->registroTCCRA = TCCR3A;
        }else if(pin.number == 5){
            //Formatear Compare Output Mode para OC3A (pin 5)
            TCCR3A &= ~((1 << COM3A0) | (1 << COM3A1));
            // Configurar modo Clear on Compare, Set at TOP para OC3A
            TCCR3A |= (1 << COM3A1);
            this->registroTCCRA = TCCR3A;
        }

        /*
        Tabla de selección de reloj (Clock Select) para Timer3 (ATmega2560)
        ------------------------------------------------------------------
        Los bits CS32, CS31 y CS30 definen la fuente de reloj y el prescaler del timer.
        La combinación de estos bits determina la velocidad a la que el contador avanza.

        CS32 | CS31 | CS30 | Fuente de reloj seleccionada
        -------------------------------------------------
        0     0     0    No clock (timer detenido)
        0     0     1    clk/1   → sin prescaler (16 MHz → tick = 62.5 ns)
        0     1     0    clk/8   → prescaler = 8 (2 MHz → tick = 0.5 µs)
        0     1     1    clk/64  → prescaler = 64 (250 kHz → tick = 4 µs)
        1     0     0    clk/256 → prescaler = 256 (62.5 kHz → tick = 16 µs)
        1     0     1    clk/1024→ prescaler = 1024 (15.6 kHz → tick = 64 µs)
        1     1     0    Fuente externa en pin Tn (flanco descendente)
        1     1     1    Fuente externa en pin Tn (flanco ascendente)

        Notas:
        - (1 << CS31) activa el bit CS31 → combinación 010 → prescaler = 8.
        - Con prescaler = 8, el reloj del timer pasa de 16 MHz a 2 MHz.
        - Cada tick dura 0.5 µs, lo que permite generar periodos largos como 20 ms (ideal para servos).
        - Ejemplo: ICR3 = 40000 → 40000 × 0.5 µs = 20 ms de periodo.
    */
        TCCR3B |= (1 << CS31); // Prescaler de 8
        this->registroTCCRB = TCCR3B; 

        // Periodo de 20 ms → 40000 ticks × 0.5 µs  
        ICR3 = 40000; 
        this->registroICR = E_REGISTRO_ICR::ICR_3;
        this->registroICRData = ICR3;

        // Pulso inicial de 1.5 ms → 3000 ticks 
        if (pin.number == 2){
            OCR3B = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_3B;
            this->registroOCRData = OCR3B;
        }else if(pin.number == 3){
            OCR3C = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_3C;
            this->registroOCRData = OCR3C;
        }else if(pin.number == 5){
            OCR3A = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_3A;
            this->registroOCRData = OCR3A;
        }
        //Pin configurado ok
        return true;
    break;
    
    case 6: // OC4A → Timer4 canal B. Configuración del Timer4 para Fast PWM con ICR4 como TOP
    case 7: // OC4C → Timer4 canal C. Configuración del Timer4 para Fast PWM con ICR4 como TOP
    case 8: // OC4B → Timer4 canal A. Configuración del Timer4 para Fast PWM con ICR4 como TOP
        /*
        Tabla de combinaciones de WGM30–WGM33 para Timer3 (ATmega2560) Waveform Generation Mode (Modo de Generación de Ondas)
        --------------------------------------------------------------
        Estos 4 bits (WGM33, WGM32, WGM31, WGM30) configuran el modo de operación del Timer3.
        El modo define cómo cuenta el timer y cómo genera la señal PWM o interrupciones.

        WGM33 | WGM32 | WGM31 | WGM30 | Modo seleccionado                  | Descripción / Uso típico
        --------------------------------------------------------------------------------------------
        0       0       0       0    Normal                              Contador libre, desborda en 0xFFFF
        0       1       0       0    CTC (TOP = OCRnA)                   Temporizador, reinicia al llegar a OCRnA
        0       0       1       0    Phase Correct PWM (TOP = OCRnA)     PWM simétrico, periodo definido por OCRnA
        0       1       1       0    Fast PWM (TOP = OCRnA)              PWM rápido, periodo definido por OCRnA
        1       0       1       0    Phase Correct PWM (TOP = ICRn)      PWM simétrico, periodo definido por ICRn
        1       1       0       0    CTC (TOP = ICRn)                    Temporizador, reinicia al llegar a ICRn
        1       1       1       0    Fast PWM (TOP = ICRn)               PWM rápido, periodo definido por ICRn (ideal para servos)
        0       0       0       1    Phase Correct PWM (TOP = 0xFF)      PWM simétrico de 8 bits
        0       0       1       1    Phase Correct PWM (TOP = 0x1FF)     PWM simétrico de 9 bits
        0       1       0       1    Phase Correct PWM (TOP = 0x3FF)     PWM simétrico de 10 bits
        0       0       1       1    Fast PWM (TOP = 0xFF)               PWM rápido de 8 bits
        0       1       1       1    Fast PWM (TOP = 0x1FF)              PWM rápido de 9 bits
        1       0       0       1    Fast PWM (TOP = 0x3FF)              PWM rápido de 10 bits

        Notas:
        - Para servos se usa normalmente: WGM33=1, WGM32=1, WGM31=1, WGM30=0 → Fast PWM con TOP = ICRn.
        - En ese modo, ICRn define el periodo (ej. 20 ms) y OCRnx define el ancho del pulso (ej. 1–2 ms).
        - Los modos con TOP fijo (0xFF, 0x1FF, 0x3FF) son útiles para PWM rápido en LEDs o ventiladores.
        - Los modos Phase Correct generan una señal más simétrica, pero con frecuencia más baja.
        */

        //Selección del canal OC basado en el pin
        if     (pin.number == 6)this->canalOC = E_CANAL_OC::OC4A;
        else if(pin.number == 7)this->canalOC = E_CANAL_OC::OC4B;
        else if(pin.number == 8)this->canalOC = E_CANAL_OC::OC4C;

        /*
        Diferencia entre TCCRnA y TCCRnB:

        - TCCRnA:
            * Bits WGMn0 y WGMn1 → seleccionan parte del modo de generación de onda.
            * Bits COMnx1 y COMnx0 → definen cómo se comporta la salida OCnx (PWM normal, invertido, toggle).
            * En resumen: controla la "forma" de la señal.

        - TCCRnB:
            * Bits WGMn2 y WGMn3 → completan la selección del modo de operación.
            * Bits CSn2, CSn1, CSn0 → seleccionan el prescaler (divisor de frecuencia del reloj).
            * En resumen: controla la "velocidad" del conteo del timer.

        Ejemplo:
            TCCR3A = (1 << WGM31) | (1 << COM3B1);   // Configura modo PWM y salida OC3B
            TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); // Configura Fast PWM con TOP=ICR3 y prescaler=8
        */
        
        //Formato Fast PWM con ICR4 como TOP
        TCCR4A &= ~((1 << WGM40) | (1 << WGM41));
        TCCR4B &= ~((1 << WGM42) | (1 << WGM43));
        //Formato prescaler: limpiar bits CS42, CS41, CS40 
        TCCR4B &= ~((1 << CS40) | (1 << CS41) | (1 << CS42));

        //Configurar modo Fast PWM con ICR4 como TOP
        TCCR4A |= (1 << WGM41);
        TCCR4B |= (1 << WGM43) | (1 << WGM42);
        
        
        /*
        Tabla de modos COMnx1:COMnx0 (Compare Output Mode)
        --------------------------------------------------
        Estos bits (COMnx1 y COMnx0) definen cómo se comporta la salida OCnx
        (ejemplo: OC3B → pin 2 en Arduino Mega) cuando el contador del timer
        alcanza el valor de comparación (OCRnx).

        COMnx1 | COMnx0 | Modo de salida en OCnx                  | Descripción detallada / Uso típico
        -----------------------------------------------------------------------------------------------
        0        0     Desconectado                             El pin no cambia, salida desconectada del timer.
                                                                Útil si solo quieres usar el timer para interrupciones.

        0        1     Toggle                                   El pin cambia de estado (HIGH ↔ LOW) cada vez que
                                                                el contador coincide con OCRnx. Útil para generar
                                                                señales cuadradas o medir frecuencia.

        1        0     Clear on Compare, Set at TOP             El pin se pone en LOW cuando el contador alcanza OCRnx
                                                                y se pone en HIGH cuando el contador reinicia en TOP.
                                                                Este es el modo típico para generar PWM normal.

        1        1     Set on Compare, Clear at TOP             El pin se pone en HIGH cuando el contador alcanza OCRnx
                                                                y se pone en LOW cuando el contador reinicia en TOP.
                                                                Es el modo PWM invertido (la lógica se invierte).

        Notas importantes:
        - "TOP" depende del modo WGM seleccionado:
            * En timers de 8 bits (Timer0, Timer2) → TOP = 0xFF (255).
            * En timers de 16 bits (Timer1, Timer3, Timer4, Timer5) → TOP puede ser OCRnA o ICRn.
        - El valor OCRnx define el instante de comparación → controla el ancho del pulso.
        - Para PWM normal se usa COMnx1=1 y COMnx0=0.
        - Para PWM invertido se usa COMnx1=1 y COMnx0=1.
        - El modo Toggle (0,1) es útil para generar señales de prueba o medir frecuencia con un osciloscopio.
        */


        if (pin.number == 6){
            //Formatear Compare Output Mode para OC4A (pin 6)
            TCCR4A &= ~((1 << COM4A0) | (1 << COM4A1));
            // Configurar modo Clear on Compare, Set at TOP para OC4A
            TCCR4A |= (1 << COM4A1);
            this->registroTCCRA = TCCR4A;
        }else if(pin.number == 7){
            //Formatear Compare Output Mode para OC4C (pin 3)
            TCCR4A &= ~((1 << COM4B0) | (1 << COM4B1));
            // Configurar modo Clear on Compare, Set at TOP para OC4C
            TCCR4A |= (1 << COM4B1);
            this->registroTCCRA = TCCR4A;
        }else if(pin.number == 8){
            //Formatear Compare Output Mode para OC4B (pin 5)
            TCCR4A &= ~((1 << COM4C0) | (1 << COM4C1));
            // Configurar modo Clear on Compare, Set at TOP para OC4B
            TCCR4A |= (1 << COM4C1);
            this->registroTCCRA = TCCR4A;
        }

        /*
        Tabla de selección de reloj (Clock Select) para Timer3 (ATmega2560)
        ------------------------------------------------------------------
        Los bits CS32, CS31 y CS30 definen la fuente de reloj y el prescaler del timer.
        La combinación de estos bits determina la velocidad a la que el contador avanza.

        CS32 | CS31 | CS30 | Fuente de reloj seleccionada
        -------------------------------------------------
        0     0     0    No clock (timer detenido)
        0     0     1    clk/1   → sin prescaler (16 MHz → tick = 62.5 ns)
        0     1     0    clk/8   → prescaler = 8 (2 MHz → tick = 0.5 µs)
        0     1     1    clk/64  → prescaler = 64 (250 kHz → tick = 4 µs)
        1     0     0    clk/256 → prescaler = 256 (62.5 kHz → tick = 16 µs)
        1     0     1    clk/1024→ prescaler = 1024 (15.6 kHz → tick = 64 µs)
        1     1     0    Fuente externa en pin Tn (flanco descendente)
        1     1     1    Fuente externa en pin Tn (flanco ascendente)

        Notas:
        - (1 << CS31) activa el bit CS31 → combinación 010 → prescaler = 8.
        - Con prescaler = 8, el reloj del timer pasa de 16 MHz a 2 MHz.
        - Cada tick dura 0.5 µs, lo que permite generar periodos largos como 20 ms (ideal para servos).
        - Ejemplo: ICR3 = 40000 → 40000 × 0.5 µs = 20 ms de periodo.
    */
        TCCR4B |= (1 << CS31); // Prescaler de 8
        this->registroTCCRB = TCCR4B; 

        // Periodo de 20 ms → 40000 ticks × 0.5 µs  
        ICR4 = 40000; 
        this->registroICR = E_REGISTRO_ICR::ICR_4;
        this->registroICRData = ICR4;

        // Pulso inicial de 1.5 ms → 3000 ticks 
        if (pin.number == 6){
            OCR4A = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_4A;
            this->registroOCRData = OCR4A;
        }else if(pin.number == 7){
            OCR4B = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_4B;
            this->registroOCRData = OCR4B;
        }else if(pin.number == 8){
            OCR4C = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_4C;
            this->registroOCRData = OCR4C;
        }

        //Pin configurado ok
        return true;
    break;

    case 11: // OC1A → Timer1 canal A. Configuración del Timer1 para Fast PWM con ICR1 como TOP
    case 12: // OC1B → Timer1 canal B. Configuración del Timer1 para Fast PWM con ICR1 como TOP
        /*
        Tabla de combinaciones de WGM30–WGM33 para Timer3 (ATmega2560) Waveform Generation Mode (Modo de Generación de Ondas)
        --------------------------------------------------------------
        Estos 4 bits (WGM33, WGM32, WGM31, WGM30) configuran el modo de operación del Timer3.
        El modo define cómo cuenta el timer y cómo genera la señal PWM o interrupciones.

        WGM33 | WGM32 | WGM31 | WGM30 | Modo seleccionado                  | Descripción / Uso típico
        --------------------------------------------------------------------------------------------
        0       0       0       0    Normal                              Contador libre, desborda en 0xFFFF
        0       1       0       0    CTC (TOP = OCRnA)                   Temporizador, reinicia al llegar a OCRnA
        0       0       1       0    Phase Correct PWM (TOP = OCRnA)     PWM simétrico, periodo definido por OCRnA
        0       1       1       0    Fast PWM (TOP = OCRnA)              PWM rápido, periodo definido por OCRnA
        1       0       1       0    Phase Correct PWM (TOP = ICRn)      PWM simétrico, periodo definido por ICRn
        1       1       0       0    CTC (TOP = ICRn)                    Temporizador, reinicia al llegar a ICRn
        1       1       1       0    Fast PWM (TOP = ICRn)               PWM rápido, periodo definido por ICRn (ideal para servos)
        0       0       0       1    Phase Correct PWM (TOP = 0xFF)      PWM simétrico de 8 bits
        0       0       1       1    Phase Correct PWM (TOP = 0x1FF)     PWM simétrico de 9 bits
        0       1       0       1    Phase Correct PWM (TOP = 0x3FF)     PWM simétrico de 10 bits
        0       0       1       1    Fast PWM (TOP = 0xFF)               PWM rápido de 8 bits
        0       1       1       1    Fast PWM (TOP = 0x1FF)              PWM rápido de 9 bits
        1       0       0       1    Fast PWM (TOP = 0x3FF)              PWM rápido de 10 bits

        Notas:
        - Para servos se usa normalmente: WGM33=1, WGM32=1, WGM31=1, WGM30=0 → Fast PWM con TOP = ICRn.
        - En ese modo, ICRn define el periodo (ej. 20 ms) y OCRnx define el ancho del pulso (ej. 1–2 ms).
        - Los modos con TOP fijo (0xFF, 0x1FF, 0x3FF) son útiles para PWM rápido en LEDs o ventiladores.
        - Los modos Phase Correct generan una señal más simétrica, pero con frecuencia más baja.
        */

        //Selección del canal OC basado en el pin
        if     (pin.number == 11)this->canalOC = E_CANAL_OC::OC1A;
        else if(pin.number == 12)this->canalOC = E_CANAL_OC::OC1B;


        /*
        Diferencia entre TCCRnA y TCCRnB:

        - TCCRnA:
            * Bits WGMn0 y WGMn1 → seleccionan parte del modo de generación de onda.
            * Bits COMnx1 y COMnx0 → definen cómo se comporta la salida OCnx (PWM normal, invertido, toggle).
            * En resumen: controla la "forma" de la señal.

        - TCCRnB:
            * Bits WGMn2 y WGMn3 → completan la selección del modo de operación.
            * Bits CSn2, CSn1, CSn0 → seleccionan el prescaler (divisor de frecuencia del reloj).
            * En resumen: controla la "velocidad" del conteo del timer.

        Ejemplo:
            TCCR3A = (1 << WGM31) | (1 << COM3B1);   // Configura modo PWM y salida OC3B
            TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); // Configura Fast PWM con TOP=ICR3 y prescaler=8
        */
        
        //Formato Fast PWM con ICR1 como TOP
        TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
        TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
        //Formato prescaler: limpiar bits CS42, CS41, CS40 
        TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));

        //Configurar modo Fast PWM con ICR1 como TOP
        TCCR1A |= (1 << WGM11);
        TCCR1B |= (1 << WGM13) | (1 << WGM12);
        
        
        /*
        Tabla de modos COMnx1:COMnx0 (Compare Output Mode)
        --------------------------------------------------
        Estos bits (COMnx1 y COMnx0) definen cómo se comporta la salida OCnx
        (ejemplo: OC3B → pin 2 en Arduino Mega) cuando el contador del timer
        alcanza el valor de comparación (OCRnx).

        COMnx1 | COMnx0 | Modo de salida en OCnx                  | Descripción detallada / Uso típico
        -----------------------------------------------------------------------------------------------
        0        0     Desconectado                             El pin no cambia, salida desconectada del timer.
                                                                Útil si solo quieres usar el timer para interrupciones.

        0        1     Toggle                                   El pin cambia de estado (HIGH ↔ LOW) cada vez que
                                                                el contador coincide con OCRnx. Útil para generar
                                                                señales cuadradas o medir frecuencia.

        1        0     Clear on Compare, Set at TOP             El pin se pone en LOW cuando el contador alcanza OCRnx
                                                                y se pone en HIGH cuando el contador reinicia en TOP.
                                                                Este es el modo típico para generar PWM normal.

        1        1     Set on Compare, Clear at TOP             El pin se pone en HIGH cuando el contador alcanza OCRnx
                                                                y se pone en LOW cuando el contador reinicia en TOP.
                                                                Es el modo PWM invertido (la lógica se invierte).

        Notas importantes:
        - "TOP" depende del modo WGM seleccionado:
            * En timers de 8 bits (Timer0, Timer2) → TOP = 0xFF (255).
            * En timers de 16 bits (Timer1, Timer3, Timer4, Timer5) → TOP puede ser OCRnA o ICRn.
        - El valor OCRnx define el instante de comparación → controla el ancho del pulso.
        - Para PWM normal se usa COMnx1=1 y COMnx0=0.
        - Para PWM invertido se usa COMnx1=1 y COMnx0=1.
        - El modo Toggle (0,1) es útil para generar señales de prueba o medir frecuencia con un osciloscopio.
        */


        if (pin.number == 11){
            //Formatear Compare Output Mode para OC1A (pin 11)
            TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1));
            // Configurar modo Clear on Compare, Set at TOP para OC1A
            TCCR1A |= (1 << COM1A1);
            this->registroTCCRA = TCCR1A;
        }else if(pin.number == 12){
            //Formatear Compare Output Mode para OC1B (pin 12)
            TCCR1A &= ~((1 << COM1B0) | (1 << COM1B1));
            // Configurar modo Clear on Compare, Set at TOP para OC1B
            TCCR1A |= (1 << COM1B1);
            this->registroTCCRA = TCCR1A;
        }

        /*
        Tabla de selección de reloj (Clock Select) para Timer3 (ATmega2560)
        ------------------------------------------------------------------
        Los bits CS32, CS31 y CS30 definen la fuente de reloj y el prescaler del timer.
        La combinación de estos bits determina la velocidad a la que el contador avanza.

        CS32 | CS31 | CS30 | Fuente de reloj seleccionada
        -------------------------------------------------
        0     0     0    No clock (timer detenido)
        0     0     1    clk/1   → sin prescaler (16 MHz → tick = 62.5 ns)
        0     1     0    clk/8   → prescaler = 8 (2 MHz → tick = 0.5 µs)
        0     1     1    clk/64  → prescaler = 64 (250 kHz → tick = 4 µs)
        1     0     0    clk/256 → prescaler = 256 (62.5 kHz → tick = 16 µs)
        1     0     1    clk/1024→ prescaler = 1024 (15.6 kHz → tick = 64 µs)
        1     1     0    Fuente externa en pin Tn (flanco descendente)
        1     1     1    Fuente externa en pin Tn (flanco ascendente)

        Notas:
        - (1 << CS31) activa el bit CS31 → combinación 010 → prescaler = 8.
        - Con prescaler = 8, el reloj del timer pasa de 16 MHz a 2 MHz.
        - Cada tick dura 0.5 µs, lo que permite generar periodos largos como 20 ms (ideal para servos).
        - Ejemplo: ICR3 = 40000 → 40000 × 0.5 µs = 20 ms de periodo.
    */
        TCCR1B |= (1 << CS11); // Prescaler de 8
        this->registroTCCRB = TCCR1B; 

        // Periodo de 20 ms → 40000 ticks × 0.5 µs  
        ICR1 = 40000; 
        this->registroICR = E_REGISTRO_ICR::ICR_1;
        this->registroICRData = ICR1;

        // Pulso inicial de 1.5 ms → 3000 ticks 
        if (pin.number == 11){
            OCR1A = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_1A;
            this->registroOCRData = OCR1A;
        }else if(pin.number == 12){
            OCR1B = 3000;
            this->registroOCR = E_REGISTRO_OCR::OCR_1B;
            this->registroOCRData = OCR1B;
        }

        //Pin configurado ok
        return true;
        break;

    default:
        break;
    }
    
    //Pin no compatible con timer
    return false; 
}



// Metodo para visualizar configuracion
void Timmer::printTimmerConfig() {
    #if DEBUG_SERVO_SG90 == 1
    
    standardMessage("🧪 Configuración Timmer Servo", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    Serial.println("Configuración del Timer para el pin: " + String(this->pin.number));
    Serial.println("Canal OC asociado: " + String(static_cast<int>(this->canalOC)));
    Serial.println("");

    // Mostrar binario completo -------------------------------------------------------------------------------
    Serial.println("Registro TCCR3A (bin): " + bin8(this->registroTCCRA));
    Serial.println("Registro TCCR3B (bin): " + bin8(this->registroTCCRB));
    Serial.println("");

    // Tabla TCCR3A -------------------------------------------------------------------------------
    Serial.println("Tabla de bits TCCR3A");
    Serial.println("| Bit | Nombre   | Valor | Significado                          |");
    Serial.println("|-----|----------|-------|--------------------------------------|");
    Serial.println("| 7   | COM3A1   | " + String((this->registroTCCRA >> COM3A1) & 1) + "     | Control salida OC3A");
    Serial.println("| 6   | COM3A0   | " + String((this->registroTCCRA >> COM3A0) & 1) + "     | Control salida OC3A");
    Serial.println("| 5   | COM3B1   | " + String((this->registroTCCRA >> COM3B1) & 1) + "     | Control salida OC3B");
    Serial.println("| 4   | COM3B0   | " + String((this->registroTCCRA >> COM3B0) & 1) + "     | Control salida OC3B");
    Serial.println("| 3   | COM3C1   | " + String((this->registroTCCRA >> COM3C1) & 1) + "     | Control salida OC3C");
    Serial.println("| 2   | COM3C0   | " + String((this->registroTCCRA >> COM3C0) & 1) + "     | Control salida OC3C");
    Serial.println("| 1   | WGM31    | " + String((this->registroTCCRA >> WGM31) & 1) + "     | Modo generación onda (parte baja)");
    Serial.println("| 0   | WGM30    | " + String((this->registroTCCRA >> WGM30) & 1) + "     | Modo generación onda (parte baja)");
    Serial.println("");

    // Tabla de modos WGM33:WGM30 -------------------------------------------------------------------------------
    Serial.println("Tabla de modos COMnx1:COMnx0");
    Serial.println("| Canal | COMnx1 | COMnx0 | Modo                         | Descripción");
    Serial.println("|-------|--------|--------|------------------------------|----------------------------------------------|");

    // --- CANAL A -------------------------------------------------------------------------------
    if (this->canalOC == E_CANAL_OC::OC3A){
        int c1 = (this->registroTCCRA >> COM3A1) & 1;
        int c0 = (this->registroTCCRA >> COM3A0) & 1;

        String modo = (c1==0 && c0==0) ? "Desconectado" :
                      (c1==0 && c0==1) ? "Toggle" :
                      (c1==1 && c0==0) ? "Clear/Set (PWM normal)" :
                                         "Set/Clear (PWM invertido)";

        String activo = (this->canalOC == E_CANAL_OC::OC3A) ? " <== ACTIVO" : "";

        Serial.println("| OC3A  |   " + String(c1) + "    |   " + String(c0) +
                        "    | " + modo + "        | Control del pin OC3A" + activo);
    };

    // --- CANAL B -------------------------------------------------------------------------------
    if (this->canalOC == E_CANAL_OC::OC3B){
        int c1 = (this->registroTCCRA >> COM3B1) & 1;
        int c0 = (this->registroTCCRA >> COM3B0) & 1;

        String modo = (c1==0 && c0==0) ? "Desconectado" :
                      (c1==0 && c0==1) ? "Toggle" :
                      (c1==1 && c0==0) ? "Clear/Set (PWM normal)" :
                                         "Set/Clear (PWM invertido)";

        String activo = (this->canalOC == E_CANAL_OC::OC3B) ? " <== ACTIVO" : "";

        Serial.println("| OC3B  |   " + String(c1) + "    |   " + String(c0) +
                        "    | " + modo + "       | Control del pin OC3B" + activo);
    }

    // --- CANAL C -------------------------------------------------------------------------------
    if (this->canalOC == E_CANAL_OC::OC3C){
        int c1 = (this->registroTCCRA >> COM3C1) & 1;
        int c0 = (this->registroTCCRA >> COM3C0) & 1;

        String modo = (c1==0 && c0==0) ? "Desconectado" :
                      (c1==0 && c0==1) ? "Toggle" :
                      (c1==1 && c0==0) ? "Clear/Set (PWM normal)" :
                                         "Set/Clear (PWM invertido)";

        String activo = (this->canalOC == E_CANAL_OC::OC3C) ? " <== ACTIVO" : "";

        Serial.println("| OC3C  |   " + String(c1) + "    |   " + String(c0) +
                        "    | " + modo + "      | Control del pin OC3C" + activo);
    };

    Serial.println("");
    Serial.println("Nota: Solo el canal marcado como ACTIVO controla el servo.");
    Serial.println("");

    // Tabla TCCR3B-------------------------------------------------------------------------------
    Serial.println("Tabla de bits TCCR3B");
    Serial.println("| Bit | Nombre   | Valor | Significado                          |");
    Serial.println("|-----|----------|-------|--------------------------------------|");
    Serial.println("| 7   | ICNC3    | " + String((this->registroTCCRB >> ICNC3) & 1) + "     | Noise cancel input capture");
    Serial.println("| 6   | ICES3    | " + String((this->registroTCCRB >> ICES3) & 1) + "     | Edge select input capture");
    Serial.println("| 4   | WGM33    | " + String((this->registroTCCRB >> WGM33) & 1) + "     | Modo generación onda (parte alta)");
    Serial.println("| 3   | WGM32    | " + String((this->registroTCCRB >> WGM32) & 1) + "     | Modo generación onda (parte alta)");
    Serial.println("| 2   | CS32     | " + String((this->registroTCCRB >> CS32) & 1) + "     | Prescaler bit 2");
    Serial.println("| 1   | CS31     | " + String((this->registroTCCRB >> CS31) & 1) + "     | Prescaler bit 1");
    Serial.println("| 0   | CS30     | " + String((this->registroTCCRB >> CS30) & 1) + "     | Prescaler bit 0");
    Serial.println("");
    
    Serial.println("Tabla de prescaler (CS32:CS30)");
    Serial.println("| CS32 | CS31 | CS30 | Prescaler / Fuente               | Descripción");
    Serial.println("|------|------|------|----------------------------------|----------------------------------------------|");

    int cs32 = (this->registroTCCRB >> CS32) & 1;
    int cs31 = (this->registroTCCRB >> CS31) & 1;
    int cs30 = (this->registroTCCRB >> CS30) & 1;

    // Detectar prescaler activo-------------------------------------------------------------------------------
    String prescaler;
    String descripcion;

    if (cs32==0 && cs31==0 && cs30==0) {
        prescaler = "No clock";
        descripcion = "Timer detenido";
    }
    else if (cs32==0 && cs31==0 && cs30==1) {
        prescaler = "clk/1";
        descripcion = "Sin prescaler (16 MHz)";
    }
    else if (cs32==0 && cs31==1 && cs30==0) {
        prescaler = "clk/8";
        descripcion = "Prescaler 8 (2 MHz)";
    }
    else if (cs32==0 && cs31==1 && cs30==1) {
        prescaler = "clk/64";
        descripcion = "Prescaler 64 (250 kHz)";
    }
    else if (cs32==1 && cs31==0 && cs30==0) {
        prescaler = "clk/256";
        descripcion = "Prescaler 256 (62.5 kHz)";
    }
    else if (cs32==1 && cs31==0 && cs30==1) {
        prescaler = "clk/1024";
        descripcion = "Prescaler 1024 (15.6 kHz)";
    }
    else if (cs32==1 && cs31==1 && cs30==0) {
        prescaler = "Ext. falling";
        descripcion = "Fuente externa (flanco descendente)";
    }
    else {
        prescaler = "Ext. rising";
        descripcion = "Fuente externa (flanco ascendente)";
    }

    Serial.println("|   " + String(cs32) + "  |   " + String(cs31) + "  |   " + String(cs30) +
                "  | " + prescaler + "                            | " + descripcion);

    Serial.println("");
    Serial.println("Nota: Esta combinación es la que determina la velocidad del Timer.");
    Serial.println("");

    // Mostrar OCR e ICR-------------------------------------------------------------------------------
    String modeOCR =  (this->registroOCR == E_REGISTRO_OCR::OCR_3B) ? "OCR3B" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_3C) ? "OCR3C" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_0B) ? "OCR0B" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_3A) ? "OCR3A" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_4A) ? "OCR4A" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_4B) ? "OCR4B" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_4C) ? "OCR4C" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_2B) ? "OCR2B" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_2A) ? "OCR2A" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_1A) ? "OCR2A" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_1B) ? "OCR1B" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_1C) ? "OCR1C" :
                      (this->registroOCR == E_REGISTRO_OCR::OCR_0A) ? "OCR0A" :
                                                                    "Desconocido";                 
    Serial.println("Modo OCR seleccionado: " + modeOCR);
    Serial.println("Valor OCR configurado: " + String(this->registroOCRData));
    Serial.println("Registro OCR utilizado: " + String(static_cast<int>(this->registroOCR)));
    Serial.println("Registro ICR utilizado: " + String(static_cast<int>(this->registroICR)));
    String modeICR =  (this->registroICR == E_REGISTRO_ICR::ICR_1) ? "ICR1" :
                      (this->registroICR == E_REGISTRO_ICR::ICR_3) ? "ICR3" :
                      (this->registroICR == E_REGISTRO_ICR::ICR_4) ? "ICR4" :
                      (this->registroICR == E_REGISTRO_ICR::ICR_5) ? "ICR5" :
                                                                    "Desconocido";
    Serial.println("Modo ICR seleccionado: " + modeICR);
    Serial.println("Valor ICR configurado: " + String(this->registroICRData));
    
    Serial.println("");
    #endif
}

// Helper para mostrar binario de 8 bits con ceros a la izquierda
String Timmer::bin8(uint8_t val) {
    String s = String(val, BIN);
    while (s.length() < 8) s = "0" + s;
    return s;
}

