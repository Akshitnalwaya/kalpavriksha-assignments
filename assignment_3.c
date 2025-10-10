#include <stdio.h>

typedef struct Student
{
    int rollNumber;
    char studentName[50];
    int studentMarks1;
    int studentMarks2;
    int studentMarks3;
} Student;

void studentDetails(Student *studentPtr, Student students[], int currentIndex);
void displayOutput(Student student);
int totalMarks(Student student);
float averageMarks(Student student);
char gradingSystem(float avg);
void performance(float avg);
void displayRollNumberList(int index, int numberOfStudents, Student students[]);


void studentDetails(Student *studentPtr, Student students[], int currentIndex)
{
    int isDuplicate;
    do
    {
        isDuplicate = 0;
        printf("Enter Roll Number: ");
        scanf("%d", &studentPtr->rollNumber);
        for (int i = 0; i < currentIndex; i++)
        {
            if (students[i].rollNumber == studentPtr->rollNumber)
            {
                printf("Roll Number already exists Enter a unique roll number.\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    getchar(); 

    printf("Enter Student Name: ");
    fgets(studentPtr->studentName, sizeof(studentPtr->studentName), stdin);

    printf("Enter Student Marks (3 subjects): ");
    scanf("%d %d %d", &studentPtr->studentMarks1, &studentPtr->studentMarks2, &studentPtr->studentMarks3);
}


int totalMarks(Student student)
{
    return student.studentMarks1 + student.studentMarks2 + student.studentMarks3;
}

float averageMarks(Student student)
{
    return totalMarks(student) / 3.0;
}

char gradingSystem(float avg)
{
    if (avg >= 85)
        return 'A';
    else if (avg >= 70)
        return 'B';
    else if (avg >= 50)
        return 'C';
    else if (avg >= 35)
        return 'D';
    else
        return 'F';
}

void performance(float avg)
{
   if (avg < 35)
    {
        return;
    }

    printf("Performance: ");
    if (avg >= 85)
        printf("*****\n");
    else if (avg >= 70)
        printf("****\n");
    else if (avg >= 50)
        printf("***\n");
    else if (avg >= 35)
        printf("**\n");
}

void displayOutput(Student student)
{
    float avg = averageMarks(student);
    int total = totalMarks(student);
    char grade = gradingSystem(avg);

    printf("\n--------------\n");
    printf("Roll: %d\n", student.rollNumber);
    printf("Name: %s", student.studentName);
    printf("Total: %d\n", total);
    printf("Average: %.2f\n", avg);
    printf("Grade: %c\n", grade);
    performance(avg);
}


void displayRollNumberList(int index, int numberOfStudents, Student students[])
{
    if (index >= numberOfStudents)
        return;

    printf("%d ", students[index].rollNumber);
    displayRollNumberList(index + 1, numberOfStudents, students);
}

int main()
{
    int numberOfStudents;

    printf("Enter number of students: ");
    scanf("%d", &numberOfStudents);

    if (numberOfStudents < 1)
    {
        printf("Invalid number of students\n");
        return 1;
    }

    Student students[numberOfStudents];

    for (int i = 0; i < numberOfStudents; i++)
    {
        printf("\nEnter details for Student %d \n", i + 1);
        studentDetails(&students[i], students, i); 
    }

    printf("\nStudent Information \n");
    for (int i = 0; i < numberOfStudents; i++)
    {
        displayOutput(students[i]);
    }


    printf("\nList of Roll Numbers ");
    displayRollNumberList(0, numberOfStudents, students);
    printf("\n");

    return 0;
}
