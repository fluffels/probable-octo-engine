# probable-octo-engine
Experiments with C

# TODO

## Major Features

1. Water mesh & shader.
2. Shadow mapping.
3. More realistic lighting.
4. Grass.

## Code Cleanup

1. Make sure there's no code to be reused between Grid and Terrain.
1. Research ways to reuse code between shader files. Possibly some sort of `#include` mechanism.

## Miscellaneous

1. Empty CompilationError is thrown if shaders/<prefix>/vertex.glsl isn't found.
1. Allow Shaders to cache anonymous uniform update requests for when they are activated again.