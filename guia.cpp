
#include <stdio.h>    
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h> 
//VARIABLES, son tal cual, no cambian para nada a la forma convencional de declarar
int numero_del_pin = 1;







void app_main(void)
{

//pinMode():
gpio_pad_select_gpio(numero_del_pin);
//El parametro es para poder seleccionar el pin, va antes que la siguiente 
 gpio_set_direction(numero_del_pin, GPIO_MODE_OUTPUT);
 /*El primer parametro  es el nunmero del pin que selecionar
 El segiundo parametro es el argumento de el tipo de entrada que puede recibir el pin
 GPIO_MODE_OUTPUT -- EL PIN ACTUARA CONMO SALIDA
 GPIO_MODE_INPUT --El pin actua como entrada
 */

//digitalWrite();
 gpio_set_level(numero_del_pin, 1);
 //el primer parametro puede ser un numero o una variable que equivalga al numero del pin
 //el segundo parametro equivale al estado al que queremos poner el pin, puede ser solo
 // 1 para estado alto y 0 para estado bajo

//Delays
     vTaskDelay(20/portTICK_PERIOD_MS);
// el primer parametro es el tiempo en milisegundos que tiene el delay
//El segundo parametro debes de usar siempre "portTICK_PERIOD_MS", que significa
//que el valor que usas esta en milsegundos, recomendable no cambiar

//Impmprimir en paralla (Serial.println();)

//en el framework no es necesario setear nada para usar la impresio por puerto serial
//por lo que justo como en c, poemos  usar un simple printf() estandar sin problemas, 
//la forma convencional de como usar printf no cambia, siente libre de usarlo.

printf("HOLA MUNDO");

}
