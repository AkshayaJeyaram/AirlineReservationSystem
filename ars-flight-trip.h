#ifndef ARS_FLIGHT_TRIP_H_INCLUDED
#define ARS_FLIGHT_TRIP_H_INCLUDED

enum seat_status
{
    SEAT_OCCUPIED,
    SEAT_FREE
};

enum trip_status
{
    TRIP_STATUS_ACTIVE, TRIP_STATUS_INACTIVE
};

struct seat
{
    enum seat_status status;
    unsigned ticket_number;
    int passenger_index;
};

//This structure is used only for internal operations
struct flight_trip_internal
{
    unsigned trip_number;            //This is a unique identifier to identify a trip
    Date date;
    enum trip_status trip_st;                   //TODO: Generate a unique number like customer id
    struct flight_schedule fs;

    //Date of travel
    //TODO : Identify what data type to be used for date

    struct seat seats[40][6];
    int number_of_free_seats;
};

//The structure is used to store the details in the file
struct flight_trip
{
    unsigned trip_number, flight_number;
    Date date;
    enum trip_status trip_st;
    struct seat seats[40][6];
    int number_of_free_seats;
};

int compare_two_dates(Date date1, Date date2);
int flight_trip_create();
int flight_trip_cancel();
int flight_trip_search(unsigned fno, Date date, struct flight_trip *file_flight_trip);
int flight_trip_prepare_chart();
int flight_trip_update_seats();
void flight_trip_display_all();
#endif // ARS-FLIGHT-TRIP_H_INCLUDED

