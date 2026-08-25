// Determinism is a CLAIM until you measure it. This instruments the loop: it
// records how far each wake-up lands from its scheduled deadline (the JITTER)
// and reports the worst case — the number that actually matters for safety.
#include <chrono>
#include <cstdio>
#include <thread>
#include <cmath>

using clk = std::chrono::steady_clock;
constexpr int    kRateHz = 50;
constexpr double kDt     = 1.0 / kRateHz;

int main() {
    const auto period = std::chrono::duration_cast<clk::duration>(
        std::chrono::duration<double>(kDt));
    auto next = clk::now();
    double worst_ms = 0.0;

    for (int tick = 0; tick < 200; ++tick) {
        next += period;
        std::this_thread::sleep_until(next);

        // How late (or early) did we actually wake up vs the deadline?
        auto now = clk::now();
        double err_ms = std::chrono::duration<double, std::milli>(now - next).count();
        worst_ms = std::max(worst_ms, std::fabs(err_ms));
    }
    std::printf("rate=%d Hz  period=%.1f ms  worst jitter=%.3f ms\n",
                kRateHz, kDt * 1000.0, worst_ms);
    std::printf("a real-time loop wants worst-case jitter well under the period\n");
    return 0;
}
