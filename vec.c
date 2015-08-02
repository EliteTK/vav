#include "vec.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define _VEC_MIN_LENGTH(a, b) ({ \
		double _a = VEC_HEADER(a)->length; \
		double _b = VEC_HEADER(b)->length; \
		_a < _b ? _a : _b; \
		})

static inline bool length_will_overflow(const uint64_t length)
{
	if (length > (UINT64_MAX / 2) - sizeof(struct vec_header))
		return true;

	return false;
}

double *vec_new(const uint64_t length, const double * const values)
{
	struct vec_header *header;

	if (length_will_overflow(length))
		return NULL;

	/* WARNING: Assuming sizeof header == sizeof (double) is a bad idea */
	if (!(header = malloc(sizeof header * (length + 1))))
		return NULL;

	header->temp = true;
	header->length = length;

	memcpy(header->vector, values, length * sizeof (double));

	return (double *)(header->vector);
}

double *vec_blank(const uint64_t length)
{
	struct vec_header *header;

	if (length_will_overflow(length))
		return NULL;

	/* WARNING: Assuming sizeof header == sizeof (double) is a bad idea */
	if (!(header = calloc(length + 1, sizeof header)))
		return NULL;

	header->temp = true;
	header->length = length;

	return (double *)(header->vector);
}

void vec_del(double * const vector)
{
	free(VEC_HEADER(vector));
}

void vec_deltemp(double * const vector)
{
	if (VEC_HEADER(vector)->temp)
		vec_del(vector);
}


double vec_dot(double * const vector_a, double * const vector_b)
{
	uint64_t length = _VEC_MIN_LENGTH(vector_a, vector_b);
	double result = 0;

	if (!vector_a || !vector_b)
		return 0; /* TODO: Find some way of dealing with errors */

	for (uint64_t i = 0; i < length; i++)
		result += vector_a[i] * vector_b[i];

	if (vector_b != vector_a)
		vec_deltemp(vector_b);

	vec_deltemp(vector_a);

	return result;
}

double *vec_cross(double * const vector_a, double * const vector_b)
{
	double i, j, k;
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);

	if (!vector_a || !vector_b)
		return NULL;

	if (vech_a->length != vech_b->length && vech_a->length != 3)
		return NULL;

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
		return vec_new(3, (double []){i, j, k});
	}
}

double vec_len(double * const vector)
{
	return sqrtf(vec_dot(vector, vector));
}

double *vec_addm(double * const vector_a, double * const vector_b,
		 const double multiplier)
{
	struct vec_header *vech_a = VEC_HEADER(vector_a);
	struct vec_header *vech_b = VEC_HEADER(vector_b);
	uint64_t length = _VEC_MIN_LENGTH(vector_a, vector_b);
	double *output;

	if (!vector_a || !vector_b)
		return NULL;

	if (vech_a->temp)
		output = vector_a;
	else if (vech_b->temp)
		output = vector_b;
	else
		output = vec_blank(length);

	for (uint64_t i = 0; i < length; i++)
		output[i] = vector_a[i] + vector_b[i] * multiplier;

	if (vech_a->temp && vech_b->temp)
		vec_deltemp(vector_b);

	return output;
}

double *vec_norm(double * const vector)
{
	struct vec_header *vech = VEC_HEADER(vector);
	double *output;
	double length;

	if (!vector)
		return NULL;

#ifdef VAV_FAST
	if (vech->temp) {
		length = 1 / vec_len(vec_perm(vector));
		vec_temp(vector);
		output = vector;
	} else {
		length = 1 / vec_len(vector);
		output = vec_blank(vech->length);
	}

	for (uint64_t i = 0; i < vech->length; i++)
		output[i] = vector[i] * length;
#else

	if (vech->temp) {
		length = vec_len(vec_perm(vector));
		vec_temp(vector);
		output = vector;
	} else {
		length = vec_len(vector);
		output = vec_blank(vech->length);
	}

	for (uint64_t i = 0; i < vech->length; i++)
		output[i] = vector[i] / length;
#endif

	return output;
}
