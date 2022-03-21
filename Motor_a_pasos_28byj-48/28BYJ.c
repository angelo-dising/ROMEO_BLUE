
#include <stdio.h>    
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h> 


int pin1 = 23;
int pin2 = 22;
int pin3 = 21;
int pin4 = 19;
int dalay = 20;

void setup(){

gpio_pad_select_gpio(pin1);
gpio_set_direction(pin1, GPIO_MODE_OUTPUT);

gpio_pad_select_gpio(pin2);
gpio_set_direction(pin2, GPIO_MODE_OUTPUT);

gpio_pad_select_gpio(pin3);
gpio_set_direction(pin3, GPIO_MODE_OUTPUT);

gpio_pad_select_gpio(pin4);
gpio_set_direction(pin4, GPIO_MODE_OUTPUT);

}


void app_main(void)
{
setup();
for(;;){

for (int i = 0; i < 512; i++)	// 512*4 = 2048 pasos
  {

gpio_set_level(pin1, 1);
gpio_set_level(pin2, 0);
gpio_set_level(pin3, 0);
gpio_set_level(pin4, 0);
vTaskDelay(dalay/portTICK_PERIOD_MS);
gpio_set_level(pin1, 0);
gpio_set_level(pin2, 1);
gpio_set_level(pin3, 0);
gpio_set_level(pin4, 0);
vTaskDelay(dalay/portTICK_PERIOD_MS);
gpio_set_level(pin1, 0);
gpio_set_level(pin2, 0);
gpio_set_level(pin3, 1);
gpio_set_level(pin4, 0);
vTaskDelay(dalay/portTICK_PERIOD_MS);
gpio_set_level(pin1, 0);
gpio_set_level(pin2, 0);
gpio_set_level(pin3, 0);
gpio_set_level(pin4, 1);
vTaskDelay(dalay/portTICK_PERIOD_MS);

  }


}

}
