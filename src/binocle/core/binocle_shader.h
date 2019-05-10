//
// Binocle
// Copyright (c) 2015-2019 Valerio Santinelli
// All rights reserved.
//

#ifndef BINOCLE_SHADER_H
#define BINOCLE_SHADER_H

#include "binocle_sdl.h"

typedef struct binocle_shader {
  GLuint vert_id;
  GLuint frag_id;
  GLuint program_id;
  char *vert_src;
  char *frag_src;
} binocle_shader;

typedef enum {
  BINOCLE_SHADER_DEFAULT_FLAT,
  BINOCLE_SHADER_DEFAULT_MAX,
} binocle_shader_defaults_slot;

binocle_shader binocle_shader_defaults[BINOCLE_SHADER_DEFAULT_MAX];

binocle_shader binocle_shader_new();

binocle_shader binocle_shader_load_from_file(char *vert_filename, char *frag_filename);

bool binocle_shader_compile(const char *src, GLenum shader_type, GLuint *shad_id);

bool binocle_shader_link(GLuint vert_id, GLuint frag_id, GLuint *prog_id);

void binocle_shader_unload(binocle_shader *shader);

char *str_replace(char *orig, char *rep, char *with);

void binocle_shader_init_defaults();

#endif //BINOCLE_SHADER_H
