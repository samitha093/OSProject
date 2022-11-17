#include <unistd.h> //low level I/o , (open,read,write,close,lseek)
#include <stdio.h> //standed I/O , (printf, perror)
#include <stdlib.h>  // high level I/o ,(exit)
#include <errno.h> //storeerror number (errno)
#include <fcntl.h> //Permisssions (0644)
#include <string.h> // controll string io
#include <stdbool.h>  //use for get bool data type
#include <sys/shm.h> //create shared memory
#include <sys/ipc.h> //inter process communication
#include <sys/wait.h> // use waitpid()

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
int SMID;


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

    SMID = shmget(ky,4096,IPC_CREAT|0666);
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
    //Create chield C1
    pid_t PID = fork();
    if (PID == -1){
        perror("C1 fork error: ");
        printf("Error No: %d\n",errno);
        exit(0);
    }else if(PID == 0){
        // Creating child s --CC1 to CC3
        pid_t PID1 = fork();
        if (PID1 == -1){
            perror("CC1 fork error: ");
            printf("Error No: %d\n",errno);
            exit(0);
        }else if(PID1 == 0){
            printf("CC1 child Process started, PID is %d & PPID is %d\n",getpid(),getppid());
            //shared memory attached
            char * childPtr = (char *)shmat(SMID,NULL,SHM_R|SHM_W);
            if(childPtr == (void *)-1){
                perror("shmgat error : ");
                printf("ERROR NO : %d \n",errno);
                exit(1); 
            }
            //CC1 process
            printf("CC1 done \n");
            //share memory deatach
            if (shmdt(childPtr) == -1){
                perror("child shmdt error: ");
                printf("Error No: %d\n",errno);
                exit(0);
            }
        }else{
            pid_t PID2 = fork();
            if (PID2 == -1){
                perror("CC2 fork error: ");
                printf("Error No: %d\n",errno);
                exit(0);
            }else if(PID2 == 0){
                printf("CC2 child Process started, PID is %d & PPID is %d\n",getpid(),getppid());
                //shared memory attached
                char * childPtr = (char *)shmat(SMID,NULL,SHM_R|SHM_W);
                if(childPtr == (void *)-1){
                    perror("shmgat error : ");
                    printf("ERROR NO : %d \n",errno);
                    exit(1); 
                }
                //CC2 process
                sleep(1);
                printf("CC2 done \n");
                //share memory deatach
                if (shmdt(childPtr) == -1){
                    perror("child shmdt error: ");
                    printf("Error No: %d\n",errno);
                    exit(0);
                }
            }else{
                pid_t PID3 = fork();
                if (PID3 == -1){
                    perror("CC3 fork error: ");
                    printf("Error No: %d\n",errno);
                    exit(0);
                }else if(PID3 == 0){
                    printf("CC3 child Process started, PID is %d & PPID is %d\n",getpid(),getppid());
                    //shared memory attached
                    char * childPtr = (char *)shmat(SMID,NULL,SHM_R|SHM_W);
                    if(childPtr == (void *)-1){
                        perror("shmgat error : ");
                        printf("ERROR NO : %d \n",errno);
                        exit(1); 
                    }
                    //CC3 process
                    printf("CC3 done \n");
                    //share memory deatach
                    if (shmdt(childPtr) == -1){
                        perror("child shmdt error: ");
                        printf("Error No: %d\n",errno);
                        exit(0);
                    }
                } else{
                    // child for P & Parent for CC1 to CC3
                    printf("C1 child Process started, PID is %d & PPID is %d\n",getpid(),getppid());
                    //shared memory attached
                    char * childPtr = (char *)shmat(SMID,NULL,SHM_R|SHM_W);
                    if(childPtr == (void *)-1){
                        perror("shmgat error : ");
                        printf("ERROR NO : %d \n",errno);
                        exit(1); 
                    }
                    //C1 process
                    printf("C1 done\n");
                    //share memory deatach
                    if (shmdt(childPtr) == -1){
                        perror("child shmdt error: ");
                        printf("Error No: %d\n",errno);
                        exit(0);
                    }
                }
            }
        }
    }else{
        printf("Parent Process started, PID is %d \n",getpid());
        //shared memory attached
        char * childPtr = (char *)shmat(SMID,NULL,SHM_R|SHM_W);
        if(childPtr == (void *)-1){
            perror("shmgat error : ");
            printf("ERROR NO : %d \n",errno);
            exit(1); 
        }
        //parent process
        //wait for end in chield process
        pid_t wpidRet = waitpid(PID,NULL,0);
        if (wpidRet == -1){
            perror("Parent waitpid error: ");
            printf("Error No: %d\n",errno);
            exit(0);
        }
        printf("Parent done \n");
        //share memory deatach
        if (shmdt(childPtr) == -1){
            perror("child shmdt error: ");
            printf("Error No: %d\n",errno);
            exit(0);
        }
        //delete shared memory
        if (shmctl(SMID,IPC_RMID,NULL) == -1){
            perror("parent shmctl error: ");
            printf("Error No: %d\n",errno);
            exit(0);
        }
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