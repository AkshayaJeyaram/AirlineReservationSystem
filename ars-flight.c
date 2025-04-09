#include<stdio.h>
#include<string.h>
#include "main.h"
#include "ars-flight.h"
#include "ars-customer.h"
#include "status.h"
#include "ars-menus.h"
#include "ars-flight-trip.h"
#include "ars-tickets.h"

int flight_schedule_cancel()
{
    int num_records;
    FILE *fp;
    unsigned fno;
    struct flight_schedule f;
    printf("Enter flight number: \n");
    scanf( "%u", &fno);
    fp = fopen("flight_details.bin", "r+"); //to prevent overwriting, file pointer points to beginning of the file
    if(fp == NULL)
    {
        printf("Error in opening Flight Details file\n");
        return FILE_OPEN_ERROR;
    }
    else
    {
        num_records = fread(&f, sizeof(f), 1, fp);
        while(num_records == 1)
        {
            if(f.flight_no == fno)
            {
                if(fseek(fp, -sizeof(f), SEEK_CUR) != 0)
                {
                    printf("Flight Details file error while reading\n");
                    fclose(fp);
                    return FILE_OPEN_ERROR; //change to FILE ERROR later
                }
                else
                {
                    f.st =  INACTIVE;
                    fwrite(&f, sizeof(f),1,fp);
                    fclose(fp);
                    printf("Flight schedule successfully canceled\n");
                    return SUCCESS;
                }
            }
            num_records = fread(&f, sizeof(f), 1, fp);
        }
    }
    fclose (fp);
    return SUCCESS;
}


int flight_schedule_display()
{
    int i;
    unsigned fno;
    struct flight_schedule f;
    printf("Enter flight number: \n");
    scanf(" %u", &fno);
    if(flight_search(fno, &f) == 0)
    {
        printf("Flight schedule does not exist\n");
        return ERROR;
    }
    else
    {
        printf("The flight record: \n");
        printf("Flight number is %u\n", f.flight_no);
        printf("Source city is %s\n", f.source_city);
        printf("Destination city is %s\n", f.dest_city);
        printf("Price is %u \n", f.price);
        printf("Number of seat rows is %d \n", f.seat_rows);
        printf("Number of seat columns is %d \n", f.seat_columns);
        printf("Days of operation of flight: \n");
        for(i = 0; i < 7; i++)
        {
            if(f.s.day[i] == 1)
            {
                if(i == 0)
                    printf("Saturday\t");
                else if(i == 1)
                    printf("Sunday\t");
                else if(i == 2)
                    printf("Monday\t");
                else if(i == 3)
                    printf("Tuesday\t");
                else if(i == 4)
                    printf("Wednesday\t");
                else if(i == 5)
                    printf("Thursday\t");
                else if(i == 6)
                    printf("Friday\t");
            }
        }
        printf("\n");
        if(f.s.minute == 0 && f.s.hour == 0)
        {
            printf("Time of operation of flight %02d:%02d \n", f.s.hour, f.s.minute);
        }
        else if(f.s.hour == 0)
        {
            printf("Time of operation of flight %02d:%d \n", f.s.hour, f.s.minute);
        }
        else if(f.s.minute == 0)
        {
            printf("Time of operation of flight %d:%02d \n", f.s.hour, f.s.minute);
        }
        else
        {
            printf("Time of operation of flight %d:%d \n", f.s.hour, f.s.minute);
        }
        if(f.st == ACTIVE)
        {
            printf("Status: ACTIVE\n");
        }
        else
        {
            printf("Status: INACTIVE\n");
        }
    }
    return SUCCESS;
}

int flight_search_with_city(char source_city[], char dest_city[], Date date, struct flight_schedule *f)
{
    FILE *fptr;
    int num_records;

    fptr = fopen("flight_details.bin", "rb");
    if(fptr == NULL)
    {
        return FLIGHT_UNAVAILABLE_BETWEEN_CITIES;
    }

    num_records = fread(f, sizeof(*f), 1, fptr);
    while(num_records == 1)
    {
        if(strcmp(source_city,f->source_city) != 0 && strcmp(dest_city, f->dest_city) != 0)
        {
            fclose(fptr);
            return FLIGHT_UNAVAILABLE_BETWEEN_CITIES;
        }
        else if(f->s.day[Week(date)] != 1)
        {
            fclose(fptr);
            return FLIGHT_UNAVAILABLE_ON_DATE;
        }
        else
        {
            fclose(fptr);
            return RECORD_EXISTS;
        }
    }
    return FLIGHT_UNAVAILABLE_BETWEEN_CITIES;
}

int flight_search(unsigned fno, struct flight_schedule *f)
{
    FILE *fptr;
    int num_records;
    fptr = fopen("flight_details.bin", "rb");
    if(fptr == NULL)
    {
        return SUCCESS;
    }
    else
    {
        num_records = fread(f, sizeof(*f), 1, fptr);
        while(num_records == 1)
        {
            if(f->flight_no == fno)
            {
                fclose(fptr);
                return RECORD_EXISTS;
            }
            num_records = fread(f, sizeof(*f), 1, fptr);
        }
    }
    fclose (fptr);
    return SUCCESS;
}

void get_days(int *arr)
{
    int i;
    for(i = 0; i < 7; i++)
    {
        arr[i] = 0;
    }
    char day[32];
    char *token;
    start: printf("Enter the days of operation of the flight (Example: Mon,Wed,Thu) :  \n");
    scanf(" %s", day);
    token = strtok(day, ",");
    while(token != NULL)
    {
        if(strcmp(strlwr(token),"sat") == 0)
        {
            arr[0] = 1;
        }
        else if(strcmp(strlwr(token), "sun") == 0)
        {
            arr[1] = 1;
        }
        else if(strcmp(strlwr(token), "mon") == 0)
        {
            arr[2] = 1;
        }
        else if(strcmp(strlwr(token), "tue") == 0)
        {
            arr[3] = 1;
        }
        else if(strcmp(strlwr(token), "wed") == 0)
        {
            arr[4] = 1;
        }
        else if(strcmp(strlwr(token), "thu") == 0)
        {
            arr[5] = 1;
        }
        else if(strcmp(strlwr(token), "fri") == 0)
        {
            arr[6] = 1;
        }
        else
        {
            printf("Wrong value entered\n");
            goto start;
        }
        token = strtok(NULL, ",");
    }
}

void flight_create_schedule()
{
    FILE *fp;
    struct flight_schedule f,temp;
    printf("Enter flight number: \n");
    scanf(" %u", &f.flight_no);
    printf("Enter source city: \n");
    scanf(" %s", f.source_city);
    printf("Enter destination city: \n");
    scanf(" %s", f.dest_city);
    printf("Enter price: \n");
    scanf(" %u", &f.price);
    printf("Enter number of seat rows: \n");
    scanf(" %d", &f.seat_rows);
    printf("Enter number of seat columns: \n");
    scanf(" %d", &f.seat_columns);
    f.st = ACTIVE;
    get_days(f.s.day);
    printf("Enter the time of operation of flight as hours:minutes \n");
    scanf("%d:%d", &f.s.hour, &f.s.minute);
    if(flight_search(f.flight_no, &temp) == 0)
    {
        fp = fopen("flight_details.bin", "ab");
        if(fp == NULL)
        {
            printf("Error in opening flight details file\n");
        }
        else
        {
            fwrite(&f, sizeof(f), 1, fp);
            fclose(fp);
        }
    }
    else
    {
        printf("Flight already exists\n");
    }
}
