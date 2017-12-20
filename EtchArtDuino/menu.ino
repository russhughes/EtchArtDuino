
//
// displayMenu
//

int displayMenu(MENU menu[], const __FlashStringHelper *title, char *data)
{
	int menuCount = 0;
	int	again = 1;
	int	waiting = 1;
	int	first = 0;
  	int count = 0;
  	int	line = 0;
  	int	rc = 0;
  	int	i;
  	int	vx;
  	int	vy;

	oled.clear();
	oled.print(title);
	if (data)
		oled.print(data);
	
	while(menu[i++].option)
		menuCount++;

  	while (again)
  	{
  		count = 0;
  		i = 0;	
  		
		while(menu[i].option)
		{
			if (i >= first)
			{
				if (count < MENU_LINES)
				{
					oled.setCursor(1,count+2);	
               		if (line == count)
               		{
               			oled.print(">");
               			rc = menu[i].value;
               		}
               		else
               			oled.print(" ");
                			
               		oled.print(menu[i].option);    
               		oled.clearToEOL();
					count++;	
				}
			}
			i++;
		}

  		while (count < MENU_LINES)
  		{
  			oled.setCursor(1,count+2);
  			oled.clearToEOL();
  			count++;
  		}
  		
  		if (!waiting)
		{
			delay(250);
			waiting = 1;
		}
		
		while (waiting)
		{
  			vx = map(analogRead(A0)+X_OFFSET,0, 1023, 3, -3);    // read the input pin
  			if (vx == JOY_UP)
  			{
  				waiting = 0;
  				if(line >0)
  					line--;
  				else
  					if (first)
  						first--;
  			}
  		
  			if (vx == JOY_DOWN)
  			{
  				waiting = 0;
  				if (line < MENU_LINES-1)
  					line++;	
  				else
  					if (first <= menuCount - MENU_LINES)
  						first++;
  			}
  			
  			vy = map(analogRead(A1)+Y_OFFSET,0, 1023, 3, -3);    // read the input pin
  			if (vy == JOY_LEFT)
  			{
  				waitY();
  				waiting = 0;
  				again = 0;
  				rc = 0;
  			}
  			
  			if (vy == JOY_RIGHT)
  			{
  				waitY();
  				waiting = 0;
  				again = 0;
  			}
  			
  			Button.update();
  			if (Button.read() == LOW)
  			{
    			waitButton();
      			waiting = 0;
      			again = 0;
      		
  			}
		}
  	}
  	return rc;
}
