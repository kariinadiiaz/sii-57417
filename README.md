# Sistemas Informáticos Industriales — sii-57417

Karina Díaz Escalante · ETSIDI-UPM · Curso 2026-2027

Juego del tenis en C++ (OpenGL/GLUT) que se va ampliando en cada práctica.

## Compilar y ejecutar

```bash
cd workspace
mkdir -p build && cd build
cmake ..
make
```

## Práctica 1 — Entorno de desarrollo
- Movimiento de las raquetas (`Raqueta::Mueve`) y de la esfera (`Esfera::Mueve`).
- Controles: raqueta izquierda `w`/`s`, raqueta derecha `o`/`l`.
- Fichero `.gitignore` para no subir `workspace/build/`.

## Práctica 2 — Tuberías con nombre (FIFO)
- Nuevo programa `logger`: crea la FIFO `/tmp/FIFO_logger`, la lee e imprime cada punto.
- El tenis abre la FIFO en `Init()`, escribe en `OnTimer()` al marcar un punto y la cierra en el destructor.
- La partida termina cuando un jugador llega a 3 puntos (tenis y logger acaban).
- Orden de ejecución: `./logger` y después `./tenis`.

## Práctica 3 — Fichero proyectado en memoria
- Nuevo programa `bot` que mueve la raqueta 1 automáticamente.
- Comparten la clase `DatosMemCompartida` (esfera, raqueta1 y acción) mediante el fichero `/tmp/datos_bot` proyectado con `mmap`.
- Orden de ejecución: `./logger`, `./tenis` y después `./bot`.
