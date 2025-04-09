#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "main.h"
#include "status.h"
#include "ars-menus.h"
#include "ars-flight.h"
#include "ars-flight-trip.h"
#include "ars-customer.h"
#include "ars-tickets.h"

struct flight_trip_internal g_current_flight_trip_number;
extern struct system_info g_system_info;
//To be used in admin mode. This function will be used to create a flight trip.

int compare_two_dates(Date date1, Date date2)
{
    if(date1.day == date2.day && date1.month == date2.month && date1.year == date2.year)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int flight_trip_create()
{
    FILE *trip_ptr;
    int no_of_rows, no_of_columns, i, j;
    unsigned fno; //Input Flight Number : Validate whether this flight is valid and get the flight_schedule details
    struct flight_trip_internal f_trip;
    struct flight_trip f_trip_file, temp;

start:
    printf("Enter the flight number: \n");
    scanf(" %u", &fno);
    if(flight_search(fno, &f_trip.fs) == SUCCESS)
    {
        printf("Flight does not exist\n");
        goto start;
    }
    else
    {
        printf("Flight exists\n");
    }

//Input Date of travel - Validate whether the date entered is within the days of travel in schedule, Check if there is no other trip created on this date
re_enter_date:
    printf("Enter Date as (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &f_trip.date.day , &f_trip.date.month , &f_trip.date.year);
    if(f_trip.fs.s.day[Week(f_trip.date)] != 1)
    {
        printf("No flight scheduled on this day\n");
        goto re_enter_date;
    }
    else
    {
        printf("Valid date\n");
    }
    if(flight_trip_search(fno, f_trip.date, &temp) == RECORD_EXISTS && temp.trip_st == TRIP_STATUS_ACTIVE)
    {
            printf("Flight trip entered already exists\n");
    }
    //FREEING OF SEATS
    else
    {
        no_of_rows = f_trip.fs.seat_rows;
        no_of_columns = f_trip.fs.seat_columns;
        f_trip.number_of_free_seats = no_of_rows * no_of_columns;
        for(i = 0; i < no_of_rows; i++)
        {
            for(j = 0; j < no_of_columns; j++)
            {
                f_trip.seats[i][j].status = SEAT_FREE;
            }
        }
        f_trip.trip_number = ++g_system_info.last_flight_trip_number;
        f_trip_file.flight_number = f_trip.fs.flight_no;
        f_trip_file.number_of_free_seats = f_trip.number_of_free_seats;
        f_trip_file.date = f_trip.date;
        memcpy((void *)f_trip_file.seats, (void*)f_trip.seats, sizeof(f_trip.seats));
        f_trip_file.trip_number = f_trip.trip_number;

        trip_ptr = fopen("Flight_Trip.bin", "ab");
        if(trip_ptr == NULL)
        {
            printf("Error in opening Flight Trip file\n");
            return FILE_OPEN_ERROR;
        }
        fwrite(&f_trip_file, sizeof(f_trip_file),1,trip_ptr);
        fclose(trip_ptr);
        printf("Flight trip successfully created\n");
    }
    return ERROR;
}

int flight_trip_cancel()
{
    unsigned fno;
    int num_records;
    struct flight_trip file_flight_trip, temp;
    FILE *f_trip_search;

    printf("Enter the flight number:\n");
    scanf(" %u", &fno);
    printf("Enter Date as (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &file_flight_trip.date.day, &file_flight_trip.date.month , &file_flight_trip.date.year);
    if(flight_trip_search(fno, file_flight_trip.date, &temp) == RECORD_DOES_NOT_EXIST)
    {
        printf("Flight trip entered does not exist \n");
        return RECORD_DOES_NOT_EXIST;
    }
    else
    {
        f_trip_search = fopen("Flight_Trip.bin", "rb+");
        if(f_trip_search == NULL)
        {
            printf("Error in opening Flight Trip file\n");
            return FILE_OPEN_ERROR;
        }
        else
        {
            num_records = fread(&file_flight_trip, sizeof(file_flight_trip), 1, f_trip_search);
            while(num_records == 1)
            {
                if(file_flight_trip.flight_number == fno)
                {
                    if(fseek(f_trip_search, -sizeof(file_flight_trip), SEEK_CUR) != 0)
                    {
                        printf("Flight Trip file error while reading \n");
                        fclose(f_trip_search);
                        return FILE_OPEN_ERROR;
                    }
                    else
                    {
                        file_flight_trip.trip_st = TRIP_STATUS_INACTIVE;
                        printf("Trip status before writing to file : %d\n",file_flight_trip.trip_st);
                        fwrite(&file_flight_trip, sizeof(file_flight_trip), 1, f_trip_search);
                        fclose(f_trip_search);
                        printf("File trip successfully canceled\n");
                        //TO DO: CANCEL FLIGHT TICKETS
                        return SUCCESS;
                    }
                }
                num_records = fread(&file_flight_trip, sizeof(file_flight_trip), 1, f_trip_search);
            }
        }
        fclose(f_trip_search);
        return SUCCESS;
    }
}

void flight_trip_display_all()
{
    int num_records;
    FILE *f_trip_search;
    struct flight_trip file_flight_trip;

    f_trip_search = fopen("Flight_Trip.bin", "rb");
    if(f_trip_search == NULL)
    {
        printf("Error in opening Flight Trip file\n");
        return;
    }

    num_records = fread(&file_flight_trip, sizeof(file_flight_trip), 1, f_trip_search);
    while(num_records == 1)
    {
        printf("Trip # : %4d, Fl # : %4d, Status : %1d\n",file_flight_trip.trip_number,file_flight_trip.flight_number,file_flight_trip.trip_st);
        num_records = fread(&file_flight_trip, sizeof(file_flight_trip), 1, f_trip_search);
    }

    return;
}


int flight_trip_search(unsigned fno, Date date, struct flight_trip *file_flight_trip)
{
    FILE *f_trip_search;
    int num_records;
    f_trip_search = fopen("Flight_Trip.bin", "rb");
    if(f_trip_search == NULL)
    {
        return RECORD_DOES_NOT_EXIST;
    }
    else
    {
        num_records = fread(file_flight_trip, sizeof(*file_flight_trip), 1, f_trip_search);
        while(num_records == 1)
        {
            if(file_flight_trip->flight_number == fno && compare_two_dates(file_flight_trip->date, date))
            {
                fclose(f_trip_search);
                return RECORD_EXISTS;
            }
            num_records = fread(file_flight_trip, sizeof(*file_flight_trip), 1, f_trip_search);
        }
    }
    fclose(f_trip_search);
    return RECORD_DOES_NOT_EXIST;
}

int flight_trip_prepare_chart()
{
    struct flight_trip temp;
    struct flight_schedule f_temp;
    unsigned fno;
    //struct flight_trip_internal chart_flight_trip;
    Date date_temp;
    printf("Enter flight number:\n");
    scanf(" %u", &fno);
    printf("Enter Date as (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &date_temp.day , &date_temp.month , &date_temp.year);
    if(flight_trip_search(fno, date_temp, &temp) == RECORD_DOES_NOT_EXIST)
    {
        printf("Flight trip does not exist\n");
        return RECORD_DOES_NOT_EXIST;
    }
    else
    {
        printf("The flight record: \n");
        flight_search(temp.flight_number, &f_temp);
        printf("Flight Number: %u\t\t\t", f_temp.flight_no);
        if(f_temp.s.minute == 0 && f_temp.s.hour == 0)
        {
            printf(" Date and Time: %d/%d/%d   %02d:%02d\n", temp.date.day, temp.date.month, temp.date.year, f_temp.s.hour, f_temp.s.minute);
        }
        else if(f_temp.s.hour == 0)
        {
            printf(" Date: %d/%d/%d Time: %02d:%d\n", temp.date.day, temp.date.month, temp.date.year, f_temp.s.hour, f_temp.s.minute);
        }
        else if(f_temp.s.minute == 0)
        {
            printf(" Date: %d/%d/%d Time: %d:%02d\n", temp.date.day, temp.date.month, temp.date.year, f_temp.s.hour, f_temp.s.minute);
        }
        else
        {
            printf(" Date: %d/%d/%d Time: %d:%d\n", temp.date.day, temp.date.month, temp.date.year, f_temp.s.hour, f_temp.s.minute);
        }
        printf("From: %s\t\t\t\t To: %s\n", f_temp.source_city, f_temp.dest_city);

        if(temp.trip_st == TRIP_STATUS_ACTIVE)
            printf("Trip Status: Active\t\t\t ");
        else if(temp.trip_st == TRIP_STATUS_INACTIVE)
            printf("Trip Status: Inactive\t\t\t ");
        printf("Number of vacant seats: %d\n", temp.number_of_free_seats);

    }
    //Input:
    //      Flight Number
    //      Date
    //
    //Output:
    //      Seat Number, Status, Name of passenger, Sex, DOB
}

//int flight_trip_update_seats(tickets)
//{

//}
