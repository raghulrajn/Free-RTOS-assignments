/******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html

/* Standard includes. */
#include <stdio.h>
#include <assert.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"



// Define bits position of specific event flags in event group
#define q1 0x01   //00001 - 1 // Queue 1 is ready
#define q2a 0x02  //00010 - 2 // Queue 2a is ready
#define q2b 0x04  //00100 - 4 // Queue 2b is ready
#define c1 0x08   //01000 - 8 // Controller 1 is ACTIVE
#define c2 0x10   //10000 - 16 // Controller 2 is ACTIVE

int config = 15; //01111 - q1,q2a,q2b,controller 1 is active

QueueHandle_t queue1,queue2a,queue2b;
QueueHandle_t xQueueTickCount;

TaskHandle_t consumer1;    //Task handle for consumer1
TaskHandle_t consumer2;    //Task handle for consumer2
TaskHandle_t sensor1;    //Task handle for sensor1
TaskHandle_t sensor2a;    //Task handle for sensor2a
TaskHandle_t sensor2b;    //Task handle for sensor2b
TaskHandle_t schedulerhandler;    //Task handle for sensor2b


//Initial sensor values for 3 different sensors
int counter1  = 100;
int counter2a = 200;
int counter2b = 250;


int sensorval1, sensorval2;
uint32_t uCurrentTickCount;

void scheduler(void *pvParameter);

void consumer_1(void *pvParameter)
{
	// EventBits_t uxBits;
	while(TRUE)
	{
		xQueuePeek(xQueueTickCount, &uCurrentTickCount, (TickType_t)0);

		if (uCurrentTickCount == 2000){
			printf("-------------------------Controller 1 failed at %d-------------------\n", uCurrentTickCount);
			// Delete the controller handle after 2000ms
			consumer1 = NULL;
			
			config = config & ~c1; //Deactivating controller 1 to stop consuming sensor values
			config = config | c2; //activating controller 2 after the failure of controller 1 to start receiving sensor values
			// config = config | q2a;
			// config = config | q1;
			vTaskDelete(consumer1);
			return;
		}


		//Controller 1 will only accept the values when c1 flag is set
		if(config & c1) 
		{
			if ((config & (q1)) && ((config & (q2a))))
			{
				//accepting latest values from the queue 1 and 2a only if both values are present in the queue
				xQueueReceive(queue2a, &sensorval2, (TickType_t) 0);

				xQueueReceive(queue1, &sensorval1, (TickType_t) 0);

				//toggling the config bits for Queue1 and 2a
				config = config & ~q1;
				config = config & ~q2a;
				printf("Controller 1 has received data at %ld :Sensor 1:%d, Sensor2: %d\n",uCurrentTickCount, sensorval1, sensorval2);
			}
			else if ((config & (q1)) && ((config & (q2b))))
			{
				//accepting latest values from the queue 1 and 2b only if both values are present in the queue
				xQueueReceive(queue2b, &sensorval2, (TickType_t) 0);

				xQueueReceive(queue1, &sensorval1, (TickType_t) 0);
				//toggling the config bits for Queue1 and 2b
				config = config & ~q1;
				config = config & ~q2b;

				printf("Controller 1 has received data at %ld :Sensor 1:%d, Sensor2: %d\n",uCurrentTickCount, sensorval1, sensorval2);
			}
			vTaskSuspend(consumer1);
		}	
	}
		
}


void consumer_2(void *pvParameter)
{
	EventBits_t uxBits;
	for(;;)
	{
		
		xQueuePeek(xQueueTickCount, &(uCurrentTickCount), (TickType_t)1);
		//Controller 2 will only accept the values when c2 flag is set and c2 is set only after controller1 becomes faulty
		if((config & c2))
		{
			if ((config & (q1)) && ((config & (q2a))))
			{
				xQueueReceive(queue2a, &sensorval2, (TickType_t) 1);
				xQueueReceive(queue1, &sensorval1, (TickType_t) 1);
				config = config & ~q1;
				config = config & ~q2a;
				printf("Controller 2 has received data at %ld :Sensor 1:%d, Sensor2: %d\n",uCurrentTickCount, sensorval1, sensorval2);
			}
			else if ((config & (q1)) && ((config & (q2b))))
			{
				xQueueReceive(queue2b, &sensorval2, (TickType_t) 0);
				xQueueReceive(queue1, &sensorval1, (TickType_t) 0);
				
				config = config & ~q1;
				config = config & ~q2b;
				printf("Controller 2 has received data at %ld :Sensor 1:%d, Sensor2: %d\n",uCurrentTickCount, sensorval1, sensorval2);
			}
		}
		vTaskSuspend(consumer2);
	}
}

void sensor1_producer(void *pvParameter)
{
	while(TRUE)
	{
		if(counter1>=200)
		{
			counter1 = 100;
		}
		++counter1;
		xQueueOverwrite(queue1, &counter1);
		config = config | q1; //Once sensor value is written in its queue its respective flag is set to 1
		// printf("Sensor1 sent:%d\n",counter1);
		vTaskSuspend(sensor1);
	}
}

void sensor2a_producer(void *pvParameter)
{
	while(TRUE)
	{
		if(counter2a>=250)
		{
			counter2a = 200;
		}
		++counter2a;

		xQueueOverwrite(queue2a, &counter2a);

		config = config | q2a; //Once sensor value is written in its queue its respective flag is set to 1
		// printf("Sensor2a sent:%d\n",counter2a);
		vTaskSuspend(sensor2a);	
	}

}

void sensor2b_producer(void *pvParameter){
	while(TRUE)
	{
		if(counter2b>=300){
			counter2b = 250;
		}
		++counter2b;
		xQueueOverwrite(queue2b, &counter2b);
		config = config | q2b; //Once sensor value is written in its queue its respective flag is set to 1
		// printf("Sensor2b sent:%d\n",counter2b);
		vTaskSuspend(sensor2b);
	}
}

void main_exercise( void )
{

	//seperate queues are created for each sensor task and 1 queue is created to track timeperiod
	//queue size is set to 1, as we are only interested in the latest value in the queue
	queue1 =xQueueCreate(1,sizeof(int));
	queue2a =xQueueCreate(1,sizeof(int));
	queue2b =xQueueCreate(1,sizeof(int));
	xQueueTickCount =xQueueCreate(1,sizeof(int));

	xTaskCreate(scheduler, "scheduler", configMINIMAL_STACK_SIZE, NULL, +2, &schedulerhandler);
    xTaskCreate(sensor1_producer, "sensor1_producer", configMINIMAL_STACK_SIZE, NULL, +3, &sensor1);
    xTaskCreate(sensor2a_producer, "sensor2a_producer", configMINIMAL_STACK_SIZE, NULL, +3, &sensor2a);
    xTaskCreate(sensor2b_producer, "sensor2b_producer", configMINIMAL_STACK_SIZE, NULL, +3, &sensor2b);
	xTaskCreate(consumer_1, "consumer_1", configMINIMAL_STACK_SIZE, NULL, +1, &consumer1);
	xTaskCreate(consumer_2, "consumer_2", configMINIMAL_STACK_SIZE, NULL, +2, &consumer2); 
	vTaskStartScheduler();   //Starting the scheduler

}
/*-----------------------------------------------------------*/

void scheduler(void *pvParameter){
	
	uint32_t uCurrentTickCount = 0;
	
	while (TRUE)
	{
		//Sensor values should be consumed by controller 1 until it fails. and 
		// After 2000ms controller 1 fails and controller 2 starts accepting sensor values
		if(consumer1 !=NULL)
		{
			if((uCurrentTickCount>0))
			{
				vTaskResume(consumer1);
			}
		}
		
		if((uCurrentTickCount>2000))
		{
			vTaskResume(consumer2);
		}
		


		//Each sensor has seperate intervals to produce values and each secor tasks are activated respectiv to their frequencies
		//eg Sensor 1 increments its value every 200ms, uCurrentTickCount is the time period and for every 200ms, sensor1 task is activated
		if((uCurrentTickCount>=200)&&(uCurrentTickCount % 200)==0 )
		{
			vTaskResume(sensor1);
		}
		if((uCurrentTickCount>=500)&&(uCurrentTickCount % 500)==0 )
		{

			vTaskResume(sensor2a);
		}
		if((uCurrentTickCount>=1400)&&(uCurrentTickCount % 1400)==0 )
		{

			vTaskResume(sensor2b);
		}
		//currentTickcount is updated after every 1ms to count the timeperiod
		++uCurrentTickCount;
		xQueueOverwrite(xQueueTickCount, &uCurrentTickCount);
		vTaskDelay(1);
		}
		
	}

