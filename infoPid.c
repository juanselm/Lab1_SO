/* infoPid.c */
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* Función genérica para extraer valores de líneas específicas */
void extraer_valor(const char* linea, const char* prefijo, const char* formato, void* destino) {
    if (strncmp(linea, prefijo, strlen(prefijo)) == 0) {
        sscanf(linea + strlen(prefijo), formato, destino);
    }
}

/* Función para obtener información del proceso */
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
        extraer_valor(line, "Name:", "%63s", info->name);
        extraer_valor(line, "State:", "%63s", info->state);
        extraer_valor(line, "VmSize:", "%ld", &info->vmSize);
        extraer_valor(line, "VmExe:", "%ld", &info->vmExe);
        extraer_valor(line, "VmData:", "%ld", &info->vmData);
        extraer_valor(line, "VmStk:", "%ld", &info->vmStk);
        extraer_valor(line, "voluntary_ctxt_switches:", "%ld", &info->voluntary);
        extraer_valor(line, "nonvoluntary_ctxt_switches:", "%ld", &info->nonvoluntary);
    }
    fclose(f);
    return 0;
}

/* Función para imprimir información de un proceso */
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

/* Función para imprimir información de múltiples procesos */
void print_multiple_pids(int num_pids, char* pids[]) {
    printf("-- Información recolectada!!!\n");
    int has_valid_pid = 0;

    for (int i = 0; i < num_pids; ++i) {
        if (!es_pid_valido(pids[i])) {
            printf("Pid: %s\n", pids[i]);
            printf("Error: El PID no es válido o no existe.\n\n");
            continue;
        }

        has_valid_pid = 1; // Indicar que al menos un PID es válido
        ProcInfo info;
        if (get_proc_info(pids[i], &info) == 0) {
            print_proc_info(&info);
        } else {
            printf("Pid: %s\n", pids[i]);
            printf("Error: No se pudo leer información del proceso.\n\n");
        }
    }

    if (!has_valid_pid) {
        printf("No se encontró información válida para los PIDs proporcionados.\n");
    }
}