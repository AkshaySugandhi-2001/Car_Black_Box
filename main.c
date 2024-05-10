/*
 * File:   main.c
 * Author: Admin
 *
 * Created on 28 March, 2024, 1:59 PM
 */


#include <xc.h>
#include"main.h"


void init_config()
{
    
    ADCON1 = 0X0F;
    init_matrix_keypad();
    init_adc();
    init_clcd();
    init_timer0();
    init_i2c();
    init_ds1307();
    init_uart();
    
    //pass_add=200;
    /*for(int x=0; x<4; x++)
    {
        write_external_eeprom(pass_add++,'0');
    }*/
    
    pass_add=200;
    for(int x=0; x<4; x++)
    {
        original[x] = read_external_eeprom(pass_add++);
    }
}

void main(void) 
{
    init_config();
    get_time();
    event_capture();
    event_count++;
    
    
    while(1)
    {
        get_time();
        
        key = read_switches(STATE_CHANGE);
        
        while(key == MK_SW11)
        {
            key = read_switches(LEVEL_CHANGE);
            delay1++;
            if(delay1 > 50000)
            {
                key = 13;
                delay1 = 0;
                break;
            }
        }
        if(delay1 < 50000 && delay1 > 50)
        {
            key = 11;
            delay1 = 0;
        }
        
        while(key == MK_SW12)
        {
            key = read_switches(LEVEL_CHANGE);
            delay2++;
            if(delay2 > 50000)
            {
                key = 14;
                delay2 = 0;
                break;
            }
        }
        if(delay2 < 50000 && delay2 > 50)
        {
            key = 12;
            delay2 = 0;
        }
        
        if(main_flag == DASHBOARD)
        {
            dashboard();
        }
        if(main_flag == PASSWORD)
        {
            password();
        }
        if(main_flag == MENU)
        {
            menu_disp();
        }
        if(main_flag == MENU_ENTER)
        {
            if(menu_flag == VIEWLOG)
            {
                view_log();
            }
            else if(menu_flag == DOWNLOADLOG)
            {
                download_log();
            }
            else if(menu_flag == CLEARLOG)
            {
                clear_log();
            }
            else if(menu_flag == SETTIME)
            {
                set_time();
            }
            else if(menu_flag == CHANGEPASS)
            {
                change_password();
            }
        }
    }
    return;
}

