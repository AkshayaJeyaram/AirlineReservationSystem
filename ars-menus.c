#include<stdio.h>
#include<stdlib.h>
#include "main.h"
#include "ars-customer.h"
#include "ars-flight.h"
#include "ars-menus.h"
#include "status.h"
#include "ars-flight-trip.h"
#include "ars-tickets.h"

void admin_menu()
{
    int choice;
    start: printf("MENU \n");
    printf("\t1.Create Flight Schedule\n\t2.Display Flight Schedule\n\t3.Cancel Flight Schedule\n\t4.Create Flight Trip\n\t5.Cancel Flight Trip\n\t6.Display Trip Chart\n\t7.Exit\n");
    printf("Choose your option: ");
    scanf("%d", &choice);
    switch(choice)
    {
        case 1: flight_create_schedule();
                break;
        case 2: flight_schedule_display();
                break;
        case 3: flight_schedule_cancel();
                break;
        case 4: flight_trip_create();
                break;
        case 5: flight_trip_cancel();
                break;
        case 6: flight_trip_prepare_chart();
                break;
        case 7: exit(0);
        default: printf("Wrong option\n");
                 goto start;
    }
}
void user_menu()
{
    int choice;
    start: printf("MENU \n");
    printf("\t1. Login\n\t2. Signup\n\t3.Book a Ticket\n\t4.Cancel a Ticket\n\t5.Show booking history\n\t6.Exit\n");
    printf("Choose your option: ");
    scanf("%d", &choice);
    switch(choice)
    {
        case 1: customer_login();
                break;
        case 2: customer_sign_up();
                break;
        case 3: book_tickets();
                break;
        case 4: //cancel_ticket();
                //break;
        case 5: //display_booking_history();
                //break;
        case 6: exit(0);
        default: printf("Wrong option\n");
                 goto start;
    }
}

