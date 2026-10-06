#ifndef BMI088_H
#define BMI088_H

#include "main.h"

typedef struct
{
  int16_t acceleration_x;
  int16_t acceleration_y;
  int16_t acceleration_z;

  int16_t angular_velocity_x;
  int16_t angular_velocity_y;
  int16_t angular_velocity_z;
} Bmi088Data;

HAL_StatusTypeDef bmi088_init(void);
HAL_StatusTypeDef bmi088_read_data(Bmi088Data *data);

#endif
