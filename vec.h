#ifndef __VEC_H

#include <stdint.h>

struct vec_header {
	uint64_t temp   : 1;
	uint64_t length : 63;
	double vector[];
};

double *vec_new(const uint64_t, const double * const);
double *vec_blank(const uint64_t);
void vec_del(double * const);
void vec_deltemp(double * const);
double *vec_temp(double * const);
double *vec_perm(double * const);

double vec_dot(double * const, double * const);
double *vec_cross(double * const, double * const);
double vec_len(double * const);
double *vec_addm(double * const, double * const, const double);

#define VEC_HEADER(vector) ((struct vec_header *)(vector - 1))

#define vec(...) vec_new(sizeof((double []){ __VA_ARGS__ }) / sizeof(double), (double []){ __VA_ARGS__ })

#define __VEC_H
#endif /* __VEC_H */
