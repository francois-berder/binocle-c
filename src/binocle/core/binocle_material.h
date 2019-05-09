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

typedef struct binocle_material {
    binocle_blend blend_mode;
    struct binocle_texture *texture;
    struct binocle_shader *shader;
} binocle_material;

binocle_material binocle_material_new();

#endif //BINOCLE_MATERIAL_H
