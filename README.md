# Free-RTOS Assignment Tasks
## Chatter Box
The “chatterbox”-
application basically consists of 3 instances of the “chatterbox”-task. Every time the chatterbox-task is executed, it outputs a string (in this exercise the string is simply printed to the console output). The behavior of a task instance is defined at its creation by two values: a) the output string and an integer flag which can have either value 0 (infinite task instance executions) or 1 (task instance shall only be executed 5 times)

## Frame-based Scheduler
Emulating the behavior of a frame-based scheduler in FreeRTOS for the frame list
![image](https://github.com/raghulrajn/Free-RTOS-assignments/assets/76444923/802d36d6-8bb3-49ca-8d38-1b00ee91a07a)

## Intertask communication

The scenario consists of the following components:
- There are three producer tasks (one for Sensor 1, one for Sensor 2A and one for Sensor 2B) which put sensor values into messages queues.
- There is one message queue per sensor task and each sensor task writes its sensor values only into its assigned queue.
- Also there are two consumer tasks which can take sensor values from messages queues (one for Controller 1 and one for Controller 2) and then proceed to do some computation with the sensor values.
<img src="https://github.com/raghulrajn/Free-RTOS-assignments/assets/76444923/619569b1-420c-4c07-9a55-92157d225da4" width="100" height="100" />
![image](https://github.com/raghulrajn/Free-RTOS-assignments/assets/76444923/619569b1-420c-4c07-9a55-92157d225da4 =100x100)
