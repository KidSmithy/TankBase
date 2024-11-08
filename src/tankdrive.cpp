// #include "main.h"
// #include "pros/motors.h"

// void disabled() {}
// void competition_initialize() {}
// void autonomous() {}

// void serialRead(void *params) {
//   vexGenericSerialEnable(SERIALPORT - 1, 0);
//   vexGenericSerialBaudrate(SERIALPORT - 1, 115200);
//   pros::delay(10);
//   pros::screen::set_pen(COLOR_BLUE);
//   double distX, distY = 0;
//   while (true) {
//     uint8_t buffer[256];
//     int bufLength = 256;
//     int32_t nRead = vexGenericSerialReceive(SERIALPORT - 1, buffer, bufLength);
//     if (nRead >= 0) {
//       std::stringstream dataStream("");
//       bool recordOpticalX, recordOpticalY = false;
//       for (int i = 0; i < nRead; i++) {
//         char thisDigit = (char)buffer[i];
//         if (thisDigit == 'D' || thisDigit == 'I' || thisDigit == 'A' ||
//             thisDigit == 'X' || thisDigit == 'C' || thisDigit == 'Y') {
//           recordOpticalX = false;
//           recordOpticalY = false;
//         }
//         if (thisDigit == 'C') {
//           recordOpticalX = false;
//           dataStream >> distX;
//           pros::lcd::print(1, "Optical Flow:");
//           pros::lcd::print(2, "distX: %.2lf", distX / 100);
//           dataStream.str(std::string());
//         }
//         if (thisDigit == 'D') {
//           recordOpticalY = false;
//           dataStream >> distY;
//           global_distY = distY / 100;
//           pros::lcd::print(3, "distY: %.2lf", distY / 100);
//           dataStream.str(std::string());
//         }
//         if (recordOpticalX)
//           dataStream << (char)buffer[i];
//         if (recordOpticalY)
//           dataStream << (char)buffer[i];
//         if (thisDigit == 'X')
//           recordOpticalX = true;
//         if (thisDigit == 'Y')
//           recordOpticalY = true;
//       }
//     }
//     pros::Task::delay(25);
//   }
// }

// void brake() {
//   lf.brake();
//   lm.brake();
//   lb.brake();

//   rf.brake();
//   rm.brake();
//   rb.brake();
//   pros::delay(2);
// }

// double bound_value(double value) {
//   if (value > MAX_RPM)
//     return MAX_RPM;
//   if (value < -MAX_RPM)
//     return -MAX_RPM;
//   return value;
// }

// void slamDunk() {
//   double Derivative = 0.0;
//   double prevError = 0.0;
//   double Error = 0.0;
//   double Integral = 0.0;
//   while (true) {
//     if (slammingState == 0) {
//       slam_target = 1650;
//     } else if (slammingState == 1) {
//       slam_target = 1920;
//     } else if (slammingState == 2) {
//       slam_target = 3400;
//     }

//     Derivative = prevError - Error;
//     Error = fabs(slam_target - slam_dunk.get_value());
//     Integral += Error;
//     double motorPower =
//         slam_Kp * Error + slam_Kd * Derivative + slam_Ki * Integral;

//     if (fabs(Error) <= 10) {
//       slam_dunk_l.move(0);
//       slam_dunk_r.move(0);
//       slam_dunk_l.brake();
//       slam_dunk_r.brake();
//     } else {
//       if (slam_target > slam_dunk.get_value() + 10) {
//         slam_dunk_l.move(motorPower);
//         slam_dunk_r.move(motorPower);
//       } else if (slam_target < slam_dunk.get_value() - 10) {
//         slam_dunk_l.move(-motorPower);
//         slam_dunk_r.move(-motorPower);
//       }
//     }
//     prevError = Error;
//     pros::Task::delay(15);
//   }
// }

// void initialize() {
//   pros::lcd::initialize();
//   lf.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//   lm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//   lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

//   rf.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//   rm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//   rb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

//   slam_dunk_l.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//   slam_dunk_r.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

//   intakeLower.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//   intakeUpper.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//   conveyor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

//   imu.reset(true);
//   imu.set_data_rate(5);

//   // pros::Task serial_read(serialRead);
//   slam_dunk.calibrate();

//   // pros::Task serial_read(serialRead);
//   pros::Task slam_dunk(slamDunk);

//   master.clear();
// }

// void opcontrol() {
//   while (true) {
//     leftY = bound_value(master.get_analog(ANALOG_LEFT_Y));
//     rightY = bound_value(master.get_analog(ANALOG_RIGHT_Y));
//     lf.move(leftY);
//     lm.move(leftY);
//     lb.move(leftY);
//     rf.move(rightY);
//     rm.move(rightY);
//     rb.move(rightY);

//     if (master.get_digital(DIGITAL_L2)) {
//       intakeLower.move(110);
//       intakeUpper.move(110);
//     } else if (master.get_digital(DIGITAL_L1)) {
//         actuated_frontroller = false;
//       intakeLower.move(-110);
//       intakeUpper.move(-110);
//     } else {
//       intakeLower.move(0);
//       intakeUpper.move(0);
//     }

//     if (master.get_digital(DIGITAL_R1)) {
//       conveyor.move(110);
//     } else if (master.get_digital(DIGITAL_R2)) {
//       conveyor.move(-110);
//     } else {
//       conveyor.move(0);
//     }
//     if (master.get_digital_new_press(DIGITAL_Y))
//       actuated_frontroller = !actuated_frontroller;

//     if (master.get_digital_new_press(DIGITAL_B))
//       actuated = !actuated;
    
//     if (actuated_frontroller)
//       front_roller.set_value(0);
//     else
//       front_roller.set_value(1);

//     if (actuated)
//       solenoid.set_value(0);
//     else
//       solenoid.set_value(1);


//     if (master.get_digital_new_press(DIGITAL_DOWN) ) {
//       slammingState -= 1;
//       slammingState = fabs(slammingState);
      
//     }

//     // if (master.get_digital(DIGITAL_DOWN) and slammingState == 0) {
//     //   slammingState = 1;
//     // }

//     if (master.get_digital_new_press(DIGITAL_RIGHT)) {
//       slammingState = 2;
//       actuated_frontroller = true;
    
//     }

//     pros::delay(5);
//   }
// }
