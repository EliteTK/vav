/* vec.c */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vec.h"

#define _VEC_MIN_LENGTH(a, b) ({ \
		float _a = VEC_HEADER(a)->length; \
		float _b = VEC_HEADER(b)->length; \
		_a < _b ? _a : _b; \
		})


/* Creation / Deletion. */

float *vec_new(const uint32_t length, const float * const values)
{
	struct vec_header *vector = malloc(sizeof(struct vec_header) * (length + 1));

	vector->temp   = 1;
	vector->length = length;

	memcpy(vector + 1, values, length * sizeof(float));

	return (float *)(vector + 1);
}

float *vec_blank(const uint32_t length)
{
	struct vec_header *vector = calloc(sizeof(struct vec_header), (length + 1));

	vector->temp   = 1;
	vector->length = length;

	return (float *)(vector + 1);
}

void vec_del(float * const vector)
{
	free(vector - 1);
}

void vec_deltemp(float * const vector)
{
	if (VEC_HEADER(vector)->temp)
		vec_del(vector);
}

inline float *vec_temp(float * const vector)
{
	VEC_HEADER(vector)->temp = 1;
	return vector;
}

inline float *vec_perm(float * const vector) {
	VEC_HEADER(vector)->temp = 0;
	return vector;
}


/* Mathematical. */

float vec_dot(float * const vector_a, float * const vector_b)
{
	uint32_t length = _VEC_MIN_LENGTH(vector_a, vector_b);

	float result = 0;

	for (uint32_t i = 0; i < length; i++)
		result += vector_a[i] * vector_b[i];

	vec_deltemp(vector_a);
	vec_deltemp(vector_b);

	return result;
}

float *vec_cross(float * const vector_a, float * const vector_b)
{
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);

	if (vech_a->length != vech_b->length && vech_a->length != 3)
		return NULL;

	float i, j, k;
	i = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
	j = vector_a[2] * vector_b[0] - vector_a[0] * vector_b[2];
	k = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];

	if (vech_a->temp) {
		vec_deltemp(vector_b);
		vector_a[0] = i;
		vector_a[1] = j;
		vector_a[2] = k;
		return vector_a;
	} else if (vech_b->temp) {
		vector_b[0] = i;
		vector_b[1] = j;
		vector_b[2] = k;
		return vector_b;
	} else {
		return vec_new(3, (float []){i, j, k});
	}
}

float vec_len(float * const vector_a)
{
	return sqrtf(vec_dot(vector_a, vector_a));
}

float *vec_addm(float * const vector_a, float * const vector_b, const float multiplier)
{
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);

	uint32_t length = _VEC_MIN_LENGTH(vector_a, vector_b);

	float *output;

	if (vech_a->temp)
		output = vector_a;
	else if (vech_b->temp)
		output = vector_b;
	else
		output = vec_blank(length);

	for (uint32_t i = 0; i < length; i++)
		output[i] = vector_a[i] + vector_b[i] * multiplier;

	if (vech_a->temp && vech_b->temp)
		vec_deltemp(vector_b);
	else if (vech_b->temp);
	else {
		vec_deltemp(vector_a);
		vec_deltemp(vector_b);
	}

	return output;
}

#undef _VEC_MIN_LENGTH
