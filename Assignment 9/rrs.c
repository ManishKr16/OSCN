#include <stdio.h>

#define MAX 50

int main() {
    int n = 5;
    int tq = 3;

    int at[5] = {4, 0, 6, 2, 1};
    int bt[5] = {7, 5, 3, 9, 4};

    int rem[5], ct[5], tat[5], wt[5], rt[5];
    int started[5] = {0}, inQueue[5] = {0};

    int queue[MAX];
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++)
        rem[i] = bt[i];

    int time = 0, completed = 0;

    // Find first arriving process
    int minAT = 999, first = -1;
    for (int i = 0; i < n; i++)
        if (at[i] < minAT)
            minAT = at[i], first = i;

    time = minAT;
    queue[rear++] = first;
    inQueue[first] = 1;

    while (completed < n) {

        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++)
                if (at[i] <= time && !inQueue[i]) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            continue;
        }

        int p = queue[front++];

        // First response
        if (!started[p]) {
            rt[p] = time - at[p];
            started[p] = 1;
        }

        // Execution
        if (rem[p] > tq) {
            rem[p] -= tq;
            time += tq;
        } else {
            time += rem[p];
            rem[p] = 0;
            ct[p] = time;
            completed++;
        }

        // Add newly arrived processes (before requeueing p)
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && !inQueue[i]) {
                queue[rear++] = i;
                inQueue[i] = 1;
            }
        }

        // If p still needs CPU -> put at end
        if (rem[p] > 0) {
            queue[rear++] = p;
        }
    }

    // Compute TAT and WT
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // Output
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            i+1, at[i], bt[i], ct[i], tat[i], wt[i], rt[i]);

    return 0;
}
