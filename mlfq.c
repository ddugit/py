#include <stdio.h>

void mlfq(int n, int burst[]) {
    int i;
    int remaining[n];
    int completion[n], turnaround[n], waiting[n];

    for (i = 0; i < n; i++) {
        remaining[i] = burst[i];
    }

    int tq1 = 4, tq2 = 8, time = 0;

    // Queue 1: Round Robin with TQ = 4
    for (i = 0; i < n; i++) {
        if (remaining[i] > tq1) {
            time += tq1;
            remaining[i] -= tq1;
        } else if (remaining[i] > 0) {
            time += remaining[i];
            remaining[i] = 0;
            completion[i] = time;
        }
    }

    // Queue 2: Round Robin with TQ = 8
    for (i = 0; i < n; i++) {
        if (remaining[i] > tq2) {
            time += tq2;
            remaining[i] -= tq2;
        } else if (remaining[i] > 0) {
            time += remaining[i];
            remaining[i] = 0;
            completion[i] = time;
        }
    }

    // Queue 3: FCFS
    for (i = 0; i < n; i++) {
        if (remaining[i] > 0) {
            time += remaining[i];
            remaining[i] = 0;
            completion[i] = time;
        }
    }

    // Calculate Turnaround and Waiting Time
    for (i = 0; i < n; i++) {
        turnaround[i] = completion[i];
        waiting[i] = turnaround[i] - burst[i];
    }

    // Print Results
    printf("\nProcess\t Burst\t Completion\t Turnaround\t Waiting\n");
    for (i = 0; i < n; i++) {
        printf("P%d \t %d \t %d \t\t %d \t\t %d \n", i + 1, burst[i], completion[i], turnaround[i], waiting[i]);
    }
    printf("\nTotal CPU time: %d\n", time);
}

int main() {
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int burst[n];
    for (i = 0; i < n; i++) {
        printf("Enter burst time of P%d: ", i + 1);
        scanf("%d", &burst[i]);
    }

    mlfq(n, burst);

    return 0;
}
