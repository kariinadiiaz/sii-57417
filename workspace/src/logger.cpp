// logger.cpp - Práctica 2
// Crea la FIFO, la lee y escribe por pantalla lo que manda el tenis.
// Se lanza ANTES que el tenis.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_LOGGER "/tmp/FIFO_logger"

int main(int argc, char **argv) {
    int fd;
    char buffer[200];
    int n;

    // borra la FIFO por si existía previamente
    unlink(FIFO_LOGGER);
    // crea la FIFO
    if (mkfifo(FIFO_LOGGER, 0600) < 0) {
        perror("No puede crearse la FIFO");
        return 1;
    }
    // abre la FIFO en lectura (se queda esperando hasta que arranca el tenis)
    if ((fd = open(FIFO_LOGGER, O_RDONLY)) < 0) {
        perror("No puede abrirse la FIFO");
        return 1;
    }
    // bucle de lectura: read devuelve 0 cuando el tenis cierra la FIFO
    while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        printf("%s\n", buffer);
        fflush(stdout);
    }
    // cierra y destruye la FIFO
    close(fd);
    unlink(FIFO_LOGGER);
    return 0;
}
