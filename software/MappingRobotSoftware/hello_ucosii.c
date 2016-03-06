/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/
#define I2C_DEBUG

#include <stdio.h>
#include "includes.h"
#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_uart_regs.h>
#include "i2c_opencores.h"
#include "packet_buffer.h"
#include "terasic_lib/terasic_includes.h";

typedef struct {
	char* buf;
	int len
} mBuffer;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define LIDAR_I2C_ADDRESS   0xC4

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{

  char leds = 0;
  int data = 12;
  int status = 0;
  int j;

  	//Terasic I2C
  	//Test Addressing
    I2C_Start(I2C_SCL_BASE, I2C_SDA_BASE);
    status = i2c_selectAddress(I2C_SCL_BASE, I2C_SDA_BASE, LIDAR_I2C_ADDRESS);
    printf("I2C status: %i\n", status);
    I2C_Stop(I2C_SCL_BASE, I2C_SDA_BASE);

    //laser loop
    while(1) {
    	bool ack = FALSE;
    	alt_8 poll_value = 0x04;
    	alt_8 upper = 0;
    	alt_8 lower = 0;
    	int range = 0;
    	//Poll laser
    	while(ack == FALSE) {
    	    I2C_Start(I2C_SCL_BASE, I2C_SDA_BASE);
    		ack = I2C_WriteToDeviceRegister(I2C_SCL_BASE, I2C_SDA_BASE, LIDAR_I2C_ADDRESS, 0x00, &poll_value, 1);
    		I2C_Stop(I2C_SCL_BASE, I2C_SDA_BASE);
    		OSTimeDlyHMSM(0, 0, 1, 0);
    	}
        I2C_Start(I2C_SCL_BASE, I2C_SDA_BASE);
    	if(FALSE == I2C_ReadFromDeviceRegister(I2C_SCL_BASE, I2C_SDA_BASE, LIDAR_I2C_ADDRESS, 0x0f, &upper, 1, FALSE)) {
    		printf("I2C READ1 FAILED\n");
    	}
    	else {
        	I2C_Stop(I2C_SCL_BASE, I2C_SDA_BASE);
    		I2C_Start(I2C_SCL_BASE, I2C_SDA_BASE);
    		if(FALSE == I2C_ReadFromDeviceRegister(I2C_SCL_BASE, I2C_SDA_BASE, LIDAR_I2C_ADDRESS, 0x10, &lower, 1, TRUE)) {
    			printf("I2C READ2 FAILED\n");
    		}
    		else {
    			printf("\nresults: %i, %i\n", upper, lower);
    			range = (upper << 8) + lower;
    			printf("RANGE: %i\n", range);
    		}
    	}
    	I2C_Stop(I2C_SCL_BASE, I2C_SDA_BASE);
    }




    //Init I2C
    //I2C_init(I2C_OPENCORES_LIDAR_BASE, ALT_CPU_CPU_FREQ, 35000);
    //I2C_init(I2C_OPENCORES_LIDAR_BASE, ALT_CPU_CPU_FREQ, 100000);

    //printf("I2C initialized\n");
    //Writing to EEPROM
    //status = I2C_start(I2C_OPENCORES_LIDAR_BASE, 0x50, 0);
    //I2C_write(I2C_OPENCORES_LIDAR_BASE, 0, 1);
    //printf("Status: %i\n", status);
    //frustration test
    /*
    for (j = 0; j < 128; j++) {
    	status = I2C_start(I2C_OPENCORES_LIDAR_BASE, j, 0);
    	I2C_write(I2C_OPENCORES_LIDAR_BASE, 0, 1);
    	printf("Status: %i\n", status);
    	if(status == 0) {
    		printf("Address: %i\n", j);
    		break;
    	}
    	OSTimeDlyHMSM(0, 0, 1, 0);
    }
	*/

//    for (j = 0; j < 7; j++) {
  	  //I2C_write(I2C_OPENCORES_LIDAR_BASE, j, 0);
//    }
    /*
    //Last write
    I2C_write(I2C_OPENCORES_LIDAR_BASE, j, 1);
    printf("Finished writing to EEPROM\n");
    //Ensure completion
    //while(I2C_start(I2C_OPENCORES_LIDAR_BASE, 0x50, 0));
    I2C_start(I2C_OPENCORES_LIDAR_BASE, 0x50, 0);
    //Reading from EEPROM
    //note that you must write the the address you want to read
    //from the EEPROM to the EEPROM before reading
    I2C_write(I2C_OPENCORES_LIDAR_BASE, 0, 0);
    I2C_start(I2C_OPENCORES_LIDAR_BASE, 0x50, 1);
    printf("Reading from EEPROM\n");
    for(j = 0; j < 7; j++) {
  	  data = I2C_read(I2C_OPENCORES_LIDAR_BASE, 0);
  	  printf("%i\n", data);
    }
    //Last read
    data = I2C_read(I2C_OPENCORES_LIDAR_BASE, 1);
    printf("%i\n", data);
    printf("Finished reading from EEPROM\n");
	*/
  printf("LEDs are changing\n");
  while (1)
  {
    IOWR(PIO_LEDS_BASE, 0, leds++);

//    if(IORD_ALTERA_AVALON_UART_STATUS(UART_BLUETOOTH_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK) {
//        printf("Writing: 5\n");
//    	IOWR_ALTERA_AVALON_UART_TXDATA(UART_BLUETOOTH_BASE, '5');
//    }
    //printf("%d\n", leds);
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}
/* Queue Configuration */
// TODO: Make sure QUEUE_LENGTH is big enough for packets being sent
// Needs to be able to contain roughly an entire packet at once
// because the interrupts get fired in rapid succession when a packet
// is sent, before the task has a chance to empty it out.
// TODO: Maybe rethink what data structure we're using here
#define QUEUE_LENGTH 256
OS_EVENT* queue;
void* queueBuf[QUEUE_LENGTH];

#define QUEUE2_LENGTH 10
OS_EVENT* queue2;
void* queue2Buf[QUEUE_LENGTH];

void task2(void* pdata) {
	char message = '1';
	INT8U err;
	PacketBuffer pb;
	init(&pb);
	mBuffer buf;
	int len;

	while(1) {
		message = (char) OSQPend(queue, 0, &err);
		printf("Received %i\n", message);
		if(message == START_BYTE) {
			printf("Start of Packet!\n");
			clear(&pb);
		} else if(message == END_BYTE) {
			printf("End of Packet!\n");
			buf.buf = (char*) malloc(BUF_SIZE * sizeof(char));
			memset(buf.buf, 0, BUF_SIZE);
			buf.len = mRead(&pb, buf.buf);
			printf("Packet contents: %s\n", buf);
			OSQPost(queue2, (void*) &buf);
			// TODO: Do something with the buffer now
		} else {
			if(pushChar(&pb, message)) {
				printf("Putting char: %s\n", &message);
			} else {
				printf("No room in buffer!\n");
			}
		}

	}
}
static void uart_irq(void* context) {
	static char read;
	// NOTE: This read also clears the status register, notifying
	// the UART core that the next interrupt can be fired.
	read = IORD_ALTERA_AVALON_UART_RXDATA(UART_BLUETOOTH_BASE);
	OSQPost(queue, (void*) read);
}

void task3(void* pdata) {
	mBuffer buf;
	INT8U err;
	int nextByte = 0;
	while(1) {
		nextByte = 0;
		buf = *((mBuffer*) OSQPend(queue2, 0, &err));

		printf("From task 3: packet: %s\n", buf.buf);

		printf("Task 3 :: Sending\n");
		while(!(IORD_ALTERA_AVALON_UART_STATUS(UART_BLUETOOTH_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_BLUETOOTH_BASE, START_BYTE);

		printf("Task 3 :: Into Loop\n");
		while(nextByte < buf.len) {
			printf("Task 3 :: Sending byte %i: '%i'\n", nextByte, buf.buf[nextByte]);
			while(!(IORD_ALTERA_AVALON_UART_STATUS(UART_BLUETOOTH_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_BLUETOOTH_BASE, buf.buf[nextByte++]);
		}
		printf("Task 3 :: Out of Loop\n");
		while(!(IORD_ALTERA_AVALON_UART_STATUS(UART_BLUETOOTH_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_BLUETOOTH_BASE, END_BYTE);

		free(buf.buf);
	}
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
  
    queue = OSQCreate(queueBuf, QUEUE_LENGTH);
    queue2 = OSQCreate(queue2Buf, QUEUE_LENGTH);
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task3,
                  NULL,
                  (void *)&task3_stk[TASK_STACKSIZE-1],
                  TASK3_PRIORITY,
                  TASK3_PRIORITY,
                  task3_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  if(alt_ic_isr_register(
				  UART_BLUETOOTH_IRQ_INTERRUPT_CONTROLLER_ID,
				  UART_BLUETOOTH_IRQ,
				  &uart_irq,
				  NULL,
				  NULL
  )) {
          printf("Register failed\n");
  }


  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
