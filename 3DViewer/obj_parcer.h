
#ifndef __HEADER_H
#define __HEADER_H
#define _GNU_SOURCE

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 256

typedef enum {
  OK,
  ERROR,
} return_code;

typedef struct {
  unsigned int *vertex_indices;
  int num_indices_in_face;
} face_t;

typedef struct {
  double x, y, z;
} vertex_t;

typedef struct {
  int num_vertices;
  int num_faces;
  int line_num;
  face_t *faces;
  vertex_t *points;
} object_t;

int create_obj(object_t *obj);
int add_vertex_to_obj(object_t *obj, double *coord);
int add_face_to_obj(object_t *obj, unsigned *indices, int num_indices);
int write_coord_vertex(char *line, object_t *obj);
int write_ind_vertices(char *line, object_t *obj);
void clean_obj(object_t *obj_ptr);
int parse_obj_file(const char *filename, object_t *obj);
int is_digit(char c);
int allocate_vertices(object_t *obj, int num_vertices);
int allocate_faces(object_t *obj, int num_faces);
int count_vertices(FILE *file);
int count_faces(FILE *file);
int move_x(object_t *obj, double value);
int move_y(object_t *obj, double value);
int move_z(object_t *obj, double value);
int rotate_x(object_t *obj, double value);
int rotate_y(object_t *obj, double value);
int rotate_z(object_t *obj, double value);
int scale(object_t *obj, double value);
float parse_float_number(char *str);
vertex_t parse_vertex(char *str);
int find_min_max_coords(object_t *obj, double *min_x, double *max_x,
                        double *min_y, double *max_y, double *min_z,
                        double *max_z);

#endif  //__HEADER_H
