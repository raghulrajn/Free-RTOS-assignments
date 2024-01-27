/* FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS project threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this project application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
 * NOTE 2:  This file only contains the source code that is specific to exercise 2
 * Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * NOTE:  Console input and output relies on Windows system calls, which can
 * interfere with the execution of the FreeRTOS Windows port.  This demo only
 * uses Windows system call occasionally.  Heavier use of Windows system calls
 * can crash the port.
 */

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* TODO: Priorities at which the tasks are created.*/
#define TASK1_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK2_PRIORITY (tskIDLE_PRIORITY + 1)
#define TASK3_PRIORITY (tskIDLE_PRIORITY + 3)



/* TODO: output frequencey*/
#define mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS pdMS_TO_TICKS( 1000 )

/*-----------------------------------------------------------*/

/*
  * TODO: data structure*/

struct outputString
{
	char string[16];
	int flag;
};

int count = 0;

/*
 * TODO: C function (prototype) for task*/

struct outputString task1 = { "TASK1", 0 };
struct outputString task2 = { "TASK2", 0 };
struct outputString task3 = { "TASK3", 1 };

void printTask1(void);
void printTask2(void);
void printTask3(void);
/*-----------------------------------------------------------*/
TaskHandle_t xT1Handle;
TaskHandle_t xT2Handle;
TaskHandle_t xT3Handle;
/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_exercise( void )
{
	xTaskCreate(printTask1,                   /*function that has to be implemented by task*/
                "T1",                         /*Text name for the task*/
                configMINIMAL_STACK_SIZE,     /*Stack size in words*/
                NULL,                         /*parameters passed into task(function)*/
                TASK1_PRIORITY,               /*Priority of the task*/
                &xT1Handle);                        /*Used to pass out the created task's handle*/

	xTaskCreate(printTask2, 
                "T2", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                TASK2_PRIORITY, 
                &xT2Handle);
    xTaskCreate(printTask3, 
                "T3", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                TASK3_PRIORITY, 
                &xT3Handle);
	/* Starting the scheduler.*/
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* 
 * TODO: C function for tasks*/
void printTask1(void)
{
    TickType_t xNextWakeTime;
    /*Initializing the blocking time between printing strings of a task instance */
    const TickType_t xBlockTime = mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS;
    /* Initialise xNextWakeTime */
    xNextWakeTime = xTaskGetTickCount();
    /*Output the string of task1 for every 1000ms infinitely.*/
    while (1)
    {
        printf("%s \n", task1.string);
        vTaskDelayUntil(&xNextWakeTime, xBlockTime);
    }
}
void printTask2(void)
{
    TickType_t xNextWakeTime;
    /*Initializing the blocking time between printing strings of a task instance */
    const TickType_t xBlockTime = mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS;
    /* Initialise xNextWakeTime */
    xNextWakeTime = xTaskGetTickCount();
    /*Output the string of task2 for every 1000ms infinitely.*/
    while (1)
    {
        printf("%s \n", task2.string);
        vTaskDelayUntil(&xNextWakeTime, xBlockTime);
    }
}
void printTask3(void)
{
    TickType_t xNextWakeTime;
    /*Initializing the blocking time between printing strings of a task instance */
    const TickType_t xBlockTime = mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS;
    /* Initialise xNextWakeTime */
    xNextWakeTime = xTaskGetTickCount();
    /*Output the string of task3 for every 1000ms for 5 times.*/
    while (count < 5)
    {
        printf("%s \n", task3.string);
        vTaskDelayUntil(&xNextWakeTime, xBlockTime);
        count++;
    }
    /*Terminates the task printTask3 and output the termination message and delete
    the task using the task handle*/
    if (count == 5)
    {
        printf("\nTask3_terminated\n");
        vTaskDelete(xT3Handle);  /*Deletes the task printTask3*/
    }
}
