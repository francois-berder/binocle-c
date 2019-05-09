//
// Binocle
// Copyright (c) 2015-2019 Valerio Santinelli
// All rights reserved.
//

#ifndef BINOCLE_CAMERA_H
#define BINOCLE_CAMERA_H

#include <stdint.h>
#include <stdbool.h>
#include <kazmath/kazmath.h>

struct binocle_viewport_adapter;

typedef struct binocle_camera {
  kmMat4 transform_matrix;
  kmMat4 inverse_transform_matrix;
  kmVec2 position;
  kmVec2 origin;
  float rotation;
  float zoom;
  float min_zoom;
  float max_zoom;
  bool are_matrixes_dirty;
  bool are_bounds_dirty;
  kmAABB2 bounds;
  struct binocle_viewport_adapter *viewport_adapter;
  float near_distance;
  float far_distance;
} binocle_camera;

binocle_camera binocle_camera_new(struct binocle_viewport_adapter *adapter);
kmVec2 binocle_camera_screen_to_world_point(binocle_camera camera, kmVec2 screen_position);
void binocle_camera_update_matrixes(binocle_camera *camera);
void binocle_camera_force_matrix_update(binocle_camera *camera);
kmMat4 *binocle_camera_get_transform_matrix(binocle_camera *camera);
struct binocle_viewport_adapter *binocle_camera_get_viewport_adapter(binocle_camera camera);
kmAABB2 binocle_camera_get_viewport(binocle_camera camera);
void binocle_camera_set_position(binocle_camera *camera, float x, float y);

#endif //BINOCLE_CAMERA_H
