#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tasks {
    int id;
    char taskName[50];
    char assignedTo[50];
} t;

struct employee {
    int id;
    char eName[50];
    char department[50];
    int eNumber;
    char taskName[50];
} e;

FILE *fp;

void addTask();
void viewTaskList();
void removeTask();
void assignTask();
void viewAssignedTaskList();
void completeTask();

int main() {
    int n;
    while (1) {
        system("cls");
        printf("<== Employee Work Management System ==>\n\n");
        printf("1. Add Task\n");
        printf("2. View Task List\n");
        printf("3. Remove Task\n");
        printf("4. Assign Task\n");
        printf("5. Assigned Task List\n");
        printf("6. Complete Task\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &n);

        switch (n) {
            case 0:
                exit(0);
            case 1:
                addTask();
                break;
            case 2:
                viewTaskList();
                break;
            case 3:
                removeTask();
                break;
            case 4:
                assignTask();
                break;
            case 5:
                viewAssignedTaskList();
                break;
            case 6:
                completeTask();
                break;
            default:
                printf("Invalid Choice!\n");
        }
        printf("Press any button to continue...");
        getch();
    }
    return 0;
}

void addTask() {
    fp = fopen("tasks.txt", "ab");
    printf("Enter Task ID: ");
    scanf("%d", &t.id);
    printf("Enter Task Name: ");
    fflush(stdin);
    gets(t.taskName);
    printf("Enter Assigned To (Employee Name): ");
    fflush(stdin);
    gets(t.assignedTo);
    printf("\nTask Added Successfully.\n\n");
    fwrite(&t, sizeof(t), 1, fp);
    fclose(fp);
}

void viewTaskList() {
    system("cls");
    printf("<== Available Tasks ==>\n\n");
    printf("%-10s %-30s %-20s\n", "Task ID", "Task Name", "Assigned To");
    fp = fopen("tasks.txt", "rb");
    while (fread(&t, sizeof(t), 1, fp) == 1) {
        printf("%-10d %-30s %-20s\n", t.id, t.taskName, t.assignedTo);
    }
    fclose(fp);
}

void removeTask() {
    int id, f = 0;
    system("cls");
    printf("<== Remove Task ==>\n");
    printf("Enter Task ID: ");
    scanf("%d", &id);
    FILE *ft;
    fp = fopen("tasks.txt", "rb");
    ft = fopen("tmp.txt", "wb");
    while (fread(&t, sizeof(t), 1, fp) == 1) {
        if (id == t.id) {
            f = 1;
        } else {
            fwrite(&t, sizeof(t), 1, ft);
        }
    }
    fclose(fp);
    fclose(ft);
    if (f == 1) {
        printf("\nTask Removed Successfully.\n");
    } else {
        printf("\nNo Task Found.\n");
    }
    remove("tasks.txt");
    rename("tmp.txt", "tasks.txt");
}

void assignTask() {
    int f = 0;
    system("cls");
    printf("\n<== Assign Task ==>\n");
    printf("Enter Task ID: ");
    scanf("%d", &e.id);
    fp = fopen("tasks.txt", "rb");
    while (fread(&t, sizeof(t), 1, fp) == 1) {
        if (t.id == e.id) {
            strncpy(e.taskName, t.taskName, sizeof(e.taskName) - 1);
            e.taskName[sizeof(e.taskName) - 1] = '\0';  // Ensure null-terminated string
            f = 1;
            break;
        }
    }
    fclose(fp);
    if (f == 0) {
        printf("\nNo Task Found With This ID.");
        printf("\nTry Again With Another ID.");
        return;
    }
    fp = fopen("assign.txt", "ab");
    printf("Enter Employee Name: ");
    fflush(stdin);
    fgets(e.eName, sizeof(e.eName), stdin);
    e.eName[strcspn(e.eName, "\n")] = '\0';  // Remove newline character
    printf("Enter Department: ");
    fflush(stdin);
    fgets(e.department, sizeof(e.department), stdin);
    e.department[strcspn(e.department, "\n")] = '\0';  // Remove newline character
    printf("Enter Employee Number: ");
    scanf("%d", &e.eNumber);
    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);
}

void viewAssignedTaskList() {
    system("cls");
    printf("\n<== Assigned Tasks ==>\n");
    printf("%-10s %-30s %-20s %-10s %-30s\n", "E.id", "Name", "Department", "Number", "Task Name");
    fp = fopen("assign.txt", "rb");
    while (fread(&e, sizeof(e), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s\n", e.id, e.eName, e.department, e.eNumber, e.taskName);
    }
    fclose(fp);
}

void completeTask() {
    int id, f = 0;
    system("cls");
    printf("<== Complete Task ==>\n");
    printf("Enter Task ID to mark as complete: ");
    scanf("%d", &id);

    fp = fopen("assign.txt", "rb");
    FILE *ft = fopen("tmp_assign.txt", "wb");

    while (fread(&e, sizeof(e), 1, fp) == 1) {
        if (id == e.id) {
            // Task found in assigned list, mark it as completed
            strncpy(e.taskName, "Completed", sizeof(e.taskName) - 1);
            e.taskName[sizeof(e.taskName) - 1] = '\0';
            f = 1;
        }
        fwrite(&e, sizeof(e), 1, ft);
    }

    fclose(fp);
    fclose(ft);

    if (f == 1) {
        printf("\nTask Marked as Completed Successfully.\n");
    } else {
        printf("\nNo Task Found in Assigned List.\n");
    }

    remove("assign.txt");
    rename("tmp_assign.txt", "assign.txt");
}
