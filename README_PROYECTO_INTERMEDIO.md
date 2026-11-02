# Proyecto intermedio - notas de integración (Isabel Serrano Martín) 

## Controles de P3S_Scene
- `ESPACIO`: dispara el proyectil ligero desde la cámara.
- `P`: dispara el proyectil pesado desde la cámara.
- `+` / `-`: aumenta/disminuye el factor de velocidad del siguiente disparo.
- `G`: activa/desactiva gravedad.
- `R`: activa/desactiva viento.
- `T`: activa/desactiva torbellino.
- `E`: dispara una explosión temporal.
- `U`: activa/desactiva el emisor con distribución uniforme.
- `N`: activa/desactiva el emisor con distribución gaussiana. (ahora desactivado, el código  está comentado)

## Archivos añadidos
- `Projectile.h/.cpp`: dos tipos de proyectil, masa, velocidad, escalado de velocidad/energía/fuerza y disparo.
- `ParticleGenerator.h/.cpp`: partícula modelo, emisión periódica y muestreo uniforme, normal y gaussiano.
- `ParticleSystem.h/.cpp`: gestiona emisores y registro de fuerzas activos por sistema.
- `P3S_Scene.h/.cpp`: integra controles, proyectiles, emisores y generadores de fuerza.

## Requisitos
- Requisito A.1) -> "Particle.cpp" 81-103
- Requisito A.2) -> "Particle.h" 50 (atributo masa), "Proyectile.cpp" 8 (definición de dos tipos de proyectiles usando la masa)
- Requisito A.3) -> "Particle.cpp" 88-90
- Requisito B) -> "Proyectile.h" 16-17, 27 (escalado de velocidad, energía y fuerzas), 10 (definición en un enum de dos tipos de proyectiles),
                    "Proyectile.cpp" 6-13 (implementación de los dos tipos de proyectiles)
- Requisito C) -> "ParticleSystem.h", "ParticleSystem.cpp"
- Requisito D.1) -> "ParticleGenerator.h" 22 (definición de particleModel)
- Requisito D.2) -> "ParticleGenerator.cpp" 23-43 (utiliza el tipo de distribución para generar la posición de la nueva partícula)
- Requisito D.3) -> "ParticleSystem.h" 49, "ParticleSystem.cpp" 167-171, 178-188, (vector de generadores de partículas y gestión de estos)
- Requisito E.1) -> "Particle.cpp" 139-154 (gestión de fuerzas y generadores en la partícula)
- Requisito E.2) -> "ForceGenerator.h" (los generadores de fuerza solo aplican una fuerza a la partícula, no son excluyentes sino incrementales)
- Requisito E.3) -> "ParticleSystem.h" 47 (vector con los generadores de fuerzas del sistema actual y si están activos o no)
- Requisito E.4) -> "P3S_Scene.cpp" 16-19 (cuatro tipos de generadores de fuerza: gravedad, viento, torbellino y explosión)