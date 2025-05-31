#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void getCPUUsage() {
    static long prevIdleTime = 0, prevTotalTime = 0;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        return;
    }

    fscanf(fp, "cpu  %ld %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    fclose(fp);

    long idleTime = idle + iowait;
    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    long diffIdle = idleTime - prevIdleTime;
    long diffTotal = totalTime - prevTotalTime;
    float cpuUsage = 100.0 * (1.0 - ((float)diffIdle / (float)diffTotal));

    prevIdleTime = idleTime;
    prevTotalTime = totalTime;

    printf("CPU Usage: %.2f%%\n", cpuUsage);
}

void getMemoryUsage() {
    FILE* fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    long memTotal = 0, memFree = 0;
    char label[64];
    while (fscanf(fp, "%s %ld kB\n", label, &memTotal) != EOF) {
        if (strcmp(label, "MemTotal:") == 0)
            break;
    }

    while (fscanf(fp, "%s %ld kB\n", label, &memFree) != EOF) {
        if (strcmp(label, "MemAvailable:") == 0)
            break;
    }

    fclose(fp);

    long used = memTotal - memFree;
    float usagePercent = 100.0 * used / memTotal;
    printf("Memory Usage: %.2f%%\n", usagePercent);
}

int main() {
    while (1) {
        getCPUUsage();
        getMemoryUsage();
        sleep(2);
    }
    return 0;
}
