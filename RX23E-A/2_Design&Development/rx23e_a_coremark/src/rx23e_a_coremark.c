/***********************************************************************
*
*  FILE        : rx23e_a_coremark.c
*  DATE        : 2023-11-07
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "r_smc_entry.h"
#include "platform.h"
#include <stdio.h>
#include <string.h>


static bool sw_check (void);
void main(void);

extern void coremain(void);

extern volatile uint8_t  g_sci1_tx_end;
void my_sw_charput_function(char output_char)
{
	R_Config_SCI1_Serial_Send((uint8_t *)&output_char, 1);
	while(g_sci1_tx_end == false);
	g_sci1_tx_end = false;
}

double calculatePi_Leibniz(int iterations) {
    double pi = 0.0;
    int sign = 1;
    for (int i = 0; i < iterations; i++) {
        pi += (double) sign / (2 * i + 1);
        sign *= -1;
    }
    return 4 * pi;
}

void main(void)
{
	int timer = 0;
	R_Config_SCI1_Start();
	R_Config_TMR0_TMR1_Start();

	printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __func__, __LINE__);

	int iterations = 1000000;
	double pi;

	pi = calculatePi_Leibniz(iterations);
	printf("calculatePi_Leibniz\niterations=%d pi=%lf\n", iterations, pi);


	printf("start coremain!\n");
	coremain();
	printf("terminated coremain!\n");

	while(1)
	{
		printf("printf:hello world 1, %d sec,R_LED1_toggle\n", timer);

//		R_Config_SCI1_Serial_Send("sci send:hello world 2\n", strlen("hello world 2\n"));
//		while(g_sci1_tx_end == false);
//		g_sci1_tx_end = false;

		R_LED1_toggle();

		R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);

		timer++;
	}

}

static bool sw_check (void)
{
    static uint8_t  s_sw_last       = 0U;
    static uint8_t  s_sw_last_fix   = 0U;
    uint8_t         sw_input        = 0U;
    bool            ret             = false;

    /** SW input */
    if (0U == D_SW_PORT)
    {
        /** SW ON */
        sw_input = 1U;
    }

    /** SW pressing judgment */
    if (sw_input == s_sw_last)
    {
        if ((0U == s_sw_last_fix) && (1U == sw_input))
        {
            /** SW press detection */
            ret = true;
        }
        s_sw_last_fix = sw_input;
    }
    s_sw_last = sw_input;

    return ret;
}
