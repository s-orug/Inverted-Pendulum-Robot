#include "RPi_Pico_TimerInterrupt.h"

#define TIMER0_INTERVAL_MS        1000
#define TIMER1_INTERVAL_MS        2000

//Init RPI_PICO_Timer, can use any from 0-15 pseudo-hardware timers
RPI_PICO_Timer ITimer0(0);
RPI_PICO_Timer ITimer1(1);
 


bool TimerHandler0(struct repeating_timer *t)
{ 
  Serial.print("Timer 0:\t");
  Serial.println(TIMER0_INTERVAL_MS);
  ITimer0.restartTimer();
  return true;
}

bool TimerHandler1(struct repeating_timer *t)
{
  Serial.print("Timer 1:\t");
  Serial.println(TIMER1_INTERVAL_MS);
  ITimer1.restartTimer();
  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("-------hello------");

  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
  ITimer0.restartTimer();

  ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1);
  ITimer1.restartTimer();
}

void loop(){

}
