# ArduinoMorse

MorceLCD2B.ino is a program written for Arduino where you can write morse code using two buttons, one for each of short ('.') and long ('-') signals, which is then translated into normal text and displayed on the LCD screen.   
MorceLCD1B.ino does basically the exact same thing as explained above with the difference that you only write morse code with one button, sending a short signal ('.') by pressing the button quickly, and a long signal ('-') by holding the button down for a little longer.   
The connections for the LCD pins are specified in each of the programs as well as the pins for the buttons. I only had push-to-break buttons at hand while writing this code, therefore the macro BTNPRESSED is set to 0. If you want to use a push-to-make button, change the macro to 1.
