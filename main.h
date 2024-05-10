/* 
 * File:   main.h
 * Author: Admin
 *
 * Created on 28 March, 2024, 2:22 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define DASHBOARD               0
#define PASSWORD                1
#define MENU                    2
#define MENU_ENTER              3
#define VIEWLOG                 0
#define DOWNLOADLOG             1
#define CLEARLOG                2
#define SETTIME                 3
#define CHANGEPASS              4

#include"matrixkeypad.h"
#include"adc.h"
#include"clcd.h"
#include"eeprom.h"
#include"timer0.h"
#include"isr.h"
#include<string.h>
#include"i2c.h"
#include "ext_eeprom.h"
#include"rtc.h"
#include"uart.h"

unsigned char clock_reg[3];

long int delay1=0;
long int delay2=0;

int main_flag = 0;
int menu_flag = 0;

char time[9] = "00:00:00";
unsigned char key;
unsigned char prekey;
unsigned char gear[8][3] = {"ON","GN","GR","G1","G2","G3","G4","CO"};
unsigned char speed[3]="00";
unsigned char menu[5][16] = {"View log      ","Download log  ","Clear log     ","Set time      ","Change pass   "};

int pass_add=200;
char original[5]="0";//store the original password in a character array
char entered[5];   
char second[4]="0";
int sec;
int passflag=0;

int add = 0x00;
char captured_event[17];
int event_count=0;
int disp_count=0;

int ind1=0;           
int ind2=1;
int s_flag=0;
int star_ind = 0;


void dashboard(void);
void event_capture(void);
void password(void);
void menu_disp(void);
void get_time(void);

void view_log(void);
int add1 = 0x00;
int add2;
int event_flag=0;

void download_log(void);

void clear_log(void);

void set_time(void);
int time_flag=1;
int once_time=0;
int hr=0;
int min=0;
int secs=0;

void change_password(void);
int pass_flag=0;
char new_pass1[5];
char new_pass2[5];
int m;
int n;
int passdelay;
int hr_blink_delay=0;
int min_blink_delay=0;
int sec_blink_delay=0;
//int pass_add=200;


#endif	/* MAIN_H */

