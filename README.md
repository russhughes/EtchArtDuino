

# EtchArtDuino

![](https://raw.githubusercontent.com/russhughes/EtchArtDuino/master/images/IMG_0001_preview_featured.jpeg)


## Description

Arduino powered Etch A Sketch with joystick controller and SD card storage. Arduino source included in EtchArtDuino folder.  The EtchArtDuinoSimple folder contains a simplified version of the software that just allows you to draw using the joystick that is usefull for younger children.

## Instructions

Sorry, not a lot of build details here. I just built as I went with parts I had on hand in order to play with the steppers. It's one of my first OpenSCAD and Arduino projects so the code is, what it is. Don't judge it too harshly.  

The menu system is pretty basic but it allows you to create and save a new drawing and redraw or delete an existing drawing.  


Joystick| Menu usage  
:------:|:----------:
Up/Down | Moves cursor  
Left | Back/Cancel  
Right or Press | Select  

Joystick Drawing usage is as you would expect:up, down, left, right. Press the joystick to end the drawing. It will ask if you are done, select yes to save and exit the drawing or select no to continue drawing.   

Since the Etch ArtDuino has no way of knowing where the drawing stylus is at the beginning of a drawing, the display will ask you to choose a starting position (upper left or right, lower left or right, or the center) and then ask you to move the stylus there then press the joystick. It will save the starting position as the first byte of the file so it can prompt you to move the stylus to the correct position when redrawing the file.  

Drawings are stored on a SD card using the Arduino SD library. You can cheat and create the files on a computer. The files must have an ".eas" extension and be in the root directory.  

    The file format is:  
	
	1st byte is binary and contains the suggested starting position   
		1: Upper Left  
		2: Upper Right  
		3: Lower Left  
		4: Lower Right  
		5: Center  

	The rest of the file is a string of single character   
    drawing commands  

		u: move up  
		d: move down  
		l: move left  
		r: move right  

    An example of a tiny 3x3 box would be:  
    (binary value of 1,2,3,4 or 5)rrrdddllluuu  

### Wiring chart
  
Arduino | Connection
:------:|:---------:
VCC	 | +5V Supply  
GND	 | Ground  
D2	| Horiz/Left Stepper IN1  X-axis  
D3	| Horiz/Left Stepper IN2  
D4	| Horiz/Left Stepper IN3  
D5	| Horiz/Left Stepper IN4  
D6	| Vert/Right Stepper IN1 Y-axis  
D7	| Vert/Right Stepper IN2  
D8	| Vert/Right Stepper IN3  
D9	| Vert/Right Stepper IN4  
D10	 | SPI SS SD Card  
D11	 | SPI MOSI SD Card  
D12	 | SPI MISO SD Card  
D13 | SPI SCK SD Card  
A0 | 	Joystick VX  
A1 | 	Joystick VY  
A2 | 	Joystick Button  
A4 | 	I2C SDA		OLED  
A5 | 	I2C SCL		OLED  

### Parts List:  
Qty | Description
:--:|------------
1 | 3D Printed EtchArtDuino.stl  
1 | 3D Printed gears.stl  
1 | 3D Printed joystick.stl  
1 | Pocket Etch A Sketch  
2 | 28BYJ-48 5V DC 4-Phase 5-Wire + ULN2003 Driver Board (ebay/amazon)  
1 | Catalex microsd card adapter (ebay/electrodragon/amazon)  
1 | Parallax.com Circuit Overlay Board (Part #32999) 
1 | Arduino Pro Mini 328 - 5V/16MHz (sparkfun)  
1 | 0.96" I2C 128X64 OLED (ebay/electrodragon/amazon)  
1 | PS2 Joystick Module marked "Keyes_SJoys" (ebay/electrodragon/amazon)
1 | 5v Power supply and mating connector.  
  Miscellaneous | 2mm screws and nuts. 
Miscellaneous | jumper wires  

### Additional SCAD Sources:  

Parkinbot's Excellent Gear Library http://www.thingiverse.com/thing:636119  

### Additional Arduino Libraries:  

SSD1306Ascii	https://github.com/greiman/SSD1306Ascii  
Bounce2  https://github.com/thomasfredericks/Bounce2

### License
http://www.thingiverse.com/thing:948117
licensed under the Creative Commons - Attribution - Share Alike license. http://creativecommons.org/licenses/by-sa/3.0/
