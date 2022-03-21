     
#include <stdio.h>    
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h> 



#define pin1 22 //led incluido
#define pin2 19
#define pin3 23
#define pin4 18


   

void app_main(void)
{

     gpio_pad_select_gpio(pin1);
    gpio_set_direction(pin1, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(pin2);
    gpio_set_direction(pin2, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(pin3);
    gpio_set_direction(pin3, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(pin4);
    gpio_set_direction(pin4, GPIO_MODE_OUTPUT);
    
    for(;;){
   
for (int i = 0; i < 512; i++)
{
     gpio_set_level(pin1, 1);
     gpio_set_level(pin2, 0);
     gpio_set_level(pin3, 0);
     gpio_set_level(pin4, 0);
     vTaskDelay(15/portTICK_PERIOD_MS); //delays
     gpio_set_level(pin1, 0);
     gpio_set_level(pin2, 1);
     gpio_set_level(pin3, 0);
     gpio_set_level(pin4, 0);
     vTaskDelay(15/portTICK_PERIOD_MS); //delays
     gpio_set_level(pin1, 0);
     gpio_set_level(pin2, 0);
     gpio_set_level(pin3, 1);
     gpio_set_level(pin4, 0);
     vTaskDelay(15/portTICK_PERIOD_MS); //delays
     gpio_set_level(pin1, 0);
     gpio_set_level(pin2, 0);
     gpio_set_level(pin3, 0);
     gpio_set_level(pin4, 1);
     vTaskDelay(20/portTICK_PERIOD_MS); //delays
    
}

}

}