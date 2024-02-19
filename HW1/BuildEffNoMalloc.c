#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// void insertSort(struct Building** head, struct Building* new_building) {
//     struct Building* current;
//     if(*head == NULL || new_building->efficacy > (*head)->efficacy)
// }
// void swap (struct Building** a, struct Building** b) {
//     struct Building* temp = *a;
//     struct Building* tempnext = (*a)->next;
//     *a = *b;
//     (*a)->next = (*b)->next; 
//     *b = temp; 
//     (*b)->next = tempnext;
// }

// void sortBuildings(struct Building** head) {
//     int switches;
//     struct Building** current;
//     if (*head == NULL) {
//         return;
//     }
//     switches = 0;
//     current = head;
//     while ((*current) ->next != NULL) {
//         // printf("%p\n", (*current)->next);
//         // printf("%p\n", (*current)->next->next);
//         struct Building* next = (*current)->next;
//         if(compareBuildings(*current, (*current)->next) > 0)
//         {
//             swap(current, &((*current)->next));
//             puts("swap");
//             switches = 1;
//         }
//         // printf("%p\n", (*current)->next);
//         // printf("%p\n", next->next);
//         // fflush(stdout);
//         current = &((*current)->next);
//     } 
// } 

   // sortBuildings(&head);
    // printf("help");
    // while (current!= NULL) {
    //     printf("%s %.1f\n", current->name, current->efficacy);
    //     current = current->next;
    // }

//create building structure (lol)
struct Building {
    char name[63];
    float sqFeet;
    float kwh;
    float efficacy;
};


 int compareBuildings(const struct Building* buildingA, const struct Building* buildingB) {
    if (buildingA->efficacy < buildingB->efficacy){
        return 1;
    } else if (buildingA->efficacy > buildingB->efficacy) {
        return -1;
    } else {
        return strcmp(buildingA->name, buildingB->name);
    }
 }

void sortBuildings (struct Building* buildings, size_t numBuildings) {
    for(size_t i = 0; i < numBuildings - 1; i++) {
        for(size_t j = 0; j <numBuildings - i - 1; j++) {
            if (compareBuildings(&buildings[j], &buildings[j + 1]) > 0){
                struct Building temp = buildings[j];
                buildings[j] = buildings[j+1];
                buildings[j +1] = temp;
            }
        }
    }
}
int main (int argc, char *argv[]) {
    FILE *fp;
    char str[63];
    char* filename = argv[1];
    fp = fopen(filename, "r");
    //file opened and ready to read
    size_t bufsize = 63;
    const char * donestr = "DONE";
    // comparable string to check if file is done being read
    struct Building buildings[1000];
 
    char tempname[63] = {0};
    fscanf(fp, "%s", tempname);
    //temporary string compared against 'done' string
    int index = 0;
    //while the line is not equal to 'done'
    if(strcmp(tempname, donestr) == 0){
        printf("BUILDING FILE IS EMPTY");
        return EXIT_SUCCESS;
    }

    while(strcmp(tempname, donestr) != 0) {
        strcpy(&buildings[index].name, tempname);
        fscanf(fp, "%f", &buildings[index].sqFeet);      
        fscanf(fp, "%f", &buildings[index].kwh);     
        buildings[index].efficacy = (buildings[index].kwh)/(buildings[index].sqFeet);
        //need to reset the building name each time
        memset(tempname, '\0', sizeof(tempname));
        fscanf(fp, "%s", tempname);
        index++;
    }

    printf("Sorted buildings:\n");
    sortBuildings(buildings, index);
    for (size_t i = 0; i < index; i++) {
        printf("%s %f\n", buildings[i].name, buildings[i].efficacy);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}