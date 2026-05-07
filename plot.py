#!/usr/bin/env python3
"""
plot.py  —  grafica clasico vs Strassen y marca N0
"""
import sys
import numpy as np
import matplotlib.pyplot as plt

def load(path):
  ns, tc, ts = [], [], []
  with open(path) as f:
    for line in f:
      line = line.strip()
      # Saltar líneas vacías o comentarios
      if not line or line.startswith('#'):
        continue
            
      parts = line.split()
      # Solo procesar líneas que tengan exactamente 3 columnas
      if len(parts) != 3:
        continue
            
      try:
        # Intentar convertir a números, si falla (como en el encabezado), saltar
        a, b, c = map(float, parts)
        ns.append(int(a))
        tc.append(b)
        ts.append(c)
      except ValueError:
        continue
                
  return np.array(ns), np.array(tc), np.array(ts)

def find_n0(ns, tc, ts):
  """primer n donde strassen < clasico de forma sostenida"""
  for i in range(len(ns)-1):
    # Verifica que Strassen sea menor ahora y en el siguiente punto
    if ts[i] < tc[i] and ts[i+1] < tc[i+1]:
      return ns[i]
  return None

def main():
  path = sys.argv[1] if len(sys.argv) > 1 else "data.dat"
  ns, tc, ts = load(path)
    
  if len(ns) == 0:
    print(f"Error: No se pudieron extraer datos de {path}")
    return

  n0 = find_n0(ns, tc, ts)

  fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5))
  fig.suptitle(
    "Strassen vs Clásico — Análisis de Desempeño",
    fontsize=13, fontweight='bold'
  )

  # ── Gráfico 1: Tiempos totales ────────────────────────────
  ax1.plot(ns, tc, 'o-',  color='steelblue', lw=2, ms=5, label='Clásico O(n³)')
  ax1.plot(ns, ts, 's--', color='tomato',    lw=2, ms=5, label='Strassen O(n^2.81)')
  if n0:
    ax1.axvline(n0, color='green', ls=':', lw=1.8, label=f'N₀ ≈ {n0}')
    
  ax1.set_xlabel('n (tamaño de la matriz)')
  ax1.set_ylabel('Tiempo (µs)')
  ax1.set_title('Tiempos de ejecución completos')
  ax1.legend()
  ax1.grid(True, alpha=0.3)

  # ── Gráfico 2: Zoom en la zona de interés ─────────────────
  # Filtramos datos para ver mejor el cruce (n < 512 o similar)
  limit = n0 * 2 if n0 else 300
  mask = ns <= limit
    
  if any(mask):
    ax2.plot(ns[mask], tc[mask], 'o-',  color='steelblue', lw=2, ms=6, label='Clásico')
    ax2.plot(ns[mask], ts[mask], 's--', color='tomato',    lw=2, ms=6, label='Strassen')
    if n0:
      ax2.axvline(n0, color='green', ls=':', lw=1.8, label=f'N₀ ≈ {n0}')
    ax2.set_title(f'Zoom: región del cruce (N₀ ≈ {n0})' if n0 else 'Zoom: primeras muestras')
  else:
    ax2.set_title("Datos insuficientes para zoom")

  ax2.set_xlabel('n')
  ax2.set_ylabel('Tiempo (µs)')
  ax2.legend()
  ax2.grid(True, alpha=0.3)

  plt.tight_layout()
  out = path.replace('.dat', '_grafico.png')
  plt.savefig(out, dpi=150, bbox_inches='tight')
    
  print(f"Gráfico guardado en: {out}")
  if n0:
    print(f"Punto de cruce (N0) estimado en: n = {n0}")
  else:
    print("No se detectó un punto de cruce claro donde Strassen sea más rápido.")

if __name__ == '__main__':
  main()

