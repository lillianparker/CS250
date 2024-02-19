#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//create building structure (lol)
struct Building {
    char name[63];
    float sqFeet;
    float kwh;
    float efficacy;
    struct Building* next;
};

 int compareBuildings(const struct Building* buildingA, const struct Building* buildingB) {
    if (buildingA->efficacy < buildingB->efficacy){
        return -1;
    } else if (buildingA->efficacy > buildingB->efficacy) {
        return 1;
    } else {
        if (strcmp(buildingB->name, buildingA->name) > 0) {
            return 1;
        }
            else {
                return -1;
            }
    }
 }

int main (int argc, char *argv[]) {
    FILE *fp;
    char str[63];

    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        perror("BUILDING FILE EMPTY");
        return EXIT_FAILURE;
    }
    //file opened and ready to read
    const char * donestr = "DONE";
    // comparable string to check if file is done being read
    struct Building* head = NULL;
    struct Building* current = NULL;
    char tempname[63] = {0};
    fscanf(fp, "%s", tempname);
    //temporary string compared against 'done' string
    //while the line is not equal to 'done'
    if(strcmp(tempname, donestr) == 0){
        printf("BUILDING FILE IS EMPTY");
        fclose(fp);
        return EXIT_SUCCESS;
    }

    while(strcmp(tempname, donestr) != 0) {
        struct Building* new_building = (struct Building*)malloc(sizeof(struct Building));
        if (new_building == NULL) {
            perror("Memory allocation failed");
            fclose(fp);
            return 1;
        }
        strcpy(new_building->name, tempname);
        fscanf(fp, "%f", &new_building->sqFeet);      
        fscanf(fp, "%f", &new_building->kwh); 
        if (new_building->kwh == 0.0) {
            new_building->efficacy = 0.0;
        }  else if (new_building->sqFeet == 0.0) {
            new_building->efficacy = 0.0;
        } else {
            new_building->efficacy = new_building->kwh / new_building->sqFeet;
            new_building->next = NULL;
        } 
        //if this is first building structure
        if(head == NULL) {
            head = new_building;
            current = new_building;
        } else {
            struct Building** curr = &head;
            while(*curr != NULL && compareBuildings(new_building, *curr) < 0) {
            curr = &((*curr)->next);
        }   
        new_building->next = *curr;
        *curr = new_building;
        }
        //need to reset the building name each time
        fscanf(fp, "%s", tempname);
    }
    //the following prints the output and sets current = head
    current = head;
    while (current!= NULL) {
        printf("%s %f\n", current->name, current->efficacy);
        current = current->next;
    } 
    //the following closes the file because we are done reading
    fclose(fp);
    current = head;
    //the following frees the memory
    while(current != NULL) {
        struct Building* temp = current->next;
        free(current);
        current = temp;     
    }
    return EXIT_SUCCESS;
}