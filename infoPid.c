/* infoPid.c */
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void print_pid(char* argv[]) {
    /* Construir ruta al archivo /proc/[pid]/status */
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/status", argv);
    snprintf(path, sizeof(path), "/proc/%s/status", argv[0]); // Corregido índice argv[0]

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

int get_proc_info(const char* pid, ProcInfo* info) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Error: No se pudo abrir el archivo para el PID %s. Razón: %s\n", pid, strerror(errno));
        return -1;
    }

    char line[256];
    strcpy(info->pid, pid);
    info->name[0] = info->state[0] = 0;
    info->vmSize = info->vmExe = info->vmData = info->vmStk = 0;
    info->voluntary = info->nonvoluntary = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line + 5, "%63s", info->name);
        } else if (strncmp(line, "State:", 6) == 0) {
            sscanf(line + 6, "%63s", info->state);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line + 7, "%ld", &info->vmSize);
        } else if (strncmp(line, "VmExe:", 6) == 0) {
            sscanf(line + 6, "%ld", &info->vmExe);
        } else if (strncmp(line, "VmData:", 7) == 0) {
            sscanf(line + 7, "%ld", &info->vmData);
        } else if (strncmp(line, "VmStk:", 6) == 0) {
            sscanf(line + 6, "%ld", &info->vmStk);
        } else if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
            sscanf(line + 24, "%ld", &info->voluntary);
        } else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
            sscanf(line + 27, "%ld", &info->nonvoluntary);
        }
    }
    fclose(f);
    return 0;
}

void print_proc_info(const ProcInfo* info) {
    printf("Pid: %s\n", info->pid);
    printf("Nombre del proceso: %s\n", info->name);
    printf("Estado: %s\n", info->state);
    printf("Tamaño total de la imagen de memoria: %ld KB\n", info->vmSize);
    printf("Tamaño de la memoria TEXT: %ld KB\n", info->vmExe);
    printf("Tamaño de la memoria DATA: %ld KB\n", info->vmData);
    printf("Tamaño de la memoria STACK: %ld KB\n", info->vmStk);
    printf("Número de cambios de contexto (voluntarios - no voluntarios): %ld - %ld\n",
           info->voluntary, info->nonvoluntary);
    printf("\n");
}

void print_multiple_pids(int num_pids, char* pids[]) {
    printf("-- Información recolectada!!!\n");
    for (int i = 0; i < num_pids; ++i) {
        ProcInfo info;
        if (get_proc_info(pids[i], &info) == 0) {
            print_proc_info(&info);
        } else {
            printf("Pid: %s\n", pids[i]);
            printf("Error: No se pudo leer información del proceso.\n\n");
        }
    }
}