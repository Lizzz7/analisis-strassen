# Análisis Empírico: Strassen vs. Multiplicación Clásica
---

## Descripción

Experimento controlado que contrasta empíricamente la afirmación de la *Lecture 3* del MIT (Demaine, 2005): *"Strassen's algorithm beats the
ordinary algorithm on today's machines for n ≥ 32 or so"*.

Se mide el tiempo de ejecución de ambos algoritmos para matrices cuadradas
de orden `n ∈ {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048}` y se
determina empíricamente el umbral **N₀** a partir del cual Strassen
supera al algoritmo estándar O(n³).

---

## Estructura del repositorio

```
analisis-strassen/
├── main.c          # Implementaciones: clásico e Strassen + medición
├── plot.py         # Graficación y estimación de N₀
├── Makefile        # Automatización: compilar, correr, graficar
├── data_grafico.png# Gráfico comparativo (generado)
└── README.md       # Este archivo
```

---

## Requisitos

| Herramienta | Versión mínima | Instalación (Ubuntu/Debian) |
|-------------|----------------|-----------------------------|
| GCC         | 9.0            | `sudo apt install gcc`      |
| Python      | 3.8            | `sudo apt install python3`  |
| NumPy       | 1.20           | `pip3 install numpy`        |
| Matplotlib  | 3.3            | `pip3 install matplotlib`   |
| Make        | 4.0            | `sudo apt install make`     |

---

## Reproducción del experimento

```bash
# 1. Clonar el repositorio
git clone https://github.com/Lizzz7/analisis-strassen.git
cd analisis-strassen

# 2. Compilar, ejecutar y graficar (todo en un paso)
make plot

# Alternativamente, paso a paso:
make          # solo compilar → genera ./experimento
make run      # compilar + correr → genera data.dat
make plot     # todo + graficar → genera data_grafico.png

# 3. Limpiar artefactos generados
make clean
```

La ejecución completa toma aproximadamente **5–10 minutos** dependiendo
del hardware (el caso n=2048 es el más costoso).

---

## Parámetros configurables

En `main.c`, al inicio del archivo:

```c
#define BASE_CASE 64   // umbral de recursión de Strassen (probar: 1, 32, 64, 128)
#define REPS      15   // repeticiones por tamaño (recomendado: 10-20)
```

> **Nota:** Cambiar `BASE_CASE` afecta significativamente el N₀ estimado.
> Con `BASE_CASE = 1` (sin umbral), el cruce aparece antes pero con mayor
> ruido. Con `BASE_CASE = 128`, el cruce se desplaza a n > 512.

---

## Resultados obtenidos

| n    | Clásico (µs) | Strassen (µs) | Ganador    |
|------|-------------|---------------|------------|
| 2    | 0           | 0             | —          |
| 4    | 0           | 0             | —          |
| 8    | 0           | 0             | —          |
| 16   | 5           | 4             | Strassen   |
| 32   | 16          | 16            | Empate     |
| 64   | 130         | 129           | Strassen   |
| 128  | 1 271       | 1 583         | **Clásico**|
| 256  | 11 001      | 12 533        | **Clásico**|
| 512  | 86 719      | 85 314        | Strassen   |
| 1024 | 804 665     | 439 744       | **Strassen**|
| 2048 | 6 051 661   | 3 241 661     | **Strassen**|

**N₀ empírico ≈ 512** (primer cruce sostenido en dos puntos consecutivos).

---

## Detalles de implementación

- **Tipo numérico:** `double` (IEEE 754, 64 bits) — evita overflow con entradas en [-100, 100].
- **Semilla aleatoria:** `srand(42)` — resultados reproducibles.
- **Estadístico:** mediana de 15 repeticiones (con Merge Sort propio).
- **Padding:** Strassen requiere n = 2^k; matrices no potencia de 2 se copian a la potencia siguiente.
- **Cronómetro:** `clock_gettime(CLOCK_MONOTONIC)` — resolución nanosegundo.
- **Flags de compilación:** `-O2 -std=c11 -Wall -Wextra -pedantic`.

---

## Ambiente de ejecución

```
SO:          Ubuntu 24.04 LTS 
Compilador:  GCC (gcc --version)
Procesador:  AMD Ryzen 7 4700U
RAM:         16.0 GB
```

---

## Referencias

- Demaine, E. D. (2005). *Lecture 3: Divide & Conquer*. MIT 6.046J. [OCW](https://ocw.mit.edu/courses/6-046j-introduction-to-algorithms-sma-5503-fall-2005/)
- Cormen et al. (2022). *Introduction to Algorithms*, 4th ed. MIT Press.

---

## Licencia

Uso académico — UNSA 2026.
