#include <stdio.h>
#define TOTAL_SUBJECTS 3
#define GRADE_A 85
#define GRADE_B 70
#define GRADE_C 50
#define GRADE_D 35
#define GRADE_F 35

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
int isValidMarks(int m1, int m2, int m3);


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

int isValidMarks(int m1, int m2, int m3)
{
    return (m1 >= 0 && m1 <= 100) &&
           (m2 >= 0 && m2 <= 100) &&
           (m3 >= 0 && m3 <= 100);
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

void displayPerformance(float averageMarks)
{
    if (averageMarks < GRADE_F)
    {
        return;
    }

    printf("Performance: ");
    if (averageMarks >= GRADE_A)
        printf("*****\n");
    else if (averageMarks >= GRADE_B)
        printf("****\n");
    else if (averageMarks >= GRADE_C)
        printf("***\n");
    else if (averageMarks >= GRADE_D)
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

    if (numberOfStudents < 1 || numberOfStudents > 100)
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
