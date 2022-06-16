#include<Arduino.h>
#define c 16//definicion de el largo de el primer antebrazo
#define b 18//definicion de el largo de el segundo antebrazo
float a = 0;//definicion de la variable a
float A = 0;//definicion de la variable A
float B = 0;//definicion de la variable B

int servo_pin =13;//pin de servo del segundo antebrazo
int dir_pin1 = 14;//pin de direccion del primer antebrazo
int step_pin1= 12;//pin de paso del primer antebrazo
int magnet_negative = 27;//pin de magnetizacion del primer antebrazo
int magnet_positive = 26;//pin de magnetizacion del segundo antebrazo
int cv_pin1_cw = 25;//pin de control de velocidad del primer antebrazo
int cv_pin2_ccw= 33;//pin de control de velocidad del segundo antebrazo
int AD  =32;//pin de control del master
int BD  =35;//pin de control del master

int rev_per_min = 30;//definicion de la variable rpm
EventGroupHandle_t EVENT_GET_CARGO;

const int Elbow1F = BIT0;
const int Elbow2F = BIT1;


//parametros a usar durante el programa
typedef struct 
{
	int steps;
	bool sentido_de_giro;
	int rpm;
}		Angle_Elbow_parameters;//parametros de los codos

typedef struct 
{
	int digreees;
}		servoAngle;//parametros del brazo




//funcion para mover el primer motor
void mt_ph296_02b_m(void *params) 
{
Angle_Elbow_parameters *X = (Angle_Elbow_parameters*) params;
		int delay_;
		int rpm = X->rpm;
		int dir = X->sentido_de_giro;
			delay_ = 0.001*(60L * 1000L * 1000L / 200 / rpm);//funcion que convierte los rpm a un delay
while (true)
{
		digitalWrite(dir_pin1, dir);
			for (int i = 0; i < X->steps; i++)
				{
					digitalWrite(step_pin1, 1);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
					digitalWrite(step_pin1, 0);
					vTaskDelay(delay_/portTICK_PERIOD_MS);
				}
				 xEventGroupSetBits(EVENT_GET_CARGO, Elbow1F);
					vTaskDelete(NULL);
				   
vTaskDelay(10);

}
}

void mt_mgg95_m(void *params) 
{
servoAngle *X = (servoAngle*) params;
 
		float delay_up = 0;
		float delay_down = 0;
		/*
480 & 19520- 0d
1230 & 18780- 90
2091 & 17909- 180
*/
switch (X->digreees)
{
case 180:
		delay_up = 2091;
		delay_down = 17909;
	break;
case 90:
		delay_up = 1230;
		delay_down = 18780;
	break;
case 0:
		delay_up = 450;
		delay_down = 19520;
	break;	

default:
printf("Error en el angulo del servo\n");
	break;
}



while (true)
{
	for (int i = 0; i < 50; i++)
	{
		digitalWrite(servo_pin, 1);	
		delayMicroseconds(delay_up);
		digitalWrite(servo_pin, 0);
		delayMicroseconds(delay_down);
	}
		xEventGroupSetBits(EVENT_GET_CARGO, Elbow2F);
		vTaskDelete(NULL);
				
	
				//esto crea la senal para el driver de el motor
vTaskDelay(10);

}
}

//funcion para calcular los grados que se debe mover el motor
float Teorum_of_cosenum_degree1(int a_)	
	{
		A =  (pow(b,2) + pow(c,2) - pow(a_,2));///(2 * distancia_deseada * b);
		A = A/(2 * b * c);
		A =  acos(A) * 180 / M_PI;			
				return round(A);
	}
//funcion para calcular los grados que se debe mover el motor
float Teorum_of_cosenum_degree2(int a_)	
	{
		B =  (pow(a_,2) + pow(c,2) - pow(b,2));///(2 * distancia_deseada * b);
		B = B/(2 * a_ * c);
		B =  acos(B) * 180 / M_PI;
				return round(B);
	}

/*Creamos una funcion que nos sirve para inicializar los pines de entrada y salida, los cuales son 
clasificados en pines de entrada y salida, mas adelante iniciamos la funcion en el app_main*/
void Show_infor(){
Serial.println("ROMEO_BLUE_V1.0\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5775#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~~~~~~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~~^~~^~!YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YBBY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&BY7~^^~^~!JP#@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@&GJ!~^~~^~!JG&@@@@@@@@@@@@@@@@@@&GY!~^~~^~!JG&@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~^^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P?~^^~^^~?5B@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@&BY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@&GJ!~^~~^~7YG&@@@@@@@@@@#GG@@@@@@@@@@@@@@@@@@BP#&@@@@@@@@@&GY7~^~~^~!JG&@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@#P?!~^~^^~75B@@@@@@@@@@&GJ7~^?@@@@@@@@@@@@@@@@@@Y^~!JP&@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@\n");
Serial.println("@@@@@@@@B5?~^^~^^~?P#@@@@@@@@@@@@P7~^~~~?@@@@@@@@@@@@@@@@@@Y^~~^^!5@@@@@@@@@@@@#P?!^^~^^~75B@@@@@@@@\n");
Serial.println("@@@@&BY7~^~~^~!JP&@@@@@@@@@@@@@@J^~~~~~~?@@@@@@@@BG@@@@@@@@Y^~~~~~^?&@@@@@@@@@@@@@&GJ!~^~~^~7YB&@@@@\n");
Serial.println("@&GJ!~^~~^~7YG&@@@@@@@@@@@@@B5JG~~~~~~~~?@@@@@@@#!~G@@@@@@@Y^~~~~~~^PYYG&@@@@@@@@@@@@&GY7~^~~^~!JG&@\n");
Serial.println("J!~^~~^~75B@@@@@@@@@@@@@@@G7~^~5~~~~~~~~?@&####B!~~~G#####@Y^~~~~~~~5!^~!P@@@@@@@@@@@@@@@B57~^~~^^!J\n");
Serial.println("7~~~~7P#@@@@@@@@@@@@@@@@@5~~~~~Y!~~~~~~~?@&Y!~~~~~~~~~~~J#@Y^~~~~~~~5~~~~^J@@@@@@@@@@@@@@@@@#P7~~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@@@@@P^~~~~~??~~~~~~~?@@@#Y~~~~~~~~J#@@@Y^~~~~~~7J~~~~~^Y@@@@@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@@@BGJ^~~~~~!Y~~~~~~~?@@@@&~~^^~^~~G@@@@Y^~~~~~~J7~~~~~~7PB@@@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@G?~~5!~~~~~~J!~~~~~~?@@@@5~7JGGY7~?@@@@Y^~~~~~~J~~~~~~~5~~7P@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@J~~~~?5~~~~~~!J~~~~~~?@@@@B#@@@@@@#B@@@@Y^~~~~~?7~~~~~^JJ~~~^?&@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@J^~~~~~YJ^~~~~~?!~~~~~?@@@@@@@@@@@@@@@@@@Y^~~~~~J~~~~~^75~~~~~^7&@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@#~~~~~~~~5?^~~~~~J~~~~~?@@@@@@@@@@@@@@@@@@Y^~~~~?!~~~~^75~~~~~~~^G@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@GGJ^~~~~~~~Y?~~~~~77~~~~?@@@@@@&#G5P#@@@@@@Y^~~~!?~~~~^75!~~~~~~^?GG&@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@B7^~5?^~~~~~~~JJ~~~~~J!~~~?@@@@@B!~~^^!JPB@@@Y^~~~J~~~~~?Y~~~~~~~~7P!^!G@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@#!^~~~Y?~~~~~~~~?Y!~~~!J~~~?@@@@@G^~~~~~!!!&@@Y^~~?7~~~~JJ~~~~~~~~7Y!~~~~B@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@5^~~~~~7?!~~~~~~~!Y7~~~??^~?@@@@@J^~~~~^P@#@@@Y^~7J~~~!J7~~~~~~~~??~~~~~^J@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@Y^~~~~~~~77!~~~~~~~?J~~~Y?^?@@@@B~~~~~~^J@@@@@Y^7Y~~~?J~~~~~~~~77!~~~~~~^?@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@#!^~~~~~~~~!!!~~~~~~!J?~~5??@@@&7~~~~~~~~B@@@@Y7P~~7J7~~~~~~!!!~~~~~~~~^~G@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@G557~~~~~~~~~!7!!~~~~~?Y?~PB@@B7~~~~~~~~~!P&@@#G!7Y?~~~~~!!7!~~~~~~~~~7YPP@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@#~~7JYJ7!~~~~~~~~!77!~~^~JP5BP?~~~~~~~~~~~~~!JPB5PY!^~~!777~~~~~~~~!7JYJ7~^G@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@P^~~~~!7???7!~~~~~~!7??7!~~Y5^~~~~~~~~~~~~~~~^^JP!~!7??7!~~~~~~!77??7!~~~~^Y@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@B~~~~~~~~~~!77777!!~~~~!7???P~~~~~~~~~~~~~~~~~^YY!1?!~~~~!!!7777!~~~~~~~~~^P@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@?^~~~~~~~~~~~~~!!77777!!!!?5~~~~~~~~~~~~~~~~~^YY!!!!77777!!~~~~~~~~~~~~~^7&@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@&Y7!!!~~~~~~~~~~~~~~~~~!!!?5~~~~~~~~~~~~~~~~~~JJ!!!!~~~~~~~~~~~~~~~~!!!7J&@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@?!7?????????7777777777777JY^~~~~~~~~~~~~~~~~~JY77777777777777????????777&@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@J^~~~~~~~~~~~~~~~~~~~~!77?P^~~~~~~~~~~~~~~~~^5Y77!!~~~~~~~~~~~~~~~~~~~^7@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@&J~^~~~~~~~~~~~~!!!777!!!75J!^~~~~~~~~~~~~^~J57!!!777!!!~~~~~~~~~~~~^^?#@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@BJ!!!!77777777!!~!!!!77!!Y&57~~~~~~~~~~!J#P!!77!!!!~!!!7777777!!!!?G@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@Y!7!!!~~~~~!!7!!777!7?B@@@5!!~~~~~!?#@@&?77!77!!77!~~~~~!!!7!?&@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@#?~^^~~!777!!!!!!!!!!B@@@G~~77!!!!!~5@@@#77?!!7?7!!777!~~^^~7B@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@@#GP7!!~~!7?7~~7?!!B@@@#~~~~~!!~~~~~G@@@&?~??~-1!!!~~!!75G#@@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@@@@@#Y???7~^^!?!~Y&@@@@Y^~~~~~~~~~~^?@@@@@5~!?7~^~7???YB@@@@@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~^5@@@@@@@@@@@@@@@@@@@@@@#GPPGGY5#@@@@@@Y^~~~!777~^~~J@@@@@@#PYPBPPPB@@@@@@@@@@@@@@@@@@@@@@5^~~~7\n");
Serial.println("7~~~~7P#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#J~?5J!~7P5!!B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P7~~~~7\n");
Serial.println("J!~^~~^~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&?YPYB7~~~~!BP5PY&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~^~~^^!J\n");
Serial.println("@&GJ!~^~~^~7YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@P~~~~~~~~P@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~!JG&@\n");
Serial.println("@@@@&BY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@Y~~~~~~~~~~J&@@@@@@@@@@@@@@@@@@@@@@@@@&GJ!~^~~^~7YB&@@@@\n");
Serial.println("@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@#7~7~!~~~!!!~~!G@@@@@@@@@@@@@@@@@@@@@#P?!^^~^^~?5B@@@@@@@@\n");
Serial.println("@@@@@@@@@@@#PJ!~^~^^~75B@@@@@@@@@@@@@@@@@P~~?~!!!~~7~7~!~~J#@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@&GJ!~^~~^~!YG&@@@@@@@@@@@@Y^~J!~?~7~~?~!?~7~~Y@@@@@@@@@@@@&GY7~^~~^~!JG&@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@&BY7~^~~^~!JP#@@@@@@@@@#P5?^~?^?~~?~^J7?G&@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@&BG5?Y775YPB&@@@@@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@#PJ!~^~~^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@&BY7~^^~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY!~^~~^~!JG&@@@@@@@@@@@@@@@@@@&GJ!~^~~^~!JG&@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~^^~^~!?P#@@@@@@@@@@@@#PJ!~^~^^~7YB&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#P?~^^~^^~?5B@@@@@@#5?~^^~^^~?5#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&PJ!~^~~^~7YBBY7~^~~^~!JP#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&GY7~^~~^~~^~~^~7YG&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B57~~~~~~75B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#5775#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
Serial.println("Configuraciones iniciales\n");
Serial.println("Dispositivos de Salida:\n");
Serial.println("Motor 1 ----------> Gpio " + String(dir_pin1) + " /Gpio "+ String(step_pin1));
Serial.println("Motor 2 ----------> Gpio %d " + servo_pin);
Serial.println("\n");
Serial.println("El dispositivo esta listo para iniciar, esperando al dispositivo maesto\n");
Serial.println("\n");
}

void setup_interrupts(void* pvParameters){

for (;;)
{ 
	if (digitalRead(BD) == 1 && digitalRead(AD) == 1)
	{
		digitalWrite(2, 0);
		esp_restart();
	}
	vTaskDelay(10);
}
}



void setup(){
Serial.begin(115200);
Show_infor();//llamada de la funcion para mostrar informacion

pinMode(dir_pin1, OUTPUT);
pinMode(step_pin1, OUTPUT);
pinMode(servo_pin, OUTPUT);
pinMode(cv_pin1_cw, OUTPUT);
pinMode(cv_pin2_ccw, OUTPUT);
pinMode(AD, INPUT);
pinMode(BD, INPUT);
pinMode(2, OUTPUT);
pinMode(magnet_negative, OUTPUT);
pinMode(magnet_positive, OUTPUT);
}


void  loop(){
Angle_Elbow_parameters Elbow1_class = 
				{
					.steps = 0,
					.sentido_de_giro = false,
					.rpm = rev_per_min,
				};
servoAngle Elbow2_class = 
				{	
					.digreees = 0,
				};


xTaskCreate(setup_interrupts,"setup_interrupts",1024*2, NULL,10,NULL);//creacion de la tarea para el motor 1


//Movimiento 1 (Calibracion)
	Elbow1_class.steps = 0;
	Elbow1_class.sentido_de_giro = true; //True = derecha //False = izquierda
	Elbow2_class.digreees = 90;
	EVENT_GET_CARGO = xEventGroupCreate();
	xTaskCreate(mt_ph296_02b_m,"Task 1",1024*2, &Elbow1_class,1,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(mt_mgg95_m,"Task 2",1024*2, &Elbow2_class,1,NULL);//creacion de la tarea para el motor 2
	xEventGroupWaitBits(EVENT_GET_CARGO, Elbow1F | Elbow1F, true, true, portMAX_DELAY);
do{
	vTaskDelay(1000/portTICK_PERIOD_MS);
}while (digitalRead(AD) == 0 && digitalRead(BD) == 0);
Serial.println("El dispositivo maestro esta listo\n");
digitalWrite(2, 1);

//Movimiento 2 grab de la carga
	Elbow1_class.steps = 50;
	Elbow2_class.digreees = 0;
	EVENT_GET_CARGO = xEventGroupCreate();
	xTaskCreate(mt_ph296_02b_m,"Task 1",1024*2, &Elbow1_class,1,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(mt_mgg95_m,"Task 2",1024*2, &Elbow2_class,1,NULL);//creacion de la tarea para el motor 2
	xEventGroupWaitBits(EVENT_GET_CARGO, Elbow1F | Elbow1F, true, true, portMAX_DELAY);
delay(1000);
digitalWrite(magnet_negative, LOW);
digitalWrite(magnet_positive, HIGH);
for (int i = 0; i < 6; i++)
{
digitalWrite(2, LOW);
	delay(500);
digitalWrite(2, HIGH);
	delay(500);
}


//Movimiento 3 pos de la carga
	Elbow1_class.steps = 65;
	Elbow1_class.sentido_de_giro = false; //True = derecha //False = izquierda
	Elbow2_class.digreees = 180;
	EVENT_GET_CARGO = xEventGroupCreate();
	xTaskCreate(mt_ph296_02b_m,"Task 1",1024*2, &Elbow1_class,1,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(mt_mgg95_m,"Task 2",1024*2, &Elbow2_class,1,NULL);//creacion de la tarea para el motor 2
	xEventGroupWaitBits(EVENT_GET_CARGO, Elbow1F | Elbow1F, true, true, portMAX_DELAY);
delay(500);
digitalWrite(magnet_negative, LOW);
digitalWrite(magnet_positive, LOW);
//Movimiento 4 regreso
	Elbow1_class.steps = 15;
	Elbow1_class.sentido_de_giro = true; //True = derecha //False = izquierda
	Elbow2_class.digreees = 90;
	EVENT_GET_CARGO = xEventGroupCreate();
	xTaskCreate(mt_ph296_02b_m,"Task 1",1024*2, &Elbow1_class,1,NULL);//creacion de la tarea para el motor 1
	xTaskCreate(mt_mgg95_m,"Task 2",1024*2, &Elbow2_class,1,NULL);//creacion de la tarea para el motor 2
	xEventGroupWaitBits(EVENT_GET_CARGO, Elbow1F | Elbow1F, true, true, portMAX_DELAY);

digitalWrite(cv_pin1_cw, HIGH);
digitalWrite(cv_pin2_ccw, LOW);
delay(2000);
digitalWrite(cv_pin1_cw, LOW);
digitalWrite(cv_pin2_ccw, LOW);
}



