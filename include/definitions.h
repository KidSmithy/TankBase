#include <cstdint>
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/rotation.hpp"
#include <cmath>
#include <sstream>
#include <vector>
#include "pros/imu.hpp"

// #define LEFT_FRONT_MOTOR 13
// #define LEFT_MIDDLE_MOTOR 12
// #define LEFT_BACK_MOTOR 11

// #define RIGHT_FRONT_MOTOR 7
// #define RIGHT_MIDDLE_MOTOR 8
// #define RIGHT_BACK_MOTOR 10

// #define CONVEYOR_MOTOR 9

// #define LEFT_LIFT_MOTOR 18
// #define RIGHT_LIFT_MOTOR 10

// #define UPPER_INTAKE_MOTOR 15
// #define LOWER_INTAKE_MOTOR 14

// #define IMU_SENSOR_PORT 17
// #define SERIALPORT 19

// #define SOLENOID_SENSOR_PORT 'H'

#define LEFT_FRONT_MOTOR 13
#define LEFT_MIDDLE_MOTOR 12
#define LEFT_BACK_MOTOR 1

#define RIGHT_FRONT_MOTOR 7
#define RIGHT_MIDDLE_MOTOR 8
#define RIGHT_BACK_MOTOR 6

#define CONVEYOR_MOTOR 9

#define LEFT_LIFT_MOTOR 18
#define RIGHT_LIFT_MOTOR 10

#define UPPER_INTAKE_MOTOR 15
#define LOWER_INTAKE_MOTOR 14

#define IMU_SENSOR_PORT 5
#define SERIALPORT 19

#define SOLENOID_SENSOR_PORT 'H'

#define FRONT_ROLLER_SENSOR_PORT 'F'

#define SLAM_DUNK_SENSOR_PORT 'G'

#define ZERO_VECTOR INFINITY

#define SLAM_DUNK_L 16
#define SLAM_DUNK_R 18

pros::Controller master(pros::E_CONTROLLER_MASTER);

// pros::Motor lf(LEFT_FRONT_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor lm(LEFT_MIDDLE_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor lb(LEFT_BACK_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

// pros::Motor rf(RIGHT_FRONT_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor rm(RIGHT_MIDDLE_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor rb(RIGHT_BACK_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor lf(LEFT_FRONT_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor lm(LEFT_MIDDLE_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor lb(LEFT_BACK_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor rf(RIGHT_FRONT_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rm(RIGHT_MIDDLE_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rb(RIGHT_BACK_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor intakeLower(UPPER_INTAKE_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intakeUpper(LOWER_INTAKE_MOTOR, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor(CONVEYOR_MOTOR, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Imu imu(IMU_SENSOR_PORT);

pros::ADIDigitalOut solenoid(SOLENOID_SENSOR_PORT);


pros::ADIDigitalOut front_roller(FRONT_ROLLER_SENSOR_PORT);
pros::ADIAnalogIn slam_dunk(SLAM_DUNK_SENSOR_PORT);

pros::Motor slam_dunk_l(SLAM_DUNK_L, pros::E_MOTOR_GEAR_RED, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor slam_dunk_r(SLAM_DUNK_R, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);

extern "C" int32_t vexGenericSerialReceive( uint32_t index, uint8_t *buffer, int32_t length );
extern "C" void vexGenericSerialEnable(  uint32_t index, uint32_t nu );
extern "C" void vexGenericSerialBaudrate(  uint32_t index, uint32_t rate );
extern "C" int32_t vexGenericSerialTransmit( uint32_t index, uint8_t *buffer, int32_t length );

//Controllers


//PARAMETERS
const double MAX_RPM = 600.0;
const double WHEEL_RADIUS = 34.925;
const double SCALING_FACTOR = MAX_RPM / 127.0;
const double TO_DEGREES = (180.0 / M_PI);
const double TO_RADIANS = (M_PI / 180.0);
const double WHEEL_BASE_RADIUS = 263.0/2.0; //in mm
bool actuated = false;
bool actuated_frontroller = false;
double wheel_diameter = 69.85;  
double PosConvert = M_PI * wheel_diameter / 360; 
int slammingState = 0;
//MogoLift
const double mkP = 0.88;
const double mkI = 0.0; 
const double mkD = 1.79;

int liftTarget = 0;
bool liftEnable = false;

//Serial
double global_distY = 0.0;
double global_distX = 0.0;
double global_errorY = 0.0;
double global_errorX = 0.0;

double kP = 0.5;  // Proportional gain
double kI = 0.01; // Integral gain
double kD = 0.3;  // Derivative gain

int leftX = 0; 
int leftY = 0;
int rightX = 0;
int rightY = 0;
double degrees_per_mm = 1.6857;



double base_error = 4.0;
double decelerationThreshold = 130;

double slam_target = 0;
double slam_Kp = 0.4;
double slam_Kd = 0.1;
double slam_Ki = 0.0;