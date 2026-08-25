#include <chrono>
#include <cstdio>
#include <thread>
#include "plant.h"
#include "control.h"

using clk = std::chrono::steady_clock;

constexpr int    kRateHz = 50;
constexpr double kDt     = 1.0 / kRateHz;

int main() {
    const auto period = std::chrono::duration<double>(kDt);
    auto next = clk::now();

    AttitudePlant vehicle;
    PIDController  ctrl{ /*kp=*/6.0, /*ki=*/2.0, /*kd=*/3.0 };
    const double   setpoint = 0.5;

    for (int tick = 0; tick < 200; ++tick) {
        double measured = vehicle.angle;
        double torque   = ctrl.command(setpoint, measured, kDt);
        vehicle.step(torque, kDt);

        if (tick % 40 == 0)
            std::printf("tick=%3d  angle=%.4f  err=%.4f\n", tick, measured, setpoint - measured);

        next += std::chrono::duration_cast<clk::duration>(period);
        std::this_thread::sleep_until(next);
    }
    std::printf("final angle=%.4f (target 0.50) — PID closes the gap\n", vehicle.angle);
    return 0;
}
