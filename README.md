# Airport Traffic Control Simulator

A Data Structures project implemented in C that simulates an airport air
traffic control system for managing arrivals, departures, emergency flights,
and passengers.

The system uses queues and linked lists to organize flights and passenger
information while providing a menu-driven interface for managing airport
operations.

## Features

- Load flight and passenger information from text files
- Manage arrival flights
- Manage departure flights
- Prioritize emergency flights
- Land the next available flight
- Depart scheduled flights
- Cancel flights without permanently deleting their information
- Display flight details
- Display flight status
- Add new flights
- Manage passengers for each flight
- Validate flight IDs, dates, and times
- Dynamically allocate and free memory

## Data Structures

The project uses:

- **Queues** for managing:
  - Arrival flights
  - Departure flights
  - Emergency flights
  - Landed flights
  - Departed flights
  - Cancelled flights

- **Linked Lists** for storing passengers associated with each flight

## Flight Information

Each flight contains:

- Flight ID
- State
- Date
- Time
- Passenger list

Possible flight states include:

- Arrival
- Departure
- Emergency
- Landed
- Departed
- Cancelled

## Passenger Information

Each passenger contains:

- Name
- Passport number
- Flight ID

Passengers are linked to their corresponding flight using a linked list.

## Main Operations

The application provides a menu with the following options:

1. Load files
2. Print flights
3. Print flight details
4. Add a new flight
5. Land a flight
6. Depart a flight
7. Cancel a flight
8. Display all flight statuses
9. Manage passengers
10. Exit

## Emergency Handling

Emergency flights have priority over normal arrival flights.

When the user chooses to land a flight, the system first checks the emergency
queue. If an emergency flight exists, it is landed before any normal arrival.

## Passenger Management

The system allows the user to:

- Print passengers of a specific flight
- Add a passenger to a flight
- Remove a passenger from a flight

Passenger modification is prevented for flights that have already been landed,
departed, or cancelled.

## Input Files

The current implementation reads:

```text
flight.txt
passenger.txt
