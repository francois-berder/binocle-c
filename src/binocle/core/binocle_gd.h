//
// Created by Valerio Santinelli on 19/04/18.
//

#ifndef BINOCLE_BINOCLE_GD_H
#define BINOCLE_BINOCLE_GD_H

#include <kazmath/kazmath.h>
#include "binocle_sdl.h"

struct binocle_blend;
struct binocle_shader;
struct binocle_texture;
struct binocle_vpct;
struct binocle_material;

enum binocle_blend_factor;
enum binocle_blend_equation;

typedef struct binocle_gd {
    GLuint vbo;
    GLuint vertex_attribute;
    GLuint color_attribute;
    GLuint tex_coord_attribute;
    GLuint normal_attribute;
    GLint image_uniform;
    GLint projection_matrix_uniform;
    //GLint model_view_matrix_uniform;
    GLint view_matrix_uniform;
    GLint model_matrix_uniform;
} binocle_gd;

#define glCheck(expr) do { expr; binocle_gd_gl_check_error(__FILE__, __LINE__, #expr); } while (false)
void binocle_gd_gl_check_error(const char *file, unsigned int line, const char *expression);

binocle_gd binocle_gd_new();
void binocle_gd_init(binocle_gd *gd);
kmMat4 binocle_gd_create_model_view_matrix(float x, float y, float scale, float rotation);
void binocle_gd_draw(binocle_gd *gd, const struct binocle_vpct *vertices, size_t vertex_count, struct binocle_material material, kmAABB2 viewport);
void binocle_gd_apply_gl_states();
void binocle_gd_apply_viewport(kmAABB2 viewport);
void binocle_gd_apply_blend_mode(const struct binocle_blend blend_mode);
void binocle_gd_apply_shader(binocle_gd *gd, struct binocle_shader shader);
void binocle_gd_apply_texture(struct binocle_texture texture);
GLuint binocle_gd_factor_to_gl_constant(enum binocle_blend_factor blend_factor);
GLuint binocle_gd_equation_to_gl_constant(enum binocle_blend_equation blend_equation);

#endif //BINOCLE_BINOCLE_GD_H