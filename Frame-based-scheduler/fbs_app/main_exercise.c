/*
 * FreeRTOS Kernel V10.1.1
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
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
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
#include "semphr.h"


//Creation of Task Handles for Worker Tasks 0 to 5
TaskHandle_t th0 = NULL;    //Task handle for Worker_Task0
TaskHandle_t th1 = NULL;    //Task handle for Worker_Task1
TaskHandle_t th2 = NULL;    //Task handle for Worker_Task2
TaskHandle_t th3 = NULL;    //Task handle for Worker_Task3
TaskHandle_t th4 = NULL;    //Task handle for Worker_Task4
TaskHandle_t th5 = NULL;    //Task handle for Worker_Task5

//Below function is to check overrun tasks in previous frame

void overruncheck(int f1, int s0, int s1, int s2, int s3, int s4, int s5) //f1 = frame, s0 to s5 = Task_Flags.
{
   
    if (eTaskGetState(th0) != eSuspended)  //eTaskGetState(task handle_x) - Gets the state of task with task handle_x
    {   
        printf("Task 0 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th0);                 //vTaskSuspend(task handle_x) - Suspends the task with task handle_x
    }

    if (eTaskGetState(th1) != eSuspended)
    { 
        printf("Task 1 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th1);
    }

    if (eTaskGetState(th2) != eSuspended)
    {
        printf("Task 2 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th2);
    }

    if (eTaskGetState(th3) != eSuspended)
    {
        printf("Task 3 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th3);
    }

    if (eTaskGetState(th4) != eSuspended)
    {
        printf("Task 4 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th4);
    }

    if (eTaskGetState(th5) != eSuspended)
    {
        printf("Task 5 in frame %d was not suspended. Sad\n", f1);
        vTaskSuspend(th5);
    }
}

//Below function is to execute the eligible tasks in a particular frame

void eligibleframe(int* temp)    //temp array contains worker_tasks
{
    if (temp[0] == 1) 
    {
        vTaskResume(th0); //vTaskResume(Task_Handle_x) - Resumes the suspended task with Task_Handle_x
    }

    if (temp[1] == 1) 
    {
        vTaskResume(th1);
    }

    if (temp[2] == 1) 
    {
        vTaskResume(th2);
    }

    if (temp[3] == 1) 
    {
        vTaskResume(th3);
    }

    if (temp[4] == 1) 
    {
        vTaskResume(th4);
    }

    if (temp[5] == 1) 
    {
        vTaskResume(th5);
    }
}

void scheduler(void);
void work0(void);
void work1(void);
void work2(void);
void work3(void);
void work4(void);
void work5(void);

//Data structure - 2D array storing the task list 
int frame_list[5][6] = { {1,1,1,1,1,1},{0,0,0,0,0,0},{1,1,0,0,1,0},{0,0,1,1,0,0},{1,1,0,0,1,0} };
void main_exercise(void)
{

    //Scheduler and Worker Task Creations
    xTaskCreate(scheduler, "Scheduler_Task", configMINIMAL_STACK_SIZE, NULL, +1, NULL);
    xTaskCreate(work0, "Worker_Task0", configMINIMAL_STACK_SIZE, NULL, 0, &th0);
    xTaskCreate(work1, "Worker_Task1", configMINIMAL_STACK_SIZE, NULL, 0, &th1); 
    xTaskCreate(work2, "Worker_Task2", configMINIMAL_STACK_SIZE, NULL, 0, &th2);
    xTaskCreate(work3, "Worker_Task3", configMINIMAL_STACK_SIZE, NULL, 0, &th3);
    xTaskCreate(work4, "Worker_Task4", configMINIMAL_STACK_SIZE, NULL, 0, &th4);
    xTaskCreate(work5, "Worker_Task5", configMINIMAL_STACK_SIZE, NULL, 0, &th5);
    vTaskStartScheduler();   //Starting the scheduler
    for (;;)
        ;
}
//Below: Scheduler Task Function

void scheduler(void)
{
            int i = 0;
            while (i < 5)
            {
                int frame = i;   //Frame Selection
                //Reading the task execution status for selected frame
                int st0 = frame_list[i][0];
                int st1 = frame_list[i][1];
                int st2 = frame_list[i][2];
                int st3 = frame_list[i][3];
                int st4 = frame_list[i][4];
                int st5 = frame_list[i][5];
                int* temp = frame_list[i];
                if (i == 0)                                                //Only for the initial frame of hyper period
                {
                    printf("\n \nIt's the very first frame\n");
                    printf("Scheduling for frame 0\n");
                    printf("In frame %d, the tasks to be run: %d %d %d %d %d %d \n", i, st0, st1, st2, st3, st4, st5);
                    eligibleframe(temp);
                    // eligibleframe(st0, st1, st2, st3, st4, st5);           //Execution of required tasks
                }

                if(i>0)                                                    //For frames other than the first frame
                {
                    printf(" \n \nScheduling for frame %d\n", frame);
                    printf("In frame %d, the tasks to be run: %d %d %d %d %d %d \n",i , st0, st1, st2, st3, st4, st5);
                    printf("Checking previous frame for over run tasks %d... \n", frame - 1);
                    overruncheck(frame - 1, st0, st1, st2, st3, st4, st5); //Check of overrun tasks from the previous frame
                    eligibleframe(temp);
                    // eligibleframe(st0, st1, st2, st3, st4, st5);           //Execution of required tasks
                }
                i++;                                                       //Frame Increment
                vTaskDelay(120 * configTICK_RATE_HZ / 1000);               //Delay of Hyper Period (120ms)
                if (i == 5)                                                //Reset of Frame variable once hyper period is reached
                {
                    i = 0;
                }
            }
}

//Below: Finite Counter implementations for Worker Tasks 0 to 4

//Worker Task 0
void work0(void)       
{
    int counter = 0;
    there:
    while (counter < 1000000) 
    {
        counter++;
    }
    long ticks = xTaskGetTickCount();
    printf("Task 0 is executing... \n"),
    printf("Task 0 counted 1000000 cycles, took ~ %ld ticks.\n", ticks);
    vTaskSuspend(th0);   // vTaskSuspend(Task_Handle_x) - Suspends the task with Task_Handle_x
    counter = 0;
    goto there;          // Jumps to the while() when the vTaskResume() is called   
}

//Worker Task 1
void work1(void)
{
    
    int counter = 0;
    there:
        while (counter < 1000000)
        {
            counter++;
        }
    long ticks = xTaskGetTickCount();
    printf("Task 1 is executing... \n"),
    printf("Task 1 counted 1000000 cycles, took ~ %ld ticks.\n", ticks);
    vTaskSuspend(th1);
    counter = 0;
    goto there;
}

//Worker Task 2
void work2(void)
{
    int counter = 0;
    there:
        while (counter < 1000000)
        {
            counter++;
        }
    long ticks = xTaskGetTickCount();
    printf("Task 2 is executing... \n"),
    printf("Task 2 counted 1000000 cycles, took ~ %ld ticks.\n", ticks);
    vTaskSuspend(th2);
    counter = 0;
    goto there;
}

//Worker Task 3
void work3(void)
{
    int counter = 0;
    there:
        while (counter < 1000000)
        {
            counter++;
        }
    long ticks = xTaskGetTickCount();
    printf("Task 3 is executing... \n"),
    printf("Task 3 counted 1000000 cycles, took ~ %ld ticks.\n", ticks);
    vTaskSuspend(th3);
    counter = 0;
    goto there;
}

//Worker Task 4
void work4(void)
{
    int counter = 0;
    there:
        while (counter < 1000000)
        {
            counter++;
        }
    long ticks = xTaskGetTickCount();
    printf("Task 4 is executing... \n"),
    printf("Task 4 counted 1000000 cycles, took ~ %ld ticks.\n", ticks);
    vTaskSuspend(th4);
    counter = 0;
    goto there;
}

//Infinite Loop Execution for WorkTask_5
void work5(void)
{
    long counter = 0;
    printf("Task 5 is executing... \n");
    there5:
    while (counter < 9223372036854775807)  //Max value of long data type used.
    {
        counter++;
    }
    counter = 0;
    goto there5;
}
