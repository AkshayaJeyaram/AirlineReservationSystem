# Airline Reservation System

## Overview
The Airline Reservation System (ARS) is a C-based software application that manages flight schedules, ticket bookings, and customer data. This project allows users to book tickets, cancel bookings, and manage their flight trips, while admins can create and manage flight schedules and trips.

### Features:
- **Admin Features:**
  - Create flight schedules.
  - Display and cancel flight schedules.
  - Manage flight trips (create, cancel, and display trip details).
  - Generate a trip chart.

- **User Features:**
  - Customer login and sign-up.
  - Book and cancel tickets.
  - View booking history.

### Modules:
- **ars-customer.c**: Handles customer-related functionality such as registration, login, and account management.
- **ars-flight.c**: Manages flight schedules, including adding, displaying, and canceling flights.
- **ars-tickets.c**: Handles the ticket booking and cancellation process, managing passenger details and flight availability.
- **ars-menus.c**: Provides the user interface for both admin and customer functionalities.
- **main.c**: The main program that initializes and ties all modules together, running the application.

### Compilation:
To compile the project, you can use the following command:

```bash
gcc *.c -o AirlineReservationSystem
Usage:
Run the executable: ./AirlineReservationSystem

Select either the admin or user menu based on your role.

Follow the on-screen prompts to perform various actions like booking flights, managing flight schedules, etc.

Future Improvements:
Implement additional user features like changing password, and updating personal details.

Enhance the admin interface with better flight schedule management features.

Add a database or file-based persistence for better data storage and retrieval.
