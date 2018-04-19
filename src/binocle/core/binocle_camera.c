//
// Created by Valerio Santinelli on 17/04/18.
//

#include "binocle_camera.h"
#include "binocle_viewport_adapter.h"
#include "binocle_math.h"

binocle_camera binocle_camera_new(struct binocle_viewport_adapter *adapter) {
  binocle_camera res = {};
  kmMat4Identity(&res.transform_matrix);
  kmMat4Identity(&res.inverse_transform_matrix);
  res.position.x = 0;
  res.position.y = 0;
  res.origin.x = 0;
  res.origin.y = 0;
  res.rotation = 0.0;
  res.zoom = 1.0;
  res.min_zoom = 1.0;
  res.max_zoom = 1.0;
  res.are_matrixes_dirty = false;
  res.are_bounds_dirty = false;
  res.bounds.min.x = 0;
  res.bounds.min.y = 0;
  res.bounds.max.x = 0;
  res.bounds.max.y = 0;
  res.viewport_adapter = adapter;
  res.near = -10.0;
  res.far = 10.0;

  return res;
}

kmVec2 binocle_camera_get_position(binocle_camera camera) {
  return camera.position;
}

void binocle_camera_set_position(binocle_camera camera, float x, float y) {
  camera.position.x = x;
  camera.position.y = y;
  binocle_camera_force_matrix_update(camera);
}

kmVec2 binocle_camera_get_origin(binocle_camera camera) {
  return camera.origin;
}

void binocle_camera_set_origin(binocle_camera camera, float x, float y) {
  camera.origin.x = x;
  camera.origin.y = y;
  binocle_camera_force_matrix_update(camera);
}

float binocle_camera_get_rotation(binocle_camera camera) {
  return camera.rotation;
}

void binocle_camera_set_rotation(binocle_camera camera, float rotation) {
  camera.rotation = rotation;
  binocle_camera_force_matrix_update(camera);
}

float binocle_camera_get_zoom(binocle_camera camera) {
  return camera.zoom;
}

void binocle_camera_set_zoom(binocle_camera camera, float zoom) {
  camera.zoom = kmClamp(zoom, camera.min_zoom, camera.max_zoom);
  camera.are_matrixes_dirty = true;
}

float binocle_camera_get_min_zoom(binocle_camera camera) {
  return camera.min_zoom;
}

void binocle_camera_set_min_zoom(binocle_camera camera, float min_zoom) {
  if (camera.zoom < min_zoom) {
    binocle_camera_set_zoom(camera, min_zoom);
  }
  camera.min_zoom = min_zoom;
}

float binocle_camera_get_max_zoom(binocle_camera camera) {
  return camera.max_zoom;
}

void binocle_camera_set_max_zoom(binocle_camera camera, float max_zoom) {
  if (camera.zoom > max_zoom) {
    binocle_camera_set_zoom(camera, max_zoom);
  }
  camera.max_zoom = max_zoom;
}

kmAABB2 binocle_camera_get_bounds(binocle_camera camera) {
  if (camera.are_matrixes_dirty) {
    binocle_camera_update_matrixes(camera);
  }

  if (camera.are_bounds_dirty) {
    // top-left and bottom-right are needed by either rotated or non-rotated bounds
    kmVec2 screen_position = {.x = camera.viewport_adapter->viewport.min.x, .y = camera.viewport_adapter->viewport.min.y};
    kmVec2 top_left = binocle_camera_screen_to_world_point(camera, screen_position);
    screen_position.x = camera.viewport_adapter->viewport.min.x + camera.viewport_adapter->viewport.max.x;
    screen_position.y = camera.viewport_adapter->viewport.min.y + camera.viewport_adapter->viewport.max.y;
    kmVec2 bottom_right = binocle_camera_screen_to_world_point(camera, screen_position);

    if (camera.rotation != 0.0)
    {
      // special care for rotated bounds. we need to find our absolute min/max values and create the bounds from that
      screen_position.x = camera.viewport_adapter->viewport.min.x + camera.viewport_adapter->viewport.max.x;
      screen_position.y = camera.viewport_adapter->viewport.min.y;
      kmVec2 top_right = binocle_camera_screen_to_world_point(camera, screen_position);

      screen_position.x = camera.viewport_adapter->viewport.min.x;
      screen_position.y = camera.viewport_adapter->viewport.min.y + camera.viewport_adapter->viewport.max.y;
      kmVec2 bottom_left = binocle_camera_screen_to_world_point(camera, screen_position);

      float min_x = MINOF( top_left.x, bottom_right.x, top_right.x, bottom_left.x );
      float max_x = MAXOF( top_left.x, bottom_right.x, top_right.x, bottom_left.x );
      float min_y = MINOF( top_left.y, bottom_right.y, top_right.y, bottom_left.y );
      float max_y = MAXOF( top_left.y, bottom_right.y, top_right.y, bottom_left.y );

      camera.bounds.min.x = min_x;
      camera.bounds.min.y = min_y;
      camera.bounds.max.x = ( max_x - min_x );
      camera.bounds.max.y = ( max_y - min_y );
    }
    else
    {
      camera.bounds.min.x = top_left.x;
      camera.bounds.min.y = top_left.y;
      camera.bounds.max.x = ( bottom_right.x - top_left.x );
      camera.bounds.max.y = ( bottom_right.y - top_left.y );
    }

    camera.are_bounds_dirty = false;
  }
  return camera.bounds;
}

kmMat4 binocle_camera_get_transform_matrix(binocle_camera camera) {
  if (camera.are_matrixes_dirty) {
    binocle_camera_update_matrixes(camera);
  }
  return camera.transform_matrix;
}

kmMat4 binocle_camera_get_inverse_transform_matrix(binocle_camera camera) {
  if (camera.are_matrixes_dirty) {
    binocle_camera_update_matrixes(camera);
  }
  return camera.inverse_transform_matrix;
}

binocle_viewport_adapter *binocle_camera_get_viewport_adapter(binocle_camera camera) {
  return camera.viewport_adapter;
}

void binocle_camera_set_viewport_adapter(binocle_camera camera, binocle_viewport_adapter *viewport_adapter) {
  camera.viewport_adapter = viewport_adapter;
  camera.are_matrixes_dirty = true;
  camera.are_bounds_dirty = true;
  // TODO: should we update both matrixes and bounds automatically?
}

void binocle_camera_update_matrixes(binocle_camera camera) {
  kmMat4 temp_mat;

  kmMat4Translation(&camera.transform_matrix, -camera.position.x, -camera.position.y, 0.0); // position
  kmMat4Scaling(&temp_mat, camera.zoom, camera.zoom, 1.0); // scale
  kmMat4Multiply(&camera.transform_matrix, &camera.transform_matrix, &temp_mat);
  kmMat4RotationZ(&temp_mat, kmDegreesToRadians(camera.rotation)); // rotation
  kmMat4Multiply(&camera.transform_matrix, &camera.transform_matrix, &temp_mat);
  // TODO: clamp origin to integer values, see -> temp_mat.CreateTranslation( (int)origin.x, (int)origin.y, 0.0f );
  kmMat4Translation(&temp_mat, camera.origin.x, camera.origin.y, 0.0); // translate -origin
  kmMat4Multiply(&camera.transform_matrix, &camera.transform_matrix, &temp_mat);

  // if we have a ViewportAdapter take it into account
  kmMat4Multiply(&camera.transform_matrix, &camera.transform_matrix, &camera.viewport_adapter->scale_matrix);

  // calculate our inverse as well
  kmMat4Inverse(&camera.inverse_transform_matrix, &camera.transform_matrix);

  // whenever the matrix changes the bounds are then invalid
  camera.are_bounds_dirty = true;
  camera.are_matrixes_dirty = false;
}


/// <summary>
/// this forces the matrix and bounds dirty
/// </summary>
void binocle_camera_force_matrix_update(binocle_camera camera) {
  camera.are_matrixes_dirty = true;
  camera.are_bounds_dirty = true;
}

void binocle_camera_round_position(binocle_camera camera) {
  uint64_t x = llroundf(camera.position.x);
  uint64_t y = llroundf(camera.position.y);
  binocle_camera_set_position(camera, x, y);
  camera.are_matrixes_dirty = true;
}


void binocle_camera_center_origin(binocle_camera camera) {
  kmVec2 o = {
    .x = camera.viewport_adapter->virtual_width / 2.0,
    .y = camera.viewport_adapter->virtual_height / 2.0
  };
  binocle_camera_set_origin(camera, o.x, o.y);

  // offset our position to match the new center
  float x = camera.position.x + camera.origin.x;
  float y = camera.position.y + camera.origin.y;
  binocle_camera_set_position(camera, x, y);
}

/*
pub fn translate(&mut self, direction: Vector2<f32> ) {
  //position += Vector2.Transform( direction, Matrix.CreateRotationZ( -rotation ) );
}
*/

void binocle_camera_rotate(binocle_camera camera, float delta_radians) {
  float r = camera.rotation + delta_radians;
  binocle_camera_set_rotation(camera, r);
}

void binocle_camera_zoom_in(binocle_camera camera, float delta_zoom)
{
  float z = camera.zoom + delta_zoom;
  binocle_camera_set_zoom(camera, z);
}


void binocle_camera_zoom_out(binocle_camera camera, float delta_zoom)
{
  float z = camera.zoom - delta_zoom;
  binocle_camera_set_zoom(camera, z);
}


/// <summary>
/// converts a point from world coordinates to screen
/// </summary>
/// <returns>The to screen point.</returns>
/// <param name="worldPosition">World position.</param>
kmVec2 binocle_camera_world_to_screen_point(binocle_camera camera, kmVec2 world_position) {
  kmVec3 pos = {.x = world_position.x, .y = world_position.y, 0.0};
  kmVec3MultiplyMat4(&pos, &pos, &camera.transform_matrix);
  kmVec2 pos2 = {.x = pos.x, .y = pos.y};

  kmVec2 p = binocle_viewport_adapter_screen_to_virtual_viewport(*camera.viewport_adapter, pos2);
  return p;
}


/// <summary>
/// converts a point from screen coordinates to world
/// </summary>
/// <returns>The to world point.</returns>
/// <param name="screenPosition">Screen position.</param>
kmVec2 binocle_camera_screen_to_world_point(binocle_camera camera, kmVec2 screen_position) {
  kmVec2 pos2;
  pos2 = binocle_viewport_adapter_point_to_virtual_viewport(*camera.viewport_adapter, screen_position);
  kmVec3 pos = {.x = pos2.x, .y = pos2.y, 0};
  kmVec3 res;
  kmVec3MultiplyMat4(&res, &pos, &camera.inverse_transform_matrix);
  pos2.x = res.x;
  pos2.y = res.y;
  return pos2;
}


/// <summary>
/// gets this cameras project matrix
/// </summary>
/// <returns>The projection matrix.</returns>
kmMat4 binocle_camera_get_projection_matrix(binocle_camera camera) {
  // not currently blocked with a dirty flag due to the core engine not using this
  return binocle_math_create_orthographic_matrix_off_center(
    0.0, camera.viewport_adapter->viewport.max.x, camera.viewport_adapter->viewport.max.y, 0.0, camera.near, camera.far );
}


/// <summary>
/// gets the view-projection matrix which is the transformMatrix * the projection matrix
/// </summary>
/// <returns>The view projection matrix.</returns>
kmMat4 binocle_camera_get_view_projection_matrix(binocle_camera camera)
{
  // not currently blocked with a dirty flag due to the core engine not using this
  kmMat4 out;
  kmMat4 proj = binocle_camera_get_projection_matrix(camera);
  kmMat4Multiply(&out, &camera.transform_matrix, &proj);
  return out;
}


void binocle_camera_handle_window_resize(binocle_camera camera, kmVec2 old_window_size, kmVec2 new_window_size) {
  binocle_viewport_adapter_reset(camera.viewport_adapter, old_window_size, new_window_size);
}
