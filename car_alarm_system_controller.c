#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>


// Define a constant for the history file
#define HISTORY_FILE "history.txt"
#define MAX_EVENT_DESCRIPTION_LENGTH 50

// Define a structure to represent sensor events
typedef struct {
    char eventDescription[MAX_EVENT_DESCRIPTION_LENGTH];
    int hour, min, sec;
} SensorEvent;

// Function prototypes
void processEventsAndTakeActions();
void armSystem();
void disarmSystem();
void startEngine(int *engineState);
void stopEngine(int *engineState);
void openTrunk();
void closeTrunk();
void recordActionInHistory(const char *action);
void sendNotification(const char *message);
void logError(const char *errorMessage);
int getBatteryStatus();
void checkBatteryStatus();
void processEvent(FILE *outputFile, const SensorEvent *event);

int main() {
    int choice;
    int engineState = 0;  // 0: Engine off, 1: Engine on
    bool continueProgram = true;  // Boolean variable to control the loop

    srand(time(NULL));

    while (continueProgram) {
        // Display menu options
        printf("\nCar Alarm System Menu:\n");
        printf("1. Arm System\n");
        printf("2. Disarm System\n");
        printf("3. Start Engine\n");
        printf("4. Stop Engine\n");
        printf("5. Open Trunk\n");
        printf("6. Close Trunk\n");
        printf("7. System\n");
        printf("8. Exit\n");

        // Prompt for user input
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        // Process user choice
        switch (choice) {
            case 1:
                armSystem();
                recordActionInHistory("Armed the system");
                processEventsAndTakeActions();
                break;
            case 2:
                disarmSystem();
                recordActionInHistory("Disarmed the system");
                processEventsAndTakeActions();
                break;
            case 3:
                startEngine(&engineState);
                recordActionInHistory("Started the engine");
                processEventsAndTakeActions();
                break;
            case 4:
                stopEngine(&engineState);
                recordActionInHistory("Stopped the engine");
                processEventsAndTakeActions();
                break;
            case 5:
                openTrunk();
                recordActionInHistory("Opened the trunk");
                processEventsAndTakeActions();
                break;
            case 6:
                closeTrunk();
                recordActionInHistory("Closed the trunk");
                processEventsAndTakeActions();
                break;
            case 7:
                recordActionInHistory("Opened System menu");
                processEventsAndTakeActions();
                printf("A. Tyre pressure\n");
                printf("B. Fuel level\n");
                printf("C. Car diagnostics\n");
                printf("D. Engine tempreture\n");

                char subChoice;
                printf("Enter your sub-choice: ");
                scanf(" %c", &subChoice); 
                switch (subChoice) {
                    case 'A':
                        recordActionInHistory("Checked Tyre pressure");
                        processEventsAndTakeActions();
                        
                        printf("Tyre pressure: 32 PSI\n"); 
                        break;
                    case 'B':
                        recordActionInHistory("Checked Fuel level");
                        processEventsAndTakeActions();
                        printf("Fuel level: 57L\n"); 
                        break;
                    case 'C':
                        recordActionInHistory("Checked Car diagnostics");
                        processEventsAndTakeActions();
                        
                        printf("Car diagnostics: No errors found\n"); 
                    case 'D':
                        recordActionInHistory("Checked Engine temperature");
                        processEventsAndTakeActions();
                        printf("57Celsius\n"); 
                        break;
                    default:
                        printf("Invalid sub-choice. Please enter A, B, C, or D.\n");
                        continue; 
                }
                break;
            case 8:
    printf("Exiting the program.\n");
    continueProgram = false;  // Set the boolean variable to false to exit the loop
    break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    }

    return 0;
}


// Function to process events (placeholder)
void processEventsAndTakeActions() {
    // Placeholder for event processing
    printf("Processing events...\n");

    FILE *inputFile = fopen("events.txt", "r");
    FILE *outputFile = fopen("answers.txt", "a");

    if (inputFile == NULL) {
        perror("Error opening file events.txt");
        return;
    }

    if (outputFile == NULL) {
        perror("Error opening file answers.txt");
        fclose(inputFile);
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        if (strncmp(buffer, "[", 1) == 0) { // Check if the line starts with '[' (indicating an event line)
            SensorEvent event;

            if (sscanf(buffer, "[%d:%d:%d] %[^\n]", &event.hour, &event.min, &event.sec, event.eventDescription) == 4) {
                processEvent(outputFile, &event);
            } else {
                printf("Error reading event from file\n");
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
// Function to process individual events
void processEvent(FILE *outputFile, const SensorEvent *event) {
    // Clear the output file before writing the latest response
    freopen(NULL, "a", outputFile);

    if (strstr(event->eventDescription, "Door sensor activated")) {
        fprintf(outputFile, "Door sensor activated: Turning On Siren\n");
    } else if (strstr(event->eventDescription, "Window sensor activated")) {
        fprintf(outputFile, "Window sensor activated: Turning On Siren\n");
    } else if (strstr(event->eventDescription, "Motion sensor activated")) {
        fprintf(outputFile, "Motion sensor activated: Turning on Headlights, Sending Notification\n");
    } else if (strstr(event->eventDescription, "Tyre sensor activated")) {
        fprintf(outputFile, "Tyre sensor activated: Turning on Siren\n");
    } else if (strstr(event->eventDescription, "Battery sensor activated")) {
        fprintf(outputFile, "Sending Notification\n");
    } else if (strstr(event->eventDescription, "Hijack sensor activated")) {
        fprintf(outputFile, "Turning ON ABS System, Turning ON Siren\n");
    } else if (strstr(event->eventDescription, "Unknown sensor activated")) {
        fprintf(outputFile, "Turning on Siren\n");
    } else {
        fprintf(outputFile, "Unexpected event description\n");
    }
}



// Function to arm the system
void armSystem() {
    printf("System armed.\n");
}

// Function to disarm the system
void disarmSystem() {
    printf("System disarmed.\n");
}

// Define a mutex to synchronize access to the engine state
pthread_mutex_t engineMutex = PTHREAD_MUTEX_INITIALIZER;

// Function to start the engine with a delay and automatically stop it after 2 minutes
void* engineTimer(void *arg) {
    int *engineState = (int *)arg;

    sleep(120);

    pthread_mutex_lock(&engineMutex);
    if (*engineState == 1) {
        *engineState = 0;
        printf("Engine automatically stopped after 2 minutes.\n");

        // Send notification
        sendNotification("Engine automatically stopped after 2 minutes");

        // Record the action in history
        recordActionInHistory("Engine automatically stopped after 2 minutes");
    }
    pthread_mutex_unlock(&engineMutex);

    return NULL;
}

void startEngine(int *engineState) {
    if (engineState != NULL && *engineState == 0) {
        *engineState = 1;
        printf("Engine started.\n");

        // Create a thread for the engine timer
        pthread_t timerThread;
        if (pthread_create(&timerThread, NULL, engineTimer, (void *)engineState) != 0) {
            perror("Error creating timer thread");
            return;
        }

        // Detach the timer thread (no need to join)
        pthread_detach(timerThread);
    } else {
        printf("Engine is already running or invalid state.\n");
    }
}
// Function to stop the engine
void stopEngine(int *engineState) {
    if (engineState != NULL && *engineState == 1) {
        *engineState = 0;
        printf("Engine stopped.\n");
    } else {
        printf("Engine is already stopped or invalid state.\n");
    }
}


// Function to open the trunk
void openTrunk() {
    printf("Trunk opened.\n");
}

// Function to close the trunk
void closeTrunk() {
    printf("Trunk closed.\n");
}

// Function to record actions in history
void recordActionInHistory(const char *action) {
    // Opening history file in append mode
    FILE *historyFile = fopen(HISTORY_FILE, "a");

    if (historyFile == NULL) {
        logError("Error opening history file");
        return;
    }

    // Current time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the time and write the action to the file
    fprintf(historyFile, "[%02d:%02d:%02d] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, action);

    fclose(historyFile);
}

// Function to send notifications (placeholder)
void sendNotification(const char *message) {
    // Placeholder for sending notifications
    printf("Notification: %s\n", message);
}

// Function to log errors (placeholder)
void logError(const char *errorMessage) {
    // Placeholder for logging errors
    printf("Error: %s\n", errorMessage);
}

// Function to get battery status (placeholder)
int getBatteryStatus() {
    // Placeholder implementation
    return 1; // 1 for OK, 0 for Low
}

// Function to check battery status
void checkBatteryStatus() {
    int status = getBatteryStatus();
    if (status == 0) {
        logError("Battery status low");
        sendNotification("Battery status low");
    }
}

