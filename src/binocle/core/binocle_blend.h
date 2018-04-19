//
// Created by Valerio Santinelli on 19/04/18.
//

#ifndef BINOCLE_BINOCLE_BLEND_H
#define BINOCLE_BINOCLE_BLEND_H

/**
 * \brief Enumeration of the blending factors
 *
 * The factors are mapped directly to their OpenGL equivalents,
 * specified by glBlendFunc() or glBlendFuncSeparate().
 */
typedef enum binocle_blend_factor {
    BINOCLE_BLEND_ZERO,             ///< (0, 0, 0, 0)
    BINOCLE_BLEND_ONE,              ///< (1, 1, 1, 1)
    BINOCLE_BLEND_SRCCOLOR,         ///< (src.r, src.g, src.b, src.a)
    BINOCLE_BLEND_ONEMINUSSRCCOLOR, ///< (1, 1, 1, 1) - (src.r, src.g, src.b, src.a)
    BINOCLE_BLEND_DSTCOLOR,         ///< (dst.r, dst.g, dst.b, dst.a)
    BINOCLE_BLEND_ONEMINUSDSTCOLOR, ///< (1, 1, 1, 1) - (dst.r, dst.g, dst.b, dst.a)
    BINOCLE_BLEND_SRCALPHA,         ///< (src.a, src.a, src.a, src.a)
    BINOCLE_BLEND_ONEMINUSSRCALPHA, ///< (1, 1, 1, 1) - (src.a, src.a, src.a, src.a)
    BINOCLE_BLEND_DSTALPHA,         ///< (dst.a, dst.a, dst.a, dst.a)
    BINOCLE_BLEND_ONEMINUSDSTALPHA  ///< (1, 1, 1, 1) - (dst.a, dst.a, dst.a, dst.a)
} binocle_blend_factor;

/**
 * \brief Enumeration of the blending equations
 *
 * The equations are mapped directly to their OpenGL equivalents,
 * specified by glBlendEquation() or glBlendEquationSeparate().
 */
typedef enum binocle_blend_equation {
    BINOCLE_BLEND_ADD,     ///< Pixel = Src * SrcFactor + Dst * DstFactor
    BINOCLE_BLEND_SUBTRACT ///< Pixel = Src * SrcFactor - Dst * DstFactor
} binocle_blend_equation;

typedef struct binocle_blend {
    binocle_blend_factor color_src_factor; ///< Source blending factor for the color channels
    binocle_blend_factor color_dst_factor; ///< Destination blending factor for the color channels
    binocle_blend_equation color_equation;  ///< Blending equation for the color channels
    binocle_blend_factor alpha_src_factor; ///< Source blending factor for the alpha channel
    binocle_blend_factor alpha_dst_factor; ///< Destination blending factor for the alpha channel
    binocle_blend_equation alpha_equation;  ///< Blending equation for the alpha channel
} binocle_blend;

binocle_blend binocle_blend_new(binocle_blend_factor color_source_factor, binocle_blend_factor color_destination_factor,
                                binocle_blend_equation color_blend_equation, binocle_blend_factor alpha_source_factor,
                                binocle_blend_factor alpha_destination_factor, binocle_blend_equation alpha_blend_equation);
binocle_blend binocle_blend_new_simple(binocle_blend_factor source_factor, binocle_blend_factor destination_factor,
                                       binocle_blend_equation blend_equation);
binocle_blend binocle_blend_new_default();

#endif //BINOCLE_BINOCLE_BLEND_H
