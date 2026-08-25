// PID with ANTI-WINDUP. Real actuators saturate — a gimbal can only deflect so
// far. If the integral keeps accumulating while the actuator is maxed out, it
// "winds up" to a huge value and then overshoots badly when the error flips.
// We clamp both the command and the integral to safe limits.
#pragma once

#include <algorithm>

struct PIDController {
    double kp, ki, kd;
    double integral   = 0.0;
    double prev_error = 0.0;
    double max_torque = 5.0;   // actuator saturation limit (both directions)
    double max_integral = 3.0; // cap on the integral's own magnitude

    double command(double setpoint, double measured, double dt) {
        double error = setpoint - measured;

        integral += error * dt;
        // Anti-windup: don't let the integral grow without bound.
        integral = std::clamp(integral, -max_integral, max_integral);

        double derivative = (error - prev_error) / dt;
        prev_error = error;

        double cmd = kp * error + ki * integral + kd * derivative;
        // Saturate the command to what the actuator can actually deliver.
        return std::clamp(cmd, -max_torque, max_torque);
    }
};
