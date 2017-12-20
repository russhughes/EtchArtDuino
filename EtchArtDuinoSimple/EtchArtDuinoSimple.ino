/*
 * Etch-Art-Duino
 *
 * Russ Hughes, July 2015 russ@owt.com
 * Creative Commons - Attribution - Non-Commercial license.
 *
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <Stepper.h>
#include <Bounce2.h>
#include "menu.h"

// display
#define I2C_ADDRESS 0x3C
SSD1306AsciiWire oled;
#define DISPLAY_LINES 8
#define MENU_LINES 6

// sd card

const int PIN_SD_CS = 10;
const bool ALLOW_WIPE = false;
char fileName[13];
char fileNames[MENU_LINES][13];
int	fileCount = 0;
File dir;

// steppers

Stepper horiz(4076, 2, 4, 3, 5);  // left + / right -
Stepper vert(4076, 6, 8, 7, 9);   // up- / down +

// buttons

#define JOY_UP 3
#define JOY_DOWN -3
#define JOY_LEFT -3
#define JOY_RIGHT 3

#define X_OFFSET 8
#define Y_OFFSET 14
#define BUTTON A2

Bounce Button = Bounce();

// scale for the drawing commands 1 move command = 5 steps

int scale = 5;

//
// motorOff: turn off stepper motors
//

void motorOff() {
  for (int i = 2; i < 10; i++)
    digitalWrite(i, LOW);
}

//
// displayInit: initalize oled display
//

void displayInit()
{
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();
  oled.setFont(Adafruit5x7);
  oled.clear();
}

//
// setup: Rememeber when this used to be called main()
//

void setup()
{
  motorOff();
  displayInit();
  cls();

  // stepper speed, 5 seems to work well without missing steps

  horiz.setSpeed(5);
  vert.setSpeed(5);

  // center button on joystick

  pinMode(BUTTON, INPUT_PULLUP);
  Button.attach(BUTTON);
  Button.interval(25);

  newFile();

  /*
  // initalize sd card and open root directory

  pinMode(PIN_SD_CS, OUTPUT);
  	if (SD.begin(PIN_SD_CS))
  	{
  		dir = SD.open("/");
  		if (dir)
    		mainMenu();
    	else
    		oled.println(F("Cant open /"));
  	}
  	else
    	oled.println(F("SD Card Failure!"));
    */
}

//
// isEtchFile - if fileName is an ".eas" file return the fileName string length
//  otherwise return 0
//

int isEtchFile(char *filename)
{
  int len = strlen(filename);
  if (len > 4)
  {
    if (!strstr(strlwr(filename + (len - 4)), ".eas"))
      len = 0;
  }
  else
    len = 0;

  return len;
}

//
// countFiles: returns the number of the EAS files in the root directory.
//

int countFiles()
{
  File entry;
  int files = 0;

  if (dir)
  {
    dir.rewindDirectory();

    while (entry = dir.openNextFile())
    {
      strcpy(fileName, entry.name());

      if (isEtchFile(fileName))
        files++;
      entry.close();
    }
  }
  return files;
}

//
//	listFiles: List's EAS files in root directory on oled
//
//		first - number of first file to display
//		line - currently selected line number
//		lines - number of lines to display
//

void listFiles(int first, int line, int lines)
{
  File entry;
  int	count = 0;
  int file = 0;

  if (first == 0)
  {
    strcpy(fileNames[0], "");
    oled.setCursor(1, 2);
    if (line == 0)
      oled.print(">");
    else
      oled.print(" ");

    oled.print(F("New Drawing"));
    oled.clearToEOL();
    count = 1;
  }

  if (dir)
  {
    dir.rewindDirectory();
    while (entry = dir.openNextFile())
    {
      strcpy(fileName, entry.name());
      if (isEtchFile(fileName))
      {
        file++;

        if (file >= first)
        {
          if (count < lines)
          {
            strcpy(fileNames[count], fileName);
            oled.setCursor(1, count + 2);

            if (line == count)
              oled.print(">");
            else
              oled.print(" ");

            oled.print(fileName);
            oled.clearToEOL();
            count++;
          }
          else
          {

            entry.close();
            break;
          }
        }
      }
      entry.close();
    }

    while (count < lines)
    {
      oled.setCursor(1, count + 2);
      oled.clearToEOL();
      count++;
    }
  }
  else
    oled.print(F("SD Card Read Failure!"));
}

//
// cls: clear display and show banner
//

void cls()
{
  oled.clear();
  oled.println(F("-=- Etch ArtDuino -=-"));
  oled.setCursor(1, 2);
}

//
// Menus, menus, menus
//

#define YN_yes 1
#define YN_no 0

int ynMenu(const __FlashStringHelper *title, char *data)
{
  MENU menu[] =
  {
    { F("Yes"), YN_yes 	},
    { F("No"), 	YN_no  	},
    { NULL,		0		}
  };

  return displayMenu(menu, title, fileName);
}

#define FM_draw 1
#define FM_delete 2
#define FM_cancel 0

int fileMenu(const __FlashStringHelper *title, char *data)
{
  MENU menu[] =
  {
    { F("Draw File"),	FM_draw 	},
    { F("Delete File"),	FM_delete 	},
    { F("Cancel"),		FM_cancel 	},
    { NULL, 			0 			}
  };

  return displayMenu(menu, title, fileName);
}

#define START_ULEFT 1
#define START_URIGHT 2
#define START_LLEFT 3
#define START_LRIGHT 4
#define START_CENTER 5
#define START_CANCEL 0

int startMenu(const __FlashStringHelper *title, char *data)
{
  MENU menu[] =
  {
    { F("Upper Left"), 	START_ULEFT 	},
    { F("Upper Right"), START_URIGHT 	},
    { F("Lower Left"), 	START_LLEFT 	},
    { F("Lower Right"), START_LRIGHT 	},
    { F("Center"), 		START_CENTER 	},
    { F("Cancel"), 		START_CANCEL 	},
    { NULL,				0				}
  };

  return displayMenu(menu, title, fileName);
}

//
// mainMenu: display main menu
//

void mainMenu()
{
  int	again = 1;
  int	waiting = 1;
  int count = 0;
  int	line = 0;
  int	first = 0;
  int	vx;
  int	vy;

  cls();
  fileCount = countFiles();

  while (again)
  {
    listFiles(first, line, MENU_LINES);

    if (!waiting)
    {
      delay(250);
      waiting = 1;
    }

    while (waiting)
    {
      vx = map(analogRead(A0) + X_OFFSET, 0, 1023, 3, -3); // read the input pin
      if (vx == JOY_UP)	// up
      {
        waiting = 0;
        if (line > 0)
          line--;
        else if (first)
          first--;
      }

      if (vx == JOY_DOWN)	// down
      {
        waiting = 0;
        if (line < MENU_LINES - 1 && line < fileCount)
          line++;
        else if (first <= fileCount - MENU_LINES)
          first++;
      }

      vy = map(analogRead(A1) + Y_OFFSET, 0, 1023, 3, -3); // read the input pin

      Button.update();
      if (Button.read() == LOW)
      {
        waitButton();
        vy = JOY_RIGHT;
      }

      if (vy == JOY_RIGHT)		// right
      {
        waitY();

        if (fileNames[line][0] != 0)
        {
          switch (fileMenu(F("File: "), fileNames[line]))
          {
            case FM_draw:
              drawFile(fileNames[line]);
              break;

            case FM_delete:
              if (ynMenu(F("Delete: "), fileNames[line]))
              {
                SD.remove(fileNames[line]);
                fileCount = countFiles();
                line = 0;
                first = 0;
              }
              break;
          }
          cls();
        }
        else
        {
          newFile();
          fileCount = countFiles();
          cls();
        }
        waiting = 0;
      }
    }
  }
}

//
// waitX: waits until joystick x axis is returned to center
//

void waitX()
{
  while (map(analogRead(A0) + X_OFFSET, 0, 1023, 3, -3) != 0) // read the input pin
    delay(100);
}

//
// waitY: waits until joystick y axis is returned to center
//

void waitY()
{
  while (map(analogRead(A1) + Y_OFFSET, 0, 1023, 3, -3) != 0) // read the input pin
    delay(100);
}

//
// waitButton: waits until the button is released
//
void waitButton()
{
  while (Button.read() == LOW)
    Button.update();
}

//
// drawFile: draws given filename
//

void drawFile(char *filename)
{
  File  dataFile;
  unsigned long fileSize;
  long current = 0;
  byte start;
  char ch;

  waitY();
  cls();

  dataFile = SD.open(filename);
  if (dataFile)
  {
    start = dataFile.read();
    if (start)
    {
      if (moveToStart(start))
      {
        oled.print(F("Drawing:\n  "));
        oled.println(filename);
        oled.println("");

        fileSize = dataFile.size();
        oled.print(F("Length:  "));
        oled.println(fileSize);

        while (dataFile.available())
        {
          ch = dataFile.read();
          current++;

          switch (ch)
          {
            case 'u' : moveup();  break;
            case 'd' : movedown();  break;
            case 'l' : moveleft();  break;
            case 'r' : moveright();  break;
          }
        }
        dataFile.close();
        motorOff();
      }
    }
  }
  else 	// if the file isn't open, pop up an error:
  {
    oled.println(F("error opening "));
    oled.println(filename);
  }
}

// moveup: moves drawing stylus up scale steps

void moveup()
{
  vert.step(-scale);
}

// movedown: moves drawing stylus down scale steps

void movedown()
{
  vert.step(scale);
}

// moveleft: moves drawing stylus left scale steps

void moveleft()
{
  horiz.step(scale);
}

// moveright: moves drawing styles right scale steps

void moveright()
{
  horiz.step(-scale);
}

//
//	nameFile: Prompts user to enter filename using joystick
//

int nameFile()
{
  int	again = 1;
  int rc = 0;
  int vx, vy;
  int letter = 0;
  char c;
  int	i;
  unsigned long blinkTime = millis();;
  int blink = 0;

  memset(fileName, 0, sizeof(fileName));

  cls();
  oled.println(F("New Filename:"));

  while (again)
  {
    if (millis() - blinkTime > 250)
    {
      blinkTime = millis();
      blink = !blink;
    }

    oled.setCursor(1, 5);
    for (i = 0; i < 8; i++)
    {
      if (fileName[i])
        c = fileName[i];
      else
        c = '_';

      if (blink && letter == i)
        c = ' ';

      oled.write(c);
    }

    vx = map(analogRead(A0) + X_OFFSET, 0, 1023, 3, -3); // read the input pin
    vy = map(analogRead(A1) + Y_OFFSET, 0, 1023, 3, -3); // read the input pin

    if (vx == JOY_UP)		// up
    {
      waitX();
      if (fileName[letter])
      {
        if (fileName[letter] != '_')
        {
          fileName[letter]++;
          if (fileName[letter] > 'Z')
            fileName[letter] = '_';
        }
        else
          fileName[letter] = 'A';
      }
      else
        fileName[letter] = 'A';
    }

    if (vx == JOY_DOWN)		// down
    {
      waitX();
      if (fileName[letter])
      {
        if (fileName[letter] != '_')
        {
          fileName[letter]--;
          if (fileName[letter] < 'A')
            fileName[letter] = '_';
        }
        else
          fileName[letter] = 'Z';
      }
      else
        fileName[letter] = 'Z';
    }

    if (vy == JOY_LEFT)		// left
    {
      waitY();
      if (!letter)
      {
        rc = 0;
        again = 0;
      }
      else
        letter--;
    }

    if (vy == JOY_RIGHT)	// right
    {
      waitY();
      if (fileName[letter])
      {
        if (letter < 7)
          letter++;
        else
          letter = 0;
      }
    }

    Button.update();
    if (Button.read() == LOW)
    {
      strlcat(fileName, ".EAS", sizeof(fileName));
      waitButton();
      again = 0;
      rc = 1;
    }
  }
  return rc;
}

//
// newInstructions: display instructions for new drawing
//

void newInstructions()
{
  cls();
  oled.setCursor(0, 3);
  oled.print(F("File: "));
  oled.println(fileName);
  oled.setCursor(0, 5);
  oled.println(F("Use joystick to draw"));
  oled.print(F("then press to end"));
}

//
// moveToStart
//

int moveToStart(byte start)
{
  int again = 1;
  int	rc = 0;
  int vx, vy;

  cls();

  oled.print(F("Move stylus to:"));
  oled.setCursor(0, 3);

  switch (start)
  {
    case START_ULEFT:
      oled.println(F("Upper Left"));
      break;

    case START_URIGHT:
      oled.println(F("Upper Right"));
      break;

    case START_LLEFT:
      oled.println(F("Lower Left"));
      break;

    case START_LRIGHT:
      oled.println(F("Lower Right"));
      break;

    case START_CENTER:
      oled.println(F("Center"));
      break;
  }

  oled.setCursor(0, 5);
  oled.println(F("Press to continue"));

  while (again)
  {
    vx = map(analogRead(A0) + X_OFFSET, 0, 1023, 3, -3); // read the input pin
    vy = map(analogRead(A1) + Y_OFFSET, 0, 1023, 3, -3); // read the input pin

    if (vx == JOY_UP)
      moveup();

    if (vx == JOY_DOWN)
      movedown();

    if (vy == JOY_LEFT)
      moveleft();

    if (vy == JOY_RIGHT)
      moveright();

    Button.update();
    if (Button.read() == LOW)
    {
      waitButton();

      rc = ynMenu(F("Continue? "), fileName);
      again = 0;
    }
  }
  return rc;
}

//
// newFile: asks user for new filename and records drawing
//

void newFile()
{
  int again = 1;
  int vx, vy;
  byte start = 0;

  while (again)
  {
    vx = map(analogRead(A0) + X_OFFSET, 0, 1023, 3, -3); // read the input pin
    vy = map(analogRead(A1) + Y_OFFSET, 0, 1023, 3, -3); // read the input pin

    if (vx == JOY_UP)
    {
      //dataFile.write('u');
      moveup();
    }

    if (vx == JOY_DOWN)
    {
      //dataFile.write('d');
      movedown();
    }

    if (vy == JOY_LEFT)
    {
      //dataFile.write('l');
      moveleft();
    }

    if (vy == JOY_RIGHT)
    {
      //dataFile.write('r');
      moveright();
    }

  }
}
  //
  // Empty
  //

  void loop()
  {
  }

