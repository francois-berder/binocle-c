//
// Binocle
// Copyright (c) 2015-2019 Valerio Santinelli
// All rights reserved.
//

#ifndef BINOCLE_MATERIAL_H
#define BINOCLE_MATERIAL_H

#include "binocle_blend.h"

struct binocle_texture;
struct binocle_shader;

/**
 * \brief A material as used internally by the engine
 */
typedef struct binocle_material {
  /// The blending mode
  binocle_blend blend_mode;
  /// The diffuse texture
  struct binocle_texture *texture;
  /// The specular texture
  struct binocle_texture *specular_texture;
  /// The shader (couple of VS/FS)
  struct binocle_shader *shader;
} binocle_material;

/**
 * \brief Instantiates a new material
 * @return The new material
 */
binocle_material *binocle_material_new();

/**
 * \brief Frees the memory of the material
 * @param material the material to destroy
 */
void binocle_material_destroy(binocle_material *material);

#endif //BINOCLE_MATERIAL_H
