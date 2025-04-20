/* infoPid.c */
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_pid(char* argv){
  
    /* Construir ruta al archivo /proc/[pid]/status */
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/status", argv);

    FILE *f = fopen(path, "r");
    if (!f) {
        perror("Error al abrir /proc/[pid]/status");
        return;
    }

    /* Variables para almacenar la información requerida */
    char line[256];
    char name[64] = "";
    char state[64] = "";
    long vmSize = 0, vmExe = 0, vmData = 0, vmStk = 0;
    long voluntary = 0, nonvoluntary = 0;

    /* Leer línea a línea y extraer campos específicos */
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line + 5, "%63s", name);
        } else if (strncmp(line, "State:", 6) == 0) {
            sscanf(line + 6, "%63s", state);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line + 7, "%ld", &vmSize);
        } else if (strncmp(line, "VmExe:", 6) == 0) {
            sscanf(line + 6, "%ld", &vmExe);
        } else if (strncmp(line, "VmData:", 7) == 0) {
            sscanf(line + 7, "%ld", &vmData);
        } else if (strncmp(line, "VmStk:", 6) == 0) {
            sscanf(line + 6, "%ld", &vmStk);
        } else if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
            sscanf(line + 24, "%ld", &voluntary);
        } else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
            sscanf(line + 27, "%ld", &nonvoluntary);
        }
    }
    fclose(f);

    /* Mostrar la información en pantalla */
    printf("Nombre del proceso: %s\n", name);
    printf("Estado: %s\n", state);
    printf("Tamaño total de la imagen de memoria: %ld KB\n", vmSize);
    printf("Tamaño de la memoria TEXT: %ld KB\n", vmExe);
    printf("Tamaño de la memoria DATA: %ld KB\n", vmData);
    printf("Tamaño de la memoria STACK: %ld KB\n", vmStk);
    printf("Número de cambios de contexto (voluntarios - no voluntarios): %ld - %ld\n",
           voluntary, nonvoluntary);
}