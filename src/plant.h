// The PLANT is whatever the controller acts on. Here: the vehicle's ATTITUDE
// (its pitch angle) about one axis. We command a TORQUE; the plant integrates
// that into an angular rate and then into an angle — the same shape as a real
// vehicle responding to a gimbaled engine.
#pragma once

struct AttitudePlant {
    double angle = 0.0;   // radians, what we want to control
    double rate  = 0.0;   // rad/s, angular velocity
    double inertia = 1.0; // resistance to angular acceleration
    double damping = 0.1; // aerodynamic / structural damping

    // Apply a commanded torque for one fixed step dt.
    double step(double torque, double dt) {
        // angular accel = (torque - damping*rate) / inertia
        double ang_accel = (torque - damping * rate) / inertia;
        rate  += ang_accel * dt;   // integrate accel -> rate
        angle += rate * dt;        // integrate rate  -> angle
        return angle;
    }
};
