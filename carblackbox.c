
/*
 * File:   carblackbox.c
 * Author: Admin
 *
 * Created on 28 March, 2024, 2:20 PM
 */


#include <xc.h>
#include"main.h"

static int flag = 0;
static int onflag = 0;
static int i=0;
int sp;


void dashboard(void)
{
    clcd_print("TIME",LINE1(1));
    clcd_print("EV",LINE1(10));
    clcd_print("SP",LINE1(13));
    
    clcd_print(time,LINE2(1));
    
    if(onflag == 0)
    {
        clcd_print(gear[0],LINE2(10));
    }
    
    if(event_count > 10)
    {
        event_count = 10;
    }
    
    if(key == MK_SW1)
    {
        onflag = 1;
        
        if(flag == 3)
        {
            i=1;
            flag--;
        }
        else if(i<=5)
        {
            i++;
        }
        else if(i==6)
        {
            i=6;
        }
        clcd_print(gear[i],LINE2(10));
        event_capture();
        event_count++;

    }
    else if(key == MK_SW2)
    {
        if(flag == 3)
        {
            i=1;
            flag--;
        }
        else if(i>1 && i<7)
        {
            i--;
        }
        else
        {
            i=1;
        }
        clcd_print(gear[i],LINE2(10));
        event_capture();
        event_count++;
    }
    
    if(key == MK_SW3)
    {
        i=7;
        clcd_print(gear[i],LINE2(10));
        event_capture();
        event_count++;
        flag = 3;
    }
    
    sp = read_adc(CHANNEL4);
    
    sp = sp / 10.25;
    
    speed[0] = sp/10 +48;
    speed[1] = sp%10 + 48;
    clcd_print(speed,LINE2(13));
    
    if(key == MK_SW11)
    {
        main_flag = 1;
        CLEAR_DISP_SCREEN;
        key=ALL_RELEASED;
        return;
    }
}
     
 

void event_capture(void)
{
   
    if(event_count >= 9)
    {
        event_count=9;
    }
    
    if(add == 99)
    {
        add = 0;
    }
    
    for(int i=0; i<8; i++)
    {
        if(i == 2 || i == 5)
            continue;
        
        write_external_eeprom(add++,time[i]);
    }
        
    write_external_eeprom(add++,gear[i][0]);
    write_external_eeprom(add++,gear[i][1]);
    
    write_external_eeprom(add++,speed[0]);
    write_external_eeprom(add++,speed[1]);
    
    /*
    for(int i=0; i<10; i++)
    {
        captured_event[i] = read_external_eeprom(i);
    }
    captured_event[10] = '\0';
    CLEAR_DISP_SCREEN;
    clcd_print(captured_event,LINE2(1));
    
    while(1);
    */
}

void password(void)
{
    static int j;
    static int att=2;
    int delay = 0;
    
    if(passflag == 0)
    {
        passflag = 1;
        return;
    }
    
    if(passflag == 1)
    {
    
        if(j<4)                    
        {
            clcd_print("Enter Password :",LINE1(1));      // to print prompt on the line one
            
            if(delay++ < 200)                   // to blink the cursor
            {
                clcd_putch('_',LINE2(5+j));
            }
            else if(delay < 400)
            {
                clcd_putch(' ',LINE2(5+j));
            }
            else
            {
                delay = 0;
            }    
            
            if(key == MK_SW11)                 // if switch1 pressed
            {
                clcd_putch('*',LINE2(5+j));     // print * from 5th index
                entered[j++] = '0';             // store 0 if sw1 pressed
            }
            else if(key == MK_SW12)           //if switch2 pressed
            {
                clcd_putch('*',LINE2(5+j));   // print *
                entered[j++] = '1';           // and store 1 if sw2 pressed
            }
        }
        
        if(j==4)                // if array is full then start comparing the entered password
        {
            j = 0;
            entered[4]='\0';       // add null character at end to compare
            
            if(strcmp(original,entered) == 0)     // if equal
            {
                CLEAR_DISP_SCREEN;
                clcd_print("SUCCESS",LINE1(1));
                clcd_print("Correct pass!",LINE2(2));
                
                for(int wait = 5000; wait--;)
                    for(int wait1 = 200;wait1--;);
                
                
                key = ALL_RELEASED;
                main_flag = 2;
                CLEAR_DISP_SCREEN;
                att=2;
                return;
            }
            else
            {
                
                CLEAR_DISP_SCREEN;
                clcd_print("FAILURE!",LINE1(1));
                clcd_print("Attempt:",LINE2(2));
                
                   
                if(att >= 0)
                {
                    clcd_putch(att + 48,LINE2(11));
                    att--;    
                }
               
                for(int wait = 5000; wait--;)
                    for(int wait1 = 200;wait1--;);
                CLEAR_DISP_SCREEN;
                
                if(att < 0)
                {
                    CLEAR_DISP_SCREEN;
                    sec=120;
                    while(sec>=0)
                    {
                        clcd_print("You have blocked",LINE1(0));
                        clcd_print("wait for:",LINE2(0));
                        
                        if(sec >= 0)
                        {
                            second[0] = (sec/100)+48;
                            second[1] = ((sec/10)%10)+48;
                            second[2] = (sec % 10)+48;

                            clcd_print(second,LINE2(10));
                        }
                    }
                    CLEAR_DISP_SCREEN;
                    main_flag = 0;
                    sec=120;
                }
                
            }
        }
    }
}

void menu_disp()
{
    
    if(key == MK_SW12 && ind2 < 5)
    {
        
        if(s_flag == 1)
        {
            if(ind2 < 4)
            {
                ind1 = ind2;
                ind2++;
                menu_flag++;
            }
        }
        else if(s_flag == 0)
        {
            s_flag = 1;
            menu_flag++;
        }
    }
    else if(key == MK_SW11 && ind1 >= 0)
    {
        if(s_flag == 1)
        {
            s_flag = 0;
            menu_flag--;
        }
        else if(ind1 > 0)
        {
            ind2 = ind1;
            ind1--;
            menu_flag--;
        }
    }
    if(key == MK_SW11_LONG)
    {
        main_flag = 3;
        CLEAR_DISP_SCREEN;
        return;
    }
    
    if(s_flag == 0)
    {
        clcd_putch('*',LINE1(0));
        clcd_putch(' ',LINE2(0));
    }
    else
    {
        clcd_putch(' ',LINE1(0));
        clcd_putch('*',LINE2(0));
    }

    clcd_print(menu[ind1],LINE1(1));
    clcd_print(menu[ind2],LINE2(1));
    
    if(key == MK_SW12_LONG)
    {
        CLEAR_DISP_SCREEN;
        main_flag = 0;
        menu_flag = 0;
        onflag=0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        return;
    }

}


void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
    
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void view_log()
{
    clcd_print("LOGS :",LINE1(0));
    if(event_count == 0)
    {
        clcd_print("Log Empty!",LINE2(0));
        for(int wait = 5000; wait--;)
            for(int wait1 = 200;wait1--;);
        main_flag=2;
        menu_flag = 0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        return;
    }
    
    if(add1 <= 0)
    {
        add1=0;
    }
    if(key == MK_SW11)
    {
        if(disp_count > 0)
        {
            if(add1 != 0)
            {
                add1=add1-10;
            }
            disp_count--;
        }
    }
    else if(key == MK_SW12)
    {
        if(disp_count < event_count-1)
        {
            add1 = add1+10;
            if(add1 == 99)
            {
                add1=0;
            }
            disp_count++;
        }
    }
    
    add2=add1;
    for(int i=0; i<16; i++)
    {
        if(i==0)
        {
            captured_event[i] = disp_count+48;
            continue;
        }
        else if(i==1 || i==10 || i==13)
        {
            captured_event[i] = ' ';
            continue;
        }
        else if(i==4 || i==7)
        {
            captured_event[i] = ':';
            continue;
        }
        captured_event[i] = read_external_eeprom(add2++);
    }
    captured_event[16] = '\0';
    
    clcd_print(captured_event,LINE2(0));
    
    if(key == MK_SW12_LONG)
    {
        CLEAR_DISP_SCREEN;
        main_flag = 2;
        menu_flag = 0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        return;
    }
}

void download_log()
{
    if(event_count == 0)
    {
        clcd_print("Log Empty!",LINE1(0));
        puts("Log Empty!");
        main_flag=2;
        menu_flag = 0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        for(int wait = 10000; wait--;)
        for(int wait1 = 200;wait1--;);
        return;
    }
    else
    {
        add2=0;
        for(int k=0; k< event_count; k++)
        {
            for(int i=0; i<16; i++)
            {
                if(i==0)
                {
                    captured_event[i] = k+48;
                    continue;
                }
                else if(i==1 || i==10 || i==13)
                {
                    captured_event[i] = ' ';
                    continue;
                }
                else if(i==4 || i==7)
                {
                    captured_event[i] = ':';
                    continue;
                }
                captured_event[i] = read_external_eeprom(add2++);
            }
            captured_event[16] = '\0';    

            puts(captured_event);
            putch('\n');
            putch('\r');
        }   

        clcd_print("Downloading..",LINE1(0));
        for(int wait = 10000; wait--;)
            for(int wait1 = 200;wait1--;);
        main_flag = 2;
        menu_flag = 0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        return;
    }
}


void clear_log()
{
    clcd_print("Clearing...",LINE1(0));
    add=0;
    event_count=0;
    
    for(int wait = 10000; wait--;)
         for(int wait1 = 200;wait1--;);
         
    main_flag = 2;
    menu_flag = 0;
    ind1=0;           
    ind2=1;
    s_flag=0;
    return;
}

void set_time()
{
    
    clcd_print("HR MN SC",LINE1(0));
    
    if(once_time == 0)
    {
        once_time=1;
        
        hr = time[0]-48;
        hr = hr*10+(time[1]-48);

        min = time[3]-48;
        min = min*10+(time[4]-48);

        secs = time[6]-48;
        secs = secs*10+(time[7]-48);
    }
/*
    clcd_putch(hr/10+48,LINE2(0));
    clcd_putch(hr%10+48,LINE2(1));
    
    clcd_putch(min/10+48,LINE2(3));
    clcd_putch(min%10+48,LINE2(4));

    clcd_putch(secs/10+48,LINE2(6));
    clcd_putch(secs%10+48,LINE2(7));
    */
    
    if(time_flag == 1)
    {
        clcd_putch(min/10+48,LINE2(3));
        clcd_putch(min%10+48,LINE2(4));

        clcd_putch(secs/10+48,LINE2(6));
        clcd_putch(secs%10+48,LINE2(7));
        
        if(hr_blink_delay++ < 200)                   // to blink the cursor
        {
            clcd_putch(hr/10+48,LINE2(0));
            clcd_putch(hr%10+48,LINE2(1));
        }
        else if(hr_blink_delay < 400)
        {
            clcd_putch(' ',LINE2(0));
            clcd_putch(' ',LINE2(1));
        }
        else
        {
            hr_blink_delay = 0;
        }
    }
    else if(time_flag == 2)
    {
        clcd_putch(hr/10+48,LINE2(0));
        clcd_putch(hr%10+48,LINE2(1));
        
        clcd_putch(secs/10+48,LINE2(6));
        clcd_putch(secs%10+48,LINE2(7));
        
        if(min_blink_delay++ < 200)                   // to blink the cursor
        {
            clcd_putch(min/10+48,LINE2(3));
            clcd_putch(min%10+48,LINE2(4));
        }
        else if(min_blink_delay < 400)
        {
            clcd_putch(' ',LINE2(3));
            clcd_putch(' ',LINE2(4));
        }
        else
        {
            min_blink_delay = 0;
        }
    }
    else if(time_flag == 3)
    {
        clcd_putch(hr/10+48,LINE2(0));
        clcd_putch(hr%10+48,LINE2(1));
    
        clcd_putch(min/10+48,LINE2(3));
        clcd_putch(min%10+48,LINE2(4));
        
        if(sec_blink_delay++ < 200)                   // to blink the cursor
        {
            clcd_putch(secs/10+48,LINE2(6));
            clcd_putch(secs%10+48,LINE2(7));
        }
        else if(sec_blink_delay < 400)
        {
            clcd_putch(' ',LINE2(6));
            clcd_putch(' ',LINE2(7));
        }
        else
        {
            sec_blink_delay = 0;
        }
    }
    

    if(key == MK_SW11)
    {
        if(time_flag==4)
        {
            time_flag = 1;
        }
        else
        {
            time_flag++;
        }
    }
    
    if(key == MK_SW12)
    {
        if(time_flag == 1)
        {
            if(hr<23)
            {
                hr++;
            }
            else
            {
                hr=0;
            }
        }
        else if(time_flag == 2)
        {
            if(min<59)
            {
                min++;
            }
            else
            {
                min=0;
            }
        }
        else if(time_flag == 3)
        {
            if(secs<59)
            {
                secs++;
            }
            else
            {
                secs=0;
            }
        }
        else
        {
            time_flag=1;
        }
    }
        
    if(key == MK_SW11_LONG)
    {
        write_ds1307(HOUR_ADDR, ((hr/10)<<4)|hr%10);
        write_ds1307(MIN_ADDR, ((min/10)<<4)|min%10);
        write_ds1307(SEC_ADDR, ((secs/10)<<4)|secs%10);
    }
    
    if(key == MK_SW12_LONG)
    {
        CLEAR_DISP_SCREEN;
        main_flag = 0;
        menu_flag = 0;
        onflag=0;
        ind1=0;           
        ind2=1;
        s_flag=0;
        return;
    }
}

void change_password()
{
    pass_add=200;
    if(pass_flag == 0)
    {
        clcd_print("Enter New Pass:",LINE1(0));
        if(m<4)                    
        {
            if(passdelay++ < 200)                   // to blink the cursor
            {
                clcd_putch('_',LINE2(5+m));
            }
            else if(passdelay < 400)
            {
                clcd_putch(' ',LINE2(5+m));
            }
            else
            {
                passdelay = 0;
            }    

            if(key == MK_SW11)                 // if switch1 pressed
            {
                clcd_putch('*',LINE2(5+m));     // print * from 5th inde
                new_pass1[m++] = '0';             // store 0 if sw1 pressed
            }
            else if(key == MK_SW12)           //if switch2 pressed
            {
                clcd_putch('*',LINE2(5+m));   // print *
                new_pass1[m++] = '1';           // and store 1 if sw2 pressed
            }
        }
        else if(m==4)
        {
            pass_flag=1;
            CLEAR_DISP_SCREEN;
        }
    }
    if(pass_flag == 1)
    {
        clcd_print("Re Enter Pass:",LINE1(0));
        if(n<4)                    
        {
            if(passdelay++ < 200)                   // to blink the cursor
            {
                clcd_putch('_',LINE2(5+n));
            }
            else if(passdelay < 400)
            {
                clcd_putch(' ',LINE2(5+n));
            }
            else
            {
                passdelay = 0;
            }    

            if(key == MK_SW11)                 // if switch1 pressed
            {
                clcd_putch('*',LINE2(5+n));     // print * from 5th inde
                new_pass2[n++] = '0';             // store 0 if sw1 pressed
            }
            else if(key == MK_SW12)           //if switch2 pressed
            {
                clcd_putch('*',LINE2(5+n));   // print *
                new_pass2[n++] = '1';           // and store 1 if sw2 pressed
            }
        }
        else if(n==4)
        {
            pass_flag = 2;
            CLEAR_DISP_SCREEN;
        }
    }
    else if(pass_flag == 2)
    {
        m=0;
        n=0;
        new_pass1[4] = '\0';
        new_pass2[4] = '\0';
        
        if(strcmp(new_pass1,new_pass2) == 0)
        {
            for(int x=0; x<4; x++)
            {
                write_external_eeprom(pass_add++, new_pass1[x]);
            }
            
            clcd_print("SUCCESS !",LINE1(0));
            clcd_print("PASS Changed",LINE2(0));
            strcpy(original,new_pass1);
            pass_flag=0;
            
            for(int wait = 5000; wait--;)
                for(int wait1 = 200;wait1--;);
            main_flag = 0;
            menu_flag = 0;
            onflag=0;
            ind1=0;           
            ind2=1;
            s_flag=0;
            CLEAR_DISP_SCREEN;
            return;
        }
        else
        {
            clcd_print("FAILURE !",LINE1(0));
            clcd_print("Cannot change",LINE2(0));
            for(int wait = 5000; wait--;)
                for(int wait1 = 200;wait1--;);
            main_flag = 2;
            menu_flag = 0;
            ind1=0;           
            ind2=1;
            pass_flag=0;
            s_flag=0;
            CLEAR_DISP_SCREEN;
            return;
        }  
    }
}