//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"

#define B0 0.001033
#define B1 0.000767
#define B2 -0.000266
#define A1 -1.521886
#define A2 0.521886

#define REF 20
#define UL 1
#define LL 0

float x[3]={0};
float y[3]={0};
float vo;

//
// Função Principal
//
void main(void)
{
    // Inicialização do dispositivo
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();

    // Habilita interrupções globais e de tempo real
    EINT;
    ERTM;

    while (1)
    {
        if (SCI_getRxFIFOStatus(SCI0_BASE)>3)
        {
            protocolReceiveData(SCI0_BASE,&vo,sizeof(float));

            x[2] = x[1]; x[1] = x[0]; x[0] = REF - vo;
            y[2] = y[1]; y[1] = y[0];
            y[0] = B0*x[0] + B1*x[1] + B2*x[2] - A1*y[1] - A2*y[2];

            y[0] = (y[0] > UL) ? UL : y[0];
            y[0] = (y[0] < LL) ? LL : y[0];

            protocolSendData(SCI0_BASE, &y[0],sizeof(float));

        }

    }
}
