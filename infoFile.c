#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "funciones.h"

void generar_reporte_archivo(int count, char *pids[]) {
    // Calcular el tamaño necesario para el nombre del archivo
    size_t len_total = strlen("psinfo-report") + 1;
    for (int i = 0; i < count; ++i) {
        len_total += strlen(pids[i]) + 1; // para '-' y el PID
    }
    len_total += strlen(".info") + 1;

    char *filename = malloc(len_total);
    if (!filename) {
        perror("Error al asignar memoria para el nombre del archivo");
        return;
    }

    strcpy(filename, "psinfo-report");
    for (int i = 0; i < count; ++i) {
        strcat(filename, "-");
        strcat(filename, pids[i]);
    }
    strcat(filename, ".info");

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se pudo crear el archivo de reporte");
        free(filename);
        return;
    }

    for (int i = 0; i < count; ++i) {
        fprintf(file, "Pid: %s\n", pids[i]);
        FILE *f;
        char path[64], line[256];
        snprintf(path, sizeof(path), "/proc/%s/status", pids[i]);
        f = fopen(path, "r");
        if (!f) {
            fprintf(file, "Error: No se pudo abrir /proc/%s/status\n\n", pids[i]);
            continue;
        }

        char name[64] = "", state[64] = "";
        long vmSize = 0, vmExe = 0, vmData = 0, vmStk = 0;
        long voluntary = 0, nonvoluntary = 0;

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

        fprintf(file, "Nombre del proceso: %s\n", name);
        fprintf(file, "Estado: %s\n", state);
        fprintf(file, "Tamaño total de la imagen de memoria: %ld KB\n", vmSize);
        fprintf(file, "Tamaño de la memoria TEXT: %ld KB\n", vmExe);
        fprintf(file, "Tamaño de la memoria DATA: %ld KB\n", vmData);
        fprintf(file, "Tamaño de la memoria STACK: %ld KB\n", vmStk);
        fprintf(file, "Número de cambios de contexto (voluntarios - no voluntarios): %ld - %ld\n\n",
                voluntary, nonvoluntary);
    }

    fclose(file);
    printf("Archivo de salida generado: %s\n", filename);
    free(filename);
}
