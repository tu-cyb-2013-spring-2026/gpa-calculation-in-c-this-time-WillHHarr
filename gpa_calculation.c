#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define InputLimit 20
#define InitialTermCapacity 10

typedef struct {
    char session[InputLimit];
    int year;
    double weightsTotal;
    int hoursTotal;
} TermEntry;

typedef struct {
    TermEntry *entries;
    size_t size;
    size_t capacity;
} EntryList;

EntryList termDict;

void initDict(EntryList *list, size_t initialCapacity){
    list -> entries = malloc(initialCapacity * sizeof(TermEntry));
    list -> size = 0;
    list -> capacity = initialCapacity;
}

void expandDict(EntryList *list){
    size_t newCapacity = list->capacity * 2;
    
    TermEntry *newEntries = realloc(list->entries, newCapacity * sizeof(TermEntry));
    if( !newEntries ){
        printf("Internal Allocation Errors, Critical Faliure");
        exit(1);
    }
    
    list->entries = newEntries;
    list->capacity = newCapacity;
}

TermEntry* getOrCreateTerm(const char *session, int year){
    for (int i = 0; i < termDict.size; i++){
        if(termDict.entries[i].year == year && strcmp(termDict.entries[i].session, session) == 0){
        return &termDict.entries[i];}
    }    
    if (termDict.size >= termDict.capacity){
        expandDict(&termDict);
    }
    
    
    strncpy(termDict.entries[termDict.size].session, session, 19);
    termDict.entries[termDict.size].session[19] = '\0';
    termDict.entries[termDict.size].year = year;
    termDict.entries[termDict.size].weightsTotal = 0.0;
    termDict.entries[termDict.size].hoursTotal = 0;
    return &termDict.entries[termDict.size++];
}   

double getGrade(char Grade){
    char grade = toupper(Grade);
    if(grade == 'A'){
    return 4.0; }
    if(grade == 'B'){
    return 3.0; }
    if(grade == 'C'){
    return 2.0; }
    if(grade == 'D'){
    return 1.0; }
    if(grade == 'F'){
    return 0.0; }
    
    return -1.0;
}

void capitalize(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int checkTerm(char *str) {
    if (strcmp(str, "FALL") == 0 || strcmp(str, "SPRING") == 0 || strcmp(str, "SUMMER") == 0){
        return 0;}
    return -1;
}   

int checkYear(int year){
    if(year >= 2000 && year <2100){ 
        return 0;}
    return -1;
}

int calculateInputs(char Letter, int Hours, TermEntry *term){
    
    double weight;
    
    weight = getGrade(Letter);
    
    if(weight == -1){
    printf("Invalid term grade, Please re-enter input\n");
    return -1;}
    
    if( term != NULL ) {
        term -> weightsTotal += weight * Hours;
        term -> hoursTotal += Hours;
    }   
    
    return 0;
}

void printTerm(TermEntry *term){
    if(term == NULL){
    printf("Error:NullTerm - Data Not Proccessed\n");
    return;
    }
    
    double gpa = (term -> hoursTotal > 0) ? 
                 (term -> weightsTotal / term -> hoursTotal) : 0.0;
    
    printf("Term: %s %d (%d Hours) GPA: %.2f\n", term -> session, term -> year, term -> hoursTotal, gpa);
}

void addTotals(TermEntry *term, double *totalWeight, int *totalHours){
    if(term == NULL){
    return;}
    
    *totalWeight += term -> weightsTotal;
    *totalHours += term -> hoursTotal;
    
    return;
}

int main()
{
    
    initDict(&termDict, (size_t)InitialTermCapacity);
    
    
    printf("Enter Grades in the following format: Letter Hour Term Year\n");
    printf("-------> To stop entering grades, please input a blank line\n");
    
    while(1){
        char input[InputLimit], letter, termSession[InputLimit];
        
        int termYear;
        
        int hours;
        
        TermEntry *selectedTerm;
        
        printf("Enter a letter grade and hours for a course with its year and semester: ");
        fgets(input, sizeof(input), stdin);
        
        if(strcmp(input, "\n") == 0){
        break; }
        
        if( sscanf(input, "%c %d %s %d", &letter, &hours, termSession, &termYear) == 4 ){
            
            capitalize(termSession);
            
            if( checkTerm(termSession) != 0 ){
                printf("Invalid Term, Please re-enter input\n");
                continue;
            }
            
            if( checkYear(termYear) != 0 ){
                printf("Invalid Year, Please re-enter input with a year post-2000 and prior-2100\n");
                continue;
            }
            
            selectedTerm = getOrCreateTerm(termSession, termYear);
            
            if(selectedTerm == NULL){
            continue;}
            
            if( calculateInputs(letter, hours, selectedTerm) == 0 ){
            continue;}
            
        } else {
            printf("Input Error, Please re-enter input\n");
        }
    }
    
    double weightsTotal = 0.0;
    int hoursTotal = 0;    
       
    printf("--------------------------------\n");
    printf("Unoffical Term Grades:\n");
     
    if(termDict.size > 0){
        for(int i = 0; i < termDict.size; i++){
            printTerm(&termDict.entries[i]);
            addTotals(&termDict.entries[i], &weightsTotal, &hoursTotal); 
        }
    } 
    
    printf("\n--\n");
    printf("Unoffical GPA\n"
           "Total GPA: %.2f\n"
           "Total Credit Hours: %d\n"
           "--------------------------------\n", 
           (weightsTotal/hoursTotal), hoursTotal);
    
    
    
    printf("Press Enter to exit...");
    getchar();
    
    return 0;
}
