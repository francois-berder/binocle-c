//
// Created by Valerio Santinelli on 19/04/18.
//

#include "binocle_sprite.h"
#include "binocle_texture.h"
#include "binocle_subtexture.h"
#include "binocle_material.h"
#include "binocle_gd.h"
#include "binocle_vpct.h"

binocle_sprite binocle_sprite_from_material(binocle_material *material) {
  binocle_sprite res = {};
  res.origin.x = 0;
  res.origin.y = 0;
  res.material = material;
  res.subtexture = binocle_subtexture_with_texture(material->texture, 0, 0, material->texture->width, material->texture->height);
  return res;
}

void binocle_sprite_draw(binocle_sprite sprite, binocle_gd *gd, uint64_t x, uint64_t y, kmAABB2 viewport) {
  size_t vertex_count = 6;
  binocle_vpct vertices[vertex_count];
  vertices[0].pos.x = sprite.origin.x + x;
  vertices[0].pos.y = sprite.origin.y + sprite.subtexture.rect.max.y + y;
  vertices[0].color = binocle_color_white();
  vertices[0].tex.x = 0;//sprite.subtexture.rect.min.x;
  vertices[0].tex.y = 1;//sprite.subtexture.rect.max.y;

  vertices[1].pos.x = sprite.origin.x + sprite.subtexture.rect.max.x + x;
  vertices[1].pos.y = sprite.origin.y + sprite.subtexture.rect.max.y + y;
  vertices[1].color = binocle_color_white();
  vertices[1].tex.x = 1;//sprite.subtexture.rect.max.x;
  vertices[1].tex.y = 1;//sprite.subtexture.rect.max.y;

  vertices[2].pos.x = sprite.origin.x + x;
  vertices[2].pos.y = sprite.origin.y + y;
  vertices[2].color = binocle_color_white();
  vertices[2].tex.x = 0;//sprite.subtexture.rect.min.x;
  vertices[2].tex.y = 0;//sprite.subtexture.rect.min.y;

  vertices[3].pos.x = sprite.origin.x + sprite.subtexture.rect.max.x + x;
  vertices[3].pos.y = sprite.origin.y + sprite.subtexture.rect.max.y + y;
  vertices[3].color = binocle_color_white();
  vertices[3].tex.x = 1;//sprite.subtexture.rect.max.x;
  vertices[3].tex.y = 1;//sprite.subtexture.rect.max.y;

  vertices[4].pos.x = sprite.origin.x + sprite.subtexture.rect.max.x + x;
  vertices[4].pos.y = sprite.origin.y + y;
  vertices[4].color = binocle_color_white();
  vertices[4].tex.x = 1;//sprite.subtexture.rect.max.x;
  vertices[4].tex.y = 0;//sprite.subtexture.rect.min.y;

  vertices[5].pos.x = sprite.origin.x + x;
  vertices[5].pos.y = sprite.origin.y + y;
  vertices[5].color = binocle_color_white();
  vertices[5].tex.x = 0;//sprite.subtexture.rect.min.x;
  vertices[5].tex.y = 0;//sprite.subtexture.rect.min.y;

  binocle_gd_draw(gd, vertices, vertex_count, *sprite.material, viewport);
}