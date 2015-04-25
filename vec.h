/** @file */
#ifndef __VEC_H
#define __VEC_H

#include <stdint.h>

/** Underlying vector datatype for storing vector metadata.
 * @see vec_new()
 */
struct vec_header {
	uint64_t temp   : 1;	/**< This value indicates whether the vector is
				 * temporary. Temporary vectors can be
				 * overwritten and reused by VAV functions.
				 */
	uint64_t length : 63;	/**< This value indicates the length of the
				 * stored vector.
				 */
	double vector[];	/**< This indicates the start of the memory
				 * which holds the vector.
				 */
};

/** Get the header of a vector.
 * This macro, when used on a vector, provides a quick method of accessing the
 * vector's header.
 * @param vector The vector for which the header is to be returned.
 */
#define VEC_HEADER(vector) ((struct vec_header *)(vector - 1))

/** Vector constructor.
 * Creates a new vector from a @a length and a pointer to memory containing the
 * @a values.
 * @param length The length of the new vector.
 * @param values The values the vector should hold.
 * @return A pointer to memory which can be treated as an array of doubles.
 * (Should never be directly freed.)
 * @see vec_blank()
 */
double *vec_new(const uint64_t length, const double * const values);

/** Vector constructor (zero).
 * Creates a new blank (zero) vector from a length.
 * @param length The length of the new vector.
 * @return A pointer to memory which can be treated as an array of doubles.
 * (Should never be directly freed.)
 * @see vec_new()
 */
double *vec_blank(const uint64_t length);

/** Vector deconstructor.
 * Frees the vector correctly.
 * @param vector The vector to be freed.
 * @see vec_deltemp()
 */
void vec_del(double * const vector);

/** Vector deconstructor (temp).
 * Frees the vector only if it is temporary.
 * @param vector The vector to be freed.
 * @see vec_del()
 */
void vec_deltemp(double * const vector);

/*! Make the vector temporary.
 * Sets the vector passed to be temporary.
 * @param vector The vector to be made temporary.
 * @see vec_header
 * @see vec_perm()
 * @return The passed in vector (temporary).
 */
static inline double *vec_temp(double * const vector)
{
	VEC_HEADER(vector)->temp = 1;
	return vector;
}

/*! Make the vector permanent.
 * @param vector The vector to be made permanent.
 * @return The passed in vector (permanent).
 * @see vec_header
 * @see vec_perm()
 */
static inline double *vec_perm(double * const vector)
{
	VEC_HEADER(vector)->temp = 0;
	return vector;
}


/*! Dot product.
 * Return the dot product of two vectors.
 * @param vector_a One of two vectors to be multiplied together.
 * @param vector_b One of two vectors to be multiplied together.
 * @return The dot product of vector_a and vector_b.
 * @see vec_cross()
 */
double vec_dot(double * const vector_a, double * const vector_b);

/*! Cross product.
 * Return the cross product of two vectors.
 * @param vector_a One of two vectors to be multiplied together.
 * @param vector_b One of two vectors to be multiplied together.
 * @return The cross product of vector_a and vector_b.
 * @see vec_dot()
 */
double *vec_cross(double * const vector_a, double * const vector_b);

/*! Vector length.
 * Return the length of the vector (actual length not number of elements).
 * @param vector The vector for which length is to be calculated.
 * @return The length of the vector.
 */
double vec_len(double * const vector);

/*! Vector addition (with multiplication).
 * Adds two vectors together, multiplying the second vector by the specified
 * multiplier.
 * @param vector_a The vector to which vector_b is to be added.
 * @param vector_b The vector which is meant to be multiplied by the multiplier
 * before being added to vector_a.
 * @param multiplier The multiplier which is to be applied to vector_b before
 * addition.
 * @return The resulting vector.
 */
double *vec_addm(double * const vector_a, double * const vector_b, const double multiplier);

/** Vector normalization.
 * Normalizes the passed in vector.
 * @param vector The vector to be normalized.
 * @return The normalized vector.
 */
double *vec_norm(double * const vector);

/** Vector constructor (code sugar).
 * vec() is a macro which counts the number of parameters it receives, creates
 * an array initializer from the parameters and passes these two things to
 * vec_new().
 * @return A vector created from the passed in parameters.
 */
#define vec(...) vec_new(sizeof((double []){ __VA_ARGS__ }) / sizeof(double), (double []){ __VA_ARGS__ })

/** Use optimisations.
 * When VEC_FAST is defined during the compilation of vec.c, certain
 * optimisations may be used which might cause a loss of precision.
 */
#define VEC_FAST

#endif /* __VEC_H */
