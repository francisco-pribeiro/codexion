#include "codexion.h"

// tirar a sim dos araguenteos das funcoes, usar o pointrer do coder
int	dongle_acquire(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	queue_push(dongle, coder, sim);

	while (1)
	{
		// Dongle disponível, coder primeiro da fila, o cooldown passou
		if (dongle->is_available
			&& coder->id == dongle->queue[0].coder_id
			&& cooldown_elapsed(dongle, sim))
		{
			queue_pop(dongle);
			dongle->is_available = 0;
			break;
		}
		if (sim->stop)
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}
 
void	dongle_release(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = 1;
	dongle->released_at = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
