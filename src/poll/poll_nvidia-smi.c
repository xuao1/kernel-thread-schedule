#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isGpuIdle(int deviceId) {
    char command[128];
    FILE *fp;
    char output[10240];

    sprintf(command, "nvidia-smi -i %d --query-gpu=utilization.gpu --format=csv,noheader,nounits", deviceId);

    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run command\n");
        return -1;
    }

    if (fgets(output, sizeof(output)-1, fp) != NULL) {
        int gpuUtilization = atoi(output);
        pclose(fp);
        printf("GPU Utilization is %d\n", gpuUtilization);
        return gpuUtilization == 0;
    } else {
        pclose(fp);
        return -1;
    }
}

int main() {
    int deviceId = 0;
    if (isGpuIdle(deviceId)) {
        printf("GPU %d is idle.\n", deviceId);
    } else {
        printf("GPU %d is busy.\n", deviceId);
    }
    return 0;
}
