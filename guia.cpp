
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

//Digital read
 int estado = gpio_get_level(22);
 //debes de crear un avariable que equivalga al nivel del gpio.
printf("%d",estado);
estado = gpio_get_level(22);
 
//Ejemplo I2C--------------------------------------------------------------------------------------------------------
//No usar por el momento

/*
include"stdio.h"
#include"freertos/FreeRTOS.h"//libreria de el sistema operativo
#include"freertos/task.h"///libreria de sistema operativo
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"

#define SDA_GPIO 26
#define SCL_GPIO 25
#define giroscope_adress 0X68 //Direccion del sensor i2c


TaskHandle_t myTask1Handle = NULL;//declaracion de tareas !no borrar
TaskHandle_t myTask2Handle = NULL;//declaracion de tareas !no borrar
//ejecucuon de codigo en el core 1 -----------------------------------------------------------------------
void task (void *arg){
while(1){
printf("hola, esto esta siendo ejecutado en el core #1");
vTaskDelay(1000/portTICK_PERIOD_MS);
}}//-------------------------------------------------------------------------------------------------------



//ejecucion de tareas en el core numero 2-----------------------------------------------------------------
void task2 (void *arg)
void app_main()
{
for (int i  = 0 ;i < 5 ; i++ ){
printf("Hola esta tarea se esta ejecutando el el core #2");
vTaskDelay(1000/portTICK_PERIOD_MS);
}}//------------------------------------------------------------------------------------------------------

// main principal/////////////////////////////////////////////////////////////////////////////////////////
void app_main(){
xTaskCreate(task, "task1",4096,NULL ,10 , &myTask1Handle);
xTaskCreatePinnedToCore(task2, "task2",4096,NULL ,10 , &myTask1Handle, 1);
int i2c_master_port = 0;
#define I2C_MASTER_SCL_IO           22      //!< GPIO number used for I2C master clock 
#define I2C_MASTER_SDA_IO           21     //!< GPIO number used for I2C master data  
#define I2C_MASTER_FREQ_HZ          400000                     //!< I2C master clock frequency 
#define I2C_MASTER_TX_BUF_DISABLE   0                          //!< I2C master doesn't need buffer 
#define I2C_MASTER_RX_BUF_DISABLE   0                          //!< I2C master doesn't need buffer 
i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,         // select GPIO specific to your project
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = I2C_MASTER_SCL_IO,         // select GPIO specific to your project
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,  // select frequency specific to your project
    // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. 
};
    i2c_param_config(i2c_master_port, &conf);

     printf(i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));


    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    uint8_t raw[2];
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (giroscope_adress << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd_handle, (uint8_t *)&raw, 2, I2C_MASTER_ACK);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_handle);

    bool isNeg = false;
    if (raw[0] & 0x80)
    {
        isNeg = true;
        raw[0] = raw[0] & 0x7f;
    }

    int16_t data = (raw[0] << 8 | raw[1]) >> 5;
    float temperature = (data * 0.125) * (isNeg? -1 : 1);
    printf("temperature %f\n", temperature);
}----------------------------------------------------------------------------------------------------------------------
*/


}
