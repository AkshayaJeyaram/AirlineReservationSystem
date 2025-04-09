#include <stdio.h>
#include <string.h>
#include "main.h"
#include "ars-customer.h"
#include "status.h"
#include "ars-flight.h"
#include "ars-menus.h"
#include "ars-flight-trip.h"
#include "ars-tickets.h"

extern struct system_info g_system_info;
struct customer g_current_customer;
int g_is_logged_in = 0;

int customer_init()
{
    printf("Initializing customer module\n");
    return SUCCESS;
}

void customer_cleanup()
{
    printf("Cleaning up customer module\n");
    return;
}

int customer_exists(char *email)
{
    int num_records;
    struct customer customer_detail;
    FILE *fptr;
    fptr = fopen("user_details.bin", "rb");
    if(fptr == NULL)
    {
        return RECORD_DOES_NOT_EXIST; //email doesn't already exist
    }
    else
    {
        num_records = fread(&customer_detail, sizeof(customer_detail), 1, fptr);
        while(num_records == 1)
        {
            if(strcmp(email, customer_detail.email) == 0)
            {
                fclose(fptr);
                return RECORD_EXISTS; //email already exists
            }
            num_records = fread(&customer_detail, sizeof(customer_detail), 1, fptr);
        }
    }
    fclose(fptr);
    return RECORD_DOES_NOT_EXIST;
}

int customer_search(char *email, struct customer *customer_info)
{
    FILE *fptr;
    int num_records;
    fptr = fopen("user_details.bin", "rb");
    if(fptr == NULL)
    {
        return SUCCESS; //email doesn't already exist
    }
    else
    {
        num_records = fread(customer_info, sizeof(*customer_info), 1, fptr);
        while(num_records == 1)
        {
            if(strcmp(email, customer_info->email) == 0)
            {
                fclose(fptr);
                return RECORD_EXISTS; //email already exists
            }

            num_records = fread(customer_info, sizeof(*customer_info), 1, fptr);
        }
    }
    fclose(fptr);
    return SUCCESS;
}

int customer_sign_up()
{
    struct customer c;
    FILE *fp;
    char g[8];
    printf("Enter your name: \n");
    scanf(" %s", c.name);
    printf("Enter your email id: \n");
    scanf(" %s", c.email);
    printf("Enter your password: \n");
    scanf(" %s", c.pwd);
    printf("Enter your date of birth(dd/mm/yyyy): \n");
    scanf(" %s", c.dob);
    gen: printf("Gender(male/female): \n");
    scanf(" %s", g);

    if(strcmp(strlwr(g), "male") == 0)
        c.gen = GENDER_MALE;
    else if(strcmp(strlwr(g), "female") == 0)
        c.gen = GENDER_FEMALE;
    else
    {
        printf("Wrong Input for gender\n");
        goto gen;
    }
    if(customer_exists(c.email) == 1)
    {
        printf("Customer already exists\n");
        return RECORD_EXISTS;
    }
    c.customer_id = ++g_system_info.last_customer_id;
    fp = fopen("user_details.bin", "ab");
    if(fp == NULL)
    {
        printf("Error in opening user details file\n");
        return FILE_OPEN_ERROR;
    }
    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);
    g_is_logged_in = 1;
    g_current_customer = c;
    return SUCCESS;
}

int customer_login()
{
    struct customer c, check;
    printf("Enter email: \n");
    scanf(" %s", c.email);
    printf("Enter password: \n");
    scanf(" %s", c.pwd);
    if(customer_search(c.email, &check) == RECORD_DOES_NOT_EXIST)
    {
        return INVALID_USER;
    }
    if(strcmp(c.pwd,check.pwd) != 0)
    {
        printf("Password : Invalid username or password\n");
        return INVALID_USER;
    }
    else
    {
        g_is_logged_in = 1;
        g_current_customer = check;
        printf("Login successful\n");
    }
    return SUCCESS;
}


