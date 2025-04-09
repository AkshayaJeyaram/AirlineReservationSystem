#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

struct system_info
{
    unsigned int last_customer_id, last_flight_trip_number;
};

typedef struct Date
{

	int day, month, year;

}Date;


int Week(Date date);

#endif // MAIN_H_INCLUDED
