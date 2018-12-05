

#include <stdio.h>
#include "xc.h"
#include "pic24_all.h"

#define CONFIG_LED1() CONFIG_RB15_AS_DIG_OUTPUT()
#define RED_LED (_LATB15) 

#define CONFIG_LED2() CONFIG_RB14_AS_DIG_OUTPUT()
#define WHITE_LED (_LATB14) 

#define CONFIG_LED3() CONFIG_RB13_AS_DIG_OUTPUT()
#define GREEN_LED (_LATB13) 
 

#define CONFIG_LED5() CONFIG_RB2_AS_DIG_OUTPUT()
#define SERVO_2 (_LATB2) 

#define CONFIG_LED6() CONFIG_RB3_AS_DIG_OUTPUT()
#define SERVO_3 (_LATB3) 



typedef enum{
    STATE_LOCKED,
    STATE_UNLOCKED,
    IN_PROGRESS,
    ONE,
    TWO,
    THREE,
}state_t;

const char* apsz_state_names[] = {
    "STATE_LOCKED",
    "STATE_UNLOCKED",
    "IN_PROGRESS",
    "ONE",
    "TWO",
    "THREE",
};
int state;
void do_Heartbeat(void){
    
    if ( state ){
    WHITE_LED = !WHITE_LED;
    DELAY_MS(500);
    }
}
// Provide a convenient function to print out the state.
void print_state(state_t e_state) {
  // Force an initial print of the state
  static state_t e_last_state = 0xFFFF;

  // Only print if the state changes.
  if (e_state != e_last_state) {
    e_last_state = e_state;
    // Verify that the state has a string representation before printing it.
    ASSERT(e_state <= N_ELEMENTS(apsz_state_names));
    outString(apsz_state_names[e_state]);
    outChar('\n');
  }
}

void update_state(state_t e_state) {
   // static state_t e_state = STATE_LOCKED;
    
    switch(e_state){
         case STATE_LOCKED:
             //red light on
            // RED_LED = 1;
             WHITE_LED = 1;
             break;
         case IN_PROGRESS:
             //blink white light
             //keep red light on
             state = 1;
             break;
         case STATE_UNLOCKED:
             //turn off red && white light
             //turn on green light
             GREEN_LED = 1;
             WHITE_LED = 0;
             printf("\nCORRECT PASSCODE. ACCESS GRANTED");
             break;
         default:
            ASSERT(0);
     }
     print_state(e_state);
}


static state_t e_state2;
double passcode[] = {'1', '2', '3'};

int main(void) {
    
    //configure hardware here. LED1-3 IS LEDS, LED4-6 IS SERVOS
    CONFIG_LED1();
    CONFIG_LED2();
    CONFIG_LED3();
    CONFIG_LED5();
    CONFIG_LED6();
    //initialize variables
    
    // sets motors to the lock state. send digital code: 000
    SERVO_2 = 0;
    SERVO_3 = 0;
    
    update_state(STATE_LOCKED);
    DELAY_MS(1500);
    
    while(1){
        static state_t e_state2 = ONE;
        
        
        switch(e_state2){
            static uint8_t usr1;
        static uint8_t usr2;
        static uint8_t usr3;
            
            case ONE:
                state = 1;
                printf("Enter  1st digit of passcode: ");
                usr1 = inCharEcho();
                if (usr1 == passcode[0]){
                    update_state(IN_PROGRESS);
                    //adjust motor 1; send digital code: 001
                    SERVO_2 = 0;
                    SERVO_3 = 1;
                    //unlock
                    e_state2 = TWO;
                }
                else{
                    printf("\nINCORRECT PASSWORD. RESETTING");
                    state = 0;
                    return 0;
                    //reset
                }
                break;
            case TWO:
                printf("\nEnter  2nd digit of passcode: ");
                usr2 = inCharEcho();
                if (usr2 == passcode[1]){
                    update_state(IN_PROGRESS);
                    //adjust motor 2; send digital code: 011
                    SERVO_2 = 1;
                    SERVO_3 = 0;
                    //unlock
                    e_state2 = THREE;
                }
                else{
                    printf("\nINCORRECT PASSWORD. RESETTING");
                    //RESET MOTOR 1; send digital code: 000
                    SERVO_2 = 0;
                    SERVO_3 = 0;
                    state = 0;
                    return 0;
                //reset
                }
            case THREE:
                printf("\nEnter  3rd digit of passcode: ");
                usr2 = inCharEcho();
                if (usr2 == passcode[2]){
                    update_state(STATE_UNLOCKED);
                    //adjust motor 3; send digital code: 101
                    SERVO_2 = 1;
                    SERVO_3 = 1;
                    //unlock
                    state = 0;
                }
                else{
                    printf("\nINCORRECT PASSWORD. RESETTING");
                    //RESET MOTOR 1 & 2; send digital code: 010
                    SERVO_2 = 0;
                    SERVO_3 = 0;
                    state = 0;
                    return 0;
                //reset
                }
    }
   
    }
    return 0;
}
