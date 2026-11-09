#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define BTN_DEBOUNCE 100
#define ROLL_ANIM_LENGTH 50
#define DISPLAY_ADDRESS 0x70
#define SWITCH_BTN 2
#define ROLL_BTN 3
#define NUM_DICE 6

Adafruit_AlphaNum4 disp = Adafruit_AlphaNum4();
unsigned limits[NUM_DICE] = {4, 6, 8, 10, 12, 20};
unsigned selectedDie = 0;
unsigned long lastButtonPress = 0UL;

bool rolling = false;
bool diceInfoDisplaying = false;
bool switchButtonDown = false;
bool rollButtonDown = false;

void setup()
{
  disp.begin(DISPLAY_ADDRESS);
  disp.setBrightness(1);
 
  displaySelectedDie();
  displayRollResult(limits[selectedDie]);

  pinMode(SWITCH_BTN, OUTPUT);
  pinMode(ROLL_BTN, OUTPUT);
}

void displayRollResult(int roll)
{
  if(roll > 99)
	roll = 99;
  if(roll < 0)
	roll = 0;

  int firstDigit = (roll / 10);
  int secondDigit = (roll - firstDigit * 10);
 
  displaySelectedDie();
  disp.writeDigitAscii(2, '0' + firstDigit);
  disp.writeDigitAscii(3, '0' + secondDigit);
  disp.writeDisplay();
}

void loop()
{
  unsigned long currentMillis = millis();

  if(!rolling)
  {
	if(currentMillis - lastButtonPress > BTN_DEBOUNCE)
	{
  	  if(digitalRead(ROLL_BTN))
  	  {
    	    if(!rollButtonDown)
    	    {
      	      rolling = true;
      	      rollButtonDown = true;
    	    }
  	  }
  	  else
  	  {
    	    rollButtonDown = false;
  	  }
 	 
  	  if(digitalRead(SWITCH_BTN))
  	  {
    	    if(!switchButtonDown)
    	    {
      	      selectedDie = (selectedDie + 1) % NUM_DICE;
      	      switchButtonDown = true;
     	 
      	      displaySelectedDie();
      	      displayRollResult(limits[selectedDie]);
    	    }
  	  }
  	  else
  	  {
    	    switchButtonDown = false;
  	  }

  	  lastButtonPress = currentMillis;
	}
  }
  else
  {
      unsigned rollChanges = 0;

      while(rollChanges < ROLL_ANIM_LENGTH)
      {
          int roll = random(1, limits[selectedDie] + 1);
          unsigned long m = millis();
      
          displaySelectedDie();
          displayRollResult(roll);

          while(millis() - m < rollChanges * 2.5) {} // Wait for a while. The wait delay increases with the number of rolls
      
          rollChanges++;
      }

      rolling = false;
      }
}