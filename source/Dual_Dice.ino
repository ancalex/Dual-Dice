#include <avr/sleep.h>
#include <avr/interrupt.h> 
uint8_t dice1;
uint8_t dice2;
uint8_t sensorState;
unsigned long startTime = millis();
unsigned long actualTime;
uint8_t seconds;

void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  digitalWrite(3, HIGH);
  pinMode(5, OUTPUT);
  sensorState = LOW;
  dice1 = random(1,7);
  dice2 = random(1,7);
}

void loop() {
  actualTime=millis();
  randomSeed(actualTime);
  if (sensorState == LOW) {
    if (actualTime  - startTime > 999 ) {
      startTime = actualTime;
      seconds ++;
      if (seconds > 10) {
        go_to_sleep();
      }
    }
    for(int k=0;k<400;k++) {
      sensorState = digitalRead(3);
      tone(5, 2000);
      for (int i=0; i<=8; i++) {
        all_Input();
        ShowDice(i);
      }
    }
    dice1 = random(1,7);
    dice2 = random(1,7);
    noTone(5);
  }
  else {  
    int t=400;
    int f=2000;
    for (int n=0;n<=6;n++){
      tone(5, f);
      dice1 = random(1,7);
      dice2 = random(1,7);
      for(int k=0;k<t;k++) {
        for (int i=0; i<=7; i++) {
          all_Input();
          ShowDice(i);
        }
      }
      t=t*1.5;
      f=f/1.1;
    }
    dice1 = random(1,7);
    dice2 = random(1,7);
    tone(5, 800,500);
    for(int k=0;k<32000;k++) {
      for (int i=0; i<=7; i++) {
        all_Input();
        ShowDice(i);
      }
    }
    sensorState = LOW;
    startTime = millis();
    seconds = 0;
  }
}

void all_Input () {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(4, INPUT);
}

void go_to_sleep() {
  all_Input();
  sleep();
  seconds=0;
  startTime = millis();
  sensorState = LOW;
  for(int k=0;k<10000;k++) {
    dice1 = 1;
    dice2 = 1;
    for (int i=0; i<=7; i++) {
      all_Input();
      ShowDice(i);
    }
  }
}

void sleep() {
  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on
  sei();                                  // Enable interrupts
}

void ShowDice(int group) {
  switch(group) {
  //Dice1
  case 0:
    if (dice1 == 1 || dice1 == 3 || dice1 == 5) {
      pinMode(0, OUTPUT);
      digitalWrite(0,HIGH);
    }     
    break;
  case 1:
    if (dice1 == 2 || dice1 == 3 || dice1 == 4 || dice1 == 5 || dice1 == 6) {
      pinMode(0, OUTPUT);
      digitalWrite(0,LOW);
    }     
    break;
  case 2:   
    if (dice1 == 4 || dice1 == 5 || dice1 == 6) {
      pinMode(1, OUTPUT);
      digitalWrite(1,HIGH);
    }     
    break;
  case 3:
    if (dice1 == 6) {
      pinMode(1, OUTPUT);
      digitalWrite(1,LOW);
    }     
    break;

    //Dice2
  case 4:
    if (dice2 == 1 || dice2 == 3 || dice2 == 5) {
      pinMode(2, OUTPUT);
      digitalWrite(2,HIGH);
    }     
    break;
  case 5:
    if (dice2 == 2 || dice2 == 3 || dice2 == 4 || dice2 == 5 || dice2 == 6) {
      pinMode(2, OUTPUT);
      digitalWrite(2,LOW);
    }     
    break;
  case 6:   
    if (dice2 == 4 || dice2 == 5 || dice2 == 6) {
      pinMode(4, OUTPUT);
      digitalWrite(4,HIGH);
    }     
    break;
  case 7:
    if (dice2 == 6) {
      pinMode(4, OUTPUT);
      digitalWrite(4,LOW);
    }     
    break;
  }
}
