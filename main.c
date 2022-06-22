#include <stdio.h>
#include <string.h>

#define DIMENSION 5

int X, Y, F;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

int is_valid(int x, int y) {
    // If the toy robot not fall off the table during movement, then the placement of the toy robot is valid, return 1
    if (x >= 0 && y >= 0 && x < DIMENSION && y < DIMENSION) return 1;
    // Otherwise return 0  
    return 0; 
}

int main(int argc, char *argv[]) {
    // Set the initial value of the toy robot to (-1, -1), which is an invalid position, until it is changed by the first valid PLACE command 
    X = -1, Y = -1; 
    FILE *fp;
    // If there is no argument behind ./main, input can be read from standard input
    if (argc == 1) fp = stdin;  
    // Otherwise input can be read from a file, simply add filename behind ./main 
    else fp = fopen(argv[1], "r"); 
    // Stores each line of command 
    char buf[20];  
    // Read one line at a time until the end of the file or enter an EOF
    while (fgets(buf, sizeof buf, fp) != NULL) {  
        // Ignore commands before the first valid PLACE
        if (is_valid(X, Y) == 0 && buf[0] != 'P') continue; 
        // Read the PLACE command, if not PLACE command (spelling error), skip
        if (buf[0] == 'P') { 
            char command[7] = {};
            sscanf(buf, "%s", command);
            if (strcmp(command, "PLACE")) continue;  

            // If the PLACE command is used, check to see if the x, y ranges are out of bounds and the directions are spelled correctly
            int x, y;
            char f[6] = {};
            sscanf(buf, "%s%d,%d,%s", command, &x, &y, f); 
            if (is_valid(x, y) == 0) continue;
            if (strcmp(f, "NORTH") && strcmp(f, "SOUTH") && strcmp(f, "EAST") && strcmp(f, "WEST")) continue; 

            // If x and y are valid and the direction is spelled correctly, put the toy robot on the table in position x, y
            X = x, Y = y;
            if (strcmp(f, "NORTH") == 0) F = 0;
            else if (strcmp(f, "EAST") == 0) F = 1;
            else if (strcmp(f, "SOUTH") == 0) F = 2;
            else if (strcmp(f, "WEST") == 0) F = 3;
        } else if (buf[0] == 'M') {  
            // Read the MOVE command, if not MOVE command (spelling error), skip
            char command[7] = {};
            // Remove the last '\n'
            sscanf(buf, "%s", command); 
            if (strcmp(command, "MOVE")) continue;

            // If the MOVE command is valid, MOVE the toy robot one unit forward in the direction it is currently facing
            int t_x = X, t_y = Y;
            X += dx[F], Y += dy[F];
            // If the move goes out of bounds, restore
            if (is_valid(X, Y) == 0) X = t_x, Y = t_y; 
        } else if (buf[0] == 'L') {  
            // Read the LEFT command, if not LEFT command (spelling error), skip
            char command[7] = {};
            sscanf(buf, "%s", command);
            if (strcmp(command, "LEFT")) continue;
            // If the LEFT command is valid, rotate the robot 90 degrees to the left 
            F = (F + 4 - 1) % 4;
        } else if (buf[0] == 'R') {  
            // An R command might be RIGHT or REPORT
            char command[7] = {};
            sscanf(buf, "%s", command);
            // If the command is RIGHT, rotate the robot 90 degrees to the right 
            if (strcmp(command, "RIGHT") == 0) F = (F + 1) % 4;  
            // If the command is REPORT，announce the X,Y and F of the robot
            else if (strcmp(command, "REPORT") == 0) { 
                char f[6] = {};
                if (F == 0) strcpy(f, "NORTH");
                else if (F == 1) strcpy(f, "EAST");
                else if (F == 2) strcpy(f, "SOUTH");
                else if (F == 3) strcpy(f, "WEST");
                printf("Output: %d,%d,%s\n", X, Y, f);
            }
        }
    }
    fclose(fp);
    return 0;
}