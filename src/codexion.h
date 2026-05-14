#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

/* forward declaration so t_coder can reference t_simulation */
typedef struct s_simulation	t_simulation;

/* ---- queue entry struct ---- */
typedef struct s_queue_entry
{
	int		coder_id;
	long	arrival_time;
	long	deadline;
}	t_queue_entry;

/* ---- dongle struct ---- */
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				id;
	int				is_available;
	long			released_at;
	t_queue_entry	queue[2];
	int				queue_size;
}	t_dongle;

/* ---- coder struct ---- */
typedef struct s_coder
{
	int				id;
	int				compile_count;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_simulation	*sim;
	long			last_compile;
	int				alive;
}	t_coder;

/* ---- simulation struct ---- */
struct s_simulation
{
	int				number_of_coders;
	long			start_time;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			number_of_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
	int				stop;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
};

/* ---- prototypes ---- */

/* parser.c */
int		validate_args(int argc, char **argv);
int		parse_args(int ac, char **av, t_simulation *sim);

/* utils.c */
long	get_time_ms(void);
void	log_state(t_simulation *sim, int coder_id, char *message);
void	ms_sleep(long ms);
int		cooldown_elapsed(t_dongle *dongle, t_simulation *sim);

/* queue.c */
void	queue_push(t_dongle *dongle, t_coder *coder, t_simulation *sim);
int		queue_pop(t_dongle *dongle);

/* dongle.c */
int		dongle_acquire(t_dongle *dongle, t_coder *coder, t_simulation *sim);
void	dongle_release(t_dongle *dongle);

/* coder.c */
void	*coder_routine(void *arg);

/* monitor.c */
void	*monitor_routine(void *arg);

#endif
