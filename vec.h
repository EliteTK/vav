#ifndef __VEC_H
#define __VEC_H

#include <stdint.h>

struct vec_header {
	uint64_t temp   : 1;
	uint64_t length : 63;
	double vector[];
};

#define VEC_HEADER(vector) ((struct vec_header *)(vector - 1))

double *vec_new(const uint64_t length, const double * const values);
double *vec_blank(const uint64_t length);
void vec_del(double * const vector);
void vec_deltemp(double * const vector);

static inline double *vec_temp(double * const vector)
{
	VEC_HEADER(vector)->temp = 1;
	return vector;
}

static inline double *vec_perm(double * const vector)
{
	VEC_HEADER(vector)->temp = 0;
	return vector;
}


double vec_dot(double * const vector_a, double * const vector_b);
double *vec_cross(double * const vector_a, double * const vector_b);
double vec_len(double * const vector);
double *vec_addm(double * const vector_a, double * const vector_b, const double multiplier);
double *vec_norm(double * const vector);

#define vec(...) vec_new(sizeof((double []){ __VA_ARGS__ }) / sizeof(double), (double []){ __VA_ARGS__ })

#define VEC_FAST

#endif /* __VEC_H */
