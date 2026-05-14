#include "codexion.h"

// todo: agarrar os dois e registar depois
void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (1)
	{
		if (coder->id % 2 == 0)
		{
			if (!dongle_acquire(coder->left_dongle, coder, sim))
				break ;
			log_state(sim, coder->id, "has taken a dongle");
			if (!dongle_acquire(coder->right_dongle, coder, sim))
			{
				dongle_release(coder->left_dongle);
				break ;
			}
		}
		else
		{
			if (!dongle_acquire(coder->right_dongle, coder, sim))
				break ;
			log_state(sim, coder->id, "has taken a dongle");
			if (!dongle_acquire(coder->left_dongle, coder, sim))
			{
				dongle_release(coder->right_dongle);
				break ;
			}
		}
		log_state(sim, coder->id, "has taken a dongle");
		coder->last_compile = get_time_ms();
		log_state(sim, coder->id, "is compiling");
		ms_sleep(sim->time_to_compile);
		dongle_release(coder->left_dongle);
		dongle_release(coder->right_dongle);
		coder->compile_count++;
		if (sim->stop || coder->compile_count >= sim->number_of_compiles_required)
			break ;
		log_state(sim, coder->id, "is debugging");
		ms_sleep(sim->time_to_debug);
		if (sim->stop)
			break ;
		log_state(sim, coder->id, "is refactoring");
		ms_sleep(sim->time_to_refactor);
		if (sim->stop)
			break ;
	}
	return (NULL);
}
