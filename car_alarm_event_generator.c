#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_EVENT_DESCRIPTION_LENGTH 50

typedef struct SensorEventNode {
    struct SensorEventNode *next;
    char eventDescription[MAX_EVENT_DESCRIPTION_LENGTH];
    int hour, min, sec;
} SensorEventNode;

void generateRandomEvents(int interval);
void logError(const char *errorMessage);
int getBatteryStatus();
void checkBatteryStatus();
void insertEvent(SensorEventNode **head, const char *description, int hour, int min, int sec);

int main() {
    int interval;

    srand((unsigned int)time(NULL));

    char buffer[256]; // Assuming a reasonable buffer size

    printf("Enter the interval for random events (1 to 120 minutes): ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        logError("Error reading input");
        return 1;
    }

    if (sscanf(buffer, "%d", &interval) != 1 || interval <= 0 || interval > 120) {
        printf("Please enter a valid number from 1 to 120 minutes.\n");
        return 1;
    }

    printf("Event generation started\n");

    checkBatteryStatus(); // Check battery status only once

    generateRandomEvents(interval);

    return 0;
}

void insertEvent(SensorEventNode **head, const char *description, int hour, int min, int sec) {
    SensorEventNode *newEvent = malloc(sizeof(SensorEventNode));
    if (newEvent == NULL) {
        logError("Memory allocation failed for sensor event");
        exit(EXIT_FAILURE);
    }

    strncpy(newEvent->eventDescription, description, MAX_EVENT_DESCRIPTION_LENGTH - 1);
    newEvent->hour = hour;
    newEvent->min = min;
    newEvent->sec = sec;

    newEvent->next = *head;
    *head = newEvent;
}

void generateRandomEvents(int interval) {
    int numSensors = 10;
    SensorEventNode *eventList = NULL;

    FILE *file = fopen("events.txt", "a");
    if (file == NULL) {
        perror("Error opening file events.txt");
        logError("Error opening file events.txt");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "\nRandom Sensor Events:\n");

    for (int i = 0; i < numSensors; ++i) {
        int event = rand() % 6; // Adjusted to match the number of event cases

        time_t rawtime = time(NULL);
        struct tm *timeinfo = localtime(&rawtime);

        char eventDescription[MAX_EVENT_DESCRIPTION_LENGTH];

        switch (event) {
            case 0:
                strncpy(eventDescription, "Door sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            case 1:
                strncpy(eventDescription, "Window sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            case 2:
                strncpy(eventDescription, "Motion sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            case 3:
                strncpy(eventDescription, "Tyre sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            case 4:
                strncpy(eventDescription, "Battery sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            case 5:
                strncpy(eventDescription, "Hijack sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
                break;
            default:
                printf("Unexpected event type: %d\n", event);
                strncpy(eventDescription, "Unknown sensor activated", MAX_EVENT_DESCRIPTION_LENGTH - 1);
        }

        insertEvent(&eventList, eventDescription, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        fprintf(file, "[%02d:%02d:%02d] %s.\n", eventList->hour, eventList->min, eventList->sec, eventList->eventDescription);

        fflush(file); // Ensure that the data is written immediately

        sleep((unsigned int)interval * 60);  // Convert minutes to seconds
    }

    fclose(file);
}

void logError(const char *errorMessage) {
    char fullMessage[strlen(errorMessage) + 50];

    sprintf(fullMessage, "Error: %s\n", errorMessage);
    FILE *logFile = fopen("error_log.txt", "a");

    if (logFile != NULL) {
        fprintf(logFile, "%s", fullMessage);
        fclose(logFile);
    } else {
        fprintf(stderr, "Error opening log file: %s\n", fullMessage);
    }
}

int getBatteryStatus() {
    return rand() % 101; // Random battery level between 0 and 100
}

void checkBatteryStatus() {
    int batteryLevel = getBatteryStatus();
    printf("Current battery level: %d%%\n", batteryLevel);

    if (batteryLevel < 20) {
        printf("Warning: Battery level is low.\n");
        logError("Battery level low");
    }
}
