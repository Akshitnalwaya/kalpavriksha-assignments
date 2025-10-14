#include <stdio.h>
#include <string.h>
#define TOTAL_SUBJECTS 3
#define GRADE_A 85
#define GRADE_B 70
#define GRADE_C 50
#define GRADE_D 35
#define MAX_MARKS 100
#define MIN_MARKS 0

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
char calculateGrade(float averageMarks);
void displayPerformance(char grade);
void displayRollNumberList(int index, int numberOfStudents, Student students[]);
int isValidMarks(int marks1, int marks2, int marks3);

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
    currentStudent->studentName[strcspn(currentStudent->studentName, "\n")] = '\0';

    do
    {
        printf("Enter marks for 3 subjects (0-100): ");
        scanf("%d %d %d", &currentStudent->studentMarks1, &currentStudent->studentMarks2, &currentStudent->studentMarks3);

        if (!isValidMarks(currentStudent->studentMarks1, currentStudent->studentMarks2, currentStudent->studentMarks3))
        {
            printf("Invalid marks entered. Marks should be between 0 and 100.\n");
        }
    } while (!isValidMarks(currentStudent->studentMarks1, currentStudent->studentMarks2, currentStudent->studentMarks3));
}

int isValidMarks(int marks1, int marks2, int marks3)
{
    return (marks1 >= MIN_MARKS && marks1 <= MAX_MARKS) &&
           (marks2 >= MIN_MARKS && marks2 <= MAX_MARKS) &&
           (marks3 >= MIN_MARKS && marks3 <= MAX_MARKS);
}

int calculateTotalMarks(Student student)
{
    return student.studentMarks1 + student.studentMarks2 + student.studentMarks3;
}

float calculateAverageMarks(Student student)
{
    return calculateTotalMarks(student) / (float)TOTAL_SUBJECTS;
}

char calculateGrade(float averageMarks)
{
    if (averageMarks >= GRADE_A)
        return 'A';
    else if (averageMarks >= GRADE_B)
        return 'B';
    else if (averageMarks >= GRADE_C)
        return 'C';
    else if (averageMarks >= GRADE_D)
        return 'D';
    else
        return 'F';
}

void displayPerformance(char grade)
{
    printf("Performance: ");
    switch (grade)
    {
        case 'A':
            printf("*****\n");
            break;
        case 'B':
            printf("****\n");
            break;
        case 'C':
            printf("***\n");
            break;
        case 'D':
            printf("**\n");
            break;
        default:
            return;
    }
}

void displayStudentDetails(Student student)
{
    float averageMarks = calculateAverageMarks(student);
    int total = calculateTotalMarks(student);
    char grade = calculateGrade(averageMarks);

    printf("\n--------------\n");
    printf("Roll: %d\n", student.rollNumber);
    printf("Name: %s\n", student.studentName);
    printf("Total: %d\n", total);
    printf("Average: %.2f\n", averageMarks);
    printf("Grade: %c\n", grade);
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

    if (numberOfStudents < 1 || numberOfStudents > 100)
    {
        printf("Invalid number of students\n");
        return 1;
    }

    Student students[numberOfStudents];

    for (int i = 0; i < numberOfStudents; i++)
    {
        printf("\nEnter details for Student %d\n", i + 1);
        inputStudentDetails(&students[i], students, i);
    }

    printf("\nStudent Information\n");
    for (int i = 0; i < numberOfStudents; i++)
    {
        displayStudentDetails(students[i]);

        float avg = calculateAverageMarks(students[i]);
        char grade = calculateGrade(avg);

        if (grade == 'F')
        {
            continue;
        }

        displayPerformance(grade);
    }

    printf("\nList of Roll Numbers: ");
    displayRollNumberList(0, numberOfStudents, students);
    printf("\n");

    return 0;
}
