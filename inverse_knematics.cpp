#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "math.h"

#define BLINK_GPIO 2
#define LONGITUD_1 25 //cm
#define LONGITUD_2 25 //cm
float distancia_deseada = 30; //cm;
int angulo = 0;//grados
float gradoB = 0;
float gradoA = 0;
float gradoC = 0;
int grado_motorA = 0;
int grado_motorB = 0;
bool flag_motorA = false;
bool flag_motorB = false;

TaskHandle_t Task2, Task3;

void motor_1_(int grados, int velocidad){
  int rpm = velocidad;
  int sentido_de_giro = 1;
  int pasos = (grados + angulo)/1.8;
  int number_of_steps = 200;
  int delay_ = 0;
  if (grados < 0){
    sentido_de_giro = 0;
  } 
  delay_ = 0.001*(60L * 1000L * 1000L / number_of_steps / rpm);
  
         gpio_set_level(21, sentido_de_giro);
            for (int i = 0; i < pasos; i++){
                gpio_set_level(19, 1);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);
                gpio_set_level(19, 0);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);}
}
void motor_2_(int grados, int velocidad){

}

//void loop2
void mover_motor_1_(void *parameter){

  int rpm = 30;
  int sentido_de_giro = 1;
  int pasos = (grado_motorB + angulo)/1.8;
  int number_of_steps = 200;
  int delay_ = 0;
  if (grado_motorB < 0){
    sentido_de_giro = 0;
  } 
  delay_ = 0.001*(60L * 1000L * 1000L / number_of_steps / rpm);
  
         gpio_set_level(13, sentido_de_giro);
            for (int i = 0; i < pasos; i++){
                gpio_set_level(12, 1);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);
                gpio_set_level(12, 0);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);}  
                 
                    vTaskDelete(NULL);
                

       vTaskDelay(10);
  
}
//void loop3
void mover_motor_2(void *parameter){

  int rpm = 30;
  int sentido_de_giro = 1;
  int pasos = (grado_motorB + angulo)/1.8;
  int number_of_steps = 200;
  int delay_ = 0;
  if (grado_motorB < 0){
    sentido_de_giro = 0;
  } 
  delay_ = 0.001*(60L * 1000L * 1000L / number_of_steps / rpm);
  
         gpio_set_level(14, sentido_de_giro);
            for (int i = 0; i < pasos; i++){
                gpio_set_level(27, 1);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);
                gpio_set_level(27, 0);
                  vTaskDelay(delay_/portTICK_PERIOD_MS);}
        flag_motorA = true;
        
          vTaskDelete(NULL);
        
        
        
        
       vTaskDelay(1000/portTICK_PERIOD_MS);
        
  
}
void conveyor_belt_move_foward(int Time_up, int time_down, int Time){
   int Delay_up = Time_up;
   int Delay_Down = time_down;
   int cicle = Delay_Down + Time_up;
   int miliseconds_goal = Time * 60000;
   for (int miliseconds = 0;miliseconds <= miliseconds_goal ;)
   {
gpio_set_level(22, 1);
gpio_set_level(19, 0);
vTaskDelay(Delay_up/portTICK_PERIOD_MS);
gpio_set_level(22, 0);
gpio_set_level(19, 0);
vTaskDelay(Delay_Down/portTICK_PERIOD_MS);
miliseconds = miliseconds + cicle;
   }
   vTaskDelay(1000/portTICK_PERIOD_MS);
   printf("Final de la tarea");
}




void conveyor_belt_move_foward_undefinedly(int time_up, int time_down, int Stop_pin, int stop_event){
 int Delay_up = time_up;
   int Delay_Down = time_down;
   gpio_pad_select_gpio(Stop_pin);
gpio_set_direction(Stop_pin, GPIO_MODE_INPUT);
bool stop_value = gpio_get_level(Stop_pin);
while(stop_value != true)
{
 gpio_set_level(22, 1);
gpio_set_level(19, 0);
vTaskDelay(Delay_up/portTICK_PERIOD_MS);
gpio_set_level(22, 0);
gpio_set_level(19, 0);
vTaskDelay(Delay_Down/portTICK_PERIOD_MS);
}}

void app_main(){

gpio_pad_select_gpio(13);
gpio_set_direction(13, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(12);
gpio_set_direction(12, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(14);
gpio_set_direction(14, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(27);
gpio_set_direction(27, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(26);
gpio_set_direction(26, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(25);
gpio_set_direction(25, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(34);
gpio_set_direction(34, GPIO_MODE_INPUT);

gradoA =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(distancia_deseada,2));///(2 * distancia_deseada * LONGITUD_2);
gradoA = gradoA/(2 * distancia_deseada * LONGITUD_2);
grado_motorA =  acos(gradoA) * 180 / M_PI;
printf("Grados de motor A :%d\n", grado_motorA);
gradoB =  (pow(LONGITUD_1,2) + pow(distancia_deseada,2) - pow(LONGITUD_2,2));///(2 * distancia_deseada * LONGITUD_2);
gradoB = gradoB/(2 * distancia_deseada * LONGITUD_2);
grado_motorB =  acos(gradoB) * 180 / M_PI;
printf("Grados de motor B : %d\n", grado_motorB);


  xTaskCreatePinnedToCore(mover_motor_1_, "mover_motor_1_", 1000, NULL, 1, &Task2, 0); 
  vTaskDelay(1000/portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(mover_motor_2, "mover_motor_2", 1000, NULL, 1, &Task3, 1);

   
    
}


