#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>

#define listSize 90

typedef struct {
char student_index[20]; //EG/XXXX/XXXX
float assgnmt01_marks; //15%
float assgnmt02_marks; //15%
float project_marks; //20%
float finalExam_marks; //50%
}student_marks;

student_marks markList[listSize];
int regNumberList[listSize];


float numGenarator(int lower, int upper){
    int num1 = (rand() % (upper - lower + 1)) + lower;
    int num2 = (rand() % (99 - 1));
    float num = num1 + (float)num2/100;
    if(num > upper){
        num -= 1;
    }
    if(num < lower){
        num += 1;
    }
    return num;
}

int regNumberGen(int lower, int upper){
    int arrayLength =0;
    int num = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i<listSize; i++){
        if(regNumberList[i] == num){
           num = regNumberGen(lower,upper); 
        }
        if(regNumberList[i] == 0){
            arrayLength = i;
        }
    }
    regNumberList[arrayLength] = num;
    return num;
}

student_marks randomStudent(){
    int num = regNumberGen(3300,3600);
    char snum[100];
    sprintf(snum, "EG/2018/%d", num);
    student_marks tempStudent;
    strncpy(tempStudent.student_index,snum,12);
    tempStudent.assgnmt01_marks = numGenarator(0,15);
    tempStudent.assgnmt02_marks = numGenarator(0,15);
    tempStudent.project_marks = numGenarator(0,20);
    tempStudent.finalExam_marks = numGenarator(0,50);
    return tempStudent;
}

student_marks* studentList(){
    for(int i = 0; i<listSize; i++){
        student_marks student = randomStudent();
        markList[i] = student;
        printf("%d\t%s\t%f\t%f\t%f\t%f\n",i+1,student.student_index,student.assgnmt01_marks,student.assgnmt02_marks,student.project_marks,student.finalExam_marks);
    }
    return markList;
}

void main(){
    for(int a = 0;a<listSize;a++){
        regNumberList[a] = 0;
    }
    int fd;
    fd = open("studentData.txt", O_RDWR | O_CREAT | O_TRUNC,0644);
    student_marks* tempMarkList = studentList();
    for(int j = 0; j < listSize; j++){
        student_marks tempStudent = *(tempMarkList + j);
        int errorWrite = write(fd,&tempStudent,sizeof(tempStudent)); 
        if(errorWrite == -1){
            perror("Wrting error : ");
            printf("Error No: %d ",errno);
            exit(1);
        }
        printf("Saved : %s\n",(tempStudent.student_index));
    }
    close(fd);
    //read
    int fd2;
    fd2 = open("studentData.txt", O_RDONLY,0644);
    student_marks tempStudent2[100];
    lseek(fd2,0,SEEK_SET);
    read(fd2,&tempStudent2, sizeof(tempStudent2));
    for(int z = 0; z <listSize ; z++){
        printf(" Saved data : %d\t%s\t%f\t%f\t%f\t%f\n",z+1,tempStudent2[z].student_index,tempStudent2[z].assgnmt01_marks,tempStudent2[z].assgnmt02_marks,tempStudent2[z].project_marks,tempStudent2[z].finalExam_marks);
    }

    close(fd2);
}