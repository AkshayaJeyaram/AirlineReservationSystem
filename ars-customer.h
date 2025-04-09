#ifndef ARS_CUSTOMER_H_INCLUDED
#define ARS_CUSTOMER_H_INCLUDED

enum gender
{
    GENDER_MALE, GENDER_FEMALE
};

struct customer
{
    unsigned int customer_id;
    char name[32];
    char email[32];
    char pwd[32];
    char dob[32];
    enum gender gen;

};

int customer_init();
void customer_cleanup();
int customer_exists(char *); //input = email, output = yes or no
int customer_sign_up(); //user
int customer_login(); //user
int customer_remove(); //admin
int customer_search(char *email, struct customer *customer_info); //internal function

#endif // ARS-CUSTOMER_H_INCLUDED
