#include <stdio.h>// libreria estandar de c
#include "freertos/FreeRTOS.h"//Libreia del sdk de freertos
#include "freertos/task.h"//Libreia del sdk de freertos
#include "esp_system.h"//Libreia del sdk de freertos
#include "driver/gpio.h"//libreria para el control de gpios
#include "math.h"//libreria para la funcion de math
#define LONGITUD_1 16//definicion de el largo de el primer antebrazo
#define LONGITUD_2 18//definicion de el largo de el segundo antebrazo
#define dir_pin1 23//pin de direccion del primer antebrazo
#define dir_pin2 21//pin de direccion del segundo antebrazo
#define step_pin1 22//pin de paso del primer antebrazo
#define step_pin2 19//pin de paso del segundo antebrazo
#define cv_pin1 18//pin de control de velocidad del primer antebrazo
#define cv_pin2 5//pin de control de velocidad del segundo antebrazo
#define input_pin_bwl_gripper  36//pin de interuptor de la brazo
#define input_pin_uper_gripper  39//pin de interuptor de la brazo
#define gripper_motor_pin1  13//pin de control del gripper
#define gripper_motor_pin2  12//pin de control del gripper

float distancia_deseada = 30; //cm;
int grado_motorA = 0;
int grado_motorB = 0;
//parametros a usar durante el programa
typedef struct 
{
	int angle;
	int rpm;
	int dir_pin;
	int step_pin;
}		Angle_Elbow_parameters;//parametros de los codos

typedef struct {
	int fed_rate;
	int time_to_run;
	int sensor_deteccion_de_carro;
	int sensor_inicio_de_carrera;
	int distancia_deseada;
	int pwm_up_time;
	int pwm_down_time;
}conveyor_velt_parameters;//parametros del conveyor

typedef struct {
int pins_start_stop;
int color_pinR;
int color_pinG;
int color_pinB;
}interups_input_parameters;//parametros de los interuptores

/*Creamos una funcion que nos sirve para inicializar los pines de entrada y salida, los cuales son 
clasificados en pines de entrada y salida, mas adelante iniciamos la funcion en el app_main*/
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

//Fncion para mover el conveyor belt hacia adelante
void conveyor_belt_move_foward_until_sensor_activates(void *params){
	conveyor_velt_parameters *X = (conveyor_velt_parameters *)params;//casteamos el parametro a un tipo de dato de la estructura

		int Delay_up = X->pwm_up_time;//delay de subida, que apunta al puntero de la estructura
		int Delay_Down = X->pwm_down_time;//delay de bajada, que apunta al puntero de la estructura
	/*Debemos de encarcelar las instrucciones en un loop infinito el cual evite a la funcion 
	de regresar un valor ya que en free rtos las tareas deben cumplir con la regla de ser 
	una funcion void que no regrese, por lo que usar un loop inifito le inpide regresa un valor*/
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
			//si el sensor de deteccion de carro esta activado, entonces se detiene el movimiento del conveyor
	vTaskDelay(10);
   	vTaskDelete(NULL);
}
}

//funcion para mover el primer motor
void Move_position_motor1(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
		int delay_;
		int rpm = X->rpm;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rpm);//funcion que convierte los rpm a un delay
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
				//esto crea la senal para el driver de el motor
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
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rev_per_minute);//funcion que convierte los rpm a un delay
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
				//esto crea la senal para el driver de el motor
vTaskDelay(10);
vTaskDelete(NULL);
}
}

void gripper_go_up(void *params){
	while(true){
		do{
			gpio_set_level(gripper_motor_pin1, 1);
			gpio_set_level(gripper_motor_pin2, 0);
		}while(gpio_get_level(input_pin_uper_gripper) != 1);//mover el motor hasta que el sensor de deteccion de carro esta activado
		vTaskDelay(10);

		vTaskDelete(NULL);
	}
}

void gripper_go_down(void *params){
	while(true){
		do{
			gpio_set_level(gripper_motor_pin1, 0);
			gpio_set_level(gripper_motor_pin2, 1);
		}while(gpio_get_level(input_pin_bwl_gripper) != 1);//mover el motor hasta que el sensor de deteccion de carro esta activado
		vTaskDelay(10);
		vTaskDelete(NULL);
	}
}
//funcion para calcular los grados que se debe mover el motor
int Teorum_of_cosenum_degree1(int Distance_from_center){
int gradoA = 0;
gradoA =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(distancia_deseada,2));///(2 * distancia_deseada * LONGITUD_2);
gradoA = gradoA/(2 * distancia_deseada * LONGITUD_2);
grado_motorA =  acos(gradoA) * 180 / M_PI;	
return grado_motorA;
}
//funcion para calcular los grados que se debe mover el motor
int Teorum_of_cosenum_degree2(int Distance_from_center){
int gradoB = 0;
gradoB =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(distancia_deseada,2));///(2 * distancia_deseada * LONGITUD_2);
gradoB = gradoB/(2 * distancia_deseada * LONGITUD_1);
grado_motorB =  acos(gradoB) * 180 / M_PI;
return grado_motorB;
}


void app_main(){
setup();//llamada de el setup

	Angle_Elbow_parameters Elbow1_distance = {//parametros para el motor 1
		.angle = Teorum_of_cosenum_degree1(10)/1.8,
		.rpm = 30,
		.dir_pin = 23,
		.step_pin = 22
	};
	Angle_Elbow_parameters Elbow2_distance = {//parametros para el motor 2
		.angle = Teorum_of_cosenum_degree2(10)/1.8,
		.rpm = 30,
		.dir_pin = 21,
		.step_pin = 19
	};
	conveyor_velt_parameters Conveyor_velocity_pins= {//parametros para la cinta de transporte
		.fed_rate = 10, //velocidad den cm/s
		.time_to_run = 10, //tiempo de ejecucion en segundos
		.sensor_deteccion_de_carro = 15, //sensor de final de carrera
		.sensor_inicio_de_carrera = 13, //sensor de inicio de carrera
		.pwm_up_time = 1, //tiempo de subida del pwm
		.pwm_down_time = 1, //tiempo de bajada del pwm
	};


	xTaskCreate(Move_position_motor1,"Task 1",1024*2, &Elbow1_distance,2,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(Move_position_motor2,"Task 2",1024*2, &Elbow2_distance,1,NULL);//creacion de la tarea para el motor 2
	xTaskCreate(conveyor_belt_move_foward_until_sensor_activates,"Task 3",1024*2, &Conveyor_velocity_pins,1,NULL);//creacion de la tarea para la cinta de transporte
  while (true) {
	vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  
}

