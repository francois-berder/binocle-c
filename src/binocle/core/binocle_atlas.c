//
// Created by Valerio Santinelli on 26/11/2018.
//

#include "binocle_atlas.h"
#include "binocle_sdl.h"
#include "binocle_log.h"
#include "binocle_subtexture.h"
#include <parson/parson.h>

void binocle_atlas_load_texturepacker(char *filename, struct binocle_texture *texture, struct binocle_subtexture *subtextures, int *num_subtextures) {
  *num_subtextures = 0;
  binocle_log_info("Loading TexturePacker file: %s", filename);
  SDL_RWops *file = SDL_RWFromFile(filename, "rb");
  if (file == NULL) {
    binocle_log_error("Cannot open JSON file");
    return;
  }
  char content[BINOCLE_ATLAS_MAX_FILESIZE];

  Sint64 res_size = SDL_RWsize(file);
  char *res = (char *) malloc(res_size + 1);

  Sint64 nb_read_total = 0, nb_read = 1;
  char *buf = res;
  while (nb_read_total < res_size && nb_read != 0) {
    nb_read = SDL_RWread(file, buf, 1, (res_size - nb_read_total));
    nb_read_total += nb_read;
    buf += nb_read;
  }
  SDL_RWclose(file);
  if (nb_read_total != res_size) {
    binocle_log_error("Size mismatch");
    free(res);
    return;
  }

  res[nb_read_total] = '\0';
  strcpy(content, res);
  free(res);

  JSON_Value *root_value;
  JSON_Object *root;
  JSON_Array *frames;
  JSON_Object *frame;
  JSON_Object *meta;

  root_value = json_parse_string(content);
  if (json_value_get_type(root_value) != JSONObject) {
    binocle_log_error("Error parsing JSON, root object isn't an Object");
    return;
  }

  root = json_value_get_object(root_value);
  if (root == NULL) {
    binocle_log_error("Error parsing JSON, cannot read root object");
    return;
  }

  meta = json_object_get_object(root, "meta");
  int atlas_w = (int)json_object_dotget_number(meta, "size.w");
  int atlas_h = (int)json_object_dotget_number(meta, "size.h");

  frames = json_object_get_array(root, "frames");
  for (int i = 0 ; i < json_array_get_count(frames) ; i++) {
    frame = json_array_get_object(frames, i);
    int x, y, w, h = 0;
    const char *frameFilename;
    JSON_Object *innerFrame;

    frameFilename = json_object_get_string(frame, "filename");
    innerFrame = json_object_get_object(frame, "frame");
    x = (int)json_object_get_number(innerFrame, "x");
    y = (int)json_object_get_number(innerFrame, "y");
    w = (int)json_object_get_number(innerFrame, "w");
    h = (int)json_object_get_number(innerFrame, "h");

    // We need to invert the coordinates as our reference is with 0,0 in the bottom left corner, thus
    // the y becomes (atlas height - y - height of the frame)
    subtextures[*num_subtextures] = binocle_subtexture_with_texture(texture, x, atlas_h - y - h, w, h);
    strcpy(subtextures[*num_subtextures].name, frameFilename);
    (*num_subtextures)++;
  }

  binocle_log_debug("Atlas loaded.");
}

void binocle_atlas_load_libgdx(struct binocle_subtexture *subtextures, char *filename) {

}
