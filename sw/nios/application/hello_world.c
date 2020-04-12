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


#include <stdio.h>
#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_adc.h"
#include <math.h>
#include <time.h>

///* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
//OS_STK    task1_stk[TASK_STACKSIZE];
//OS_STK    task2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define START_ADDR 0x040000a0 //67108896
#define END_ADDR 0x07FFFFFF //134217727
#define DELTA_ADDR 0x00CCCCC6
#define DURATION 5
#define EFFECT_DURATION 4
#define SAMPLING_RATE 48000
#define NUM_ELEM DURATION*SAMPLING_RATE
#define SAMPLING_PERIOD 0.0208333333
#define FLASHING_PERIOD 0.5
#define IDLE 0
#define RECORDING 1
#define PLAYBACK 2
#define PLAYBACK_FILTERED 3
#define NB_LEDS 10
#define NB_FLASHES 5
#define IDLE 0
#define FIRST_RECORDING 1
#define SECOND_RECORDING 2
#define THIRD_RECORDING 3
#define FOURTH_RECORDING 4
#define FIFTH_RECORDING 5
#define CONTINUOUS_PLAYBACK 6
#define FIRST_PLAYBACK 7
#define FIRST_COMBO 1
#define SECOND_COMBO 2
#define THIRD_COMBO 3
#define FOURTH_COMBO 4



unsigned int* audio_buffer=(unsigned int*)(START_ADDR);
unsigned int* audio_buffer2=(unsigned int*)((START_ADDR+DELTA_ADDR));
unsigned int buffer_avg=0;
unsigned int buffer2_avg=0;
unsigned int* effect1_buffer=(unsigned int*)((START_ADDR+2*DELTA_ADDR));
unsigned int effect1_avg=0;
unsigned int* effect2_buffer=(unsigned int*)((START_ADDR+3*DELTA_ADDR));
unsigned int effect2_avg=0;
unsigned int* effect3_buffer=(unsigned int*)((START_ADDR+4*DELTA_ADDR));
unsigned int effect3_avg=0;
unsigned int* effect4_buffer=(unsigned int*)((START_ADDR+5*DELTA_ADDR));
unsigned int effect4_avg=0;
unsigned int interrupted=0;
unsigned int captured_val=9;
unsigned int sw_val = 10;
unsigned int state=IDLE;
unsigned int beat_state=IDLE;
unsigned int current_index = 0;
unsigned int current_index2 = 0;
alt_up_audio_dev * audio_dev;
alt_up_av_config_dev *audio_conf;

void delay(float milli_seconds)
{
    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}

void record(unsigned int* addr,unsigned int duration,unsigned int* avg,alt_up_audio_dev * audio_dev){
	alt_printf("Get ready to mingle\n");
	alt_up_audio_reset_audio_core(audio_dev);
	for(int index=0;index<duration*SAMPLING_RATE;index++){
		int fifospace = alt_up_audio_read_fifo_avail (audio_dev, ALT_UP_AUDIO_RIGHT);
		if ( fifospace > 0 ) // check if data is available
		{
			alt_up_audio_read_fifo (audio_dev, (addr+index), 1, ALT_UP_AUDIO_RIGHT);
			alt_up_audio_read_fifo (audio_dev, (addr+index), 1, ALT_UP_AUDIO_LEFT);
			*avg=*avg+*(addr+index);
		}
		else{
			index=index-1;
		}
	}
	*avg=*avg/(DURATION*SAMPLING_RATE);
}



void combined_play(unsigned int* first_sound,unsigned int first_duration,unsigned int* second_sound,unsigned int second_duration,alt_up_audio_dev * audio_dev,unsigned int volume){
	unsigned int intermediate=0;
	unsigned int min_duration=first_duration;
	unsigned int max_duration=second_duration;
	unsigned int* max_sound =second_sound;
	unsigned int* min_sound =first_sound;
	if(first_duration>second_duration){
		min_duration=second_duration;
		max_duration=first_duration;
		max_sound=first_sound;
		min_sound =second_sound;
	}
	for(int index=current_index2;index<max_duration*SAMPLING_RATE;index++){
		delay((1000/SAMPLING_RATE));
		if(index<min_duration*SAMPLING_RATE)
			intermediate = volume*((*(max_sound+index))+(*(min_sound+index)));
		else
			intermediate = volume*(*(max_sound+index));
		alt_up_audio_write_fifo (audio_dev, &intermediate, 1, ALT_UP_AUDIO_RIGHT);
		alt_up_audio_write_fifo (audio_dev, &intermediate, 1, ALT_UP_AUDIO_LEFT);
		delay((1000/SAMPLING_RATE));
	}
	current_index2 = 0;
	alt_up_audio_reset_audio_core(audio_dev);
}

void play(unsigned int* addr,unsigned int duration,unsigned int volume,alt_up_audio_dev * audio_dev,unsigned int delaying){
	unsigned int intermediate=0;
	for(int index=0;(index<duration*SAMPLING_RATE) && (beat_state==IDLE);index++){
		current_index = index;
		delay((8000/SAMPLING_RATE));
		alt_up_audio_write_fifo_head(audio_dev, (*(addr+index))*volume, ALT_UP_AUDIO_RIGHT);
		alt_up_audio_write_fifo_head(audio_dev, (*(addr+index))*volume, ALT_UP_AUDIO_LEFT);
		delay((8000/SAMPLING_RATE));
	}

	if(!interrupted)
		alt_up_audio_reset_audio_core(audio_dev);
}

/* Prints "Hello World" and sleeps for three seconds */
//void task1(void* pdata)
//{
//  while (1)
//  {
//    printf("Hello from task1\n");
//    OSTimeDlyHMSM(0, 0, 3, 0);
//  }
//}
///* Prints "Hello World" and sleeps for three seconds */
//void task2(void* pdata)
//{
//  while (1)
//  {
//    printf("Hello from task2\n");
//    OSTimeDlyHMSM(0, 0, 3, 0);
//  }
//}
/* The main function creates two task and starts multi-tasking */
void effects_isr(void* context){

	captured_val=IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_BUTTONS_BASE);
	if(captured_val==1){ // Right Button
		beat_state=FIRST_COMBO;
	}
	else if(captured_val==0xd){
		beat_state=SECOND_COMBO;
	}
	else if(captured_val==0xb){
		beat_state=THIRD_COMBO;
	}
	else if(captured_val==0x7){
		beat_state=FOURTH_COMBO;
	}
	interrupted=1;
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_BUTTONS_BASE, 0xFFFFF);

}
void recording_isr(void* context){

	sw_val=IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW_BASE, 0xFFFFF);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW_BASE, 0);
	switch(sw_val)
	{
	case 0x200:
		state=FIRST_RECORDING;
		break;
	case 0x100:
		state=SECOND_RECORDING;
		break;
	case 0x80:
		state=THIRD_RECORDING;
		break;
	case 0x40:
		state=FOURTH_RECORDING;
		break;
	case 0x20:
		state=FIFTH_RECORDING;
		break;
	case 0x10:
		state=CONTINUOUS_PLAYBACK;
		break;
	case 0x8:
		state=IDLE;
		break;
	case 0x4:
		state=FIRST_PLAYBACK;
		break;
	case 0x2:

		break;
	case 0x1:

	break;
	}

	interrupted=1;
}
int main(void)
{
	unsigned int l_buf=0;
	unsigned int curr_idx=0;
	unsigned int start_idx=0;
	audio_dev = alt_up_audio_open_dev ("/dev/audio_0");
	audio_conf = alt_up_av_config_open_dev("/dev/audio_and_video_config_0");
		if ( audio_dev == NULL)
			alt_printf ("Error: could not open audio device \n");
		else
			alt_printf ("Opened audio device \n");
		/* read and echo audio data */
	alt_up_av_config_reset(audio_conf);
	alt_up_audio_reset_audio_core(audio_dev);
	alt_ic_isr_register(PIO_BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, PIO_BUTTONS_IRQ,effects_isr, NULL, NULL);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_BUTTONS_BASE, 0xFF);
	alt_ic_isr_register(PIO_SW_IRQ_INTERRUPT_CONTROLLER_ID, PIO_SW_IRQ,recording_isr, NULL, NULL);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW_BASE, 0xFFFF);
//	alt_ic_isr_register(PIO_SW_IRQ_INTERRUPT_CONTROLLER_ID, PIO_SW_IRQ,effects_isr, NULL, NULL);
//	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW_BASE, 0xFF);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_LEDS_BASE, 0xFF);
	alt_printf("Getting there\n");
	unsigned int current_idx=0;
	while(1){
		if(state==CONTINUOUS_PLAYBACK){
			if(interrupted!=1)
				play(audio_buffer,DURATION,6,audio_dev,0);
			else
			{
				current_index2=current_index;
				switch(beat_state){
				case FIRST_COMBO:
					combined_play(audio_buffer,DURATION,effect1_buffer,EFFECT_DURATION,audio_dev,6);
					state= CONTINUOUS_PLAYBACK;
					beat_state=IDLE;
					interrupted=0;
					break;
				case SECOND_COMBO:
					combined_play(audio_buffer,DURATION,effect2_buffer,EFFECT_DURATION,audio_dev,6);
					state= CONTINUOUS_PLAYBACK;
					beat_state=IDLE;
					interrupted=0;
					break;
				case THIRD_COMBO:
					combined_play(audio_buffer,DURATION,effect3_buffer,EFFECT_DURATION,audio_dev,6);
					state= CONTINUOUS_PLAYBACK;
					beat_state=IDLE;
					interrupted=0;
					break;
				case FOURTH_COMBO:
					combined_play(audio_buffer,DURATION,effect4_buffer,EFFECT_DURATION,audio_dev,6);
					state= CONTINUOUS_PLAYBACK;
					beat_state=IDLE;
					interrupted=0;
					break;
				}
			}
		}
		else if(interrupted==1){
			IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW_BASE, 0);
			switch(state)
				{
				case FIRST_RECORDING:
					state=IDLE;
					record(audio_buffer,DURATION,&buffer_avg,audio_dev);
					delay(1000);
					play(audio_buffer,DURATION,4,audio_dev,1);
					break;
				case SECOND_RECORDING:
					state=IDLE;
					record(effect1_buffer,EFFECT_DURATION,&effect1_avg,audio_dev);
					delay(1000);
					play(effect1_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case THIRD_RECORDING:
					state=IDLE;
					record(effect2_buffer,EFFECT_DURATION,&effect2_avg,audio_dev);
					delay(1000);
					play(effect2_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case FOURTH_RECORDING:
					state=IDLE;
					record(effect3_buffer,EFFECT_DURATION,&effect3_avg,audio_dev);
					delay(1000);
					play(effect3_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case FIFTH_RECORDING:
					state=IDLE;
					record(effect4_buffer,EFFECT_DURATION,&effect4_avg,audio_dev);
					delay(1000);
					play(effect4_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case FIRST_PLAYBACK:
					state=IDLE;
					delay(1000);
					play(audio_buffer,DURATION,4,audio_dev,1);
					break;
				}
			IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW_BASE, 0xFFFFF);
			IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW_BASE, 0);
			delay(100);
			IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW_BASE, 0xFFFFF);
			IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW_BASE, 0xFFFF);
			interrupted=0;

			switch(beat_state){
				case FIRST_COMBO:
					beat_state=IDLE;
					delay(1000);
					play(effect1_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case SECOND_COMBO:
					beat_state=IDLE;
					delay(1000);
					play(effect2_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case THIRD_COMBO:
					beat_state=IDLE;
					delay(1000);
					play(effect3_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
				case FOURTH_COMBO:
					beat_state=IDLE;
					delay(1000);
					play(effect4_buffer,EFFECT_DURATION,4,audio_dev,1);
					break;
			}
		}
	}
//	record(audio_buffer,DURATION,&buffer_avg,audio_dev);
//	play(audio_buffer,DURATION,1,audio_dev);
//	delay(2000);
//	record(effect1_buffer,EFFECT_DURATION,&effect1_avg,audio_dev);
//	play(effect1_buffer,EFFECT_DURATION,1,audio_dev);
//	alt_printf("Get ready\n");
//	delay(1000);
//	combined_play(audio_buffer,DURATION,effect1_buffer,EFFECT_DURATION,audio_dev);
//	record(effect2_buffer,EFFECT_DURATION,&effect2_avg,audio_dev);
//	play(effect2_buffer,EFFECT_DURATION,1,audio_dev);
//	record(effect3_buffer,EFFECT_DURATION,&effect3_avg,audio_dev);
//	play(effect3_buffer,EFFECT_DURATION,1,audio_dev);
//	record(effect4_buffer,EFFECT_DURATION,&effect4_avg,audio_dev);
//	play(effect4_buffer,EFFECT_DURATION,1,audio_dev);

	alt_printf("Value of avg is : %x\n",buffer_avg);
	if(start_idx==(DURATION*SAMPLING_RATE)-1)
		start_idx=0;

//	while(1){
//		int fifospace = alt_up_audio_read_fifo_avail (audio_dev, ALT_UP_AUDIO_RIGHT);
//		if ( fifospace > 0 ) // check if data is available
//		{
//			// read audio buffer
//			alt_up_audio_read_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_RIGHT);
//			alt_up_audio_read_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_LEFT);
//			// write audio buffer
//			alt_up_audio_write_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_RIGHT);
//			alt_up_audio_write_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_LEFT);
//		}
//	}
//  OSTaskCreateExt(task1,
//                  NULL,
//                  (void *)&task1_stk[TASK_STACKSIZE-1],
//                  TASK1_PRIORITY,
//                  TASK1_PRIORITY,
//                  task1_stk,
//                  TASK_STACKSIZE,
//                  NULL,
//                  0);
//
//
//  OSTaskCreateExt(task2,
//                  NULL,
//                  (void *)&task2_stk[TASK_STACKSIZE-1],
//                  TASK2_PRIORITY,
//                  TASK2_PRIORITY,
//                  task2_stk,
//                  TASK_STACKSIZE,
//                  NULL,
//                  0);
//  OSStart();
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
