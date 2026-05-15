*This project has been created as part of the 42 curriculum by fdinis-d.*

# Codexion

## Description

The dining philosophers problem, but with coders and USB dongles.
<img width="1536" height="1024" alt="ChatGPT Image May 15, 2026, 04_21_45 PM" src="https://github.com/user-attachments/assets/5dbbdce1-ad33-49a9-bf5c-9e6e4af1db4b" />

Each coder sits at a circular table and needs two dongles — one shared with each
neighbour — to compile. Since dongles are shared, coders must coordinate access
without deadlocking, starving, or burning out. The simulation ends either when
every coder has compiled the required number of times, or when one burns out.

All times are in milliseconds. `dongle_cooldown` is the minimum time a dongle
must rest after being released before it can be acquired again.

## Instructions

### Compilation

```sh
make        # build the binary
make re     # full rebuild
make clean  # remove object files
make fclean # remove object files and binary
```

Requires a C compiler and POSIX threads (`-pthread`). No external libraries needed.

### Execution

```sh
./codexion <n_coders> <time_to_burnout> <time_to_compile> <time_to_debug> \
           <time_to_refactor> <compiles_required> <dongle_cooldown> <fifo|edf>
```

| Argument | Description |
|---|---|
| `n_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max ms a coder can go without compiling |
| `time_to_compile` | Ms spent compiling |
| `time_to_debug` | Ms spent debugging after each compile |
| `time_to_refactor` | Ms spent refactoring after debugging |
| `compiles_required` | Compiles needed for the simulation to end cleanly |
| `dongle_cooldown` | Ms a dongle must rest before being reacquired |
| `fifo\|edf` | Scheduling policy for the dongle wait queue |

```sh
make run                                      # default: 5 coders, fifo
make run ARGS="3 800 200 200 200 3 0 edf"    # 3 coders, EDF scheduler
make valgrind ARGS="4 800 200 100 100 3 30 fifo"
make helgrind ARGS="4 800 200 100 100 3 30 fifo"
```

## Blocking cases handled

**Deadlock prevention (Coffman's conditions)** — A deadlock requires four
conditions to hold simultaneously: mutual exclusion, hold-and-wait, no
preemption, and circular wait. The first three are inherent to the problem.
Circular wait is broken by imposing a consistent dongle acquisition order:
even-ID coders grab their left dongle first, odd-ID coders grab their right
dongle first. No two adjacent coders ever compete for the same two dongles in
the same order, making a deadlock cycle impossible.

**Starvation prevention** — Coders wait in a per-dongle queue rather than
spinning freely. A coder can only acquire a dongle when it is first in that
queue, ensuring every waiting coder eventually gets its turn. The EDF scheduler
further prioritises coders closest to burning out by sorting the queue by
deadline, preventing any coder from being skipped indefinitely.

**Cooldown handling** — After a dongle is released, it must rest for
`dongle_cooldown` ms before being reacquired. Waiting coders use
`pthread_cond_timedwait` with a timeout set to the exact remaining cooldown
duration, so they wake up precisely when the dongle becomes acquirable again
without relying on external polling or spurious wakeups.

**Burnout detection** — A dedicated monitor thread checks every ~1ms whether
any coder has exceeded `time_to_burnout` ms since its last compile. On
detection it logs `burned out`, sets the stop flag under `stop_mutex`, and
broadcasts to all dongle condition variables so waiting threads unblock and
exit cleanly.

**Log serialization** — All output goes through `log_state`, which holds
`log_mutex` for the duration of the `printf` call. This prevents lines from
different threads from interleaving on stdout.

## Thread synchronization mechanisms

| Primitive | Protects |
|---|---|
| `dongle->mutex` + `dongle->cond` | `is_available`, `released_at`, `queue` |
| `coder->coder_mutex` | `compile_count`, `last_compile` |
| `sim->stop_mutex` | `sim->stop` |
| `sim->log_mutex` | stdout |

**Acquiring dongles** — `dongle_acquire` locks the dongle mutex, pushes the
coder into the wait queue, then enters a loop. Each iteration checks whether
the dongle is available, the coder is at the front of the queue, and the
cooldown has elapsed. If any condition fails, the coder calls
`pthread_cond_timedwait` with a deadline matching the remaining cooldown — it
either wakes early when another coder broadcasts on release, or times out
exactly when the cooldown expires. All reads and writes to dongle state happen
inside the lock.

**Releasing dongles** — `dongle_release` locks the dongle mutex, marks the
dongle available, records the release timestamp, then calls
`pthread_cond_broadcast` to wake all coders waiting on that dongle. Only one
will satisfy the queue and cooldown conditions and proceed; the rest re-evaluate
and sleep again.

**Monitor communication** — The stop flag is the only shared state between the
monitor and coder threads outside of dongle locks. The monitor writes it under
`stop_mutex`; coders read it via `has_stoped`, also under `stop_mutex`. After
setting the flag, the monitor broadcasts to every dongle condition variable so
coders blocked in `pthread_cond_timedwait` unblock immediately rather than
waiting for their timeout.

**Scheduling** — `queue_push` determines insert position before the coder
blocks. FIFO appends to the back. EDF computes each coder's deadline
(`last_compile + time_to_burnout`) and inserts at position 0 if the new coder's
deadline is earlier than the current front, keeping the queue sorted by urgency.

## Resources

- [CodeVault — Multithreading in C playlist](https://www.youtube.com/watch?v=7ge7u5VUSbE&list=PL9vTTBa7QaQPdvEuMTqS9McY-ieaweU8M) — practical walkthrough of pthreads, mutexes, and condition variables in C
- [Core Dumped](https://www.youtube.com/@CoreDumpped) — low-level systems and concurrency videos
- [GeeksForGeeks — Multithreading in C](https://www.geeksforgeeks.org/c/multithreading-in-c/) — reference for POSIX thread API
- [`man pthread_cond_timedwait`](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html) — POSIX specification for condition variable timed wait
- E.W. Dijkstra, *Solution of a problem in concurrent programming control* (1965) — original formulation of the mutual exclusion problem

**AI usage** — Claude Code (Anthropic) was used throughout this project for
architecture decisions (dongle acquisition protocol, scheduler design, monitor
thread responsibilities), debugging Helgrind warnings, writing the Makefile, and
writing this README. Test runs were also executed through the Claude Code CLI.
