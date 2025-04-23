# Lab1_SO
Laboratorio 1 de la asignatura de Sistemas operativos y laboratios ingeniería de sistemas Udea

### Documento para el desarrollo del laboratorio
[Documento guía](https://docs.google.com/document/d/1rEcxJF22PGmmgD_FsE3m_aYGj9yiaK6nIYqVizMqr_Q/edit?tab=t.0#heading=h.69exl28wgcl9)

### Guía de compilación.

- Primero: compile el archivo usando make.

```
make
```

- Una vez el archivo está compilado ejecútalo usando:
```
./psinfo <PID>
```
```
./psinfo -l <PID> <PID> <PID>...
```
```
./psinfo -r <PID> <PID> <PID>...
```

### Uso de la opción -l

- Para listar información de múltiples procesos, use la opción `-l` seguida de los PIDs:
```
./psinfo -l <PID1> <PID2> <PID3>
```

* Ejemplo:
```
./psinfo -l 10898 1342 2341
```

### Uso de la opción -r

- Para listar información de múltiples procesos y guardarlos en un archivo, use la opción `-r` seguida de los PIDs:
```
./psinfo -r <PID1> <PID2> <PID3>
```

* Ejemplo:
```
./psinfo -r 10898 1342 2341
```

### Comandos adicionales.

- Para eliminar el ejecutable ./psinfo
```
make clean
```