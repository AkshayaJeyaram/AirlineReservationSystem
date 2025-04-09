#ifndef ARS_FLIGHT_H_INCLUDED
#define ARS_FLIGHT_H_INCLUDED

enum flight_schedule_status
{
    ACTIVE, INACTIVE
};

struct schedule
{
    int day[7];
    int hour, minute;
};
struct flight_schedule
{
    unsigned flight_no, price;
    int seat_rows, seat_columns;
    char source_city[16];
    char dest_city[16];
    struct schedule s;
    enum flight_schedule_status st;
};

void flight_create_schedule();
int flight_search();
int flight_search_with_city(char source_city[], char dest_city[], Date date, struct flight_schedule *f);
int flight_schedule_cancel();
int flight_schedule_display();

#endif // ARS-FLIGHT_H_INCLUDED
