#include <unistd.h> //low level I/o , (open,read,write,close,lseek)
#include <stdio.h> //standed I/O , (printf, perror)
#include <stdlib.h>  // high level I/o ,(exit)
#include <errno.h> //storeerror number (errno)
#include <fcntl.h> //Permisssions (0644)
#include <string.h> // controll string io
#include <stdbool.h> //use for get bool data type


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
void dataView();
void menuSelector();
bool isCorrectFormat();
void printDataRecord(student_marks record);
bool saveData ();
int getDataRecord();
void dataListInput();
bool isCorrecrRegNum( char reg[]);

int main(){
    //read data and load in to array
    arraySize = readData();
    
    // go to menu inside
    if(arraySize <= 100){
        printf("\x1b["BACKGROUND_COL_GREEN"m");
        printf(" there are %d records in here ",arraySize);
        printf("\x1b["GEN_FORMAT_RESET"m\n");
        menuSelector();
    }else{
        printf("\x1b["BACKGROUND_COL_YELLOW"m");
        printf(" your Student recordbook is Full, there are %d records ",arraySize);
        printf("\x1b["GEN_FORMAT_RESET"m\n");
    }
    return 0;
}

bool isCorrecrRegNum(char reg[]){
    int errorCount = 0;
    if(reg[0] != 69 | reg[1] != 71 | reg[2] != 47 | reg[7] != 47 ){
        errorCount +=1;
    }
    for(int i =0; i<4;i++){
        if(reg[i+3] < 48 | reg[i+3] > 57){
            errorCount +=1;
        }
        if(reg[i+8] < 48 | reg[i+8] > 57){
            errorCount +=1;
        }
    }
    if(errorCount == 0){
        return true;
    }else{
        return false;
    }    
}

void greeting(){
    char menu[] = "---------------use below guided codes--------------\ncode \tDiscription \n01\tcreate a new student 100 record\n02\tcreate a new student a data record\n03\tUpdate a student data record\n04\tDelete a student data record\n05\tstudent data record list\n---------------------------------------------------\n";
    printf("%s",menu);
}

int readData(){
    student_marks tempStudentList, LastStudent;
    int fd,size;
    fd = open("studentData.txt", O_RDONLY,0644);
    lseek(fd,-sizeof(LastStudent),SEEK_END);
    int count = read(fd,&LastStudent, sizeof(LastStudent));
    if (count < 0){
        printf("Error Number: %d\n",errno);
        perror("Read Error: ");
        exit(1);
    }
    lseek(fd,0,SEEK_SET);
    for(int i = 0; i>=0;i++){
        int count2 = read(fd,&tempStudentList, sizeof(tempStudentList));
        if (count2 < 0){
            printf("Error Number: %d\n",errno);
            perror("Read Error: ");
            exit(1);
        }
        studentList[i] = tempStudentList;
        if(strcmp(tempStudentList.student_index,LastStudent.student_index) == 0){
            return i+1;
            break;
        }
    }  
    close(fd);
}

void dataView(){
    printf("%d\n",arraySize);
    for(int i = 0;i<arraySize;i++){
       printf(" Saved data : %d\t%s\t%f\t%f\t%f\t%f\n",i+1,studentList[i].student_index,studentList[i].assgnmt01_marks,studentList[i].assgnmt02_marks,studentList[i].project_marks,studentList[i].finalExam_marks); 
    }
}

void menuSelector(){
    arraySize = readData();
    greeting();
    printf("Select Menu id : ");
    scanf("%d",&menuId);
    switch(menuId){
        case 0:
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" You can use number only for input menu id ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        case 1:
            arraySize = 0;
                printf("\x1b["BACKGROUND_COL_RED "m");
                printf(" warning! Existing data records will deleted automatically when you are start input 100 records. ");
                printf("\x1b["GEN_FORMAT_RESET"m\n"); 
            dataListInput();
            break;
        case 2:
            if(arraySize>=100){
                printf("\x1b["BACKGROUND_COL_RED"m");
                printf(" You can't store more than 100 records. Please delete 1 record if you need input 1 record ");
                printf("\x1b[" GEN_FORMAT_RESET "m\n");
                exit(1);
            }
            dataInput();
            break;
        case 3:
            dataEdit();
            break;
        case 4:
            dataDelete();
            break;
        case 5:
            dataView();
            break;
        default:
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" Please input corect menu id ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            menuSelector();
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

void printDataRecord(student_marks record){
    printf("\x1b["BACKGROUND_COL_YELLOW"m");
    printf("-------------------%s-------------------",record.student_index);
    printf("\x1b[" GEN_FORMAT_RESET "m\n");
    printf(" marks for Assigment 1 : %.2f \n",record.assgnmt01_marks);
    printf(" marks for Assigment 2 : %.2f \n",record.assgnmt02_marks);
    printf(" marks for Project     : %.2f \n",record.project_marks);
    printf(" marks for Final Exam  : %.2f \n",record.finalExam_marks);
    printf("\x1b["FOREGROUND_COL_YELLOW "m");
    printf("-------------------Saved data-------------------");
    printf("\x1b[" GEN_FORMAT_RESET "m\n");
}

bool saveData (){
    int fd;
    fd = open("studentData.txt", O_RDWR | O_CREAT | O_TRUNC,0644);
    for(int j = 0; j < arraySize; j++){
        int errorWrite = write(fd,&studentList[j],sizeof(student_marks)); 
        if(errorWrite == -1){
            perror("Wrting error : ");
            printf("Error No: %d ",errno);
            exit(1);
        }
    }
    close(fd);
}

void dataInput(){
    //create new student data record
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(!isCorrecrRegNum(regNumber)){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" Please enter correct register number ");
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        exit(1);
    }
    //check duplication reg. numbers
    if(isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is already exist, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataInput();
        exit(0);
    }
    //get marks details
    student_marks newRecord;
    strcpy(newRecord.student_index,regNumber);
    printf("Enter the Assigment 1 marks (15 max): ");
    int error1 = scanf("%f",&newRecord.assgnmt01_marks);
        if(!(error1 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.assgnmt01_marks > 15 | newRecord.assgnmt01_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 15 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the Assigment 2 marks (15 max): ");
    int error2 = scanf("%f",&newRecord.assgnmt02_marks);
        if(!(error2 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.assgnmt02_marks > 15 | newRecord.assgnmt02_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 15 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the Project marks (20 max)    : ");
    int error3 = scanf("%f",&newRecord.project_marks);
        if(!(error3 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.project_marks > 20 | newRecord.project_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 20 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the final exam marks (50 max) : ");
    int error4 = scanf("%f",&newRecord.finalExam_marks);
        if(!(error4 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.finalExam_marks > 50 | newRecord.finalExam_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 50 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    //print saved data
    if(arraySize < 100){
        studentList[arraySize] = newRecord;
        arraySize +=1;
        saveData();
        printDataRecord(newRecord);
    }else{
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" Your array is Full ");
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
    }
}

int getDataRecord(){
    for(int i = 0 ; i < arraySize ; i++){
        if(strcmp(studentList[i].student_index,regNumber) == 0){
            return i;
            break;
        }
    }
}

void dataEdit(){
    int dataId;  
    //Check stuident id
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(!isCorrecrRegNum(regNumber)){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" Please enter correct register number ");
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        exit(1);
    }
    if(!isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is not existing, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataEdit();
        exit(1);
    }else{
        dataId = getDataRecord();
    }
    //view old details
    printDataRecord(studentList[dataId]);
    //collect new data 
    student_marks newRecord;
    strcpy(newRecord.student_index,regNumber);
    printf("Enter the Assigment 1 marks (15 max): ");
    int error1 = scanf("%f",&newRecord.assgnmt01_marks);
        if(!(error1 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.assgnmt01_marks > 15 | newRecord.assgnmt01_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 15 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the Assigment 2 marks (15 max): ");
    int error2 = scanf("%f",&newRecord.assgnmt02_marks);
        if(!(error2 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.assgnmt02_marks > 15 | newRecord.assgnmt02_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 15 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the Project marks (20 max)    : ");
    int error3 = scanf("%f",&newRecord.project_marks);
        if(!(error3 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.project_marks > 20 | newRecord.project_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 20 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    printf("Enter the final exam marks (50 max) : ");
    int error4 = scanf("%f",&newRecord.finalExam_marks);
        if(!(error4 > 0 )){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only integer values for input marks ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0);
        }
        if(newRecord.finalExam_marks > 50 | newRecord.finalExam_marks < 0){
            printf("\x1b["BACKGROUND_COL_RED"m");
            printf(" you can use only numbers between 0 to 50 ");
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
            exit(0); 
        }
    //replace data record
    studentList[dataId] = newRecord;
    saveData();
    printDataRecord(newRecord);
    exit(0);
}

void dataDelete(){
    int dataId;
    //create new student data record
    printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
    scanf("%s",regNumber);
    if(!isCorrecrRegNum(regNumber)){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" Please enter correct register number ");
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        exit(1);
    }
    if(!isExisting()){
        printf("\x1b["BACKGROUND_COL_RED"m");
        printf(" %s is not existing, please Enter new reg. Number ",regNumber);
        printf("\x1b[" GEN_FORMAT_RESET "m\n");
        dataDelete();
    }else{
        dataId = getDataRecord();
    }
    //view Selected dataset
    printDataRecord(studentList[dataId]);
    //conform msg
    char command[20];
    printf("\x1b["BACKGROUND_COL_RED"m");
    printf(" Do you need delete this data record ? (YES/NO) :");
    printf("\x1b[" GEN_FORMAT_RESET "m\n");
    scanf("%s",command);
        if(strcmp(command,"YES") == 0 | strcmp(command,"yes") == 0 ){
            arraySize -= 1;
            studentList[dataId] = studentList[arraySize];
            saveData();
            printf("\x1b["BACKGROUND_COL_BLUE "m");
            printf(" %s Data set  deleted   ",regNumber);
            printf("\x1b[" GEN_FORMAT_RESET "m\n");
        }
    exit(0);
}

void dataListInput(){
        student_marks newRecord;
        for(int i=0;i<100;i++){
            printf("------------------Student data record %d started-----------------\n",arraySize+1);
                printf("Enter the Register Number (Ex : EG/XXXX/XXXX) : ");
                scanf("%s",regNumber);
                if(!isCorrecrRegNum(regNumber)){
                    printf("\x1b["BACKGROUND_COL_RED"m");
                    printf(" Please enter correct register number ");
                    printf("\x1b[" GEN_FORMAT_RESET "m\n");
                    exit(1);
                }
                if(isExisting()){
                    printf("\x1b["BACKGROUND_COL_RED"m");
                    printf(" %s is already exist, please Enter new reg. Number ",regNumber);
                    printf("\x1b[" GEN_FORMAT_RESET "m\n");
                    dataListInput();
                    exit(0);
                }
                strcpy(newRecord.student_index,regNumber);
                printf("Enter the Assigment 1 marks (15 max): ");
                int error1 = scanf("%f",&newRecord.assgnmt01_marks);
                    if(!(error1 > 0 )){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only integer values for input marks ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0);
                    }
                    if(newRecord.assgnmt01_marks > 15 | newRecord.assgnmt01_marks < 0){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only numbers between 0 to 15 ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0); 
                    }
                
                printf("Enter the Assigment 2 marks (15 max): ");
                int error2 = scanf("%f",&newRecord.assgnmt02_marks);
                    if(!(error2 > 0 )){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only integer values for input marks ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0);
                    }
                    if(newRecord.assgnmt02_marks > 15 | newRecord.assgnmt02_marks < 0){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only numbers between 0 to 15 ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0); 
                    }
                printf("Enter the Project marks (20 max)    : ");
                int error3 = scanf("%f",&newRecord.project_marks);
                    if(!(error3 > 0 )){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only integer values for input marks ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0);
                    }
                    if(newRecord.project_marks > 20 | newRecord.project_marks < 0){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only numbers between 0 to 20 ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0); 
                    }
                printf("Enter the final exam marks (50 max) : ");
                int error4 = scanf("%f",&newRecord.finalExam_marks);
                    if(!(error4 > 0 )){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only integer values for input marks ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0);
                    }
                    if(newRecord.project_marks > 50 | newRecord.project_marks < 0){
                        printf("\x1b["BACKGROUND_COL_RED"m");
                        printf(" you can use only numbers between 0 to 50 ");
                        printf("\x1b[" GEN_FORMAT_RESET "m\n");
                        exit(0); 
                    }

            printf("--------------------Student data record %d End------------------\n",arraySize+1);
                    studentList[arraySize] = newRecord;
                    arraySize +=1;
        }
        saveData();
}