#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "ars-customer.h"
#include "ars-menus.h"
#include "status.h"
#include "ars-flight.h"
#include "ars-flight-trip.h"
#include "ars-tickets.h"

int g_admin_mode = 0;
struct system_info g_system_info;

int system_info_init()
{
    FILE *fin;
    fin = fopen("system_info.bin", "rb");
    if(fin == NULL)
    {
        //System info file does not exist. It means that overall system is not initialized or being executed for the first time
        g_system_info.last_customer_id = 1;
        g_system_info.last_flight_trip_number = 1;
    }
    else
    {
        if(fread(&g_system_info, sizeof(g_system_info), 1, fin) != 1) //returns the no of objects it reads
        {
            printf("Unable to read system_info file\n");
            fclose(fin);
            return -1;
        }
        else
        {
            printf("Last customer id: %u\n", g_system_info.last_customer_id);
            printf("Last flight trip number is %u\n", g_system_info.last_flight_trip_number);
            fclose(fin);
        }
    }
    return 0;
}

void system_info_cleanup()
{
    FILE *fout;
    fout = fopen("system_info.bin", "wb");
    if(fout == NULL)
    {
        printf("Error in accessing system_info file\n");
        return;
    }
    else
    {
        fwrite(&g_system_info, sizeof(g_system_info), 1, fout);
        fclose(fout);
    }

}

//Utility function to return the day of the week
//Source :
int Week(Date date)
{
	int d, m, y, yr, Day_of_week;

	if(date.month == 1)
    {
        date.month = 13;
        date.year--;
    }

	if(date.month == 2)
    {
        date.month = 14;
        date.year--;
    }

	d = date.day;
	m = 13 * (date.month + 1) / 5;
	yr = (date.year) % 100;
	y = (date.year) / 100;

	Day_of_week = (d + m + yr + yr/4 + y/4 - 2*y) % 7;

	return Day_of_week;
}

int main(int argc, char *argv[])
{
    //Initialize all modules
    if(customer_init() != 0)
    {
        printf("Error initializing customer module\n");
        return -1;
    }

    system_info_init();
    //admin_menu();
    user_menu();

    //Determine whether admin mode or user mode

    //display appropriate menu

    //Clean up all modules
    system_info_cleanup();
    customer_cleanup();

    return 0;
}
