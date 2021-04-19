#include <LiquidCrystal.h>
#include<string.h>

//Followed http://web.cecs.pdx.edu/~eas199/B/howto/arduino/Arduino_programming_part6_notes.pdf
/*
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

#define BREAKCHAR 600
#define ADDSPACE 1000
#define SHORT_MIN 0 //can be changed
#define SHORT_MAX 100
#define LONG_MIN 110
#define LONG_MAX 1000000 //Large so it will be simmilar to no max, but can be set lower if a max is wanted


 //Create LCD object
LiquidCrystal lcd(8, 9, 10, 11, 12, 13 );

const int morsePin = 7;

void setup() {
  pinMode(morsePin, INPUT);
  
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  
  lcd.print("Text:");
  lcd.setCursor(0,2);
  lcd.print("Entered:");
}


void loop() {
  static int written3 = 0, spaceCounter = 0;
  static char buf[21] = {0};
  static int bufIdx = 0, textIdx = 0;
  int button_pressed = 0;  

  //Prints entered char  and clears buf and line3
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
  if (written3 >= 20){
    clearLine(3);
    written3 = 0;
  }
  

  //pause here until a buttons pressed, so that too many spaces are not printed because of the last while loop.
  while(digitalRead(morsePin)){
    if (spaceCounter <= ADDSPACE){
      spaceCounter++;
    }
    delay(1);
  }

  if (spaceCounter >= ADDSPACE){
    Serial.print(' ');
    //Do not need to print a space, just increment cursor
    textIdx++;
  }
  spaceCounter = 0;

  //resets the line
  if (textIdx >= 20){
    clearLine(1);
    textIdx = 0;
  }

  //measures the time the butto is pressed
  while (digitalRead(morsePin)==0){
    button_pressed++;
    delay(1);
    
    if (button_pressed > SHORT_MIN && button_pressed < SHORT_MAX){
      lcd.setCursor(written3,3);
      lcd.print(".");
    }
    else if (button_pressed > LONG_MIN){
      lcd.setCursor(written3,3);
      lcd.print("-");
    }
    
  }

  
  //when button for short is pressed
  if (button_pressed > SHORT_MIN && button_pressed < SHORT_MAX){
    lcd.setCursor(written3,3);
    lcd.print(".");
    buf[bufIdx] = '.';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }
  //when button for long is pressed
  else if (button_pressed > LONG_MIN && button_pressed < LONG_MAX){
    lcd.setCursor(written3,3);
    lcd.print("-");
    buf[bufIdx] = '-';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }

  //pause here until button is released to avoid regestering multiple presses
  while(digitalRead(morsePin)==0){
    delay(1);
  }

  //Loop to measure time between button presses
  while (digitalRead(morsePin)){
    delay(1);
    //to avoid integer overflows
    if (spaceCounter <= BREAKCHAR){
        spaceCounter++;
    } else{
      break;
    }
  }


  
}




//Translates morse to alphanumerical
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
