#include <stdlib.h>
#include<stdio.h>

int main() {
    // Command to open an image file
    char command[100];
    snprintf(command, sizeof(command), "xdg-open upiscan.jpg");

    // Execute the command
    int result = system(command);

    // Check if the command execution was successful
    if (result == -1) {
        printf("Failed to execute the command.\n");
        return 1; // Return non-zero to indicate failure
    } else {
        printf("Image file opened successfully.\n");
        return 0; // Return zero to indicate success
    }
}
