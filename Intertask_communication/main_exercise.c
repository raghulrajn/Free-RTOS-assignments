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

/* TODO */

#define QUEUE_LENGTH     5
#define ITEM_SIZE        sizeof(int)
#define QUEUE_SET_LENGTH 2 
#define QUEUE2a_BIT      (1 << 0)
#define QUEUE2b_BIT      (1 << 1)

QueueHandle_t queue1 = NULL;
QueueHandle_t queue2a = NULL;
QueueHandle_t queue2b = NULL;

QueueSetHandle_t xQueueSet1;
QueueSetHandle_t xQueueSet2;

EventGroupHandle_t xEventGroup;

TaskHandle_t consumer1 = NULL;    //Task handle for consumer1
TaskHandle_t consumer2 = NULL;    //Task handle for consumer2
TaskHandle_t sensor1 = NULL;    //Task handle for sensor1
TaskHandle_t sensor2a = NULL;    //Task handle for sensor2a
TaskHandle_t sensor2b = NULL;    //Task handle for sensor2b


void consumer_1(void *pvParameter){
	int counter1;
	int counter2a;
	int counter2b;

	if (queue1 == NULL){
		printf("Consumer 1 Queue is not ready\n");
		return;
	}

	while (1)
	{
		xQueueReceive(queue1, &counter1, (TickType_t) 0);
		xQueueReceive(queue2a, &counter2a, (TickType_t) 0);
		xQueueReceive(queue2b, &counter2b, (TickType_t) 0);
		printf("Contoller 1 has received data at :Sensor 1:%d, Sensor2: %d", counter1, counter2a);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	
}

void consumer_2(void *pvParameter){
	int counter1;
	int counter2a;
	int counter2b;

	if (queue2a == NULL){
		printf("Consumer 2 Queue is not ready\n");
		return;
	}

	while (1)
	{
		xQueueReceive(queue1, &counter1, (TickType_t) 0);
		xQueueReceive(queue2a, &counter2a, (TickType_t) 0);
		xQueueReceive(queue2b, &counter2b, (TickType_t) 0);
		printf("Contoller 2 has received data at :Sensor 1:%d, Sensor2a: %d,Sensor2b: %d\n", counter1, counter2a, counter2b);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	
}

void sensor1_producer(void){

	const TickType_t xDelay = pdMS_TO_TICKS(200);
	
	int counter1 = 100;
	if (queue1 == NULL){
		printf("Queue1 is not ready \n");
		return;
	}
	while(1){
		if (counter1<200){
			xQueueSend(queue1, (void *)&counter1, (TickType_t)0);
			counter1++;
			vTaskDelay(200/portTICK_PERIOD_MS);
		}
		else{
			counter1 = 100;
		}
	}
}
void sensor2a_producer(void *pvParameter){
	int counter2a = 200;
	int counter;
	if (queue2a == NULL){
		printf("Queue2a is not ready \n");
		return;
	}
	while(1){
		if (counter2a<250){
			xQueueSend(queue2a, (void *)&counter2a, (TickType_t)0);
			counter2a++;
			vTaskDelay(500/portTICK_PERIOD_MS);
		}
		else{
			counter2a = 200;
		}
	}
}
void sensor2b_producer(void *pvParameter){
	int counter2b = 250;
	int counter;
	if (queue2b == NULL){
		printf("Queue2b is not ready \n");
		return;
	}
	while(1){
		if (counter2b<300){
			xQueueSend(queue2b, (void *)&counter2b, (TickType_t)0);
			counter2b++;
			vTaskDelay(1400/portTICK_PERIOD_MS);
		}
		else{
			counter2b = 250;
		}
	}
}

const TickType_t xBlockTime = pdMS_TO_TICKS(1); // Adjust the block time as needed/
void main_exercise( void )
{
	printf("helo");
	queue1 =xQueueCreate(1,sizeof(int));
	queue2a =xQueueCreate(1,sizeof(int));
	queue2b =xQueueCreate(1,sizeof(int));

	xQueueSet1 = xQueueCreateSet(3);
	xQueueSet2 = xQueueCreateSet(3);

	xQueueAddToSet(queue2a, xQueueSet2);
    xQueueAddToSet(queue2b, xQueueSet2);
	xQueueAddToSet(queue1, xQueueSet1);

	QueueSetMemberHandle_t xActivatedMember = xQueueSelectFromSet(xQueueSet_2ab);

	xTaskCreate(consumer_1, "consumer_1", configMINIMAL_STACK_SIZE, NULL, +1, &consumer1);
    xTaskCreate(consumer_2, "consumer_2", configMINIMAL_STACK_SIZE, NULL, +1, &consumer2); 
    xTaskCreate(sensor1_producer, "sensor1_producer", configMINIMAL_STACK_SIZE, NULL, 0, &sensor1);
    xTaskCreate(sensor2a_producer, "sensor2a_producer", configMINIMAL_STACK_SIZE, NULL, 0, &sensor2a);
    xTaskCreate(sensor2b_producer, "sensor2b_producer", configMINIMAL_STACK_SIZE, NULL, 0, &sensor2b);
	vTaskStartScheduler();   //Starting the scheduler

}
/*-----------------------------------------------------------*/


/* TODO */
/*-----------------------------------------------------------*/


