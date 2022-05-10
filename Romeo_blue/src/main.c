#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "math.h"
#define LONGITUD_1 16
#define LONGITUD_2 18
#define dir_pin1 23
#define dir_pin2 21
#define step_pin1 22
#define step_pin2 19
#define cv_pin1 18
#define cv_pin2 5
#define input_pin_bwl_gripper  36
#define input_pin_uper_gripper  39
#define gripper_motor_pin1  13
#define gripper_motor_pin2  12

float distancia_deseada = 30; //cm;
int angulo = 0;//grados

float gradoC = 0;
int grado_motorA = 0;
int grado_motorB = 0;
bool flag_motorA = false;
bool flag_motorB = false;

typedef struct {
	int angle;
	int rpm;
	int dir_pin;
	int step_pin;
}Angle_Elbow_parameters;

typedef struct {
	int fed_rate;
	int time_to_run;
	int sensor_deteccion_de_carro;
	int sensor_inicio_de_carrera;
	int distancia_deseada;
	int pwm_up_time;
	int pwm_down_time;
}conveyor_velt_parameters;

typedef struct {
int pins_start_stop;
int color_pinR;
int color_pinG;
int color_pinB;
}interups_input_parameters;

void setup(){
//input pins
gpio_pad_select_gpio(input_pin_bwl_gripper);
gpio_set_direction(input_pin_bwl_gripper, GPIO_MODE_INPUT);
gpio_pad_select_gpio(input_pin_uper_gripper);
gpio_set_direction(input_pin_uper_gripper, GPIO_MODE_INPUT);
gpio_pad_select_gpio(34);
gpio_set_direction(34, GPIO_MODE_INPUT);
gpio_pad_select_gpio(35);
gpio_set_direction(35, GPIO_MODE_INPUT);
//output pins
gpio_pad_select_gpio(dir_pin1);
gpio_set_direction(dir_pin1, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(step_pin1);
gpio_set_direction(step_pin1, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(dir_pin2);
gpio_set_direction(dir_pin2, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(step_pin2);
gpio_set_direction(step_pin2, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(cv_pin1);
gpio_set_direction(cv_pin1, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(cv_pin2);
gpio_set_direction(cv_pin2, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(gripper_motor_pin1);
gpio_set_direction(gripper_motor_pin1, GPIO_MODE_OUTPUT);
gpio_pad_select_gpio(gripper_motor_pin2);
gpio_set_direction(gripper_motor_pin2, GPIO_MODE_OUTPUT);
}

void conveyor_belt_move_foward_until_sensor_activates(void *params){
	conveyor_velt_parameters *X = (conveyor_velt_parameters *)params;
		int Delay_up = X->pwm_up_time;
		int Delay_Down = X->pwm_down_time;
	
while(true){
		do
   			{
			gpio_set_level(22, 1);
			gpio_set_level(19, 0);
			vTaskDelay(Delay_up/portTICK_PERIOD_MS);
			gpio_set_level(22, 0);
			gpio_set_level(19, 0);
			vTaskDelay(Delay_Down/portTICK_PERIOD_MS);				
			}while (gpio_get_level(X->sensor_deteccion_de_carro) != 1);
	vTaskDelay(10);
   	vTaskDelete(NULL);
}
}

void Move_position_motor1(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
	printf("entro a la tarea\n");
		int delay_;
		int rpm = X->rpm;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rpm);
while (true)
{
		gpio_set_level(X->dir_pin, 1);
			for (int i = 0; i < X->angle; i++)
				{
					gpio_set_level(X->step_pin, 1);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
					gpio_set_level(X->step_pin, 0);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
				}	
vTaskDelay(10);
vTaskDelete(NULL);
}
}

void Move_position_motor2(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
	printf("entro a la tarea\n");
		int delay_;
		int rev_per_minute = X->rpm;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rev_per_minute);
while (true)
{
		gpio_set_level(X->dir_pin, 1);
			for (int i = 0; i < X->angle; i++)
				{
					gpio_set_level(X->step_pin, 1);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
					gpio_set_level(X->step_pin, 0);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
				}	
vTaskDelay(10);
vTaskDelete(NULL);
}
}

void gripper_go_up(void *params){
	while(true){
		do{
			gpio_set_level(gripper_motor_pin1, 1);
			gpio_set_level(gripper_motor_pin2, 0);
		}while(gpio_get_level(input_pin_uper_gripper) != 1);
		vTaskDelay(10);

		vTaskDelete(NULL);
	}
}

void gripper_go_down(void *params){
	while(true){
		do{
			gpio_set_level(gripper_motor_pin1, 0);
			gpio_set_level(gripper_motor_pin2, 1);
		}while(gpio_get_level(input_pin_bwl_gripper) != 1);
		vTaskDelay(10);
		vTaskDelete(NULL);
	}
}

int Teorum_of_cosenum_degree1(int Distance_from_center){
int gradoA = 0;
gradoA =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(distancia_deseada,2));///(2 * distancia_deseada * LONGITUD_2);
gradoA = gradoA/(2 * distancia_deseada * LONGITUD_2);
grado_motorA =  acos(gradoA) * 180 / M_PI;	
return grado_motorA;
}

int Teorum_of_cosenum_degree2(int Distance_from_center){
int gradoB = 0;
gradoB =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(distancia_deseada,2));///(2 * distancia_deseada * LONGITUD_2);
gradoB = gradoB/(2 * distancia_deseada * LONGITUD_1);
grado_motorB =  acos(gradoB) * 180 / M_PI;
return grado_motorB;
}


void app_main(){
setup();

	Angle_Elbow_parameters Elbow1_distance = {
		.angle = Teorum_of_cosenum_degree1(10)/1.8,
		.rpm = 30,
		.dir_pin = 23,
		.step_pin = 22
	};
	Angle_Elbow_parameters Elbow2_distance = {
		.angle = Teorum_of_cosenum_degree2(10)/1.8,
		.rpm = 30,
		.dir_pin = 21,
		.step_pin = 19
	};
	conveyor_velt_parameters Conveyor_velocity_pins= {
		.fed_rate = 10, //velocidad den cm/s
		.time_to_run = 10, //tiempo de ejecucion en segundos
		.sensor_deteccion_de_carro = 15, //sensor de final de carrera
		.sensor_inicio_de_carrera = 13, //sensor de inicio de carrera
		.pwm_up_time = 1, //tiempo de subida del pwm
		.pwm_down_time = 1, //tiempo de bajada del pwm
	};


	xTaskCreate(Move_position_motor1,"Task 1",1024*2, &Elbow1_distance,2,NULL);
	xTaskCreate(Move_position_motor2,"Task 2",1024*2, &Elbow2_distance,1,NULL);
	xTaskCreate(conveyor_belt_move_foward_until_sensor_activates,"Task 3",1024*2, &Conveyor_velocity_pins,1,NULL);
  while (true) {
	vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  
}
