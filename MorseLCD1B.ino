#include <LiquidCrystal.h>
#include<string.h>

/* Connections for the LCD (LCD-pin: conection)
 * 1: gnd
 * 2: +5V
 * 3: potentiometer to change contrast for letters
 * 4: pin 8
 * 5: gnd
 * 6: pin 9
 * 7:
 * 8:
 * 9:
 * 10:
 * 11: pin 10
 * 12: pin 11
 * 13: pin 12
 * 14: pin 13
 * 15: +5V (through potentiometer or resistor to change background brightness)
 * 16: gnd
 */

#define BTNPRESSED 0        //set to 1 if push-to-make switch is used
#define LCDWIDTH 20         //Width of LCD
#define LCDHEIGHT 4         //height of lcd
#define MORSEPIN 7          //the pin that btn is connected to
#define BREAKCHAR 600       //time before translating whats written
#define ADDSPACE 1000       //time before adding a space
#define SHORT_MIN 0         //minimum time for btn press to be counted as a '.'
#define SHORT_MAX 100       //maximum time for btn press to be counted as a '.'
#define LONG_MIN 110        //minimum time for btn press to be counted as a '-'
#define LONG_MAX 1000000    //maximum time for btn press to be counted as a '-'. Intentionally large so it will be similar to no max

//Create LCD object
LiquidCrystal lcd(8, 9, 10, 11, 12, 13 );

void setup() {
  pinMode(MORSEPIN, INPUT);
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(LCDWIDTH, LCDHEIGHT);
  lcd.print("Text:");
  lcd.setCursor(0,2);
  lcd.print("Entered:");
}


void loop() {
  static int written3 = 0, spaceCounter = 0;          //writen3: number of chars written on line 4, spaceCounter: used to store measured time in between button presses
  static char buf[21] = {0};                          //buf: Where the '.' and '-' are stored before they are translated
  static int bufIdx = 0, textIdx = 0;                 //bufIdx: , textIdx: keeps track of how many chars are written on line 2
  int button_pressed_time = 0;                        //button_prssed_time: stores the measured time for how long the button is pressed

  //Prints entered char and clears buf and line 4
  if (spaceCounter >= BREAKCHAR){
    lcd.setCursor(textIdx, 1);
    char c = morseToChar(buf);
    lcd.print(c);
    Serial.print(c);
    textIdx++;
    clearArray(buf,21);
    clearLine(3);
    bufIdx = 0;
    written3 = 0;
  } 
  //makes sure that there is no buffer overflow in buf
  if (written3 >= 20){
    clearLine(3);
    clearArray(buf, 21);
    written3 = 0;
  }

  //pause here until a button is pressed, no spaces are printed after exiting the last while loop until a btn is pressed.
  while(digitalRead(MORSEPIN)!=BTNPRESSED){
    //to avoid integer overflow
    if (spaceCounter <= ADDSPACE){
      spaceCounter++;
    }
    delay(1);
  }

  if (spaceCounter >= ADDSPACE){
    Serial.print(' ');
    //Do not need to print a space on LCD, just move cursor
    textIdx++;
  }
  //reset after use
  spaceCounter = 0;
  
  //resets the line if the line is full
  if (textIdx >= LCDWIDTH){
    clearLine(1);
    textIdx = 0;
  }

  //measures the time the button is pressed
  while (digitalRead(MORSEPIN)==BTNPRESSED){
    button_pressed_time++;
    delay(1);

    //displays '.' or '-' even before button is released to help user
    if (button_pressed_time > SHORT_MIN && button_pressed_time < SHORT_MAX){
      lcd.setCursor(written3,3);
      lcd.print(".");
    } 
    else if (button_pressed_time > LONG_MIN){
      lcd.setCursor(written3,3);
      lcd.print("-");
    }
    
  }

  //if short '.'
  if (button_pressed_time > SHORT_MIN && button_pressed_time < SHORT_MAX){
    buf[bufIdx] = '.';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }
  //if long '-'
  else if (button_pressed_time > LONG_MIN && button_pressed_time < LONG_MAX){
    buf[bufIdx] = '-';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }

  //pause here until button is released to avoid regestering multiple presses
  while(digitalRead(MORSEPIN)==BTNPRESSED){
    delay(1);
  }

  //Loop to measure time between button presses
  while (digitalRead(MORSEPIN)!=BTNPRESSED){
    delay(1);
    if (spaceCounter <= BREAKCHAR){
        spaceCounter++;
    } else{
      break;
    }
  }


  
}




//Translates morse to text
char morseToChar(char str[]){
    //A,B,...,Y,Z
    char *alphamorse[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---",
                    "-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-",
                    "...-",".--","-..-","-.--","--.."};
    int alen = 26;
    // 0,1,...,9
    char *nummorse[] = {"-----",".----","..---","...--","....-",".....","-....","--...","---..","----."};
    int nlen = 10;

    for (int i = 0; i < alen; i++){
        int r = strcmp(str, alphamorse[i]);
        if (r == 0){
            return (char)(i+'A');
        }
    }

    for (int i = 0; i < nlen; i++){
        int r = strcmp(str, nummorse[i]);
        if (r == 0){
            switch (i){
                case 0:
                    return '0';
                case 1:
                    return '1';
                case 2:
                    return '2';
                case 3:
                    return '3';
                case 4:
                    return '4';
                case 5:
                    return '5';
                case 6:
                    return '6';
                case 7:
                    return '7';
                case 8:
                    return '8';
                case 9:
                    return '9';
            }
        }
    }
    //if no match as found
    return '?';
}


void clearLine(int line){
  char arr[21] = {0};
  for (int i = 0; i < 20; i++){
    arr[i] = ' ';
  }
  lcd.setCursor(0,line);
  lcd.print(arr);
  return NULL;
}

void clearArray(char *arr, int len){
  for (int i = 0; i < len; i++){
    arr[i] = 0;
  }
  return NULL;
}
