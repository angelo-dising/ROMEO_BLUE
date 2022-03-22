     #include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define BLINK_GPIO 2
int core0 = 0;
int core1 = 0;
int core2 = 0;
TaskHandle_t Task2, Task3;

void loop2(void *parameter){
  for(;;){
       printf("Hola desde el loop 2\n");
    vTaskDelay(3000/portTICK_PERIOD_MS);
    
    
  }
  vTaskDelay(10);
}

void loop3(void *parameter){
  for(;;){
       printf("Hola desde el loop 3\n");
    vTaskDelay(2000/portTICK_PERIOD_MS);
    
  }
  vTaskDelay(10);
}


void app_main()
{
     xTaskCreatePinnedToCore(
    loop2,
    "Task_2",
    1000,
    NULL,
    1,
    &Task2,
    0);

  xTaskCreatePinnedToCore(
      loop3,
      "Task_3",
      1000,
      NULL,
      1,
      &Task3,
      0);
    //void loop
    for (;;)
    {
          printf("Hola desde el loop 1\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);

    }
    
}
