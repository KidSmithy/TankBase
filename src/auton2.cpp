// #include "main.h"

// bool is_turning = false;
// double integral = 0;
// double previous_error = 0;
// double target_angle = 0;
// double max_power = 50;
// double current_time;
// #define pi 3.14

// void turn_to_angle(double angle) {
//   target_angle = angle;
//   integral = 0;
//   previous_error = 0;
//   is_turning = true;
// }

// void update_turning(double kP, double kI, double kD) {
//   if (is_turning) {
//     double current_angle = imu.get_heading();

//     double error = target_angle - current_angle;
//     // if (error > 180) error -= 360;
//     // if (error < -180) error += 360;
//     // if (error < 0) error += 360;
//     integral += error;
//     double derivative = error - previous_error;
//     previous_error = error;

//     double power = kP * error + kI * integral + kD * derivative;

//     // LEFT_MOTOR_FRONT.move(power);
//     // LEFT_MOTOR_REAR.move(power);
//     // RIGHT_MOTOR_FRONT.move(-power);
//     // RIGHT_MOTOR_REAR.move(-power);
//     if (power > max_power)
//       power = max_power;
//     else if (power < -max_power)
//       power = -max_power;
//     pros::lcd::print(2, "Current Angle: %f", current_angle);
//     pros::lcd::print(3, "Error: %f", error);
//     pros::lcd::print(4, "Power: %f", power);

//     lf.move(-power);
//     lm.move(-power);
//     lb.move(-power);
//     rf.move(power);
//     rm.move(power);
//     rb.move(power);

//     if (fabs(error) < 2) {
//       is_turning = false;
//       // LEFT_MOTOR_FRONT.move(0);
//       // LEFT_MOTOR_REAR.move(0);
//       // RIGHT_MOTOR_FRONT.move(0);
//       // RIGHT_MOTOR_REAR.move(0);
//       lf.move(0);
//       lm.move(0);
//       lb.move(0);
//       rf.move(0);
//       rm.move(0);
//       rb.move(0);
//     }
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

// void groupMove(double distance_mm, int velocity){
//     double encoder_degrees = distance_mm * degrees_per_mm;
//     int encoder_ticks = static_cast<int>(round(encoder_degrees));
//     lf.tare_position();
//     lm.tare_position();
//     lb.tare_position();
//     rf.tare_position();
//     rm.tare_position();
//     rb.tare_position();
//     while((fabs(rf.get_position()) + fabs(lf.get_position()))/2.0 < fabs(encoder_ticks)){
//         lm.move_absolute(encoder_ticks, velocity);
//         lb.move_absolute(encoder_ticks, velocity);
//         lf.move_absolute(encoder_ticks, velocity);

//         rf.move_absolute(encoder_ticks, velocity);
//         rm.move_absolute(encoder_ticks, velocity);
//         rb.move_absolute(encoder_ticks, velocity);
//         if((fabs(rf.get_position()) + fabs(lf.get_position()))/2.0 >= fabs(encoder_ticks)) break;
//     }
//     brake();
// }

// void turn_angle(double targetTurning, double turn_Kp, double turn_Kd) {
//   double initialHeading = imu.get_rotation(); // Store the initial heading
//   double currentHeading = initialHeading;
//   double turnError = 0.0;

//   // 1. Perform turning first

//   imu.tare_rotation();
//   double prevError = 0.0;
//   while (true) {
//     currentHeading = fabs(imu.get_rotation());
//     turnError = fabs(fabs(targetTurning) - (currentHeading));
//     pros::lcd::print(0, "Error: %.lf", turnError);

//     // Check if we are within the error threshold
//     if (fabs(turnError) <= 1.0) {
//       // Stop turning if we are close enough
//       brake();
//       lf.move(0);
//       lm.move(0);
//       lb.move(0);
//       rf.move(0);
//       rm.move(0);
//       rb.move(0);
//       break; // Exit turning loop
//     }
//     double turnDerivative = prevError - turnError;

//     // Calculate turn power
//     double turnPower =
//         turnError * turn_Kp + turnDerivative * turn_Kd; // Tune this gain
//     prevError = turnError;

//     // Adjust motor powers for turning
//     if (targetTurning > 0) {
//       lf.move(-turnPower); // Adjust left side for turning
//       lm.move(-turnPower);
//       lb.move(-turnPower);
//       rf.move(turnPower); // Adjust right side for turning
//       rm.move(turnPower);
//       rb.move(turnPower);
//     } else if (targetTurning < 0) {
//       lf.move(turnPower); // Adjust left side for turning
//       lm.move(turnPower);
//       lb.move(turnPower);
//       rf.move(-turnPower); // Adjust right side for turning
//       rm.move(-turnPower);
//       rb.move(-turnPower);
//     }
//     pros::delay(5); // Delay to reduce CPU load
//   }
// }

// void base_PID(double targetDistance, double base_kp, double base_ki,
//               double base_kd) {
//                    // Diameter in mm
//   // Conversion factor

//   // Movement variables
//   double powerL = 0;
//   double powerR = 0;
//   double encdleft = 0;
//   double encdright = 0;
//   double errorLeft = 0;
//   double errorRight = 0;
//   double prevErrorLeft = 0;
//   double prevErrorRight = 0;
//   double totalErrorLeft = 0;
//   double totalErrorRight = 0;

//   // Turning variables

//   bool l_move = false;
//   bool r_move = false;

//   // 2. Now perform distance movement
//   if (targetDistance != 0) {
//     l_move = true;
//     r_move = true;
//   }

//   lf.tare_position();
//   rf.tare_position();

//   while (l_move || r_move) {
//     // Get encoder values
//     encdleft = lf.get_position() * PosConvert;
//     encdright = rf.get_position() * PosConvert;

//     // Calculate distance error
//     errorLeft = fabs(targetDistance) - fabs(encdleft);
//     errorRight = fabs(targetDistance) - fabs(encdright);
//     totalErrorLeft += errorLeft;
//     totalErrorRight += errorRight;

//     // PID for left motors
//     if (fabs(errorLeft) <= base_error) {
//       powerL = 0;
//       l_move = false;
//     } else {
//       // Gradually reduce power as you approach the target
//       if (fabs(errorLeft) < decelerationThreshold) {
//         powerL *= 0.5; // Reduce power to half when close to target
//       } else {
//         powerL = base_kp * errorLeft + base_ki * totalErrorLeft +
//                  base_kd * (prevErrorLeft - errorLeft);
//       }
//     }

//     // PID for right motors
//     if (fabs(errorRight) <= base_error) {
//       powerR = 0;
//       r_move = false;
//     } else {
//       // Gradually reduce power as you approach the target
//       if (fabs(errorRight) < decelerationThreshold) {
//         powerR *= 0.5; // Reduce power to half when close to target
//       } else {
//         powerR = base_kp * errorRight + base_ki * totalErrorRight +
//                  base_kd * (prevErrorLeft - errorRight);
//       }
//     }

//     // Move the motors
//     if (targetDistance > 0) {
//       lf.move(powerL);
//       lm.move(powerL);
//       lb.move(powerL);
//       rf.move(powerR);
//       rm.move(powerR);
//       rb.move(powerR);
//     } else if (targetDistance < 0) {
//       lf.move(-powerL);
//       lm.move(-powerL);
//       lb.move(-powerL);
//       rf.move(-powerR);
//       rm.move(-powerR);
//       rb.move(-powerR);
//     }

//     pros::lcd::print(0, "ErrorL: %.lf", errorLeft);
//     pros::lcd::print(1, "ErrorR: %.lf", errorRight);

//     // Update previous errors for the next iteration
//     prevErrorLeft = errorLeft;
//     prevErrorRight = errorRight;

//     pros::delay(2); // Delay to reduce CPU load
//   }
//   brake();
// }

// void initialize() {
//   pros::lcd::initialize();
//   pros::lcd::print(0, "LCD initialized");

//   imu.reset(true);
//   pros::delay(1000);
//   imu.set_data_rate(5);
//   	lf.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 	lm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 	lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

// 	rf.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 	rm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
// 	rb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

//   master.clear();
// }

// void opcontrol() {

  
  

//   // conveyor.move(110);
// //   intakeLower.move(-127);
// //   intakeUpper.move(-127);
//   groupMove(-650, 500);
  
//   solenoid.set_value(1);

//   turn_angle(-113, 1.5, 0.5);
//   groupMove(-1095, 150);
//   groupMove(-60, 70);
//   solenoid.set_value(0);
//   pros::delay(1000);

// //   groupMove(-1900, 500);
// //   groupMove(-100, 100);
// //   pros::delay(2000);
// //   solenoid.set_value(0);
// //   conveyor.move(110);
// //   turn_angle(-60, 1.5, 0.2);
//   //     intakeLower.move(0);
// //   intakeUpper.move(0);
// // // imu.reset(true);
// // // pros::delay(1000);
// // // turn_to_angle(130);
  
// // //   while (is_turning) {
// // //     update_turning(1.5, 0, 0.8);
// // //     pros::delay(20); // Control update rate
// // //   }
// // // pros::delay(1000);

// //   base_PID(-1040, 0.5, 0, 0.4);




// //   pros::delay(1500);
// //   brake();

// // groupMove(-200, 100);
// // // pros::delay(400);

// //   turn_angle(-105, 1.5, 0.5);
// //   conveyor.move(110);
// }