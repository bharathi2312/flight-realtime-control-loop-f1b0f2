# Real-Time Flight Control Loop

An intermediate systems project in modern C++ that teaches the core of any autonomous vehicle's flight software — the real-time control loop. You build a deterministic, fixed-rate (50 Hz) loop and learn why timing determinism, not raw speed, is what makes flight software safe. You write a PID controller from scratch and use it to hold a simulated vehicle's attitude against a disturbance, then reason about integral windup and how to clamp it. You model the mission as an explicit finite state machine — idle, ascent, descent, landing, abort — and wire the transitions, including an abort path that fires the instant a safety limit is crossed. You separate sensing from control from actuation, measure your loop's jitter, and finish with a controller that holds a setpoint and a state machine that reacts to failure. By the end you can build the loop that everything else in flight software runs inside.

Built step-by-step with [KhwajaLabs Build](https://khwajalabs.com).

## Stack
- C++
- Real-Time Systems
- PID Control
- State Machines
- Determinism
