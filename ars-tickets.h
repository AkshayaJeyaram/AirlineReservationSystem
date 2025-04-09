#ifndef ARS_TICKETS_H_INCLUDED
#define ARS_TICKETS_H_INCLUDED

enum gender_of_passenger
{
    MALE, FEMALE
};

enum ticket_status
{
    TICKET_VALID, TICKET_INVALID
};

struct passengers
{
    int seat_row, seat_column;
    enum gender_of_passenger gen;
    char name[32];
    int age;
};

struct ticket
{
    unsigned ticket_number, customer_id, trip_number, price;
    enum ticket_status ticket_st;
    struct passengers passenger[8];
};

int book_tickets(); // Ssource and destination, date, passengers //USER MENU

#endif // ARS-TICKETS_H_INCLUDED
