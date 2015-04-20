#include "vec.h"
#include <check.h>
#include <stdlib.h>

START_TEST (test_vec)
{
	double *vector = vec(4, 7, 2);

	ck_assert_msg(vector[0] == 4.0, "New vector(4, 7, 2) [0] is not 4.0 (%f).", vector[0]);
	ck_assert_msg(vector[1] == 7.0, "New vector(4, 7, 2) [1] is not 7.0 (%f).", vector[1]);
	ck_assert_msg(vector[2] == 2.0, "New vector(4, 7, 2) [2] is not 2.0 (%f).", vector[2]);

	ck_assert_msg(VEC_HEADER(vector)->temp == 1, "New vector is not temporary.");

	vec_del(vector);
}
END_TEST

START_TEST (test_vec_blank)
{
	double *vector = vec_blank(3);

	ck_assert_msg(vector[0] == 0.0, "Blank vector(3) [0] is not 0.0 (%f).", vector[0]);
	ck_assert_msg(vector[1] == 0.0, "Blank vector(3) [0] is not 0.0 (%f).", vector[1]);
	ck_assert_msg(vector[2] == 0.0, "Blank vector(3) [0] is not 0.0 (%f).", vector[2]);

	ck_assert_msg(VEC_HEADER(vector)->temp == 1, "Blank vector is not temporary.");

	vec_del(vector);
}
END_TEST

START_TEST (test_vec_perm)
{
	double *vector = vec(4, 7, 2);

	vec_perm(vector);

	ck_assert_msg(VEC_HEADER(vector)->temp == 0, "Permament vector is not permament.");

	vec_del(vector);
}
END_TEST

START_TEST (test_vec_temp)
{
	double *v1 = vec(4, 7, 2);

	vec_temp(v1);

	ck_assert_msg(VEC_HEADER(v1)->temp == 1, "Temporary vector is not temporary.");

	vec_del(v1);
}
END_TEST


START_TEST (test_vec_math_dot)
{
	double result = vec_dot(vec(10, -4, 2), vec(4, 2, 12));

	ck_assert_msg(result == 56.0, "(10, -4, 2) · (4, 2, 12) is not 56.0 (%f).", result);
}
END_TEST

START_TEST (test_vec_math_cross)
{
	double *result = vec_cross(vec(3, 5, 4), vec(-2, 1, 3));

	ck_assert_msg(result[0] == 11.0, "(3, 5, 4) × (-2, 1, 3) [0] is not 11.0 (%f).", result[0]);
	ck_assert_msg(result[1] == -17.0, "(3, 5, 4) × (-2, 1, 3) [1] is not -17.0 (%f).", result[1]);
	ck_assert_msg(result[2] == 13.0, "(3, 5, 4) × (-2, 1, 3) [2] is not 13.0 (%f).", result[2]);

	vec_del(result);
}
END_TEST

START_TEST (test_vec_math_len)
{
	double result = vec_len(vec(2, -2, 1));

	ck_assert_msg(result == 3, "Length of (2, -2, 1) is not 3 (%f).", result);
}
END_TEST

START_TEST (test_vec_math_addm)
{
	double *result = vec_addm(vec(7, 3, 0), vec(1, -5, 3), 3);

	ck_assert_msg(result[0] == 10.0, "(7, 3, 0) + (1, -5, 3) × 3 [0] is not 10.0 (%f).", result[0]);
	ck_assert_msg(result[1] == -12.0, "(7, 3, 0) + (1, -5, 3) × 3 [1] is not -12.0 (%f).", result[1]);
	ck_assert_msg(result[2] == 9.0, "(7, 3, 0) + (1, -5, 3) × 3 [2] is not 9.0 (%f).", result[2]);

	vec_del(result);
}
END_TEST

Suite *vec_suite(void)
{
	Suite *s;
	TCase *tc_core;
	TCase *tc_math;

	s = suite_create("Vec");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_vec);
	tcase_add_test(tc_core, test_vec_blank);
	tcase_add_test(tc_core, test_vec_perm);
	tcase_add_test(tc_core, test_vec_temp);

	suite_add_tcase(s, tc_core);

	tc_math = tcase_create("Math");

	tcase_add_test(tc_math, test_vec_math_dot);
	tcase_add_test(tc_math, test_vec_math_cross);
	tcase_add_test(tc_math, test_vec_math_len);
	tcase_add_test(tc_math, test_vec_math_addm);

	suite_add_tcase(s, tc_math);

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = vec_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);

	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
