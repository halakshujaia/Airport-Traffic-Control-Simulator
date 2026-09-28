/*Name: Hala Khalil
  ID: 1231019
  Sec: 1 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// passenger data(Name,passportnumber,flightID)
typedef struct Passenger
{
    char name[50];
    char passport[20];
    char flightID[10];
    struct Passenger *next;
} Passenger;

// flight data(flight	ID;	state;	date;	time)
typedef struct Flight
{
    char flightID[20];
    char state[20];
    char date[20];
    char time[20];
    Passenger *passengers;
    struct Flight *next;
} Flight;

// simple queue (front, rear)
typedef struct
{
    Flight *front;
    Flight *rear;
} Queue;

Queue arrivalQ = {0,0};     // front=null , rear=null
Queue departureQ = {0,0};
Queue emergencyQ = {0,0};
Queue landedQ = {0,0};// op5
Queue departedQ = {0,0};// op6
Queue cancelledQ = {0,0};//op7

Queue *allQueues[6] = {&arrivalQ,&departureQ,&emergencyQ,&landedQ,&departedQ,&cancelledQ};

// add flight to queue
void enqueueFlight(Queue *q, Flight *f)
{
    f->next = NULL;//Because the trip we are adding will be the last one in the queue

    if (q->rear == NULL)
        q->front = f;//Meaning this is the first trip
    else
        q->rear->next = f;

    q->rear = f;//update
}

// search all queues for flight id
Flight* findFlight(char *id)
{
    for (int i = 0; i < 6; i++)
    {
        Flight *cur = allQueues[i]->front;
        while (cur != NULL)
        {
            if (strcmp(cur->flightID, id) == 0)
                return cur;

            cur = cur->next;
        }
    }
    return NULL;
}

void resetQueue(Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}
void loadFiles()
{
    //Before uploading the files, we must ensure that the queues are empty
    resetQueue(&arrivalQ);
    resetQueue(&departureQ);
    resetQueue(&emergencyQ);
    resetQueue(&landedQ);
    resetQueue(&departedQ);
    resetQueue(&cancelledQ);

    char line[200];
    FILE *fl = fopen("flight.txt", "r"); //r:read
    if (fl==NULL)
    {
        printf("flight.txt not found\n");
        return;
    }

    while (fgets(line, sizeof(line), fl))
    {
        Flight *f = malloc(sizeof(Flight));//We need space for a new trip
        f->passengers = NULL;//passengers = NULL because there are no passengers yet
        f->next = NULL;//next = NULL because it will be added as the last element in the queue
//Splitting the line into parts
        char *id = strtok(line, ";");
        char *st = strtok(NULL, ";");
        char *dt = strtok(NULL, ";");
        char *tm = strtok(NULL, ";");

        if (!id || !st || !dt || !tm)
        {
            printf("Invalid data format. Please check the file.\n");
            fclose(fl);
            return;
        }

        strcpy(f->flightID, id);
        strcpy(f->state, st);
        strcpy(f->date, dt);
        strcpy(f->time, tm);
// fgets adds a newline at the end; we remove it here.
// Only 'tm' needs trimming since it's the last field and the only one ending with '\n'.
        size_t len = strlen(f->time);
        if (len > 0 && f->time[len - 1] == '\n')
            f->time[len - 1] = '\0';

        if (strcmp(f->state, "Arrival") == 0)
            enqueueFlight(&arrivalQ, f);
        else if (strcmp(f->state, "Departure") == 0)
            enqueueFlight(&departureQ, f);
        else if (strcmp(f->state, "Emergency") == 0)
            enqueueFlight(&emergencyQ, f);
        else
        {
            printf("Invalid data format. Please check the file.\n");
            fclose(fl);
            return;
        }
    }

    fclose(fl);//Closing the flight file

// Loading the passengers file
    FILE *ps = fopen("passenger.txt", "r");
    if (!ps)
    {
        printf("passenger.txt not found\n");
        return;
    }

    while (fgets(line, sizeof(line), ps))
    {
        //Trim trailing '\n' to avoid breaking flightID comparison.
        char *n = strchr(line, '\n');
        if (n) *n = '\0';

        Passenger *p = malloc(sizeof(Passenger));

        char *nm = strtok(line, ";");
        char *pp = strtok(NULL, ";");
        char *fid = strtok(NULL, ";");

        if (!nm || !pp || !fid)
        {
            printf("Invalid data format. Please check the file.\n");
            fclose(ps);
            return;
        }

        strcpy(p->name, nm);
        strcpy(p->passport, pp);
        strcpy(p->flightID, fid);

        Flight *f = findFlight(p->flightID);// Linking the passenger to their flight
        if (f)
        {
            p->next = f->passengers;
            f->passengers = p;//insertion at head O(1)
        }
        else
        {
            free(p);
        }
    }

    fclose(ps);
    printf("Files loaded successfully.\n");
}


// print all flights
void printFlights()
{
    Queue *q[3] = { &arrivalQ, &departureQ, &emergencyQ };
    char *titles[3] = { "Arrival Flights", "Departure Flights", "Emergency Flights" };// Use the same loop and print each section title before its Flights.

    for (int i = 0; i < 3; i++)
    {
        printf("\n%s:\n", titles[i]);

        Flight *currentFlight = q[i]->front;
        while (currentFlight)
        {
            printf("%s - %s - %s - %s\n",
                   currentFlight->flightID, currentFlight->state, currentFlight->date, currentFlight->time);

            currentFlight = currentFlight->next;
        }
    }
}

void printFlightDetail()
{
    char id[20];
    printf("Enter flight ID: ");
    scanf("%s", id);

    Flight *f = findFlight(id);

    if (f == NULL)
    {
        printf("Flight not found\n");
        return;
    }

    // print flight informations
    printf("\nFlight Details:\n");
    printf("ID: %s\n", f->flightID);
    printf("State: %s\n", f->state);
    printf("Date: %s\n", f->date);
    printf("Time: %s\n", f->time);

    // print passengers
    printf("\nPassengers:\n");
    Passenger *p = f->passengers;

    if (p == NULL)
    {
        printf("No passengers\n");
        return;
    }

    while (p != NULL)
    {
        printf("%s - %s\n", p->name, p->passport);
        p = p->next;
    }
}
int isDigit(char c)
{
    if(c>='0' && c<='9')
        return 1;
    return 0;
}
//Flight ID should contains a num or small or capital letter
int isAlNum(char c)
{
    if(c>='0' && c<='9')
        return 1;
    if(c>='A' && c<='Z')
        return 1;
    if(c>='a' && c<='z')
        return 1;
    return 0;
}

int checkID(char id[])
{
    int i;
    if(strlen(id)<3)
        return 0;
    for(i=0; i<strlen(id); i++)
    {
        if(!isAlNum(id[i]))
            return 0;
    }
    if (strlen(id) > 10)
    {
        return 0;
    }

    return 1;
}

int checkState(char s[])
{
    if(strcmp(s,"Arrival")==0)
        return 1;
    if(strcmp(s,"Departure")==0)
        return 1;
    if(strcmp(s,"Emergency")==0)
        return 1;
    return 0;
}

int checkDate(char d[])
{
    int i;
    if(strlen(d)!=10)
        return 0;
    if(d[2]!='-'||d[5]!='-')
        return 0;
    int day = (d[0]-'0')*10 + (d[1]-'0');
    int month = (d[3]-'0')*10 + (d[4]-'0');
    int year = atoi(&d[6]);
    if(month < 1 || month > 12)
        return 0;
    if(day < 1 || day > 31)
        return 0;

    for(i=0; i<10; i++)
    {
        if(i==2||i==5)
            continue;
        if(!isDigit(d[i]))
            return 0;
    }
    return 1;
}

int checkTime(char t[])
{
    if(strlen(t)!=5)
        return 0;//22:20
    if(t[2]!=':')
        return 0;
    if(!isDigit(t[0])||!isDigit(t[1])||!isDigit(t[3])||!isDigit(t[4]))
        return 0;
    int h=(t[0]-'0')*10+(t[1]-'0');
    int m=(t[3]-'0')*10+(t[4]-'0');
    if(h<0||h>23)
        return 0;
    if(m<0||m>59)
        return 0;
    return 1;
}
void addNewFlight()
{
    Flight *f = malloc(sizeof(Flight));
    f->passengers = NULL;
    f->next = NULL;

    printf("Enter Flight ID: ");
    scanf("%s", f->flightID);

    if(!checkID(f->flightID))
    {
        printf("Invalid ID\n");
        free(f);
        return;
    }

    if(findFlight(f->flightID))
    {
        printf("Flight already exists\n");
        free(f);
        return;
    }

    printf("Enter State: ");
    scanf("%s", f->state);

    if(!checkState(f->state))
    {
        printf("Invalid State\n");
        free(f);
        return;
    }

    printf("Enter Date (dd-mm-yyyy): ");
    scanf("%s", f->date);

    if(!checkDate(f->date))
    {
        printf("Invalid Date\n");
        free(f);
        return;
    }

    printf("Enter Time (hh:mm): ");
    scanf("%s", f->time);

    if(!checkTime(f->time))
    {
        printf("Invalid Time\n");
        free(f);
        return;
    }

    if(strcmp(f->state,"Emergency")==0)
    {
        enqueueFlight(&emergencyQ, f);
        printf("Emergency flight added\n");
    }
    else if(strcmp(f->state,"Arrival")==0)
    {
        enqueueFlight(&arrivalQ, f);
        printf("Arrival flight added\n");
    }
    else
    {
        enqueueFlight(&departureQ, f);
        printf("Departure flight added\n");
    }
}

int compareDateTime(Flight *f1, Flight *f2)
{
    int d1, m1, y1, h1, min1;
    int d2, m2, y2, h2, min2;

    sscanf(f1->date, "%d-%d-%d", &d1, &m1, &y1);
    sscanf(f2->date, "%d-%d-%d", &d2, &m2, &y2);

    sscanf(f1->time, "%d:%d", &h1, &min1);
    sscanf(f2->time, "%d:%d", &h2, &min2);
    if (y1 < y2)
        return 1;
    if (y1 > y2)
        return -1;
    if (m1 < m2)
        return 1;
    if (m1 > m2)
        return -1;
    if (d1 < d2)
        return 1;
    if (d1 > d2)
        return -1;
    if (h1 < h2)
        return 1;
    if (h1 > h2)
        return -1;
    if (min1 < min2)
        return 1;
    if (min1 > min2)
        return -1;
    return 0;
}
void landFlight()
{
    Flight *f = NULL;//pointer

//Priority of emergency aircraft
    if (emergencyQ.front != NULL)
    {
        f = emergencyQ.front;
        emergencyQ.front = f->next;
        if (!emergencyQ.front)
            emergencyQ.rear = NULL;
    }
    else if (arrivalQ.front)
    {
        Flight *cur = arrivalQ.front;
        Flight *prev = NULL;
        Flight *oldest = cur;
        Flight *oldPrev = NULL;// the old is the first

        while (cur)
        {
            if (compareDateTime(cur, oldest) == 1)
            {
                oldest = cur;
                oldPrev = prev;
            }
            prev = cur;
            cur = cur->next;
        }

        if (oldPrev==NULL)//first elment
            arrivalQ.front = oldest->next;
        else
            oldPrev->next = oldest->next;

        if (arrivalQ.rear == oldest)
            arrivalQ.rear = oldPrev;

        f = oldest;
    }
    else
    {
        printf("No flights available to land\n");
        return;
    }

    char oldState[20];
    strcpy(oldState, f->state);

    strcpy(f->state, "Landed");
    f->next = NULL;
    enqueueFlight(&landedQ, f);

    printf("%s (%s : %s) at %s %s\n",f->flightID, oldState, f->state, f->date, f->time);
}
void departFlight()
{
    if (departureQ.front == NULL)      // no flights in departure queue
    {
        printf("No departure flights available\n");
        return;
    }

    // pointers to scan queue & find oldest flight
    Flight *cur = departureQ.front;
    Flight *prev = NULL;
    Flight *oldest = cur;
    Flight *oldPrev = NULL;

    // find the earliest flight by date/time
    while (cur != NULL)
    {
        if (compareDateTime(cur, oldest) == 1)
        {
            oldest = cur;      // update oldest flight
            oldPrev = prev;    // track the previous node
        }
        prev = cur;
        cur = cur->next;
    }

    // remove oldest from departure queue
    if (oldPrev == NULL)
        departureQ.front = oldest->next;     // oldest was first node
    else
        oldPrev->next = oldest->next;        // remove from middle

    if (oldest == departureQ.rear)
        departureQ.rear = oldPrev;           // update rear if needed

    // store old state before changing it
    char oldState[20];
    strcpy(oldState, oldest->state);
    // update flight state and move it to departed queue
    strcpy(oldest->state, "Departed");
    oldest->next = NULL;
    enqueueFlight(&departedQ, oldest);
    printf("%s (%s : %s) at %s %s\n",
           oldest->flightID, oldState, oldest->state,
           oldest->date, oldest->time);
}
void cancelFlight()
{
    char id[20];
    printf("Enter flight ID to cancel: ");
    scanf(" %s", id);

    Flight *f = findFlight(id);

    if (f == NULL)
    {
        printf("Flight not found\n");
        return;
    }

    if (strcmp(f->state, "Landed") == 0 ||strcmp(f->state, "Departed") == 0 ||strcmp(f->state, "Cancelled") == 0)
    {
        printf("Operation not allowed on this flight\n");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        Queue *q = allQueues[i];
        Flight *cur = q->front;
        Flight *prev = NULL;

        while (cur != NULL)
        {
            if (cur == f)
            {
                if (prev == NULL)
                    q->front = cur->next;
                else
                    prev->next = cur->next;

                if (cur == q->rear)
                    q->rear = prev;

                strcpy(cur->state, "Cancelled");
                cur->next = NULL;
                enqueueFlight(&cancelledQ, cur);

                printf("%s Cancelled\n", cur->flightID);
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    printf("Flight not found\n");
}

void printOneStatus(char *title, Queue q)
{
    printf("\n--- %s ---\n", title);
//Make pointer p point to the first flight in the queue.
    Flight *p = q.front;
    while (p!=NULL)
    {
        // To calcoluate passenger count
        int count = 0;
        Passenger *x = p->passengers;

        while (x!=NULL)
        {
            count++;
            x = x->next;
        }

        printf("%s - %s - %s - %s  | Passengers: %d\n",p->flightID, p->state, p->date, p->time, count);

        p = p->next;
    }
}

void displayStatus()
{
    printOneStatus("Arrival Flights",    arrivalQ);
    printOneStatus("Departure Flights",  departureQ);
    printOneStatus("Emergency Flights",  emergencyQ);
    printOneStatus("Departed Flights",   departedQ);
    printOneStatus("Cancelled Flights",  cancelledQ);
    printOneStatus("Landed Flights",     landedQ);
}

// opp 9 functions
void printPassengers()
{
    char id[20];

    printf("Enter flight ID: ");
    scanf("%s", id);

    if (checkID(id) == 0)
    {
        printf("Invalid ID\n");
        return;
    }

    // find the flight (f will be NULL if not found)
    Flight *f = NULL;

    // serch in alll lists
    Queue *q[6] = { &arrivalQ, &departureQ, &emergencyQ, &landedQ, &departedQ, &cancelledQ };

    for (int i = 0; i < 6 && !f; i++)
    {
        Flight *curr = q[i]->front;
        while (curr && !f)
        {
            if (strcmp(curr->flightID, id) == 0)
                f = curr;

            curr = curr->next;
        }
    }

    if (f == NULL)
    {
        printf("Flight not found\n");
        return;
    }

    Passenger *p = f->passengers;

    if (p == NULL)
    {
        printf("No passengers\n");
        return;
    }

    while (p != NULL)
    {
        printf("%s - %s\n", p->name, p->passport);
        p = p->next;
    }
}
void removePassenger()
{
    char id[20], passport[20];

    printf("Enter flight ID: ");
    scanf("%s", id);

    if (checkID(id) == 0)
    {
        printf("Invalid ID\n");
        return;
    }

    Flight *f = findFlight(id);
    if (f == NULL)
    {
        printf("Flight not found\n");
        return;
    }

    if (strcmp(f->state, "Landed") == 0 ||strcmp(f->state, "Departed") == 0 ||strcmp(f->state, "Cancelled") == 0)
    {
        printf("Operation not allowed on this flight\n");
        return;
    }

    if (f->passengers == NULL)
    {
        printf("No passengers\n");
        return;
    }

    printf("Enter passport number to remove: ");
    scanf("%s", passport);

    Passenger *p = f->passengers;
    Passenger *prev = NULL;

    while (p != NULL)
    {
        if (strcmp(p->passport, passport) == 0)
        {
            if (prev == NULL)
                f->passengers = p->next;
            else
                prev->next = p->next;

            free(p);
            printf("Passenger removed\n");
            return;
        }

        prev = p;
        p = p->next;
    }

    printf("Passenger not found\n");
}
void addPassenger()
{
    char id[20], name[50], passport[20];

    printf("Enter flight ID: ");
    scanf("%s", id);

    if (checkID(id) == 0)   // invalid ID format
    {
        printf("Invalid ID\n");
        return;
    }

    Flight *f = findFlight(id);
    if (f == NULL)          // flight not found in any queue
    {
        printf("Flight not found\n");
        return;
    }

    if (strcmp(f->state, "Landed") == 0 ||strcmp(f->state, "Departed") == 0 ||strcmp(f->state, "Cancelled") == 0)   // cannot modify these states
    {
        printf("Operation not allowed on this flight\n");
        return;
    }

    printf("Enter passenger name: ");
    scanf("%s", name);

    printf("Enter passport number: ");
    scanf("%s", passport);

    Passenger *p = f->passengers;

    while (p != NULL)
    {
        if (strcmp(p->passport, passport) == 0)   // passport already exists
        {
            printf("Passenger already exists\n");
            return;
        }
        p = p->next;
    }

    Passenger *newP = malloc(sizeof(Passenger));   // allocate new passenger
    strcpy(newP->name, name);
    strcpy(newP->passport, passport);
    strcpy(newP->flightID, id);

    newP->next = f->passengers;   // insert at head of list
    f->passengers = newP;

    printf("Passenger added\n");
}

void managePassengers()
{
    int op;

    do
    {
        printf("\n--- Manage Passengers ---\n");
        printf("1. Print passengers of a flight\n");
        printf("2. Add passenger to a flight\n");
        printf("3. Remove passenger from a flight\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &op);

        switch(op)
        {
        case 1:
            printPassengers();
            break;

        case 2:
            addPassenger();
            break;

        case 3:
            removePassenger();
            break;

        case 4:
            return;

        default:
            printf("Invalid choice\n");
        }

    }
    while (op!=4);
}
void freeQueue(Queue *q)
{
    Flight *f = q->front;

    while (f != NULL)
    {
        Passenger *p = f->passengers;

        while (p != NULL)
        {
            Passenger *nextP = p->next;
            free(p);          // free passenger node
            p = nextP;
        }

        Flight *nextF = f->next;
        free(f);              // free flight node
        f = nextF;
    }

    q->front = NULL;
    q->rear = NULL;
}

void freeAll()
{
    freeQueue(&arrivalQ);
    freeQueue(&departureQ);
    freeQueue(&emergencyQ);
    freeQueue(&landedQ);
    freeQueue(&departedQ);
    freeQueue(&cancelledQ);
}

int main()
{
    int choice;

    do
    {
        printf("\n*** Airport Traffic Control Menu ***\n");
        printf("1. Load files\n");
        printf("2. Print flights\n");
        printf("3. Print flight detail\n");
        printf("4. Add new flight\n");
        printf("5. Land a flight\n");
        printf("6. Depart a flight\n");
        printf("7. Cancel a flight\n");
        printf("8. Display all status\n");
        printf("9. Manage passengers\n");
        printf("10. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            loadFiles();
            break;
        case 2:
            printFlights();
            break;
        case 3:
            printFlightDetail();
            break;
        case 4:
            addNewFlight();
            break;
        case 5:
            landFlight();
            break;
        case 6:
            departFlight();
            break;
        case 7:
            cancelFlight();
            break;
        case 8:
            displayStatus();
            break;
        case 9:
            managePassengers();
            break;
        case 10:
            freeAll();
            printf("\n \n Exiting the System. GoodBye ^-^\n");
            break;
        default:
            printf("Invalid choice. Please choose between 1 and 10! :< ");
        }

    }
    while(choice != 10);

    return 0;
}
