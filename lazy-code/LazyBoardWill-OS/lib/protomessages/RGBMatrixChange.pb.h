/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.3-dev */

#ifndef PB_RGBMATRIXCHANGE_PB_H_INCLUDED
#define PB_RGBMATRIXCHANGE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _RGBMatrixChange_RGBAnimationType {
    RGBMatrixChange_RGBAnimationType_REACTIVE_PULSE = 0,
    RGBMatrixChange_RGBAnimationType_REACTIVE_RIPPLE = 1,
    RGBMatrixChange_RGBAnimationType_PULSE_INDIVIDUUAL = 2
} RGBMatrixChange_RGBAnimationType;

/* Struct definitions */
typedef struct _RGBMatrixChange {
    RGBMatrixChange_RGBAnimationType animation;
} RGBMatrixChange;


/* Helper constants for enums */
#define _RGBMatrixChange_RGBAnimationType_MIN RGBMatrixChange_RGBAnimationType_REACTIVE_PULSE
#define _RGBMatrixChange_RGBAnimationType_MAX RGBMatrixChange_RGBAnimationType_PULSE_INDIVIDUUAL
#define _RGBMatrixChange_RGBAnimationType_ARRAYSIZE ((RGBMatrixChange_RGBAnimationType)(RGBMatrixChange_RGBAnimationType_PULSE_INDIVIDUUAL+1))


/* Initializer values for message structs */
#define RGBMatrixChange_init_default             {_RGBMatrixChange_RGBAnimationType_MIN}
#define RGBMatrixChange_init_zero                {_RGBMatrixChange_RGBAnimationType_MIN}

/* Field tags (for use in manual encoding/decoding) */
#define RGBMatrixChange_animation_tag            1

/* Struct field encoding specification for nanopb */
#define RGBMatrixChange_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    animation,         1)
#define RGBMatrixChange_CALLBACK NULL
#define RGBMatrixChange_DEFAULT NULL

extern const pb_msgdesc_t RGBMatrixChange_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define RGBMatrixChange_fields &RGBMatrixChange_msg

/* Maximum encoded size of messages (where known) */
#define RGBMatrixChange_size                     2

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
