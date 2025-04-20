#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

void print_multiple_pids(int count, char *pids[]) {
    printf("-- Información recolectada!!!\n");
    for (int i = 0; i < count; i++) {
        printf("Pid: %s\n", pids[i]);
        print_pid(pids[i]); // Reutiliza la función existente para cada PID
        printf("\n");
    }
}