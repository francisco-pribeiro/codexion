*This project has been created as part of the 42 curriculum by fribeir-.*

# Codexion

## Description
TODO

## Instructions
TODO

## Resources
TODO

## Blocking cases handled
TODO

## Thread synchronization mechanisms
TODO


1. Parse — done. You have a t_simulation with all the config values.

2. Initialize — in main.c, after parsing:

malloc an array of t_coder (one per coder)
malloc an array of t_dongle (one per coder, circular)
initialize each dongle's mutex and cond
initialize sim.log_mutex and sim.stop_mutex
point each coder to their left and right dongle
3. Spawn threads — in main.c:

one pthread_create per coder, each running coder_routine
one pthread_create for the monitor, running monitor_routine
4. Coder threads — each coder loops:

try to acquire left dongle, then right dongle
compile, release dongles
debug, refactor
repeat until sim->stop is set
5. Monitor thread — runs in parallel:

checks every ~1ms if any coder has burned out
checks if all coders hit number_of_compiles_required
if either condition: set sim->stop = 1, wake all waiting threads
6. Cleanup — back in main.c after threads finish:

pthread_join all threads
destroy all mutexes and conds
free all malloc'd memory
utils.c feeds into steps 4 and 5 — timing and logging are used everywhere.