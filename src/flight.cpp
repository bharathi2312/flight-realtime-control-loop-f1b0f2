// The tick now puts SAFETY FIRST: check limits, and if breached force Abort and
// the fail-safe command BEFORE any normal phase/control logic gets to run.
#include <cstdio>
#include "phase.h"
#include "safety.h"

int main() {
    Vehicle v;
    Limits  lim;

    // Scripted: a normal climb, then a tilt spike that must trigger abort.
    double vel[]  = { 5, 5, 5, 5, 5 };
    double tilt[] = { 0.1, 0.2, 0.3, 1.5, 0.1 };  // tick 3 exceeds max_tilt

    for (int tick = 0; tick < 5; ++tick) {
        v.velocity = vel[tick];

        // SAFETY FIRST — runs before normal phase logic and overrides it.
        if (v.phase != Phase::Abort && must_abort(v, tilt[tick], lim)) {
            v.phase = Phase::Abort;
            std::printf("tick=%d  ABORT (tilt=%.2f)  cmd=%.1f\n",
                        tick, tilt[tick], failsafe_torque());
            continue;
        }

        v.phase = next_phase(v);  // normal logic only if not aborting
        std::printf("tick=%d  phase=%s  tilt=%.2f\n", tick, name(v.phase), tilt[tick]);
    }
    return 0;
}
