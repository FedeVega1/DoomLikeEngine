# DoomLikeEngine

[![Plataforma](https://img.shields.io/badge/plataforma-Windows-0078D6?logo=windows&logoColor=white)](#requisitos)
[![Lenguaje](https://img.shields.io/badge/lenguaje-C%2B%2B20-00599C?logo=cplusplus&logoColor=white)](#stack-tecnológico)
[![Build](https://img.shields.io/badge/build-CMake%20%2B%20VS2022-064F8C?logo=cmake&logoColor=white)](#compilación)
[![Estado](https://img.shields.io/badge/estado-en%20desarrollo-yellow)](#estado-del-proyecto)
[![Licencia](https://img.shields.io/badge/licencia-MIT-blue)](#licencia)

**Leer en otros idiomas:** [English](README.md)

Un **motor de juego 2.5D estilo Doom** construido desde cero, junto con su **editor de mapas**, escrito en C++20 sin ninguna librería de matemáticas externa. Originalmente construido con GDI plano y Windows Forms para el editor, ahora en migración a SDL3 y Dear ImGui.
El objetivo es reproducir el clásico enfoque de renderizado por sectores/portales de *Doom* de id Software (árboles BSP, mapas basados en sectores, raycasting de paredes/piso/techo) como ejercicio de aprendizaje.

---

## Tabla de contenidos

- [Demo](#demo)
- [Resumen](#resumen)
- [Características](#características)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Stack tecnológico](#stack-tecnológico)
- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Ejecución](#ejecución)
- [Estado del proyecto](#estado-del-proyecto)
- [Hoja de ruta](#hoja-de-ruta)
- [Contribuir](#contribuir)
- [Licencia](#licencia)
- [Agradecimientos](#agradecimientos)

---

## Demo

**Vista previa del estado actual**

![Vista previa](docs/media/gameplay.gif)

**Renderizado paso a paso** — una vista acelerada de cómo el motor renderiza cada píxel.

![Renderizado por píxel](docs/media/pixel-rendering.gif)

---

## Resumen

DoomLikeEngine está dividido en dos piezas que trabajan en conjunto:

1. **El motor (`DoomCopy`)** — un runtime de juego en tiempo real que carga mapas binarios compilados (`.bsp`) y los renderiza con un pipeline de raycasting/renderizado por software 2.5D hecho a medida.
2. **El editor (`Editor`)** — un editor de niveles nativo basado en ImGui para crear mapas por sectores (`.map`), dibujar sectores/paredes, editar propiedades de entidades y geometría, y compilar los mapas al formato binario `.bsp` que usa el motor.

Todo —vectores, colores, transformaciones, matemática de BSP, triangulación de sectores— está escrito desde cero en la librería compartida `Core`, sin usar GLM ni ninguna otra dependencia matemática externa.

## Características

### Motor (`DoomCopy`)
- Renderizador 2.5D por software/SDL3 hecho a medida (renderizado de paredes, piso y techo)
- Representación del mundo basada en sectores (`World`, `WorldTypes`) en lugar de un grafo de escena 3D genérico
- Modelo simple de objetos de juego basado en componentes (`GameObject`, `Component`, `TransformComponent`, `CameraComponent`)
- Bucle de juego con paso de tiempo fijo y frameskip (`DEFAULT_TICKS_PER_SECOND`, `MAX_FRAMESKIP`)
- Manejo de entradas personalizado (teclado + mouse look)
- Carga archivos de mapa `.bsp` precompilados generados por el Editor

### Editor
- Aplicación de escritorio nativa en C++ / SDL3 / Dear ImGui (rama docking)
- Herramientas interactivas de dibujo de sectores y paredes, con vista previa de sectores pendientes
- **Triangulación por ear-clipping** hecha a medida para polígonos de sectores arbitrarios (no convexos)
- Dibujo consciente de intersección de segmentos (evita paredes de sector que se autointersectan o se superponen)
- Gestor de selección para elegir y editar nodos, paredes y sectores
- Panel de propiedades para inspeccionar/editar los datos de los objetos del mapa
- Historial de comandos con deshacer/rehacer (`CommandHistory`, `EditorCommands`)
- Atajos de teclado y tema del editor configurables (`EditorHotKeys`, `EditorTheme`)
- E/S de archivos de mapa (`.map`) y un **compilador BSP** que convierte los mapas editables al formato `.bsp` que usa el motor en tiempo de ejecución

### Core (librería compartida)
- Primitivas matemáticas hechas a mano: `Vector2`, `Vector3`, `Vector2Int`, `Color` (sin GLM ni librerías externas)
- Funciones auxiliares de vectores/geometría usadas tanto por el editor como por el motor (intersección de segmentos, pruebas de punto sobre segmento, etc.)
- Tipos de datos del mundo compartidos y E/S de archivos `.bsp`

### Editor heredado (`LevelEditor`)
- Un editor de mapas anterior en C# / .NET 8 WinForms, **reemplazado por el editor nativo `Editor`**
- Se mantiene en el repositorio como referencia; no está en desarrollo activo

## Estructura del proyecto

```
DoomCopy/
├── Core/            # Librería estática compartida: matemáticas, tipos de mundo, E/S de BSP
├── DoomCopy/         # Ejecutable del motor/juego (renderizador, game loop, entidades)
├── Editor/           # Editor de mapas nativo en C++/SDL3/ImGui (desarrollo activo)
├── LevelEditor/      # Editor heredado en C#/.NET 8 WinForms (reemplazado, se mantiene como referencia)
├── CMakeLists.txt    # Script de build raíz (FetchContent: SDL3, Dear ImGui, COLogger)
└── CMakePresets.json # Presets x64-debug / x64-release (Visual Studio 17 2022, solo Windows)
```

## Stack tecnológico

| Área                | Tecnología                                     |
|---------------------|--------------------------------------------------|
| Lenguaje            | C++20                                            |
| Ventanas / Entradas | [SDL3](https://github.com/libsdl-org/SDL)        |
| UI del editor       | [Dear ImGui](https://github.com/ocornut/imgui) (rama `docking`) |
| Logging             | [COLogger](https://github.com/FedeVega1/COLogger) |
| Sistema de build    | CMake 3.20+ con `FetchContent`                   |
| Herramienta heredada| C# / .NET 8 (WinForms)                           |
| Matemáticas         | Implementación propia — sin GLM |

## Requisitos

- **Windows** (por ahora el proyecto solo apunta a Windows — los presets de CMake lo verifican explícitamente)
- **Visual Studio 2022** o **Visual Studio 2026** (generador: `Visual Studio 17 2022`)
- **CMake 3.20+**
- Conexión a internet para la primera configuración (CMake `FetchContent` descarga SDL3, Dear ImGui y COLogger desde GitHub)
- SDK de .NET 8, solo si se quiere compilar el editor heredado `LevelEditor`

## Compilación

```bash
# Configurar (elegir una opción)
cmake --preset x64-debug
cmake --preset x64-release

# Compilar
cmake --build out/build/x64-debug
```

También se puede abrir la solución generada directamente en Visual Studio 2022 o Visual Studio 2026:

```
out/build/x64-debug/DoomLikeEngine.sln
```

> ⚠️ Al abrir la solución, el proyecto de inicio por defecto es **`DoomCopy`** (el juego), no el **`Editor`** — cambiá el proyecto de inicio desde el Explorador de soluciones de Visual Studio si querés ejecutar el editor.

Los binarios generados se ubican en:

```
bin/<Proyecto>/<Config> - x64/
```

(notá el espacio literal en el nombre del directorio de configuración, por ejemplo `bin/Editor/Debug - x64/`)

## Ejecución

- **Jugar/probar un mapa compilado:** ejecutá el binario `DoomCopy` — actualmente carga al iniciar un mapa por defecto codificado llamado asd, esto va a cambiar en el futuro.
- **Crear o editar un mapa:** ejecutá el binario `Editor`, dibujá sectores/paredes, editá propiedades, y usá su compilador BSP para generar un `.bsp` que el motor pueda cargar.

## Estado del proyecto

Este es un proyecto hobby en constante evolución — es esperable encontrar aristas sin pulir, especialmente en:

- **Dibujo y triangulación de sectores** (`Editor/src/MapRenderer.cpp`, `Editor/src/MapData.cpp`, `Editor/src/MapView.cpp`) — esta área ha necesitado correcciones repetidas y debe tratarse como frágil.
- La sincronización manual entre `wallAdjacency` y `usedWalls` en `MapData` — no hay nada que garantice la consistencia automáticamente, así que cada alta/baja de pared o sector debe actualizar ambos.

## Hoja de ruta

# Motor

- [ ] Pulir el renderizado de paredes/piso/techo con texturas
- [ ] Sistemas de entidades/gameplay más allá de los componentes básicos
- [ ] Renderizado de sprites para objetos
- [ ] Menú principal y HUD
- [ ] Selección de nivel, eliminar la carga de mapa codificada

# Editor

- [ ] Triangulación de sectores más robusta y mejor manejo de autointersecciones
- [ ] Retirar o reemplazar por completo el `LevelEditor` heredado
- [ ] Tests automatizados para la matemática de `Core` y la compilación de BSP
- [ ] Colocación de entidades

## Contribuir

Este es actualmente un proyecto personal/individual y no está preparado para contribuciones externas, pero issues, sugerencias y forks son bienvenidos.

## Licencia

Este proyecto está licenciado bajo la [Licencia MIT](LICENSE).

## Agradecimientos

- [SDL3](https://github.com/libsdl-org/SDL) — backend de ventanas, entradas y renderizado
- [Dear ImGui](https://github.com/ocornut/imgui) — interfaz gráfica de modo inmediato para el editor
- *Doom* y *Wolfenstein 3D* de id Software — los motores clásicos que inspiran este proyecto
