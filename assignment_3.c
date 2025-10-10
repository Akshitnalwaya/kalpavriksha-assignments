#include <stdio.h>
#define TOTAL_SUBJECTS 3

typedef struct Student
{
    int rollNumber;
    char studentName[50];
    int studentMarks1;
    int studentMarks2;
    int studentMarks3;
} Student;

void inputStudentDetails(Student *currentStudent, Student students[], int currentIndex);
void displayStudentDetails(Student student);
int calculateTotalMarks(Student student);
float calculateAverageMarks(Student student);
char gradingSystem(float averageMarks);
void displayPerformance(float averageMarks);
void displayRollNumberList(int index, int numberOfStudents, Student students[]);


void inputStudentDetails(Student *currentStudent, Student students[], int currentIndex)
{
    int isDuplicate;
    do
    {
        isDuplicate = 0;
        printf("Enter Roll Number: ");
        scanf("%d", &currentStudent->rollNumber);
        for (int i = 0; i < currentIndex; i++)
        {
            if (students[i].rollNumber == currentStudent->rollNumber)
            {
                printf("Roll Number already exists Enter a unique roll number.\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    getchar(); 

    printf("Enter Student Name: ");
    fgets(currentStudent->studentName, sizeof(currentStudent->studentName), stdin);

    printf("Enter Student Marks (3 subjects): ");
    scanf("%d %d %d", &currentStudent->studentMarks1, &currentStudent->studentMarks2, &currentStudent->studentMarks3);
}


int calculateTotalMarks(Student student)
{
    return student.studentMarks1 + student.studentMarks2 + student.studentMarks3;
}

float calculateAverageMarks(Student student)
{
    return calculateTotalMarks(student)/(float)TOTAL_SUBJECTS;
}

char gradingSystem(float averageMarks)
{
    if (averageMarks >= 85)
        return 'A';
    else if (averageMarks >= 70)
        return 'B';
    else if (averageMarks >= 50)
        return 'C';
    else if (averageMarks >= 35)
        return 'D';
    else
        return 'F';
}

void displayPerformance(float averageMarks)
{
    if (averageMarks < 35)
    {
        return;
    }

    printf("Performance: ");
    if (averageMarks >= 85)
        printf("*****\n");
    else if (averageMarks >= 70)
        printf("****\n");
    else if (averageMarks >= 50)
        printf("***\n");
    else if (averageMarks >= 35)
        printf("**\n");
}

void displayStudentDetails(Student student)
{
    float averageMarks = calculateAverageMarks(student);
    int total = calculateTotalMarks(student);
    char grade = gradingSystem(averageMarks);

    printf("\n--------------\n");
    printf("Roll: %d\n", student.rollNumber);
    printf("Name: %s", student.studentName);
    printf("Total: %d\n", total);
    printf("Average: %.2f\n", averageMarks);
    printf("Grade: %c\n", grade);
    displayPerformance(averageMarks);
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
        inputStudentDetails(&students[i], students, i); 
    }

    printf("\nStudent Information \n");
    for (int i = 0; i < numberOfStudents; i++)
    {
        displayStudentDetails(students[i]);
    }


    printf("\nList of Roll Numbers ");
    displayRollNumberList(0, numberOfStudents, students);
    printf("\n");

    return 0;
}
