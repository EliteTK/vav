/* vec.h */
#ifndef __VEC_H

#include <stdint.h>

struct vec_header {
	uint32_t temp   : 1;
	uint32_t length : 31;
	float vector[];
};

float *vec_new(const uint32_t, const float * const);
float *vec_blank(const uint32_t);
void vec_del(float * const);
void vec_deltemp(float * const);
float *vec_temp(float * const);
float *vec_perm(float * const);

float vec_dot(float * const, float * const);
float *vec_cross(float * const, float * const);
float vec_len(float * const);
float *vec_addm(float * const, float * const, const float);

#define VEC_HEADER(vector) ((struct vec_header *)(vector - 1))

#define SIZEOF(array) (sizeof(array) / sizeof(typeof(array[0])))

#define vec(...) vec_new(SIZEOF( ((float []){ __VA_ARGS__ }) ), (float []){ __VA_ARGS__ })

#define __VEC_H
#endif
