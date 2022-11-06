#include <unistd.h> //low level I/o , (open,read,write,close,lseek)
#include <stdio.h> //standed I/O , (printf, perror)
#include <stdlib.h>  // high level I/o ,(exit)
#include <errno.h> //storeerror number (errno)
#include <fcntl.h> //Permisssions (0644)
#include <string.h> // controll string io
#include <stdbool.h>  


typedef struct {
char student_index[20]; //EG/XXXX/XXXX
float assgnmt01_marks; //15%
float assgnmt02_marks; //15%
float project_marks; //20%
float finalExam_marks; //50%
}student_marks;

//globale constant
#define listSize 100

//global variable
student_marks studentList[listSize];
char regNumber[20];
int menuId;
int arraySize;

//General Formatting
#define GEN_FORMAT_RESET                "0"
#define GEN_FORMAT_BRIGHT               "1"
#define GEN_FORMAT_DIM                  "2"
#define GEN_FORMAT_UNDERSCORE           "3"
#define GEN_FORMAT_BLINK                "4"
#define GEN_FORMAT_REVERSE              "5"
#define GEN_FORMAT_HIDDEN               "6"

//Foreground Colors
#define FOREGROUND_COL_BLACK            "30"
#define FOREGROUND_COL_RED              "31"
#define FOREGROUND_COL_GREEN            "32"
#define FOREGROUND_COL_YELLOW           "33"
#define FOREGROUND_COL_BLUE             "34"
#define FOREGROUND_COL_MAGENTA          "35"
#define FOREGROUND_COL_CYAN             "36"
#define FOREGROUND_COL_WHITE            "37"

//Background Colors
#define BACKGROUND_COL_BLACK            "40"
#define BACKGROUND_COL_RED              "41"
#define BACKGROUND_COL_GREEN            "42"
#define BACKGROUND_COL_YELLOW           "43"
#define BACKGROUND_COL_BLUE             "44"
#define BACKGROUND_COL_MAGENTA          "45"
#define BACKGROUND_COL_CYAN             "46"
#define BACKGROUND_COL_WHITE            "47"

//init function list
int readData();
void greeting();
void dataInput();
void dataEdit();
void dataDelete();
void menuSelector();
bool isCorrectFormat();

int main(){
    //read data and load in to array
    arraySize = readData();
    
    // go to menu inside
    if(arraySize < 100){
        printf("\x1b["BACKGROUND_COL_GREEN"m");
        printf(" there are %d records in here ",arraySize);
        printf("\x1b["GEN_FORMAT_RESET"m\n");
        menuSelector();
    }else{
        printf("\x1b["BACKGROUND_COL_YELLOW"m");
        printf(" your Student recordbook is Full, there are %d records \n",arraySize);
        printf("\x1b["GEN_FORMAT_RESET"m\n");
    }
    return 0;
}

void greeting(){
    char menu[] = "---------------use below guided codes--------------\ncode \tDiscription \n01\tcreate a new student data record\n02\tUpdate a student data record\n03\tDelete a student data record\n---------------------------------------------------\n";
    printf("%s",menu);
}

int readData(){
    student_marks tempStudentList, LastStudent;
    int fd,size;
    fd = open("studentData.txt", O_RDONLY,0644);
    lseek(fd,-sizeof(LastStudent),SEEK_END);
    read(fd,&LastStudent, sizeof(LastStudent));//Error handle
    lseek(fd,0,SEEK_SET);
    for(int i = 0; i>=0;i++){
        read(fd,&tempStudentList, sizeof(tempStudentList));//Error handle
        //printf(" Saved data : %d\t%s\t%f\t%f\t%f\t%f\n",i+1,tempStudentList.student_index,tempStudentList.assgnmt01_marks,tempStudentList.assgnmt02_marks,tempStudentList.project_marks,tempStudentList.finalExam_marks);
        studentList[i] = tempStudentList;
        if(strcmp(tempStudentList.student_index,LastStudent.student_index) == 0){
            return i+1;
            break;
        }
    }  
    close(fd);
}

void menuSelector(){
    greeting();
    printf("Select Menu id : ");
    scanf("%d",&menuId);
    switch(menuId){
        case 1:
            dataInput();
            break;
        case 2:
            dataEdit();
            break;
        case 3:
            dataDelete();
            break;
        default:
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" Please input corect menu id ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            menuSelector();
            break;
    }
}

bool isExisting(){
    for(int i = 0; i < arraySize; i++){
        if(!strcmp(studentList[i].student_index,regNumber)){
            return true;
        } 
    }
    return false;
}

void dataInput(){
    //create new student data record
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is already exist, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataInput();
    }
}

void dataEdit(){
    //create new student data record
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(!isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is not existing, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataEdit();
    }
}

void dataDelete(){
    //create new student data record
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(!isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is not existing, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataDelete();
    }
}