/*
** main.c
** Entry point. Ties everything together.
*/
#include "codexion.h"

/*
	1. parse arguments -> fill config struct
	2. initialize dongles (one per coder, circular)
	3. initialize monitor struct
	4. spawn coder threads (one per coder)
	5. spawn monitor thread
	6. wait for simulation to stop (join monitor)
	7. join all coder threads
	8. cleanup (free everything, destroy mutexes)
*/


int main(int ac, char **av)
{
	int				i;
	t_simulation	sim;
	pthread_t		monitor;
	pthread_t		*coder_threads;

	if (!parse_args(ac, av, &sim))
		return (1);

	// malloc coders and dongles
	sim.coders = malloc(sizeof(t_coder) * sim.number_of_coders);
	sim.dongles = malloc(sizeof(t_dongle) * sim.number_of_coders);
	coder_threads = malloc(sizeof(pthread_t) * sim.number_of_coders);
	
	// Initialize mutexes for simulation. 
	pthread_mutex_init(&sim.log_mutex, NULL);
	pthread_mutex_init(&sim.stop_mutex, NULL);

	sim.stop = 0;
	sim.start_time = get_time_ms();

	// Initialize coders and dongles 
	i = 0;	
	while (i < sim.number_of_coders)
	{
		sim.coders[i].id = i + 1;
		sim.dongles[i].id = i + 1;
		
		sim.coders[i].left_dongle = &sim.dongles[i];
		sim.coders[i].right_dongle = &sim.dongles[(i + 1) % sim.number_of_coders];
		
		sim.coders[i].sim = &sim;
		sim.coders[i].alive = 1;
		sim.coders[i].compile_count = 0;
		sim.coders[i].last_compile = sim.start_time;
		sim.dongles[i].is_available = 1;
		sim.dongles[i].queue_size = 0;
		
		pthread_mutex_init(&sim.dongles[i].mutex, NULL);
		pthread_cond_init(&sim.dongles[i].cond, NULL);
		
		i++;
	}

	pthread_create(&monitor, NULL, monitor_routine, &sim);
	
	i = 0;
	while (i < sim.number_of_coders)
	{
		pthread_create(&coder_threads[i], NULL, coder_routine, &sim.coders[i]);
		i++;
	}
	pthread_join(monitor, NULL);

	i = 0;
	while (i < sim.number_of_coders)
		pthread_join(coder_threads[i++], NULL);

	pthread_mutex_destroy(&sim.log_mutex);
	pthread_mutex_destroy(&sim.stop_mutex);
	i = 0;
	while (i < sim.number_of_coders)
	{
		pthread_mutex_destroy(&sim.dongles[i].mutex);
		pthread_cond_destroy(&sim.dongles[i].cond);
		i++;
	}

	free(sim.coders);
	free(sim.dongles);
	free(coder_threads);

	return (0);
}
