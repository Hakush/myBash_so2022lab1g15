Requires:

Glib and libcheck

# Lab01-SisOp-2022
## Grupo N° 15
#### Integrantes:
Otero Alan
Sharry Facundo
Collo Gaston
Rodriguez Diego

# Indice
1. [Introducción](#id1)
2. [Técnicas de Programación](#id2)
3. [Herramientas de Programación](#id3)
4. [Setup](#id4)
5. [Módulos](#id5)
6. [FeedBack](#id6)
7. [Conclusiones](#id7)

-   ## Introducción <span id="id1"/>
    ### MyBASH
    Es una [CLI](https://en.wikipedia.org/wiki/Command-line_interface) que posee tres comandos básicos **cd**, **exit** y           **help**, y que además puede ejecutar comandos simples del sistema con redirección y en pipelines pares.

-   ## Técnicas de Programación <span id="id2"/>
    Se trato de llegar siempre a un código modular y limpio, que cumpla con los requerimientos, pero manteniendo un balance con la legibilidad.
-   ## Herramientas de Programación <span id="id3"/>
    Durante todo el desarrollo del laboratorio usamos el IDE CLion con la extensión CodeWithMe para conectarnos al mismo workspace. GDB fue clave para el avance.

# Setup <span id="id4"/>

## Limpiar ejecutables

```sh
make clean
```
## Compilar

```sh
make 
```
o también
```sh
make all 
```

## Ejecutable

```sh
./mybash
```

## Ejecutar desde un solo comando

```sh
make && ./mybash
```

# Modulos <span id="id5"/>

- ## Command
  Contiene los TADs responsables de construir y dar funcionalidad a
  nuestras pipelines y comandos simples, abstrayendo su comportamiento.
- ## Builtin
  Contiene métodos para manejar los comandos internos; específicamente         ejecutarlos, enumerar y encapsular las syscalls de los builtin implementados, y chequear si un scommand es un comando interno. Ademas implementamos function pointers para lograr cierto "Polimorfismo" y que sea mas sencillo llamar al executor de nuevos comandos internos.
- ## Execute
  Es el encargado de ejecutar los comandos de una pipeline, conectando sus inputs y outputs y ademas realizarlos en procesos hijos.
- ## Parsing
  Es el encargado de iterar entre los argumentos, símbolos y demás patrones que nos interesen que se encuentren en un string y construir nuestros TAD pipeline y scommand.

# Feedback <span id="id6"/>

## Desarrollo

Nuestro proceso de desarrollo fue el siguiente:

Comenzamos desarrollando la estructura del command. En principio nos fuimos guíando por el díagrama que dejó la cátedra en el doc, y luego nos ayudamos de las específicaciones que estaban en cada .h (command.h).
Al comenzar con la estructura de command y pipeline, nos decidimos usar Glist de la librería Glib, porque tenía métodos ya implementados que ibamos a necesitar en cada función propia(como hacer push o pop).
Lo próximo que realizamos fue el parsing y el builtin, en paralelo. El builtin lo hemos ido cambiando, ya que aparecían mejores formas de desarrollo(mas elegante y modularizada) y a medida que pasaban los días, mejorabamos el código o eliminabamos cosas innecesarias, también arreglabamos problemas como "segmentation fault" que generaban los ciclos o los punteros. El último día testeamos que la llamada a los comandos internos, funcionaban, el cd, exit y help.
Al final de todo y lo que mas problema generó fue el execute.c, en especial lo que implica el fork y pipe, por sus test, pero que lograron resolverse el último día.
Pudimos resolver todo lo que implican los puntos necesarios para aprobar el proyecto, desde los comandos internos a los procesos zombies, y también realizar un punto estrella: generar un prompt con información relevante.


## Conclusiones <span id="id7"/>
Sentimos que el lab fue entretenido y pudimos adquirir los conocimientos de una manera desafiante pero satisfactoria, si bien no llegamos a cumplir todos los puntos extra, de haber tenido mas tiempo (Hablando del tiempo personal de cada uno de los integrantes), consideramos que habria sido totalmente alcanzable.
