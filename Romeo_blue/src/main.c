#include <stdio.h>// libreria estandar de c
#include "freertos/FreeRTOS.h"//Libreia del sdk de freertos
#include "freertos/task.h"//Libreia del sdk de freertos
#include "esp_system.h"//Libreia del sdk de freertos
#include "driver/gpio.h"//libreria para el control de gpios
#include "math.h"//libreria para la funcion de math
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#define LONGITUD_1 16//definicion de el largo de el primer antebrazo
#define LONGITUD_2 18//definicion de el largo de el segundo antebrazo
#define dir_pin1 23//pin de direccion del primer antebrazo
#define step_pin1 22//pin de paso del primer antebrazo
#define dir_pin2 21//pin de direccion del segundo antebrazo
#define step_pin2 19//pin de paso del segundo antebrazo
#define cv_pin1 18//pin de control de velocidad del primer antebrazo
#define cv_pin2 5//pin de control de velocidad del segundo antebrazo
#define input_pin_bwl_gripper  36//pin de interuptor de la brazo
#define input_pin_uper_gripper  39//pin de interuptor de la brazo
#define gripper_motor_pin1  2//pin de control del gripper
#define gripper_motor_pin2  4 //pin de control del gripper
#define input_pin_car_detection_1  35//pin de control del gripper
#define input_pin_car_detection_2  34//pin de interuptor de la carroceria
#define master_pin_1  32//pin de control del master

EventGroupHandle_t evtGrp;
const int Elbow1F = BIT0;
const int Elbow2F = BIT1;

float distancia_deseada = 0; //cm;
int grado_motorA = 0;
int grado_motorB = 0;
//parametros a usar durante el programa
typedef struct 
{
	int angle;
	bool sentido_de_giro;
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
gpio_pad_select_gpio(32);
gpio_set_direction(32, GPIO_MODE_INPUT);
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

void Show_infor(){
	printf("ROMEO_BLUE_V1.0\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5775#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~~~~~~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~~^~~^~!YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YBBY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&BY7~^^~^~!JP#@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@&GJ!~^~~^~!JG&@@@@@@@@@@@@@@@@@@&GY!~^~~^~!JG&@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~^^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P?~^^~^^~?5B@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@&BY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@&GJ!~^~~^~7YG&@@@@@@@@@@#GG@@@@@@@@@@@@@@@@@@BP#&@@@@@@@@@&GY7~^~~^~!JG&@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@#P?!~^~^^~75B@@@@@@@@@@&GJ7~^?@@@@@@@@@@@@@@@@@@Y^~!JP&@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@\n");
printf("@@@@@@@@B5?~^^~^^~?P#@@@@@@@@@@@@P7~^~~~?@@@@@@@@@@@@@@@@@@Y^~~^^!5@@@@@@@@@@@@#P?!^^~^^~75B@@@@@@@@\n");
printf("@@@@&BY7~^~~^~!JP&@@@@@@@@@@@@@@J^~~~~~~?@@@@@@@@BG@@@@@@@@Y^~~~~~^?&@@@@@@@@@@@@@&GJ!~^~~^~7YB&@@@@\n");
printf("@&GJ!~^~~^~7YG&@@@@@@@@@@@@@B5JG~~~~~~~~?@@@@@@@#!~G@@@@@@@Y^~~~~~~^PYYG&@@@@@@@@@@@@&GY7~^~~^~!JG&@\n");
printf("J!~^~~^~75B@@@@@@@@@@@@@@@G7~^~5~~~~~~~~?@&####B!~~~G#####@Y^~~~~~~~5!^~!P@@@@@@@@@@@@@@@B57~^~~^^!J\n");
printf("7~~~~7P#@@@@@@@@@@@@@@@@@5~~~~~Y!~~~~~~~?@&Y!~~~~~~~~~~~J#@Y^~~~~~~~5~~~~^J@@@@@@@@@@@@@@@@@#P7~~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@@@@@P^~~~~~??~~~~~~~?@@@#Y~~~~~~~~J#@@@Y^~~~~~~7J~~~~~^Y@@@@@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@@@BGJ^~~~~~!Y~~~~~~~?@@@@&~~^^~^~~G@@@@Y^~~~~~~J7~~~~~~7PB@@@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@G?~~5!~~~~~~J!~~~~~~?@@@@5~7JGGY7~?@@@@Y^~~~~~~J~~~~~~~5~~7P@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@J~~~~?5~~~~~~!J~~~~~~?@@@@B#@@@@@@#B@@@@Y^~~~~~?7~~~~~^JJ~~~^?&@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@J^~~~~~YJ^~~~~~?!~~~~~?@@@@@@@@@@@@@@@@@@Y^~~~~~J~~~~~^75~~~~~^7&@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@#~~~~~~~~5?^~~~~~J~~~~~?@@@@@@@@@@@@@@@@@@Y^~~~~?!~~~~^75~~~~~~~^G@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@GGJ^~~~~~~~Y?~~~~~77~~~~?@@@@@@&#G5P#@@@@@@Y^~~~!?~~~~^75!~~~~~~^?GG&@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@B7^~5?^~~~~~~~JJ~~~~~J!~~~?@@@@@B!~~^^!JPB@@@Y^~~~J~~~~~?Y~~~~~~~~7P!^!G@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@#!^~~~Y?~~~~~~~~?Y!~~~!J~~~?@@@@@G^~~~~~!!!&@@Y^~~?7~~~~JJ~~~~~~~~7Y!~~~~B@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@5^~~~~~7?!~~~~~~~!Y7~~~??^~?@@@@@J^~~~~^P@#@@@Y^~7J~~~!J7~~~~~~~~??~~~~~^J@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@Y^~~~~~~~77!~~~~~~~?J~~~Y?^?@@@@B~~~~~~^J@@@@@Y^7Y~~~?J~~~~~~~~77!~~~~~~^?@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@#!^~~~~~~~~!!!~~~~~~!J?~~5??@@@&7~~~~~~~~B@@@@Y7P~~7J7~~~~~~!!!~~~~~~~~^~G@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@G557~~~~~~~~~!7!!~~~~~?Y?~PB@@B7~~~~~~~~~!P&@@#G!7Y?~~~~~!!7!~~~~~~~~~7YPP@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@#~~7JYJ7!~~~~~~~~!77!~~^~JP5BP?~~~~~~~~~~~~~!JPB5PY!^~~!777~~~~~~~~!7JYJ7~^G@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@P^~~~~!7???7!~~~~~~!7??7!~~Y5^~~~~~~~~~~~~~~~^^JP!~!7??7!~~~~~~!77??7!~~~~^Y@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@B~~~~~~~~~~!77777!!~~~~!7???P~~~~~~~~~~~~~~~~~^YY!1?!~~~~!!!7777!~~~~~~~~~^P@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@?^~~~~~~~~~~~~~!!77777!!!!?5~~~~~~~~~~~~~~~~~^YY!!!!77777!!~~~~~~~~~~~~~^7&@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@&Y7!!!~~~~~~~~~~~~~~~~~!!!?5~~~~~~~~~~~~~~~~~~JJ!!!!~~~~~~~~~~~~~~~~!!!7J&@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@?!7?????????7777777777777JY^~~~~~~~~~~~~~~~~~JY77777777777777????????777&@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@J^~~~~~~~~~~~~~~~~~~~~!77?P^~~~~~~~~~~~~~~~~^5Y77!!~~~~~~~~~~~~~~~~~~~^7@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@&J~^~~~~~~~~~~~~!!!777!!!75J!^~~~~~~~~~~~~^~J57!!!777!!!~~~~~~~~~~~~^^?#@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@BJ!!!!77777777!!~!!!!77!!Y&57~~~~~~~~~~!J#P!!77!!!!~!!!7777777!!!!?G@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@Y!7!!!~~~~~!!7!!777!7?B@@@5!!~~~~~!?#@@&?77!77!!77!~~~~~!!!7!?&@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@#?~^^~~!777!!!!!!!!!!B@@@G~~77!!!!!~5@@@#77?!!7?7!!777!~~^^~7B@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@@#GP7!!~~!7?7~~7?!!B@@@#~~~~~!!~~~~~G@@@&?~??~-1!!!~~!!75G#@@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@@@@@#Y???7~^^!?!~Y&@@@@Y^~~~~~~~~~~^?@@@@@5~!?7~^~7???YB@@@@@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~^5@@@@@@@@@@@@@@@@@@@@@@#GPPGGY5#@@@@@@Y^~~~!777~^~~J@@@@@@#PYPBPPPB@@@@@@@@@@@@@@@@@@@@@@5^~~~7\n");
printf("7~~~~7P#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#J~?5J!~7P5!!B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P7~~~~7\n");
printf("J!~^~~^~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&?YPYB7~~~~!BP5PY&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~^~~^^!J\n");
printf("@&GJ!~^~~^~7YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@P~~~~~~~~P@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~!JG&@\n");
printf("@@@@&BY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@Y~~~~~~~~~~J&@@@@@@@@@@@@@@@@@@@@@@@@@&GJ!~^~~^~7YB&@@@@\n");
printf("@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@#7~7~!~~~!!!~~!G@@@@@@@@@@@@@@@@@@@@@#P?!^^~^^~?5B@@@@@@@@\n");
printf("@@@@@@@@@@@#PJ!~^~^^~75B@@@@@@@@@@@@@@@@@P~~?~!!!~~7~7~!~~J#@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@&GJ!~^~~^~!YG&@@@@@@@@@@@@Y^~J!~?~7~~?~!?~7~~Y@@@@@@@@@@@@&GY7~^~~^~!JG&@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@&BY7~^~~^~!JP#@@@@@@@@@#P5?^~?^?~~?~^J7?G&@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@&BG5?Y775YPB&@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@&BY7~^^~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY!~^~~^~!JG&@@@@@@@@@@@@@@@@@@&GJ!~^~~^~!JG&@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@@#PJ!~^~^^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P?~^^~^^~?5B@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&PJ!~^~~^~7YBBY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~~^~~^~7YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~~~~~~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5775#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printf("Configuraciones iniciales\n");
printf("Dispositivos de Salida:\n");
printf("Motor 1 ----------> Gpio %d /Gpio%d\n",dir_pin1,step_pin1);
printf("Motor 2 ----------> Gpio %d /Gpio%d\n",dir_pin2,step_pin2);
printf("Gripper Motor ----> Gpio %d /Gpio%d\n",gripper_motor_pin1,gripper_motor_pin2);
printf("\n");
printf("Dispositivos de Entrada:\n");
printf("Final de carrera gripper Down---------> Gpio %d\n",input_pin_bwl_gripper);
printf("Final de carrera gripper up ----------> Gpio %d\n",input_pin_uper_gripper);
printf("Sensor de deteccion de carro ----------> Gpio %d\n",input_pin_car_detection_1);
printf("Sensor de deteccion de carro ----------> Gpio %d\n",input_pin_car_detection_2);
printf("\n");
printf("Test de sensores\n");
printf("##############################################################################################\n");
printf("\n");
printf("Estado de de final de carrera 1, pin36: %d\n", gpio_get_level(input_pin_bwl_gripper));
printf("Estado de de final de carrera 2, pin39: %d\n", gpio_get_level(input_pin_uper_gripper));
printf("Estado de de deteccion de carro 1, pin35: %d\n", gpio_get_level(input_pin_car_detection_1));
printf("Estado de de deteccion de carro 2, pin34: %d\n", gpio_get_level(input_pin_car_detection_2));
printf("Estado de el pin maestro, pin32: %d\n", gpio_get_level(master_pin_1));
printf("\n");
printf("##############################################################################################\n");
printf("\n");
printf("El dispositivo esta listo para iniciar, esperando al dispositivo maesto\n");
printf("\n");
}
//funcion para mover el primer motor
void Move_position_motor1(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
		int delay_;
		int rpm = X->rpm;
		int dir = X->sentido_de_giro;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rpm);//funcion que convierte los rpm a un delay
while (true)
{
		gpio_set_level(X->dir_pin, dir);
			for (int i = 0; i < X->angle; i++)
				{
					gpio_set_level(X->step_pin, 1);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
					gpio_set_level(X->step_pin, 0);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
				}
				    xEventGroupSetBits(evtGrp, Elbow1F);
	
				//esto crea la senal para el driver de el motor
vTaskDelay(10);
vTaskDelete(NULL);
}
}

void Move_position_motor2(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
		int delay_;
		int rev_per_minute = X->rpm;
		int dir = X->sentido_de_giro;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rev_per_minute);//funcion que convierte los rpm a un delay
while (true)
{
		gpio_set_level(X->dir_pin, dir);
			for (int i = 0; i < X->angle; i++)
				{
					gpio_set_level(X->step_pin, 1);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
					gpio_set_level(X->step_pin, 0);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
				}
				xEventGroupSetBits(evtGrp, Elbow2F);
	
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

void input_handeler(void *params){
	while(true){






		vTaskDelay(10);
		vTaskDelete(NULL);
	}
}
//funcion para calcular los grados que se debe mover el motor
float Teorum_of_cosenum_degree1(int Distance_from_center){
int gradoA = 0;
gradoA =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(Distance_from_center,2));///(2 * distancia_deseada * LONGITUD_2);
printf("%d\n", gradoA);
gradoA = gradoA/(2 * Distance_from_center * LONGITUD_2);
printf("%d\n", gradoA);
grado_motorA =  acos(gradoA) * 180 / M_PI;	
printf("%d\n", grado_motorA);
return grado_motorA;
}
//funcion para calcular los grados que se debe mover el motor
float Teorum_of_cosenum_degree2(int Distance_from_center){
int gradoB = 0;
gradoB =  (pow(LONGITUD_1,2) + pow(LONGITUD_2,2) - pow(Distance_from_center,2));///(2 * distancia_deseada * LONGITUD_2);
printf("%d\n", gradoB);
gradoB = gradoB/(2 * Distance_from_center * LONGITUD_1);
printf("%d\n", gradoB);
grado_motorB =  acos(gradoB) * 180 / M_PI;
printf("%d", grado_motorB);
return grado_motorB;
}


void app_main(){
setup();//llamada de el setup
Show_infor();//llamada de la funcion para mostrar informacion

do
{
	vTaskDelay(1000/portTICK_PERIOD_MS);
} while (gpio_get_level(master_pin_1) == 0);

printf("Matser pin activado\n");
int i = 0;

do{
if (gpio_get_level(input_pin_car_detection_1) == 0){	
do
   			{
			gpio_set_level(cv_pin1, 1);
			}while (gpio_get_level(34) == 1);
			gpio_set_level(cv_pin1, 0);
i++;
}
}while(i != 1);

while (gpio_get_level(input_pin_car_detection_2) != 1)vTaskDelay(1000/portTICK_PERIOD_MS);//espera a que el sensor de deteccion de carro este activado
if (gpio_get_level(input_pin_car_detection_2) == 1){
{
		Angle_Elbow_parameters Elbow1_distance = {//parametros para el motor 1
		.angle = Teorum_of_cosenum_degree1(10)/1.8,
		.sentido_de_giro = false,
		.rpm = 30,
		.dir_pin = 23,
		.step_pin = 22
	};
	Angle_Elbow_parameters Elbow2_distance = {//parametros para el motor 2
		.angle = Teorum_of_cosenum_degree2(10)/1.8,
		.sentido_de_giro = false,
		.rpm = 30,
		.dir_pin = 21,
		.step_pin = 19
	};
	evtGrp = xEventGroupCreate();
	xTaskCreate(Move_position_motor1,"Task 1",1024*2, &Elbow1_distance,2,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(Move_position_motor2,"Task 2",1024*2, &Elbow2_distance,1,NULL);//creacion de la tarea para el motor 2
	xEventGroupWaitBits(evtGrp, Elbow1F | Elbow1F, true, true, portMAX_DELAY);
	printf("Primer movimiento finalizado\n");
}







  while (true) {
	vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  
}
}



