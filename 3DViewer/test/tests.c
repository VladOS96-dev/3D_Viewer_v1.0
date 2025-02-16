#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define EPSILON 1e-4

#include "../obj_parcer.h"

#define LENGTH 256

void *my_malloc() { return NULL; }

static inline int assert_double_eq(double expected, double actual) {
  if (fabs(expected - actual) > EPSILON) {
    printf("expected: %f, actual: %f\n", expected, actual);
    return 0;
  } else {
    return 1;
  }
}

START_TEST(test_obj_create) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  res = create_obj(obj);
  ck_assert_int_eq(res, OK);
  free(obj);
}
END_TEST

START_TEST(test_count_vertices) {
  FILE *fp = fopen("3DViewer/test/models/cube.txt", "r");
  if (fp == NULL) {
    ck_assert_msg(0, "Can't open file");
  }
  int res = count_vertices(fp);
  ck_assert_int_eq(res, 8);
  fclose(fp);
}
END_TEST

START_TEST(test_count_faces) {
  FILE *fp = fopen("3DViewer/test/models/cube.txt", "r");
  if (fp == NULL) {
    ck_assert_msg(0, "Can't open file");
  }
  int res = count_faces(fp);
  ck_assert_int_eq(res, 12);
  fclose(fp);
}
END_TEST

START_TEST(test_pars_2) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  char *filename = "";
  res = parse_obj_file(filename, obj);
  ck_assert_int_eq(res, ERROR);
  free(obj);
}
END_TEST

START_TEST(test_pars_3) {
  int res = 0;
  char *filename = "no_file.obj";
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  res = parse_obj_file(filename, obj);
  ck_assert_int_eq(res, ERROR);
  free(obj);
}
END_TEST

START_TEST(test_clean_1) {
  object_t *obj = malloc(sizeof(object_t));
  create_obj(obj);

  obj->num_vertices = 8;
  obj->num_faces = 6;
  obj->faces = calloc(6, sizeof(face_t));
  obj->points = calloc(8, sizeof(vertex_t));
  clean_obj(obj);

  ck_assert_int_eq(obj->num_vertices, 0);
  ck_assert_int_eq(obj->num_faces, 0);
  ck_assert_ptr_eq(obj->faces, NULL);
  ck_assert_ptr_eq(obj->points, NULL);

  free(obj);
}
END_TEST

START_TEST(test_clean_2) {
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  clean_obj(obj);
  ck_assert_int_eq(obj->num_vertices, 0);
  ck_assert_int_eq(obj->num_faces, 0);
  ck_assert_ptr_eq(obj->faces, NULL);
  ck_assert_ptr_eq(obj->points, NULL);
  free(obj);
}
END_TEST

START_TEST(test_allocate_vertices_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  int num_vertices = 8;
  res = allocate_vertices(obj, num_vertices);
  ck_assert_int_eq(res, OK);

  free(obj->points);
  free(obj);
}
END_TEST

START_TEST(test_allocate_vertices_2) {
  int res = 0;
  object_t *obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  FILE *fp = fopen("3DViewer/test/models/cube.txt", "r");
  if (fp == NULL) {
    ck_assert_msg(0, "Can't open file");
  }
  int num_vertices = count_vertices(fp);
  res = allocate_vertices(obj, num_vertices);
  ck_assert_int_eq(res, OK);
  fclose(fp);
  free(obj->points);
  free(obj);
}

END_TEST

START_TEST(test_allocate_faces_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  int num_faces = 12;
  res = allocate_faces(obj, num_faces);
  ck_assert_int_eq(res, OK);

  free(obj->faces);
  free(obj);
}

END_TEST

START_TEST(test_allocate_faces_2) {
  int res = 0;
  object_t *obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  FILE *fp = fopen("3DViewer/test/models/cube.txt", "r");
  if (fp == NULL) {
    ck_assert_msg(0, "Can't open file");
  }
  int num_faces = count_faces(fp);
  res = allocate_faces(obj, num_faces);
  ck_assert_int_eq(res, OK);
  fclose(fp);
  free(obj->faces);
  free(obj);
}

END_TEST

START_TEST(test_add_face_to_obj) {
  object_t obj;
  obj.num_faces = 0;
  obj.faces = malloc(10 * sizeof(face_t));
  for (int i = 0; i < 10; i++) {
    obj.faces[i].vertex_indices = NULL;
  }

  unsigned indices[3] = {0, 1, 2};
  int result = add_face_to_obj(&obj, indices, 3);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(obj.num_faces, 1);
  ck_assert_int_eq(obj.faces[0].num_indices_in_face, 3);
  ck_assert_uint_eq(obj.faces[0].vertex_indices[0], 0);
  ck_assert_uint_eq(obj.faces[0].vertex_indices[1], 1);
  ck_assert_uint_eq(obj.faces[0].vertex_indices[2], 2);

  free(obj.faces[0].vertex_indices);
  free(obj.faces);
}
END_TEST

START_TEST(test_add_face_max_faces) {
  object_t obj;
  obj.num_faces = 9;
  obj.faces = malloc(10 * sizeof(face_t));
  for (int i = 0; i < 10; i++) {
    obj.faces[i].vertex_indices = NULL;
  }

  unsigned indices[3] = {0, 1, 2};

  int result = add_face_to_obj(&obj, indices, 3);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(obj.num_faces, 10);

  free(obj.faces[9].vertex_indices);
  free(obj.faces);
}
END_TEST

START_TEST(test_add_face_zero_indices) {
  object_t obj;
  obj.num_faces = 0;
  obj.faces = malloc(10 * sizeof(face_t));
  for (int i = 0; i < 10; i++) {
    obj.faces[i].vertex_indices = NULL;
  }

  unsigned indices[1] = {0};

  int result = add_face_to_obj(&obj, indices, 0);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(obj.num_faces, 1);
  ck_assert_int_eq(obj.faces[0].num_indices_in_face, 0);

  free(obj.faces[0].vertex_indices);
  free(obj.faces);
}
END_TEST

START_TEST(test_add_vertex_to_obj) {
  object_t obj;
  obj.num_vertices = 0;
  obj.points = malloc(10 * sizeof(vertex_t));

  double coord[3] = {1.0, 2.0, 3.0};
  int result = add_vertex_to_obj(&obj, coord);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(obj.num_vertices, 1);
  ck_assert_double_eq(obj.points[0].x, 1.0);
  ck_assert_double_eq(obj.points[0].y, 2.0);
  ck_assert_double_eq(obj.points[0].z, 3.0);

  free(obj.points);
}
END_TEST

START_TEST(test_add_vertex_max_vertices) {
  object_t obj;
  obj.num_vertices = 9;
  obj.points = malloc(10 * sizeof(vertex_t));
  double coord[3] = {1.0, 2.0, 3.0};

  int result = add_vertex_to_obj(&obj, coord);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(obj.num_vertices, 10);

  free(obj.points);
}
END_TEST

START_TEST(test_pars_obj_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  char *filename = "3DViewer/test/models/cube.txt";
  res = parse_obj_file(filename, obj);
  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(obj->num_faces, 12);
  ck_assert_int_eq(obj->num_vertices, 8);
  ck_assert_int_eq(obj->line_num, 18);
  clean_obj(obj);
  free(obj);
}
END_TEST

START_TEST(test_move_x_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double val = 2.0;
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  res = move_x(obj, val);
  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(obj->num_vertices, 8);
  ck_assert_double_eq_tol(obj->points[0].x, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[1].x, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[2].x, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[3].x, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[4].x, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[5].x, 2.9999999, EPSILON);
  ck_assert_double_eq_tol(obj->points[6].x, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[7].x, 1.0, EPSILON);
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_move_y_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double val = 2.0;
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  res = move_y(obj, val);
  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(obj->num_vertices, 8);
  ck_assert_double_eq_tol(obj->points[0].y, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[1].y, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[2].y, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[3].y, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[4].y, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[5].y, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[6].y, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[7].y, 3.0, EPSILON);
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_move_z_1) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double angle = 2.0;
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  res = move_z(obj, angle);
  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(obj->num_vertices, 8);
  ck_assert_double_eq_tol(obj->points[0].z, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[1].z, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[2].z, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[3].z, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[4].z, 1.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[5].z, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[6].z, 3.0, EPSILON);
  ck_assert_double_eq_tol(obj->points[7].z, 1.0, EPSILON);
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_scale) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double angle = 2.0;
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  vertex_t original_points[obj->num_vertices];
  for (int i = 0; i < obj->num_vertices; i++) {
    original_points[i] = obj->points[i];
  }
  res = scale(obj, angle);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < obj->num_vertices; i++) {
    ck_assert(assert_double_eq(obj->points[i].x, original_points[i].x * angle));
    ck_assert(assert_double_eq(obj->points[i].y, original_points[i].y * angle));
    ck_assert(assert_double_eq(obj->points[i].z, original_points[i].z * angle));
  }
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_rotate_x) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double angle = M_PI / 2.0;
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  vertex_t original_points[obj->num_vertices];
  for (int i = 0; i < obj->num_vertices; i++) {
    original_points[i] = obj->points[i];
  }
  res = rotate_x(obj, angle);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < obj->num_vertices; i++) {
    ck_assert(assert_double_eq(obj->points[i].x, original_points[i].x));
    ck_assert(assert_double_eq(
        obj->points[i].y,
        original_points[i].y * cos_angle - original_points[i].z * sin_angle));
    ck_assert(assert_double_eq(
        obj->points[i].z,
        original_points[i].y * sin_angle + original_points[i].z * cos_angle));
  }
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_rotate_y) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double angle = M_PI * 2.0;
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  vertex_t original_points[obj->num_vertices];
  for (int i = 0; i < obj->num_vertices; i++) {
    original_points[i] = obj->points[i];
  }
  res = rotate_y(obj, angle);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < obj->num_vertices; i++) {
    ck_assert(assert_double_eq(
        obj->points[i].x,
        original_points[i].x * cos_angle - original_points[i].z * sin_angle));
    ck_assert(assert_double_eq(obj->points[i].y, original_points[i].y));
    ck_assert(assert_double_eq(
        obj->points[i].z,
        original_points[i].x * sin_angle + original_points[i].z * cos_angle));
  }
  clean_obj(obj);
  free(obj);
}

END_TEST

START_TEST(test_rotate_z) {
  int res = 0;
  object_t *obj;
  obj = (object_t *)malloc(sizeof(object_t));
  create_obj(obj);
  double angle = M_PI / 2.0;
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  char *filename = "3DViewer/test/models/cube.txt";
  parse_obj_file(filename, obj);
  vertex_t original_points[obj->num_vertices];
  for (int i = 0; i < obj->num_vertices; i++) {
    original_points[i] = obj->points[i];
  }
  res = rotate_z(obj, angle);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < obj->num_vertices; i++) {
    ck_assert(assert_double_eq(
        obj->points[i].x,
        original_points[i].x * cos_angle - original_points[i].y * sin_angle));
    ck_assert(assert_double_eq(
        obj->points[i].y,
        original_points[i].x * sin_angle + original_points[i].y * cos_angle));
    ck_assert(assert_double_eq(obj->points[i].z, original_points[i].z));
  }
  clean_obj(obj);
  free(obj);
}

END_TEST

Suite *Viewer3D_suite_1(void) {
  Suite *s = suite_create("\x1b[33mTEST_CREATE\x1b[0m");
  TCase *tcase_create_obj = tcase_create("test_obj_create");
  suite_add_tcase(s, tcase_create_obj);
  tcase_add_test(tcase_create_obj, test_obj_create);
  return s;
}

Suite *Viewer3D_suite_2(void) {
  Suite *s = suite_create("\x1b[33mTEST_PARS\x1b[0m");
  TCase *tcase_pars = tcase_create("test_pars");
  suite_add_tcase(s, tcase_pars);
  tcase_add_test(tcase_pars, test_pars_2);
  tcase_add_test(tcase_pars, test_pars_3);
  return s;
}

Suite *Viewer3D_suite_3(void) {
  Suite *s = suite_create("\x1b[33mTEST_CLEAN\x1b[0m");
  TCase *tcase_clean = tcase_create("test_clean");
  suite_add_tcase(s, tcase_clean);
  tcase_add_test(tcase_clean, test_clean_1);
  tcase_add_test(tcase_clean, test_clean_2);
  return s;
}

Suite *Viewer3D_suite_4(void) {
  Suite *s = suite_create("\x1b[33mTEST_COUNT\x1b[0m");
  TCase *tcase_count = tcase_create("test_count_vertices");
  suite_add_tcase(s, tcase_count);
  tcase_add_test(tcase_count, test_count_vertices);
  tcase_add_test(tcase_count, test_count_faces);
  return s;
}

Suite *Viewer3D_suite_5(void) {
  Suite *s = suite_create("\x1b[33mTEST_ALLOCATE\x1b[0m");
  TCase *tcase_allocate = tcase_create("test_allocate_vertices");
  suite_add_tcase(s, tcase_allocate);
  tcase_add_test(tcase_allocate, test_allocate_vertices_1);
  tcase_add_test(tcase_allocate, test_allocate_vertices_2);
  tcase_add_test(tcase_allocate, test_allocate_faces_1);
  tcase_add_test(tcase_allocate, test_allocate_faces_2);
  return s;
}

Suite *Viewer3D_suite_add_vertex(void) {
  Suite *s = suite_create("\x1b[33mTEST_VERTEX\x1b[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_add_vertex_to_obj);
  tcase_add_test(tc_core, test_add_vertex_max_vertices);
  suite_add_tcase(s, tc_core);
  return s;
}

Suite *Viewer3D_suite_add_face(void) {
  Suite *s = suite_create("\x1b[33mTEST_FACE\x1b[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_add_face_to_obj);
  tcase_add_test(tc_core, test_add_face_max_faces);
  tcase_add_test(tc_core, test_add_face_zero_indices);
  suite_add_tcase(s, tc_core);
  return s;
}

Suite *Viewer3D_suite_9(void) {
  Suite *s = suite_create("\x1b[33mTEST_PARSE_OBJ\x1b[0m");
  TCase *tcase_pars_obj = tcase_create("test_pars_obj");
  suite_add_tcase(s, tcase_pars_obj);
  tcase_add_test(tcase_pars_obj, test_pars_obj_1);
  return s;
}

Suite *Viewer3D_suite_10(void) {
  Suite *s = suite_create("\x1b[33mTEST_MOVE\x1b[0m");
  TCase *tcase_move = tcase_create("test_moeve");
  suite_add_tcase(s, tcase_move);
  tcase_add_test(tcase_move, test_move_x_1);
  tcase_add_test(tcase_move, test_move_y_1);
  tcase_add_test(tcase_move, test_move_z_1);
  return s;
}

Suite *Viewer3D_suite_11(void) {
  Suite *s = suite_create("\x1b[33mTEST_SCALE\x1b[0m");
  TCase *tcase_scale = tcase_create("test_scale");
  suite_add_tcase(s, tcase_scale);
  tcase_add_test(tcase_scale, test_scale);
  return s;
}

Suite *Viewer3D_suite_12(void) {
  Suite *s = suite_create("\x1b[33mTEST_ROTATE\x1b[0m");
  TCase *tcase_rotate = tcase_create("test_rotate");
  suite_add_tcase(s, tcase_rotate);
  tcase_add_test(tcase_rotate, test_rotate_x);
  tcase_add_test(tcase_rotate, test_rotate_y);
  tcase_add_test(tcase_rotate, test_rotate_z);
  return s;
}

int main(void) {
  Suite *list_cases[] = {Viewer3D_suite_1(),        Viewer3D_suite_2(),
                         Viewer3D_suite_3(),        Viewer3D_suite_4(),
                         Viewer3D_suite_5(),        Viewer3D_suite_add_vertex(),
                         Viewer3D_suite_add_face(), Viewer3D_suite_9(),
                         Viewer3D_suite_10(),       Viewer3D_suite_11(),
                         Viewer3D_suite_12(),       NULL};

  int fail_count = 0;
  printf("\x1b[36mS21_3DVIEWER TESTS...\x1b[0m\n");
  for (int i = 0; list_cases[i] != NULL; i++) {
    SRunner *suite_runner = srunner_create(list_cases[i]);
    srunner_set_fork_status(suite_runner, CK_NOFORK);
    srunner_run_all(suite_runner, CK_NORMAL);
    fail_count += srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);
  }
  printf("\x1b[32mTOTAL NUMBER OF FAILURES: %d\x1b[0m\n", fail_count);
  return fail_count;
}