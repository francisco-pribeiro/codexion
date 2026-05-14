
#include "codexion.h"

static void	stop_simulation(t_simulation *sim)
{
	int i;

	i = 0;
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	while(i < sim->number_of_coders)
		pthread_cond_broadcast(&sim->dongles[i++].cond);
}

// broadcasting every time, replace with more effiencit solutioin
void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	t_coder			*coders;
	int i;
	int j;

	sim = (t_simulation *)arg;
	coders = sim->coders;

	while(1)
	{
		i = 0;
		while(i < sim->number_of_coders)
		{
			if(coders[i].compile_count < sim->number_of_compiles_required
				&& get_time_ms() - coders[i].last_compile >= sim->time_to_burnout)
			{
				log_state(sim, coders[i].id, "burned out");
				stop_simulation(sim);
				return (NULL);
			}

			i++;
		}

		j = 0;
		while(j < sim->number_of_coders
			&& coders[j].compile_count >= sim->number_of_compiles_required)
		{
			j++;
		}
		if (j == sim->number_of_coders)
		{
			stop_simulation(sim);
			return (NULL);
		}
		i = 0;
		while (i < sim->number_of_coders)
    		pthread_cond_broadcast(&sim->dongles[i++].cond);
		ms_sleep(1);
	}
}
