

/* Inclusión de archivos */
#include "MT_ccp.h"

/* Definición de variables */
static uint8_t pwm_period = 255;

/* Definición de funciones */
void CCP1_PWM_Configure(ccp_pwm_t *pwm_param)
{
    timer2_t my_timer;
    
    /* PASO 1: Configurar como salida el pin de la señal PWM */
    TRISCbits.RC2 = 0;
    
    /* PASO 2: Configurar el Timer2 */
    pwm_period = pwm_param->timer2_top_value;
    my_timer.timer_status = TIMER2_ON;
    my_timer.prescaler = pwm_param->timer2_prescaler;
    my_timer.top_value = pwm_period;
    my_timer.postscaler = TIMER2_POSTSCALER_1;
    my_timer.match_interrupt = TIMER2_INTERRUPT_DISABLED;
    
    Timer2_Configure(&my_timer);

    /* PASO 3: Cargar el duty cycle por defecto */
    CCP1_PWM_Set_Duty_Cycle(pwm_param->duty_cycle);
    CCP1CONbits.DC1B = 0;
    
    /* PASO 4: Configurar el módulo CCP en modo PWM */
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M3 = 1;
}

void CCP1_PWM_Set_Duty_Cycle(uint8_t percent)
{
    int32_t duty_cycle;
    
    if(percent > 100)
    {
        percent = 100;
    }
    
    duty_cycle = (((((int32_t)percent) * ((int32_t)pwm_period + 1))) / 100) - 1;
    
    if(duty_cycle < 0)
    {
        CCP1CONbits.CCP1M2 = 0;
        CCP1CONbits.CCP1M3 = 0;   
        LATCbits.LATC2 = 0;
        CCPR1L = 0;
    }
    else if(duty_cycle >= pwm_period)
    {
        CCP1CONbits.CCP1M2 = 0;
        CCP1CONbits.CCP1M3 = 0;   
        LATCbits.LATC2 = 0;
        CCPR1L = pwm_period - 1;
    }
    else
    {
        CCP1CONbits.CCP1M2 = 1;
        CCP1CONbits.CCP1M3 = 1;   
        CCPR1L = (uint8_t)duty_cycle;      
    }
    
    
}


void CCP2_PWM_Configure(ccp_pwm_t *pwm_param)
{
     timer2_t my_timer;
    
    /* PASO 1: Configurar como salida el pin de la señal PWM */
    TRISCbits.RC1 = 0;
    
    /* PASO 2: Configurar el Timer2 */
    pwm_period = pwm_param->timer2_top_value;
    my_timer.timer_status = TIMER2_ON;
    my_timer.prescaler = pwm_param->timer2_prescaler;
    my_timer.top_value = pwm_period;
    my_timer.postscaler = TIMER2_POSTSCALER_1;
    my_timer.match_interrupt = TIMER2_INTERRUPT_DISABLED;
    
    Timer2_Configure(&my_timer);

    /* PASO 3: Cargar el duty cycle por defecto */
    CCP2_PWM_Set_Duty_Cycle(pwm_param->duty_cycle);
    CCP2CONbits.DC2B = 0;
    
    /* PASO 4: Configurar el módulo CCP en modo PWM */
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M3 = 1;    
}

void CCP2_PWM_Set_Duty_Cycle(uint8_t percent)
{
    int32_t duty_cycle;
   
    if(percent > 100)
    {
        percent = 100;
    }
    
    duty_cycle = (((((int32_t)percent) * ((int32_t)pwm_period + 1))) / 100) - 1;
    
    if(duty_cycle < 0)
    {
        CCP2CONbits.CCP2M2 = 0;
        CCP2CONbits.CCP2M3 = 0;   
        LATCbits.LATC1 = 0;
        CCPR2L = 0;
    }
    else if(duty_cycle >= pwm_period)
    {
        CCP2CONbits.CCP2M2 = 0;
        CCP2CONbits.CCP2M3 = 0;   
        LATCbits.LATC1 = 1;
        CCPR2L = pwm_period - 1;
    }
    else
    {
        CCP2CONbits.CCP2M2 = 1;
        CCP2CONbits.CCP2M3 = 1;   
        CCPR2L = (uint8_t)duty_cycle;
    }
}
