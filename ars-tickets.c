#include<stdio.h>
#include<stdlib.h>
#include "main.h"
#include "ars-customer.h"
#include "ars-flight.h"
#include "ars-menus.h"
#include "status.h"
#include "ars-flight-trip.h"
#include "ars-tickets.h"

extern struct system_info g_system_info;
extern struct customer g_current_customer;
extern int g_is_logged_in;

int book_tickets()
{
    struct ticket tck;
    struct flight_schedule f_schedule;
    struct flight_trip f_trip;
    char source_city[32];
    char dest_city[32];
    char g[8];
    int no_of_passengers, i;
    Date date;
    printf("Enter source city: \n");
    scanf(" %s", source_city);
    printf("Enter destination city: \n");
    scanf(" %s", dest_city);
    printf("Enter the date of travel (dd/mm/yy): \n");
    scanf("%d/%d/%d", &date.day, &date.month, &date.year);
    if(flight_search_with_city(source_city, dest_city, date, &f_schedule) == FLIGHT_UNAVAILABLE_BETWEEN_CITIES)
    {
        printf("No flights available between the two cities\n");
        return 0;
    }
    if(flight_search_with_city(source_city, dest_city, date, &f_schedule) == FLIGHT_UNAVAILABLE_ON_DATE)
    {
        printf("No flights available on this date\n");
        return 0;
    }
    if(flight_trip_search(f_schedule.flight_no, date, &f_trip) == RECORD_EXISTS)
    {
        printf("Flight(s) available\n");
    }
    else
    {
        printf("No flights available on this date\n");
        return 0;
    }
    if(f_trip.number_of_free_seats == 0)
    {
        printf("No seats available. Check again later. \n");
        return 0;
    }
    printf("Number of seats available: %d\n", f_trip.number_of_free_seats);
    printf("Price for each seat: %u\n", f_schedule.price);
    printf("Enter number of passengers: \n");
    scanf("%d", &no_of_passengers);
    for(i = 0; i < no_of_passengers; i++)
    {
        printf("Enter passenger details for passenger %d", i);
        printf("Name:\n");
        scanf(" %s", tck.passenger[i].name);
        printf("Age:\n");
        scanf(" %d", tck.passenger[i].age);
  gen:  printf("Gender(male/female): \n");
        scanf(" %s", g);
        if(strcmp(strlwr(g), "male") == 0)
            tck.passenger[i].gen = MALE;
        else if(strcmp(strlwr(g), "female") == 0)
            tck.passenger[i].gen = FEMALE;
        else
        {
            printf("Wrong Input for gender\n");
            goto gen;
        }
    }
   return 0;
}




