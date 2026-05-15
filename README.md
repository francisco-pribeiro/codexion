*This project has been created as part of the 42 curriculum by fribeir-.*

# Codexion

## Description

The dining philosophers problem, but with coders and USB dongles.
<img width="1536" height="1024" alt="ChatGPT Image May 15, 2026, 04_21_45 PM" src="https://github.com/user-attachments/assets/5dbbdce1-ad33-49a9-bf5c-9e6e4af1db4b" />

Each coder needs two dongles to compile. Dongles are shared with neighbours in a
circular arrangement, so coders must coordinate without deadlocking or starving.
A coder who goes too long without compiling burns out and ends the simulation.

```
./codexion <n_coders> <time_to_burnout> <time_to_compile> <time_to_debug> \
           <time_to_refactor> <compiles_required> <dongle_cooldown> <fifo|edf>
```

All times are in milliseconds. `dongle_cooldown` is the minimum time a dongle
must rest after being released before it can be acquired again.

## Blocking cases handled

**Deadlock prevention** — Even-ID coders grab their left dongle first, odd-ID
coders grab their right dongle first. This breaks circular wait: no two adjacent
coders ever compete for the same dongle in the same order, so a deadlock cycle
cannot form.

**Burnout detection** — The monitor thread checks every ~1ms whether any coder
has gone `time_to_burnout` ms without compiling. If so, it logs `burned out`,
sets the stop flag, and wakes all waiting threads so they exit cleanly.

**Clean completion** — When every coder has reached `compiles_required`, the
monitor sets the stop flag the same way. All threads unblock and the simulation
ends without leaks or dangling threads.

## Thread synchronization mechanisms

| Primitive | Protects |
|---|---|
| `dongle->mutex` + `dongle->cond` | `is_available`, `released_at`, `queue` |
| `coder->coder_mutex` | `compile_count`, `last_compile` |
| `sim->stop_mutex` | `sim->stop` |
| `sim->log_mutex` | stdout (prevents interleaved log lines) |

**Acquiring dongles** — `dongle_acquire` locks the dongle mutex, pushes the coder
into the wait queue, then blocks on `pthread_cond_wait` until the dongle is
available, the coder is first in queue, and the cooldown has elapsed. The mutex
is held for all reads and writes to dongle state.

**Releasing dongles** — `dongle_release` locks the dongle mutex, marks the dongle
available, records the release timestamp, then broadcasts to wake any waiting
coders. All state changes happen inside the lock.

**Scheduling** — `queue_push` decides insert order before blocking. FIFO appends.
EDF inserts at position 0 if the new coder's deadline is earlier than the current
front, maintaining the queue sorted by urgency.

## A note on helgrind

Helgrind reports "possible data race" and "unlocked lock currently held by thread
Y" warnings on `dongle->is_available` and `dongle->released_at`. These are false
positives caused by a known helgrind limitation with the condvar broadcast pattern.

The monitor wakes waiting coders by locking each dongle mutex, broadcasting, and
unlocking. When a coder is inside `pthread_cond_wait`, it has atomically released
the mutex — but helgrind's ownership tracker still associates the mutex with that
coder. When the monitor then acquires and releases the same mutex, helgrind sees a
third party unlocking a mutex it believes is owned by the waiting coder and fires.

Every access to the flagged fields happens inside `pthread_mutex_lock/unlock
(&dongle->mutex)` — readable directly in `dongle_acquire` and `dongle_release`.
ThreadSanitizer (`-fsanitize=thread`), which models condvar atomicity correctly,
reports zero races across all test scenarios.

## Usage examples

```sh
make run                                    # default: 5 coders, fifo
make run ARGS="3 800 200 200 200 3 0 edf"  # 3 coders, EDF scheduler
make valgrind ARGS="4 800 200 100 100 3 30 fifo"
make helgrind ARGS="4 800 200 100 100 3 30 fifo"
```
