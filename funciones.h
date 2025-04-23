/* funciones.h */
#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para access()

typedef struct {
    char pid[16];
    char name[64];
    char state[64];
    long vmSize;
    long vmExe;
    long vmData;
    long vmStk;
    long voluntary;
    long nonvoluntary;
} ProcInfo;

// Nueva función para leer info de un PID en una estructura
int get_proc_info(const char* pid, ProcInfo* info);

// Nueva función para imprimir la info de un ProcInfo
void print_proc_info(const ProcInfo* info);

// Nueva función para manejar la opción -l
void print_multiple_pids(int count, char* pids[]);

void generar_reporte_archivo(int count, char *pids[]);

// Función inline para validar si un PID es válido
static inline int es_pid_valido(const char *pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s", pid);
    return access(path, F_OK) == 0; // Verifica si el directorio /proc/<pid> existe
}

#endif