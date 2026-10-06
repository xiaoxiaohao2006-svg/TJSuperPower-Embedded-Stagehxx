#include "bmi088.h"

extern SPI_HandleTypeDef hspi1;

#define BMI088_ACCEL_CHIP_ID_REGISTER 0x00
#define BMI088_ACCEL_CHIP_ID 0x1E
#define BMI088_ACCEL_POWER_CONTROL_REGISTER 0x7D
#define BMI088_ACCEL_POWER_CONTROL_VALUE 0x04
#define BMI088_ACCEL_CONFIG_REGISTER 0x40
#define BMI088_ACCEL_CONFIG_VALUE 0xA8
#define BMI088_ACCEL_RANGE_REGISTER 0x41
#define BMI088_ACCEL_RANGE_VALUE 0x00
#define BMI088_ACCEL_DATA_REGISTER 0x12

#define BMI088_GYRO_CHIP_ID_REGISTER 0x00
#define BMI088_GYRO_CHIP_ID 0x0F
#define BMI088_GYRO_RANGE_REGISTER 0x0F
#define BMI088_GYRO_RANGE_VALUE 0x00
#define BMI088_GYRO_BANDWIDTH_REGISTER 0x10
#define BMI088_GYRO_BANDWIDTH_VALUE 0x07
#define BMI088_GYRO_DATA_REGISTER 0x02

static void bmi088_acceleration_write_register(
    uint8_t register_address,
    uint8_t data)
{
  uint8_t transmit_data[2];

  transmit_data[0] = register_address & 0x7F;
  transmit_data[1] = data;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  HAL_SPI_Transmit(
      &hspi1,
      transmit_data,
      2,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static uint8_t bmi088_acceleration_read_register(
    uint8_t register_address)
{
  uint8_t transmit_data[2];
  uint8_t receive_data[2];

  transmit_data[0] = register_address | 0x80;
  transmit_data[1] = 0x00;

  receive_data[0] = 0x00;
  receive_data[1] = 0x00;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  HAL_SPI_TransmitReceive(
      &hspi1,
      transmit_data,
      receive_data,
      2,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  return receive_data[1];
}

static void bmi088_gyroscope_write_register(
    uint8_t register_address,
    uint8_t data)
{
  uint8_t transmit_data[2];

  transmit_data[0] = register_address & 0x7F;
  transmit_data[1] = data;

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  HAL_SPI_Transmit(
      &hspi1,
      transmit_data,
      2,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

static uint8_t bmi088_gyroscope_read_register(
    uint8_t register_address)
{
  uint8_t transmit_data[2];
  uint8_t receive_data[2];

  transmit_data[0] = register_address | 0x80;
  transmit_data[1] = 0x00;

  receive_data[0] = 0x00;
  receive_data[1] = 0x00;

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  HAL_SPI_TransmitReceive(
      &hspi1,
      transmit_data,
      receive_data,
      2,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  return receive_data[1];
}

static HAL_StatusTypeDef bmi088_acceleration_read_data(
    int16_t *acceleration_x,
    int16_t *acceleration_y,
    int16_t *acceleration_z)
{
  uint8_t transmit_data[7];
  uint8_t receive_data[7];

  transmit_data[0] = BMI088_ACCEL_DATA_REGISTER | 0x80;
  transmit_data[1] = 0x00;
  transmit_data[2] = 0x00;
  transmit_data[3] = 0x00;
  transmit_data[4] = 0x00;
  transmit_data[5] = 0x00;
  transmit_data[6] = 0x00;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
      &hspi1,
      transmit_data,
      receive_data,
      7,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  if (status != HAL_OK)
  {
    return status;
  }

  *acceleration_x = (int16_t)(
      ((uint16_t)receive_data[2] << 8) |
      receive_data[1]);

  *acceleration_y = (int16_t)(
      ((uint16_t)receive_data[4] << 8) |
      receive_data[3]);

  *acceleration_z = (int16_t)(
      ((uint16_t)receive_data[6] << 8) |
      receive_data[5]);

  return HAL_OK;
}

static HAL_StatusTypeDef bmi088_gyroscope_read_data(
    int16_t *angular_velocity_x,
    int16_t *angular_velocity_y,
    int16_t *angular_velocity_z)
{
  uint8_t transmit_data[7];
  uint8_t receive_data[7];

  transmit_data[0] = BMI088_GYRO_DATA_REGISTER | 0x80;
  transmit_data[1] = 0x00;
  transmit_data[2] = 0x00;
  transmit_data[3] = 0x00;
  transmit_data[4] = 0x00;
  transmit_data[5] = 0x00;
  transmit_data[6] = 0x00;

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
      &hspi1,
      transmit_data,
      receive_data,
      7,
      HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  if (status != HAL_OK)
  {
    return status;
  }

  *angular_velocity_x = (int16_t)(
      ((uint16_t)receive_data[2] << 8) |
      receive_data[1]);

  *angular_velocity_y = (int16_t)(
      ((uint16_t)receive_data[4] << 8) |
      receive_data[3]);

  *angular_velocity_z = (int16_t)(
      ((uint16_t)receive_data[6] << 8) |
      receive_data[5]);

  return HAL_OK;
}

HAL_StatusTypeDef bmi088_init(void)
{
  uint8_t acceleration_chip_id;
  uint8_t gyroscope_chip_id;

  HAL_GPIO_WritePin(
      GPIOA,
      GPIO_PIN_4,
      GPIO_PIN_SET);

  HAL_GPIO_WritePin(
      GPIOB,
      GPIO_PIN_0,
      GPIO_PIN_SET);

  HAL_Delay(1);

  /*
   * BMI088 acceleration sensor starts in I2C mode.
   * A dummy SPI read switches it to SPI mode.
   */
  bmi088_acceleration_read_register(
      BMI088_ACCEL_CHIP_ID_REGISTER);

  HAL_Delay(1);

  acceleration_chip_id =
      bmi088_acceleration_read_register(
          BMI088_ACCEL_CHIP_ID_REGISTER);

  if (acceleration_chip_id != BMI088_ACCEL_CHIP_ID)
  {
    return HAL_ERROR;
  }

  gyroscope_chip_id =
      bmi088_gyroscope_read_register(
          BMI088_GYRO_CHIP_ID_REGISTER);

  if (gyroscope_chip_id != BMI088_GYRO_CHIP_ID)
  {
    return HAL_ERROR;
  }

  bmi088_acceleration_write_register(
      BMI088_ACCEL_POWER_CONTROL_REGISTER,
      BMI088_ACCEL_POWER_CONTROL_VALUE);

  HAL_Delay(50);

  bmi088_acceleration_write_register(
      BMI088_ACCEL_CONFIG_REGISTER,
      BMI088_ACCEL_CONFIG_VALUE);

  bmi088_acceleration_write_register(
      BMI088_ACCEL_RANGE_REGISTER,
      BMI088_ACCEL_RANGE_VALUE);

  bmi088_gyroscope_write_register(
      BMI088_GYRO_RANGE_REGISTER,
      BMI088_GYRO_RANGE_VALUE);

  bmi088_gyroscope_write_register(
      BMI088_GYRO_BANDWIDTH_REGISTER,
      BMI088_GYRO_BANDWIDTH_VALUE);

  HAL_Delay(10);

  return HAL_OK;
}

HAL_StatusTypeDef bmi088_read_data(Bmi088Data *data)
{
  HAL_StatusTypeDef status;

  if (data == NULL)
  {
    return HAL_ERROR;
  }

  status = bmi088_acceleration_read_data(
      &data->acceleration_x,
      &data->acceleration_y,
      &data->acceleration_z);

  if (status != HAL_OK)
  {
    return status;
  }

  status = bmi088_gyroscope_read_data(
      &data->angular_velocity_x,
      &data->angular_velocity_y,
      &data->angular_velocity_z);

  return status;
}
