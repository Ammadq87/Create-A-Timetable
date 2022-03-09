#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 100

typedef struct event{
    char eventName[100]; // x -> not valid, delete event
    int startTime;
    int endTime;
    char day;
} event;


event schedule[size];
int empty[size];
int dates[7]; // keeps track on how mayn events are there per day
int DATES[7] = {'U', 'M', 'T', 'W', 'R', 'F', 'S'};
int eventCounter = 0;
/*
Options: Add Event | View Calendar | Upload New Events

    Add Event:
        -> Properties:
            -> eventName
            -> startTime
            -> endTime
        -> Functions:
            -> Edit Certain Event
            -> Delete Certain Events
*/

int validTime(int start, int end){
    int s_hr = start/100;
    int e_hr = end/100;
    if ((0 <= start%100 && start%100 <= 60) && (0 <= end%100 && end%100 <= 60)){
        if ((0 <= e_hr && e_hr <= 24) && (0 <= s_hr && s_hr <= 24)){
            if (start > end)
                return 1;
            else if (start == end)
                return 2;
            else
                return 0;
        }
    } 

    return 3;
}

int collisions(event* e){
    for (int i=0; i<eventCounter; i++){
        if (e->day == schedule[i].day){
            if ((schedule[i].startTime <= e->startTime && e->startTime < schedule[i].endTime) ||  (schedule[i].startTime < e->endTime && e->endTime <= schedule[i].endTime)){
                return i; // false
            }
        }

        if (strcmp(e->eventName, schedule[i].eventName) == 0){
            return i;
        }
    }
    return -1;
}

void toString(event *e){
    printf("%s on %c @ [%d - %d]\n", e->eventName, e->day, e->startTime, e->endTime);
}

int validDay(char d){
    char days[7]  ={'U', 'M', 'T', 'W', 'R', 'F', 'S'};
    for (int i=0; i<7; i++){
        if (d == days[i]){
            dates[i]++;
            return 1;
        }
    }
    return 0;
}

event createEvent(){
    event e;
    event* ptr = &e; 

    char name[100];
    printf("\nEvent Title: ");
    scanf("%s", name);
    strcpy(e.eventName, name);

    char day;
    int start, end;
    printf("Enter day: ");
    scanf(" %c", &day);

    if (validDay(day) == 0){
        printf("Incorrect Day Entered");
        e.day = 'x';
        return e;
    }

    e.day = day;

    printf("Enter start-time: ");
    scanf(" %d", &start);
    printf("Enter end-time: ");
    scanf(" %d", &end);

    int valid = validTime(start, end);
    switch (valid){
        case 1: 
            printf("\n<-- Error: Event cannot start after it ends -->\n");
            return e;
            break;
        case 2:
            printf("\n<-- Error: Event period is too small -->\n");
            return e;
            break;
        case 3:
            printf("\n<-- Error: Invalid values for time entered -->\n");
        default:
            e.startTime = start;
            e.endTime = end;
    }

    toString(ptr);

    int i = collisions(ptr);
    if (i != -1){
        printf("%s @ [%d - %d] collides with %s [%d - %d] OR is a duplicate event\n ", e.eventName, e.startTime, e.endTime, schedule[i].eventName, schedule[i].startTime, schedule[i].endTime);
        e.day = 'x';
    } else {
        printf("%s @ [%d-%d] added\n", e.eventName, e.startTime, e.endTime);
    }


    return e;
}

void swap(event *a, event *b) {
  event temp = *a;
  *a = *b;
  *b = temp;
}

void selectionSort(event array[], int s) {
  for (int step = 0; step < s - 1; step++) {
    int min_idx = step;
    for (int i = step + 1; i < s; i++) {

      // To sort in descending order, change > to < in this line.
      // Select the minimum element in each loop.
      if (array[i].startTime < array[min_idx].startTime)
        min_idx = i;
    }

    // put min at the correct position
    swap(&array[min_idx], &array[step]);
  }
}

void printSchedule(){
    int j, cnt = 0;
    for (int k=0; k<7; k++){
        int length = dates[k];
        char day = DATES[k];
        printf("DAY: %c\n", day);
        event events[length];

        for (int i=0; i<eventCounter; i++){
            if (schedule[i].day == day){
                events[cnt] = schedule[i];
                cnt++;
            }
        }
        cnt = 0;
        
        selectionSort(events, length);
        for(int i=0; i<length; i++){
            event *ptr = &events[i];
            if ('A' <= events[i].eventName[0] && events[i].eventName[0] <= 'z'){
                toString(ptr);
            }
        }
        printf("------------\n");
    }
}

void addEvent(){
    for (int i=0; i<size; i++){
        if (empty[i] == 0){
            event e = createEvent();
            schedule[i] = e;
            empty[i] = 1;
            eventCounter++;
            break;
        }
    }
}

void deleteEvent(){
    if (eventCounter > 0){
        printf("Enter name of event:\n");
        char name[100];
        scanf("%s", name);
        int found = 0;
        for (int i=0; i<size; i++){
            if (strcmp(schedule[i].eventName, name) == 0){
                // event e;
                int time;
                char n[100];
                char d;
                strcpy(schedule[i].eventName, n);
                schedule[i].startTime = time;
                schedule[i].endTime = time;
                schedule[i].day = d;
                empty[i] = 0;
                found = 1;
                eventCounter--;
            } 
        }
        if (found == 0)
        printf("\n\'%s\' does not exist.\n", name);
        else
            printf("\nDeleting all instances of \'%s\'\n", name);
    } else {
        printf("Add events to delete events\n");
    }

    
}

void viewEvent(){
    // printf("Enter name of event:\n");
    // char name[100];
    // scanf("%s", name);
    // int found = 0;
    // for (int i=0; i<size; i++){
    //     if (strcmp(schedule[i].eventName, name) == 0){
    //         found = 1;
    //         toString(&schedule[i]);
    //     } 
    // }
    printSchedule();
}

int main() {

    event* ptr;
    ptr = (event*) malloc(size*sizeof(event));

    printf("[A]dd Event | [D]elete Event | [V]iew Event | [U]pload New Events | X (Quit) ");
    char option;
    scanf(" %c", &option);

    while (option != 'X'){
        switch (option){
            case 'A':
                addEvent();
                break;
            case 'D':
                deleteEvent();
                break;
            case 'V':
                viewEvent();
                break;
            default:
                break;
        }
        
        printf("[A]dd Event | [D]elete Event | [V]iew Event | [U]pload New Events | X (Quit) ");
        option;
        scanf(" %c", &option);

    }
   return 0;
}
