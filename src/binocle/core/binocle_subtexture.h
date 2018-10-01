//
// Created by Valerio Santinelli on 17/04/18.
//

#ifndef BINOCLE_BINOCLE_SUBTEXTURE_H
#define BINOCLE_BINOCLE_SUBTEXTURE_H

#include <kazmath/kazmath.h>
#include <stdint.h>

struct binocle_texture;

typedef struct binocle_subtexture {
  struct binocle_texture *texture;
  kmAABB2 rect;
} binocle_subtexture;

binocle_subtexture binocle_subtexture_with_texture(struct binocle_texture *texture, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
binocle_subtexture binocle_subtexture_with_subtexture(binocle_subtexture *subtexture, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
uint64_t binocle_subtexture_get_x(binocle_subtexture subtexture);
uint64_t binocle_subtexture_get_y(binocle_subtexture subtexture);
uint64_t binocle_subtexture_get_width(binocle_subtexture subtexture);
uint64_t binocle_subtexture_get_height(binocle_subtexture subtexture);
kmVec2 binocle_subtexture_get_size(binocle_subtexture subtexture);
kmVec2 binocle_subtexture_get_center(binocle_subtexture subtexture);
kmAABB2 binocle_subtexture_get_frame(binocle_subtexture subtexture, uint64_t index, uint64_t frame_width, uint64_t frame_height);
kmAABB2 binocle_subtexture_get_absolute_clip_rect(binocle_subtexture subtexture, kmAABB2 relative_clip_rect);

#endif //BINOCLE_BINOCLE_SUBTEXTURE_H
