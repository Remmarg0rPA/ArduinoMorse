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
#define ADDSPACE 1800

 //Create LCD object
LiquidCrystal lcd(8, 9, 10, 11, 12, 13 );

const int sMorsePin = 6;
const int lMorsePin = 7;

void setup() {
  pinMode(sMorsePin, INPUT);
  pinMode(lMorsePin, INPUT);
  
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
  

  //paus here until a buttons pressed, so that too many spaces are not printed because of the last while loop.
  while(digitalRead(lMorsePin) && digitalRead(sMorsePin)){
    if (spaceCounter <= ADDSPACE){
      spaceCounter++;
    }
    delay(1);
  }

  if (spaceCounter >= ADDSPACE){
    Serial.print(' ');
    //Dont need to print space, just move cursor
    textIdx++;
  }
  spaceCounter = 0;

  //resets the line
  if (textIdx >= 20){
    clearLine(1);
    textIdx = 0;
  }


  //when button for short is pressed
  if (digitalRead(sMorsePin) == 0){
    lcd.setCursor(written3,3);
    lcd.print(".");
    buf[bufIdx] = '.';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }
  //when button for long is pressed
  else if (digitalRead(lMorsePin) == 0){
    lcd.setCursor(written3,3);
    lcd.print("-");
    buf[bufIdx] = '-';
    bufIdx++;
    written3++;
    spaceCounter = 0;
  }

  //makes sure that the program pauses here until all buttons are released to avoid regestering multiple presses
  while(digitalRead(lMorsePin)==0 || digitalRead(sMorsePin)==0){
    delay(1);
  }

  //Loop to measure time between button presses
  while (digitalRead(lMorsePin) && digitalRead(sMorsePin)){
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
