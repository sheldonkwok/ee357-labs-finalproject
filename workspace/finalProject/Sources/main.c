/*
 * EE357 Final Project
 * Phoenix
 * 
 * Johnny Jung / Sheldon Kwok
 * 
 */
#include "support_common.h" /* include peripheral declarations and more */
#include "usc_support.h"
#include "lcd128x64.h"
#include "fonts.h"
#include "i2c.h"
#include <string.h>
#include <stdlib.h>
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

struct GameObject{
	int x;
	int y;
	int alive;
	int misc; //Dead or Alive for player || Player or enemy for bullet
	int fired; //If fired bullet
	uint8 graphic[2];
};


//gameObject variables
struct GameObject player; 
struct GameObject enemies[8];
struct GameObject bullets[20];

/* I/O Functions */

/* JOYSTICK */

//function that initializes joy stick input 
void init_joy()
{  
   /* Enable signals as digital outputs */
    MCF_GPIO_DDRTJ = 0
        | MCF_GPIO_DDRTJ_DDRTJ0
        | MCF_GPIO_DDRTJ_DDRTJ1
        | MCF_GPIO_DDRTJ_DDRTJ6
        | MCF_GPIO_DDRTJ_DDRTJ7 | MCF_GPIO_DDRTJ_DDRTJ2;
    MCF_GPIO_PORTTJ = 0xff; 
}
//function that outputs a 1 if joy stick switch is left
int get_lt_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0));
        return 1;            
        }
    }
    return 0;
}
//function that outputs a 1 if joy stick switch is right
int get_rt_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1));
        return 1;            
        }
    }
    return 0;
}
//function that outputs a 1 if joy stick switch is down
int get_down_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6));
        return 1;            
        }
    }
    return 0;
}
//added a new function that outputs a 1 if switch is up
int get_up_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7));
        return 1;            
        }
    }
    return 0;
} 

/* LCD */

void init_gpio()
{
	// Init. input buttons that are connected to PortTA
	MCF_GPIO_PTAPAR = 0 
	    | MCF_GPIO_PTAPAR_ICOC0_GPIO
	    | MCF_GPIO_PTAPAR_ICOC1_GPIO;
	/* Set Data Direction to all input */
	MCF_GPIO_DDRTA = 0;
	
    /* Enable 4 LED signals as GPIO */
    MCF_GPIO_PTCPAR = 0
        | MCF_GPIO_PTCPAR_DTIN3_GPIO
        | MCF_GPIO_PTCPAR_DTIN2_GPIO
        | MCF_GPIO_PTCPAR_DTIN1_GPIO
        | MCF_GPIO_PTCPAR_DTIN0_GPIO;
    
    /* Enable signals as digital outputs */
    MCF_GPIO_DDRTC = 0
        | MCF_GPIO_DDRTC_DDRTC3
        | MCF_GPIO_DDRTC_DDRTC2
        | MCF_GPIO_DDRTC_DDRTC1
        | MCF_GPIO_DDRTC_DDRTC0;

	/* Enable 2 LED's on iMn FM board */
	MCF_GPIO_PTHPAR = 0 
		| MCF_GPIO_PTHPAR_MB_D0_GPIO
		| MCF_GPIO_PTHPAR_MB_D2_GPIO;
	
	MCF_GPIO_DDRTH = MCF_GPIO_DDRTH_DDRTH0 | MCF_GPIO_DDRTH_DDRTH2;
}
// get button 1
int get_SW1_v2()
{
	int i;
	// Read the current state of the switch 
	if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0))
	{
		cpu_pause(5000);
		if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0))
		{
			// Wait until button is released
			while(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0));
			return 1;	
		}
	}
	return 0;
}
// get button 2
int get_SW3_v2()
{
	int i;
	/* Read the current state of the switch */
	if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1))
	{
		cpu_pause(5000);
		if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1))
		{
			// Wait until button is released
			while(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1));
			return 1;			
		}
	}
	return 0;
}


/* ACCELEROMETER */
int init_accel()
{
	/****************************************/
	/* Setup ADC0 on M52259 Tower Board        */
	/****************************************/
	/* Setting PortAN0 as ADC functionality */
	MCF_GPIO_PANPAR |= MCF_GPIO_PANPAR_PANPAR3 
					  | MCF_GPIO_PANPAR_PANPAR2 
					  | MCF_GPIO_PANPAR_PANPAR1
					  | MCF_GPIO_PANPAR_PANPAR0;

	/****************************************/
	/* Setup ADC Module					    */
	/****************************************/

	/* Initialize the CRTL1 register to 0's with SMODE = Once Sequential = 000 */
	MCF_ADC_CTRL1 = 0 | MCF_ADC_CTRL1_SMODE(0);

	/* Setting divisor in CTRL2 register */
	MCF_ADC_CTRL2 = MCF_ADC_CTRL2_DIV(3);

	/* Setting Power Register appropriately - PUDELAY & clear PD0*/
	MCF_ADC_POWER = MCF_ADC_POWER_PUDELAY(4);

	/* Set AN0 to sample channel 3 once.  Note we will only look 	*/
	/* at the result of sample 0 even though the ADC will take  	*/
	/* an 8 sample scan 											*/
	MCF_ADC_ADLST1 = 0 | MCF_ADC_ADLST1_SAMPLE0(0)
						| MCF_ADC_ADLST1_SAMPLE1(1)
						| MCF_ADC_ADLST1_SAMPLE2(2)
						| MCF_ADC_ADLST1_SAMPLE3(3);

	/* Clear stop bit */
	MCF_ADC_CTRL1 &= ~MCF_ADC_CTRL1_STOP0;
	
	return 0;
}

void updateFrame()
{
	int i;
	/* DRAW CODE */
	
	grphText(player.x,player.y,FONT_NINE_DOT,player.graphic);			//Draw player
	for(i = 0; i < 10; i++) {
		if(enemies[i].alive) {
			grphText(enemies[i].x,enemies[i].y,FONT_NINE_DOT,enemies[i].graphic);
		}
	}
	for(i = 0; i < 20; i++) {
		if(bullets[i].alive) {
			grphText(bullets[i].x,bullets[i].y,FONT_NINE_DOT,bullets[i].graphic);
		}
	}
	
	grphUpdate(SCRN_TOP,SCRN_BOTTOM);
}

int main(void)
{	
	//write buffers
	char mybuf[15];
	char output[7];
		
	//accelerometer variables
	int x=2000,y=2000,z=2000,sample = 0;
	
	//joystick variables
	int left;
	int right;
	int xprev;
	
	//game state variables
	int enemiesAlive;
	int gameMenu, initMenu;
	int menuAnimation;
	int i,j,count;
	int score,highscore;
	int bomb,bombScore;
		
	//Initiate player stats
	player.x = 60; player.y = 54; player.alive = 1; player.misc = 0; player.fired = 0; player.graphic[0] = 'A'; player.graphic[1] = 0;
	score = 0;
	xprev = 2000;
	bomb = 0;
	bombScore = 0;	
	//Initiate enemy variables
	for(i = 0; i < 8; i++) {
		enemies[i].graphic[0] = 'Y'; enemies[i].graphic[1] = 0;
	}
	
	//Initiate bullet graphics
	for(i = 0; i < 20; i++) {
		bullets[i].graphic[0] = '.'; bullets[i].graphic[1] = 0;  	
	}
	
	//Menu
	gameMenu = 1;
	initMenu = 1;
	
	init_gpio();
	init_accel();
	init_joy();	
	init_lcd();                                         

  	while (1){
  		
		/* ACCELEROMETER CODE */
		/* Toggle the LED's */
		MCF_GPIO_PORTTC = ~MCF_GPIO_PORTTC;
		/* Clear stop bit */
		MCF_ADC_CTRL1 &= ~MCF_ADC_CTRL1_STOP0;
		/* Set start bit */
		MCF_ADC_CTRL1 |= MCF_ADC_CTRL1_START0;
		/* Waiting for the last "Ready" bit to be set so we don't start the next scan to early*/
		while (!(MCF_ADC_ADSTAT & MCF_ADC_ADSTAT_RDY7));
		/* Toggle the LED's */
		MCF_GPIO_PORTTC = ~MCF_GPIO_PORTTC;
		x = MCF_ADC_ADRSLT(0) >> 3;
		y = MCF_ADC_ADRSLT(1) >> 3;
		z = MCF_ADC_ADRSLT(2) >> 3;
		sample = MCF_ADC_ADRSLT(3) >> 3;
		//myprintf("Accelerometer: X=%d, Y=%d, Z=%d, POT=%d\r", x,y,z,sample); // get rid of this later
		// need sudden change mechanism (save previous values, boolean, etc.)
		
		//Button 1 pressed
		if(get_SW1_v2()){
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTH_PORTTH0;
			gameMenu = !gameMenu;	
			
			if(gameMenu) {
				initMenu = 1;
			} else if(!player.misc) {
				score = 0;
				
				//Reset game values
				player.x = 60; player.y = 54; player.alive = 1; player.misc = 1; player.fired = 0; bomb = 1; bombScore = 0;
				for(i = 0; i < 8; i++) {
					enemies[i].x = i*16 + 4; enemies[i].y = 2; enemies[i].alive = 0;  enemies[i].fired = 0;
				}
				for(i = 0; i < 20; i++) {
					bullets[i].alive = 0; 
				}
				printf("Resetting");
			}
			
			grphErase();
		}
		
		if(gameMenu) {
			//Draw game menu
			if(initMenu) {
				grphErase();
				grphText(4,4,FONT_NINE_DOT,(unsigned char *)"Phoenix");
				grphText(4,16,FONT_SIX_DOT,(unsigned char *)"Johnny Jung");
				grphText(4,24,FONT_SIX_DOT,(unsigned char *)"Sheldon Kwok");
				output[0]='S';output[1]='c'; output[2] = 'o'; output[3]='r'; output[4]='e';output[5]=':';output[6]=' ';
				sprintf(mybuf,"%d",score);
				if(score < 10) {
					output[7] = mybuf[0]; 
				} else {
					output[7] = mybuf[0]; 
				}
				grphText(4,32,FONT_SIX_DOT,(unsigned char *)output);
				grphText(4,46,FONT_SIX_DOT,(unsigned char*)"SW1-Start/Pause");
				grphText(4,54,FONT_SIX_DOT,(unsigned char*)"SW3-Shoot");
				grphText(100,54,FONT_NINE_DOT,(unsigned char *)"A");
				
				initMenu = 0;
				menuAnimation = 44;
			}
			
			//Menu animation
			grphText(100,menuAnimation,FONT_NINE_DOT,(unsigned char *)".");
			grphText(100,4,FONT_NINE_DOT,(unsigned char*)"Y");
			menuAnimation -= 2;
			if(menuAnimation == 0) {
				menuAnimation = 44;
			}
			grphUpdate(SCRN_TOP,SCRN_BOTTOM);
		} else {
			//Bomb
			if((xprev-x > 250 || xprev-x < -250) && bomb) {
				for(j = 0; j < 8; j++) { bombScore += enemies[j].alive; enemies[j].alive = 0;}
				score += bombScore;
				bomb = 0;
				grphErase();
			}
			xprev = x;
			
			
			//Generate enemy bullets
			for(j=0; j<8; j++) {
				if(x%7 == 0 && x%2 == 0 && !enemies[j].fired && enemies[j].alive) {
					//printf("wat");
					//Generate bullets
					for(i = 0; i < 20; i++) {
						if(!bullets[i].alive) {
							bullets[i].x = enemies[j].x; bullets[i].y = enemies[j].y + 6; bullets[i].alive = 1; bullets[i].misc = 0; enemies[j].fired = 1;
							break;
						}
					}	
					break;
				}
			}
			
			//Reset enemey fire
			for(j = 0; j < 8; j++) {
				if(enemies[j].alive && !enemies[j].fired) {
					break;
				}
			}
			if(j == 8) {
				for(j = 0; j < 8; j++) { enemies[j].fired = 0; }
			}
			
			//Bullet movement code
			for(i = 0; i < 20; i++) {
				if(bullets[i].alive) {
					if(bullets[i].y < 5 && count == -1) {
						//Enemy collision detection
						for(j = 0; j < 8; j++) {
							if(bullets[i].x <= enemies[j].x + 4 && bullets[i].x >= enemies[j].x - 4) {
								enemies[j].alive = 0;
								score++;
							}
						}
						
						bullets[i].alive = 0;
						grphErase();
					} else if(bullets[i].y > 54 && !bullets[i].misc &&
							bullets[i].x <= player.x + 4 && bullets[i].x >= player.x - 4) { //Check to see if in range of player and enemy bullet
						player.misc = 0;
						gameMenu = 1;
						initMenu = 1;
						printf("Died.");
					} else if(bullets[i].y > 54) {
						bullets[i].alive = 0;
					} else {
						if(bullets[i].misc) { //Check to see if player bullet
							bullets[i].y = bullets[i].y - 2; 
						} else {
							bullets[i].y = bullets[i].y + 2;
						}
					}		
				}
			}
			
			// Check to see if all enemies are alive
			enemiesAlive = 0;
			for(i = 0; i < 8; i++) {
				if(enemies[i].alive) {
					enemiesAlive = 1;
				}
			}
			
			// Animate enemies respawning 
			if(!enemiesAlive) {
				for(i = 0; i <8; i++) { enemies[i].alive = 1; enemies[i].y= -8; enemies[j].fired = 0;}
				count = 0;
			}
			if(count < 10 && count > -1) {
				for(i = 0; i <8; i++) {enemies[i].y++;}
				count++;
				cpu_pause(50000);
				if(count == 10) {
					count = -1;
				}
			}
			
			
			/* JOYSTICK CODE */
			left = get_lt_sw_v2();
			right = get_rt_sw_v2();
			if(left || right) grphErase();
			if(left) if(player.x>0)player.x-=3;		
			if(right) if(player.x<118)player.x+=3;	
			
			/* BUTTON CODE */
			//Button 2 pressed
			if(get_SW3_v2()){		
				MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTJ_PORTTJ2;
				
				//Generate bullets
				for(i = 0; i < 20; i++) {
					if(!bullets[i].alive) {
						bullets[i].x = player.x; bullets[i].y = player.y - 10; bullets[i].alive = 1; bullets[i].misc = 1;
						break;
					}
				}
				
			}
			updateFrame();
		}
  	}
}
