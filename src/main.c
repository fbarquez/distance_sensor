/*********************************************************************************
 ********************** Distance Sensor/Abstandssensor **************************
 *********************************by Fbarquez***********************************
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "lcd 20x4_1.h"  // Die Bibliothek 4x20 LCD ist enthalten. Das Display wird an Port B angeschlossen.
#include "mcc_generated_files/mcc.h"

#define PinEcho PORTAbits.RA1     // Pin RA1 ist mit dem Echo-Pin verbunden. (digitaler Eingang)
#define PinTrig LATAbits.LATA0    // Pin RA0 ist mit Pin Trig. (digitaler Ausgang)
#define Led_Red LATCbits.LATC0    // Der Pin RC0 ist der roten LED zugewiesen.
#define Led_Green LATCbits.LATC1  // Der Pin RC1 ist der grünen LED zugewiesen.
#define Buzzer LATCbits.LATC2     // Der Pin RC2 ist dem Buzzer zugewiesen.
#define HIGH 1                    // Hohes Niveau
#define LOW 0                     // Niedriges Niveau.

// Globale Variablen für erweiterte Funktionalitäten
float Velocidad_Sonido = 34000.0;  // Geschwindigkeit des Schalls.
float Distancia        = 0.0;      // Variable vom Typ Fließkomma 32 Bit
float Tiempo           = 0.0;      // Variable vom Typ Fließkomma 32 Bit für die Zeit.
float Temperatura =
    0.0;  // Variable vom Typ 32 Bit Float, hier wird der Temperaturwert gespeichert.
float Humedad =
    0.0;  // Variable vom Typ 32 Bit Float, in der der Feuchtigkeitswert gespeichert wird.

char Buffer[20] = " ";      // Einrichtung zum Speichern der Variablen, die über die serielle
                            // Kommunikation gesendet werden.
char     LCD_Buffer[20];    // Einrichtung für die Anzeige der Variablen auf dem 4x20 LCD Display.
uint16_t Lecturas[10];      // Einrichtung für 10 Leseelemente.
volatile char Caracter_RX;  // Die Variable "Caracter_RX" muss flüchtig sein, da ihr Wert in sie
                            // geladen wird von der Interrupt Funktion.
volatile uint8_t Buffer_RX[20];
volatile uint8_t Contador_RX;

void         Timer1_Init(void);       // Funktion zur Initialisierung von Timer 1.
unsigned int ObtenerDistancia(void);  // Funktion zur Ermittlung des Abstands (der Entfernung).

void main(void)
{
    unsigned int distancia;  // Variable Abstand.

    // Nicht verwendete, aber vorbereitete Variablen für spätere Funktionserweiterung:
    // unsigned int anterior;        // Vorherige Variable.
    // unsigned int dismax;          // Variable dis_max. (maximaler Abstand).
    // unsigned int dismin;          // Variable dis_min. (kleinster Abstand).

    SYSTEM_Initialize();  // Systeminitialisierung.
    ANSELA = 0x00;        // Wir deaktivieren die analogen Eingänge von Anschluss A.
    ANSELB = 0x00;        // Wir deaktivieren die analogen Eingänge von Anschluss B.
    TRISA &= ~(1 << 0);   // Wir konfigurieren den Pin RA0 als digitalen Ausgang.
    TRISA |= (1 << 1);    // Wir konfigurieren den Pin RA1 als digitalen Eingang.
    TRISC = 0x00;         // Wir konfigurieren den Port C als digitalen Ausgang.
    LATC  = 0x00;         // Der Port C wird anfangs auf "low" gesetzt.
    Timer1_Init();        // Wir rufen die Startfunktion von Timer1 auf.
    LCD_Init();           // Wir initialisieren die 4x20 LCD-Display.

    LCD_gotoxy_putc(1, 1,
                    "   PROJEKT IT/VS    ");  // Auf dem 4x20 LCD Display wird ein Text angezeigt.
    LCD_gotoxy_putc(2, 1,
                    "     PIC16F1936     ");  // Auf dem 4x20 LCD Display wird ein Text angezeigt.
    LCD_gotoxy_putc(3, 1,
                    "Fernando Barriga Vas");  // Auf dem 4x20 LCD Display wird ein Text angezeigt.
    __delay_ms(5000);                         // Eine Verzögerung von 5s.

    while (1)
    {
        distancia = ObtenerDistancia();  // Die Variable distancia=al valor, der von der Funktion
                                         // "ObtenerDistancia" zurückgegeben wird.

        sprintf(LCD_Buffer, "   DISTANCE: %03ucm", distancia);  // Wert in LCD-Puffer schreiben.
        LCD_gotoxy_putc(3, 1, LCD_Buffer);                      // Abstand auf dem LCD anzeigen.

        printf("G%03u, distancia\n", distancia);  // Abstand an die serielle Schnittstelle senden.
        printf("T%03u, distancia\n", distancia);  // Abstand an die serielle Schnittstelle senden.

        if (distancia <= 50 && distancia >= 30)
        {
            LCD_gotoxy_putc(
                1, 1,
                "********ALARM*******");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                2, 1,
                "   SHORT DISTANCE   ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                4, 1,
                "     LED RED ON     ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            Led_Red   = HIGH;             // Rote LED an.
            Led_Green = LOW;              // Grüne LED ist aus.
            Buzzer    = LOW;              // Buzzer aus.
            printf("NR0G0B0\n");
            printf("LR255G0B0\n");
            printf("MR0G0B0\n");
            printf("ALARM\n");
            printf("SHORT DISTANCE\n");
            printf("DISTANCE: %s\n", LCD_Buffer);  // Abstand erneut anzeigen.
            __delay_ms(100);
            printf("\f");
        }
        else if (distancia < 30)
        {
            LCD_gotoxy_putc(
                1, 1,
                "********ALARM*******");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                2, 1,
                " DANGEROUS DISTANCE ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                4, 1,
                " BUZZER--LED RED ON ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            Led_Red   = HIGH;             // Rote LED an.
            Led_Green = LOW;              // Grüne LED ist aus.
            Buzzer    = HIGH;             // Buzzer an.
            printf("LR255G0B0\n");
            printf("MR255G233B0\n");
            printf("NR0G0B0\n");
        }
        else if (distancia > 50)
        {
            LCD_gotoxy_putc(
                1, 1,
                "      NO ALARMS     ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                2, 1,
                "    OUT OF RANGE    ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            LCD_gotoxy_putc(
                4, 1,
                "    LED GREEN ON    ");  // Auf dem 4x20 LCD Display wird eine Nachricht angezeigt.
            Led_Red   = LOW;              // Rote LED aus.
            Led_Green = HIGH;             // Grüne LED an.
            Buzzer    = LOW;              // Buzzer aus.
            printf("NR0G255B0\n");
            printf("LR0G0B0\n");
            printf("MR0G0B0\n");
        }

        __delay_ms(100);
    }
}

void Timer1_Init(void)  // Startfunktion Timer1.
{
    T1CONbits.TMR1CS = 0b00;  // Der Taktgeber von Timer1 muss Fosc/4 sein.
    T1CONbits.T1CKPS = 0b00;  // Der Pre-Scaler von Timer1 wird auf 1 gesetzt.
    T1CONbits.TMR1ON = 0;     // Timer 1 wird angehalten.
    TMR1             = 0;     // Counter von Timer1 ist gleich 0
}

unsigned int ObtenerDistancia(void)
{
    uint16_t duracion;
    uint16_t distancia;
    uint16_t Timer1;

    PinTrig = 1;
    __delay_us(10);
    PinTrig = 0;

    while (PinEcho == 0);
    T1CONbits.TMR1ON = 1;
    while (PinEcho == 1);
    T1CONbits.TMR1ON = 0;

    Timer1   = TMR1;
    duracion = Timer1;

    if (duracion < 116)
        distancia = 0;
    else if (duracion <= 23200)
        distancia = duracion / 58;
    else
        distancia = 0;

    duracion = 0;
    TMR1     = 0;

    return distancia;
}

/**
 End of File
*/
