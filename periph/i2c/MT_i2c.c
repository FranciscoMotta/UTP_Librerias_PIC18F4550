
/* Inclusi�n de archivos */
#include "MT_i2c.h"

/* Definici�n de funciones */

uint8_t I2Cn_Master_Start_Condition(i2c_module_t i2c_n)
{
    uint8_t i2c_output = I2C_OK;
    
    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            /* Enviar una condici�n de START */
            SSPCON2 &= ~0x01F;
            SSPCON2bits.SEN = 1;
            /* Esperar a que la transmisi�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }
            
            /* Verificar la condici�n de START */
            if(SSPCON2bits.SEN == 1)
            {
                i2c_output = I2C_ERROR;
            }            
                        
            break;
            
        default:
            i2c_output = I2C_ERROR;
            break;
    }
    
    return i2c_output;
}


uint8_t I2Cn_Master_Restart_Condition(i2c_module_t i2c_n)
{
    uint8_t i2c_output = I2C_OK;
    
    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            /* Enviar una condici�n de START */
            SSPCON2 &= ~0x01F;
            SSPCON2bits.RSEN = 1;
            /* Esperar a que la transmisi�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }
            
            /* Verificar la condici�n de START */
            if(SSPCON2bits.RSEN == 1)
            {
                i2c_output = I2C_ERROR;
            }            
                        
            break;
            
        default:
            i2c_output = I2C_ERROR;
            break;
    }
    
    return i2c_output;
}



uint8_t I2Cn_Master_Tx_Address(i2c_module_t i2c_n, uint8_t slave_address, i2c_address_mode_t addr_mode)
{
	uint8_t i2c_output = I2C_OK;

    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            
            /* Cargar la direcci�n del esclavo e iniciar la transmisi�n */
            if(addr_mode == I2C_ADDR_READ_MODE){
                SSPBUF = (slave_address << 1) | 0x01;	
            }else{
                SSPBUF = (slave_address << 1) & 0xFE;
            }
   
            /* Esperar a que la transmisi�n inicie */
            while(SSPSTATbits.RW == 0)
            {
                /* No hacer nada */
            }             
            
            /* Esperar a que la transmisi�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }    
            

            /* Verificar si se recibi� un ACK o un NACK */
            if(SSPCON2bits.ACKSTAT == 1)
            {
               i2c_output = I2C_ERROR;
            }                
           
            break;
            
        default:
            i2c_output = I2C_ERROR;
            break;
    }
    
    return i2c_output;
            
}


uint8_t I2Cn_Master_Tx_Byte(i2c_module_t i2c_n, uint8_t byte)
{
	uint8_t i2c_output = I2C_OK;

    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            
            /* Cargar el dato e iniciar la transmisi�n */
            SSPBUF = byte;	
            
            /* Esperar a que la transmisi�n inicie */
            while(SSPSTATbits.RW == 0)
            {
                /* No hacer nada */
            }             
            
            /* Esperar a que la transmisi�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }    
            

            /* Verificar si se recibi� un ACK o un NACK */
            if(SSPCON2bits.ACKSTAT == 1)
            {
               i2c_output = I2C_ERROR;
            }                
           
            break;
            
        default:
            i2c_output = I2C_ERROR;
            break;
    }
    
    return i2c_output;    
}


uint8_t I2Cn_Master_Rx_Byte_ACK(i2c_module_t i2c_n, uint8_t *byte_p)
{

	uint8_t i2c_output = I2C_OK;
    
    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            
            /* Limpiar la bandera del buffer de recepci�n */
            SSPSTATbits.BF = 0;
            
            /* Colocar al Maestro en modo lectura */
            SSPCON2 &= ~0x01F;
            SSPCON2bits.RCEN = 1;
            
             /* Esperar a que la recepci�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }             
            
            /* Verificar la recepcion */
            if(SSPSTATbits.BF == 1)
            {
                *byte_p = SSPBUF;
            }
            else
            {
                i2c_output = I2C_ERROR;
            }
            
            if(i2c_output != I2C_ERROR)
            {
                /* Limpiar la bandera de eventos en el bus I2C */
                PIR1bits.SSPIF = 0;     
                
                /* Generar un ACK */
                SSPCON2bits.ACKDT = 0;
                SSPCON2 &= ~0x01F;
                SSPCON2bits.ACKEN = 1;
                /* Esperar a que la transmisi�n se complete */
                while(PIR1bits.SSPIF == 0)
                {
                    /* No hacer nada */
                }                        
            }
            break;
            
        default:
            
            i2c_output = I2C_ERROR;
            break;
            
    }        
            
	return i2c_output;
}


uint8_t I2Cn_Master_Rx_Byte_NACK(i2c_module_t i2c_n, uint8_t *byte_p)
{

	uint8_t i2c_output = I2C_OK;
    
    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            
            /* Limpiar la bandera del buffer de recepci�n */
            SSPSTATbits.BF = 0;
            
            /* Colocar al Maestro en modo lectura */
            SSPCON2 &= ~0x01F;
            SSPCON2bits.RCEN = 1;
            
             /* Esperar a que la recepci�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }             
            
            /* Verificar la recepcion */
            if(SSPSTATbits.BF == 1)
            {
                *byte_p = SSPBUF;
            }
            else
            {
                i2c_output = I2C_ERROR;
            }
            
            if(i2c_output != I2C_ERROR)
            {
                /* Limpiar la bandera de eventos en el bus I2C */
                PIR1bits.SSPIF = 0;     
                
                /* Generar un NACK */
                SSPCON2bits.ACKDT = 1;
                SSPCON2 &= ~0x01F;
                SSPCON2bits.ACKEN = 1;
                /* Esperar a que la transmisi�n se complete */
                while(PIR1bits.SSPIF == 0)
                {
                    /* No hacer nada */
                }                        
            }
            break;
            
        default:
            
            i2c_output = I2C_ERROR;
            break;
            
    }        
            
	return i2c_output;
}


uint8_t I2Cn_Master_Write_Data(i2c_module_t i2c_n, uint8_t slave_addr, uint8_t n_bytes, uint8_t *output_buffer)
{
	uint8_t i2c_output = I2C_OK;
	uint8_t index;

	/* Generar la condici�n de start */
	i2c_output = I2Cn_Master_Start_Condition(i2c_n);
	
    /* Transmitir direcci�n del esclavo y modo (lectura o escritura) */
    if(i2c_output != I2C_ERROR){
        i2c_output = I2Cn_Master_Tx_Address(i2c_n, slave_addr, I2C_ADDR_WRITE_MODE);
	}

	/* Transmitir los datos */
	if(i2c_output != I2C_ERROR){
        for(index = 0; index < n_bytes; index++){
            i2c_output = I2Cn_Master_Tx_Byte(i2c_n, *(output_buffer + index));
            if(i2c_output == I2C_ERROR){
                break;
            }
        }
	}
    
    /* Generar la condici�n de STOP */
	if(i2c_output != I2C_ERROR){
		i2c_output = I2Cn_Master_Stop_Condition(i2c_n);	
	}
    
	return i2c_output;    
}



uint8_t I2Cn_Master_Read_Data(i2c_module_t i2c_n, uint8_t slave_addr, uint8_t n_bytes, uint8_t *input_buffer)
{
 	uint8_t i2c_output = I2C_OK;
	uint8_t index;

	/* Generar la condici�n de START */
	i2c_output = I2Cn_Master_Start_Condition(i2c_n);
    
    
    /* Transmitir direcci�n del esclavo y modo (lectura) */
	if(i2c_output != I2C_ERROR){
		i2c_output = I2Cn_Master_Tx_Address(i2c_n, slave_addr, I2C_ADDR_READ_MODE);
	}
	
	
	/* Transmitir los datos */
	if(i2c_output != I2C_ERROR){
        
        for(index = 0; index < n_bytes; index++){

            if(index < (n_bytes - 1)){
                i2c_output = I2Cn_Master_Rx_Byte_ACK(i2c_n, input_buffer + index);
            }else{
                i2c_output = I2Cn_Master_Rx_Byte_NACK(i2c_n, input_buffer + index);
            }

            if(i2c_output == I2C_ERROR){
                break;
            }
        }		
	}

	/* Generar la condici�n de STOP */
	if(i2c_output == I2C_ERROR){
		i2c_output = I2Cn_Master_Stop_Condition(i2c_n);;
	}

	return i2c_output;   
}


uint8_t I2Cn_Master_Stop_Condition(i2c_module_t i2c_n)
{

	uint8_t i2c_output = I2C_OK;
    
    switch(i2c_n)
    {
        case I2C1_MOD:
            
            /* Limpiar la bandera de eventos en el bus I2C */
            PIR1bits.SSPIF = 0; 
            /* Limpiar los 5 bits LSB de SSPCON2 */
            SSPCON2 &= ~0x01F;
            /* Enviar una condici�n de STOP */
            SSPCON2bits.PEN = 1;
            /* Esperar a que la transmisi�n se complete */
            while(PIR1bits.SSPIF == 0)
            {
                /* No hacer nada */
            }
            
            /* Verificar la condici�n de STOP */
            if(SSPCON2bits.PEN == 1)
            {
                i2c_output = I2C_ERROR;
            }
            
            break;
            
        default:    
            i2c_output = I2C_ERROR;
            break;    
    }
    
   
	return i2c_output;    

}
