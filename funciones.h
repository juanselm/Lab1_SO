/* funciones.h */
#ifndef FUNCIONES_H
#define FUNCIONES_H

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

void print_pid(char* argv[]);

// Nueva función para leer info de un PID en una estructura
int get_proc_info(const char* pid, ProcInfo* info);

// Nueva función para imprimir la info de un ProcInfo
void print_proc_info(const ProcInfo* info);

// Nueva función para manejar la opción -l
void print_multiple_pids(int count, char* pids[]);

#endif