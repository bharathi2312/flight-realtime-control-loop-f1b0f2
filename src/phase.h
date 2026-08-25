// Flight isn't one task — it's a sequence of PHASES, each with its own goal.
// We model them as an explicit finite state machine. Making the states explicit
// (not implicit in a tangle of ifs) is what makes the logic reviewable.
#pragma once

enum class Phase { Idle, Ascent, Descent, Landing, Abort };

inline const char* name(Phase p) {
    switch (p) {
        case Phase::Idle:    return "IDLE";
        case Phase::Ascent:  return "ASCENT";
        case Phase::Descent: return "DESCENT";
        case Phase::Landing: return "LANDING";
        case Phase::Abort:   return "ABORT";
    }
    return "?";
}

// Telemetry the transition guards read each tick.
struct Vehicle {
    double altitude = 0.0;  // meters
    double velocity = 0.0;  // m/s, + up
    Phase  phase    = Phase::Idle;
};

// Decide the NEXT phase from the current one and the vehicle state. Each arm is
// a guarded transition: a condition that must hold to advance.
inline Phase next_phase(const Vehicle& v) {
    switch (v.phase) {
        case Phase::Idle:    return v.velocity > 0.0 ? Phase::Ascent : Phase::Idle;
        case Phase::Ascent:  return v.velocity <= 0.0 ? Phase::Descent : Phase::Ascent;
        case Phase::Descent: return v.altitude < 50.0 ? Phase::Landing : Phase::Descent;
        case Phase::Landing: return v.altitude <= 0.0 ? Phase::Idle    : Phase::Landing;
        case Phase::Abort:   return Phase::Abort;   // terminal
    }
    return v.phase;
}
