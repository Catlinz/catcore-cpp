
namespace cat {
	CxI32 pthread_barrier_destroy(pthread_barrier_t *in_barrier) {
		CxI32 err = pthread_mutex_destroy(&(in_barrier->mutex));
		if (err != 0) { return err; }
		err = pthread_cond_destroy(&(in_barrier->cond));
	   in_barrier->count = in_barrier->max = 0;
		return err;
	}
	
	CxI32 pthread_barrier_init(pthread_barrier_t *in_barrier, void *in_attr,
										CxU32 in_count) {
		CxI32 err = pthread_mutex_init(&(in_barrier->mutex), 0);
		if (err != 0) { return err; }
		err = pthread_cond_init(&(in_barrier->cond), 0);
		if (err != 0) { return err; }
		in_barrier->count = 0;
		in_barrier->max = in_count;
		in_barrier->flag = false;
		return 0;
	}
	
	CxI32 pthread_barrier_wait(pthread_barrier_t *in_barrier) {
		CxI32 err = pthread_mutex_lock(&(in_barrier->mutex));
		if (err != 0) { return err; }

		CxBool flag = in_barrier->flag;
		++(in_barrier->count);

		if (in_barrier->count == in_barrier->max) {
			in_barrier->count = 0;
			in_barrier->flag = !(in_barrier->flag);
			
			err = pthread_cond_broadcast(&(in_barrier->cond));
			if (err != 0) { return err; }
			err = pthread_mutex_unlock(&(in_barrier->mutex));
			if (err != 0) { return err; }
			
			return PTHREAD_BARRIER_SERIAL_THREAD;
		}

		while (in_barrier->flag == flag) {
			err = pthread_cond_wait(&(in_barrier->cond), &(in_barrier->mutex));
			if (err != 0) {
				pthread_mutex_unlock(&(in_barrier->mutex));
				return err;
			}
		}

		err = pthread_mutex_unlock(&(in_barrier->mutex));
		if (err != 0) { return err; }
		else { return 0; }
	}
  
	
} // namespace cat
