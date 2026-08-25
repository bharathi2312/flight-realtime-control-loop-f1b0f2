// Abort is not just another phase transition — it PRE-EMPTS everything. Before
// any normal phase logic runs, we check hard safety limits. If any is breached,
// we force Abort and command a known fail-safe, regardless of what the
// controller wanted. Safety logic runs FIRST and WINS.
#pragma once

#include "phase.h"

struct Limits {
    double max_tilt   = 1.2;   // rad; past this the vehicle is tumbling
    double max_descent = 80.0; // m/s; faster than this on descent = unrecoverable
};

// Returns true if a hard limit is breached and the vehicle must abort.
inline bool must_abort(const Vehicle& v, double tilt, const Limits& lim) {
    if (tilt > lim.max_tilt)              return true;  // attitude lost
    if (-v.velocity > lim.max_descent)    return true;  // descending too fast
    return false;
}

// The fail-safe command: neutral, predictable, no clever control.
inline double failsafe_torque() { return 0.0; }
