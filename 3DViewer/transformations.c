#include "obj_parcer.h"

int move_x(object_t *obj, double value) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    obj->points[i].x += value;
  }
  return code;
}

int move_y(object_t *obj, double value) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    obj->points[i].y += value;
  }
  return code;
}

int move_z(object_t *obj, double value) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    obj->points[i].z += value;
  }
  return code;
}

int scale(object_t *obj, double value) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    obj->points[i].x *= value;
    obj->points[i].y *= value;
    obj->points[i].z *= value;
  }
  return code;
}

int rotate_x(object_t *obj, double angle) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    double y = obj->points[i].y;
    double z = obj->points[i].z;
    obj->points[i].y = y * cos(angle) - z * sin(angle);
    obj->points[i].z = z * cos(angle) + y * sin(angle);
  }
  return code;
}

int rotate_y(object_t *obj, double angle) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    double x = obj->points[i].x;
    double z = obj->points[i].z;
    obj->points[i].x = x * cos(angle) + z * sin(angle);
    obj->points[i].z = z * cos(angle) - x * sin(angle);
  }
  return code;
}

int rotate_z(object_t *obj, double angle) {
  int code = OK;
  if (obj == NULL) {
    code = ERROR;
  }
  for (int i = 0; i < obj->num_vertices; i++) {
    double x = obj->points[i].x;
    double y = obj->points[i].y;
    obj->points[i].x = x * cos(angle) - y * sin(angle);
    obj->points[i].y = y * cos(angle) + x * sin(angle);
  }
  return code;
}

/*int  find_min_max_coords(object_t *obj, double *min_x, double *max_x, double
*min_y, double *max_y, double *min_z, double *max_z) { int code = OK; if (obj ==
NULL) { code = ERROR;
    }
    *min_x = *max_x = obj->points[0].x;
    *min_y = *max_y = obj->points[0].y;
    *min_z = *max_z = obj->points[0].z;

    for (int i = 1; i < obj->num_vertices; i++) {
        if (obj->points[i].x < *min_x) {
            *min_x = obj->points[i].x;
        } else if (obj->points[i].x > *max_x) {
            *max_x = obj->points[i].x;
        }
        if (obj->points[i].y < *min_y) {
            *min_y = obj->points[i].y;
        } else if (obj->points[i].y > *max_y) {
            *max_y = obj->points[i].y;
        }
        if (obj->points[i].z < *min_z) {
            *min_z = obj->points[i].z;
        } else if (obj->points[i].z > *max_z) {
            *max_z = obj->points[i].z;
        }
    }

    return code;
}*/
