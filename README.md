# MyBash - Shell en C

## 1. Instrucciones de Compilación y Ejecución

Para compilar el proyecto, ejecutar en la terminal:

```bash
make
```

Para correr el shell:

```bash
./mybash
```
---

## 2. Descripción de Archivos Principales:

- **`command.c`**: Manejo de comandos individuales (agregado, acceso, destrucción, etc.).
- **`execute.c`**: Lógica de ejecución de los comandos, redirecciones, pipes y procesos.
- **`parsing.c`**: Parsing del input del usuario, separación por tokens, construcción del pipeline.
- **`builtin.c`**: Implementación de comandos internos (`cd`, `exit`, `help`).
- **`mybash.c`**: Punto de entrada del programa (`main()`), loop principal de lectura y ejecución.

---

## 3. Metodología de Trabajo en Equipo

### Integrantes

- **Joaquín Pairetti**
- **Máximo Ortega**
- **Franco Galassi**
- **Lissandro Bosque**

### El grupo se organizó dividiendo tareas en ramas específicas. Nos referiremos a creador, como el principal autor de la estructura del modulo, y colaborador como la persona que ayudo con la implementacionde dicho modulo (resolviendo problemas, depurando, etc):
- Modulo Command:
    - Creadores: Lissandro Bosque, Franco galassi, Maximo Ortega, Joaquin Pairetti  
- Modulo Parsing: 
    - creadores: Maximo Ortega, Joaquin Pairetti 
- Modulo Execute:
    - creadores: Lissandro Bosque
    - colaborador: Franco Galassi
- Modulo Builtin: 
    - creadores: Franco Galassi
    - colaboradores: Joaquin Pairetti
- Documentación:
    - creadores: Joaquin Pairetti 
- Video: 
    - creadores: Franco Galassi

### El trabajo se distribuyó equitativamente y se integró con revisión cruzada entre los integrantes.

---

## 4. Uso de Asistentes de IA

Principal uso:
- Depuracion de dudas teoricas:
    - Syscall 
    - Procesos
    - Facilitar la compresion de consignas (Ejemplo: Entender el comportamiento de funciones) 
- Depuracion de dudas de la implementacion:
    - Manejo de memoria y punteros
    - Glib y otras librerias
- Entendimiento de errores que surgian, con el fin de facilitar su correcion, Ejemplos:
    - Depuracion de parse_command, Error: no tomaba correctamente los argumentos
    - Depuracion de scommand_set_redir_out/out, Error: mal manejo de memoria
- **`conclusion`**: Fue de gran utilidad para agilizar el tratamiento de errores y amenizar el entendimiento del material teorico, nos enfocamos en un uso  responsable de la herramienta, porque sabemos que su uso abusivo convierte soluciones en problemas.  

---

## 5. Enlace al Video

👉 [Ver video de demostración](https://www.youtube.com/watch?v=DDv3d7gQj98)

---