#include <unistd.h> //low level I/o , (open,read,write,close,lseek)
#include <stdio.h> //standed I/O , (printf, perror)
#include <stdlib.h>  // high level I/o ,(exit)
#include <errno.h> //storeerror number (errno)
#include <fcntl.h> //Permisssions (0644)
#include <string.h> // controll string io
#include <stdbool.h>  //use for get bool data type
#include <sys/shm.h> //create shared memory
#include <sys/ipc.h> //inter process communication

typedef struct {
char student_index[20]; //EG/XXXX/XXXX
float assgnmt01_marks; //15%
float assgnmt02_marks; //15%
float project_marks; //20%
float finalExam_marks; //50%
}student_marks;

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

//globale constant
#define listSize 100
int arraySize;

//global variable
student_marks studentList[listSize];

//init functions
int readData();

int main(){
    //create shared memory
    key_t ky = ftok("myFile",78);
    if (ky == -1){
        perror("ftok error: ");
        printf("Error No: %d\n",errno);
        exit(1);
    }

    int SMID = shmget(ky,4096,IPC_CREAT|0666);
    if (SMID == -1){
        perror("shmget error: ");
        printf("Error No: %d\n",errno);
        exit(1);
    }
    //create parelal process
    arraySize = readData();
    printf("\x1b["BACKGROUND_COL_BLUE "m");
    printf(" there are %d records in here ",arraySize);
    printf("\x1b["GEN_FORMAT_RESET"m\n");
    pid_t PID = fork();
    if (PID == -1){
        perror("C1 fork error: ");
        printf("Error No: %d\n",errno);
        exit(0);
    }else if(PID == 0){
        // "C!" chield
        pid_t PID1 = fork();
        pid_t PID2 = fork();
        pid_t PID3 = fork();
        if(PID1 == -1 | PID2 == -1 | PID3 == -1){
            perror("CC fork error: ");
            printf("Error No: %d\n",errno);
            exit(0);
        }else if(PID1 == 0){
            //"CC1" chield

        }else if (PID2 == 0){
            //"CCI2" chield

        }else if(PID3 == 0){
            //"CCI3" chield

        }else{
            //"C1" chield
        }

    }else{
        //"P" parent process
    }
    return 0;
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