//
// Created by Valerio Santinelli on 13/04/18.
//

#include "binocle_viewport_adapter.h"
#include "binocle_window.h"

binocle_viewport_adapter binocle_viewport_adapter_new(
  binocle_window window, binocle_viewport_adapter_kind kind, binocle_viewport_adapter_scaling_type scaling_type, uint32_t width, uint32_t height, uint32_t virtual_width, uint32_t virtual_height
) {
  binocle_viewport_adapter res = {};
  kmMat4Identity(&res.scale_matrix);
  res.original_viewport.min.x = 0;
  res.original_viewport.min.y = 0;
  res.original_viewport.max.x = width;
  res.original_viewport.max.y = height;
  res.viewport.min.x = 0;
  res.viewport.min.y = 0;
  res.viewport.max.x = width;
  res.viewport.max.y = height;
  res.virtual_width = virtual_width;
  res.virtual_height = virtual_height;
  res.kind = kind;
  res.scaling_type = scaling_type;

  binocle_viewport_adapter_reset(res, window);

  return res;
}

uint32_t binocle_viewport_adapter_get_virtual_width(binocle_viewport_adapter adapter) {
  return adapter.virtual_width;
}

uint32_t binocle_wiewport_adapter_get_virtual_height(binocle_viewport_adapter adapter) {
  return adapter.virtual_height;
}

uint32_t binocle_viewport_adapter_get_viewport_width(binocle_viewport_adapter adapter) {
  return adapter.viewport.max.x;
}

uint32_t binocle_viewport_adapter_get_viewport_height(binocle_viewport_adapter adapter) {
  return adapter.viewport.max.y;
}

kmVec2 binocle_viewport_adapter__point_to_virtual_viewport(binocle_viewport_adapter adapter, kmVec2 point) {
  return point;
}


kmVec2 binocle_viewport_adapter_screen_to_virtual_viewport(binocle_viewport_adapter adapter, kmVec2 point) {
  return point;
}

void binocle_viewport_adapter_reset(binocle_viewport_adapter adapter, binocle_window window) {
  if (adapter.kind == BINOCLE_VIEWPORT_ADAPTER_KIND_SCALING) {
    kmVec2 oldWindowSize = { .x = adapter.virtual_width, .y = adapter.virtual_height};
    kmVec2 newWindowSize = { .x = window.width, .y = window.height };
    float ratioX = newWindowSize.x / oldWindowSize.x;
    float ratioY = newWindowSize.y / oldWindowSize.y;
    kmAABB2 originalViewport = binocle_viewport_adapter_get_original_viewport(adapter);
    if (adapter.scaling_type == BINOCLE_VIEWPORT_ADAPTER_SCALING_TYPE_FREE) {
      adapter.viewport.min.x = originalViewport.min.x * ratioX;
      adapter.viewport.min.y = originalViewport.min.y * ratioY;
      adapter.viewport.max.x = originalViewport.max.x * ratioX;
      adapter.viewport.max.y = originalViewport.max.y * ratioY;
      float scaleX = (float) binocle_viewport_adapter_get_viewport_width(adapter) / adapter.virtual_width;
      float scaleY = (float) binocle_viewport_adapter_get_viewport_height(adapter) / adapter.virtual_height;
      kmMat4 identity;
      kmMat4Identity(&identity);
      kmVec3 origin = {.x = scaleX, .y = scaleY, .z = 1.0f};
      kmMat4Scaling(&adapter.scale_matrix, origin.x, origin.y, origin.z);
    } else if (adapter.scaling_type == BINOCLE_VIEWPORT_ADAPTER_SCALING_TYPE_PIXEL_PERFECT) {
      // minimum multiplier
      int multiplier = 1;
      float scaleX = newWindowSize.x / adapter.virtual_width;
      float scaleY = newWindowSize.y / adapter.virtual_height;

      // find the multiplier that fits both the new width and height
      int maxScale = (int) scaleX < (int) scaleY ? (int) scaleX : (int) scaleY;
      if (maxScale > multiplier) {
        multiplier = maxScale;
      }

      // viewport origin translation
      float diffX = (newWindowSize.x / 2.0f) - (adapter.virtual_width * multiplier / 2.0f);
      float diffY = (newWindowSize.y / 2.0f) - (adapter.virtual_height * multiplier / 2.0f);

      // build the new viewport
      adapter.viewport.min.x = diffX;
      adapter.viewport.min.y = diffY;
      adapter.viewport.max.x = adapter.virtual_width * multiplier;
      adapter.viewport.max.y = adapter.virtual_height * multiplier;

      // compute the scaling matrix
      float matMulX = (adapter.viewport.max.x - adapter.viewport.min.x)/adapter.virtual_width;
      float matMulY = (adapter.viewport.max.y - adapter.viewport.min.y)/adapter.virtual_height;
      kmMat4Identity(&adapter.scale_matrix);
      kmMat4Translation(&adapter.scale_matrix, diffX, diffY, 0.0f);
      kmMat4Scaling(&adapter.scale_matrix, matMulX, matMulY, 1.0f);
    } else if (adapter.scaling_type == BINOCLE_VIEWPORT_ADAPTER_SCALING_TYPE_BOXING) {
      int HorizontalBleed = 16;
      int VerticalBleed = 16;

      auto worldScaleX = (float) newWindowSize.x/adapter.virtual_width;
      auto worldScaleY = (float) newWindowSize.y/adapter.virtual_height;

      auto safeScaleX = (float) newWindowSize.x/(adapter.virtual_width - HorizontalBleed);
      auto safeScaleY = (float) newWindowSize.y/(adapter.virtual_height - VerticalBleed);

      auto worldScale = kmMax(worldScaleX, worldScaleY);
      auto safeScale = kmMin(safeScaleX, safeScaleY);
      auto scale = kmMin(worldScale, safeScale);

      auto width = (int) (scale*adapter.virtual_width + 0.5f);
      auto height = (int) (scale*adapter.virtual_height + 0.5f);

      auto x = newWindowSize.x/2 - width/2;
      auto y = newWindowSize.y/2 - height/2;
      adapter.viewport.min.x = x;
      adapter.viewport.min.y = y;
      adapter.viewport.max.x = width;
      adapter.viewport.max.y = height;

      float scaleX = (float) binocle_viewport_adapter_get_viewport_width(adapter) / adapter.virtual_width;
      float scaleY = (float) binocle_viewport_adapter_get_viewport_height(adapter) / adapter.virtual_height;
      kmMat4 identity;
      kmMat4Identity(&identity);
      kmMat4Scaling(&adapter.scale_matrix, scaleX, scaleY, 1.0f);
    }
  }
}

void binocle_viewport_adapter_set_viewport(binocle_viewport_adapter adapter, kmAABB2 viewport) {
  adapter.viewport = viewport;
  adapter.original_viewport = viewport;
}

kmAABB2 binocle_viewport_adapter_get_viewport(binocle_viewport_adapter adapter) {
  return adapter.viewport;
}

kmAABB2 binocle_viewport_adapter_get_original_viewport(binocle_viewport_adapter adapter) {
  return adapter.original_viewport;
}