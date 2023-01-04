#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threadpool.h"

threadpool* create_threadpool(int num_threads_in_pool) {
    //check if the variable is legitimate
    if (num_threads_in_pool <= 0 || num_threads_in_pool > MAXT_IN_POOL) {
        return NULL;
    }
    
    threadpool* pool = (threadpool*)malloc(sizeof(threadpool));
    if(pool == NULL) {
        perror("malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    // ==================== Initializing the threadpool ====================
    pool->num_threads = num_threads_in_pool;
    pool->qsize = 0;
    // Allocate an array of threads of <num_threads> size
    pool->threads = (pthread_t*)malloc(pool->num_threads * sizeof(pthread_t));
    if(pool->threads == NULL) {
        perror("malloc() failed\n");
        free(pool);
        exit(EXIT_FAILURE);
    }
    pool->qhead = NULL;
    pool->qtail = NULL;
    // Create the mutex
    if (pthread_mutex_init(&(pool->qlock), NULL) != 0) {
        perror("pthread_mutex_init() failed\n");
        free(pool->threads);
        free(pool);
        exit(EXIT_FAILURE);
    }
    // Create the q_empty conditional variable
    if(pthread_cond_init(&(pool->q_empty), NULL) != 0) {
        perror("pthread_cond_init() failed\n");
        pthread_mutex_destroy(&(pool->qlock));
        free(pool->threads);
        free(pool);
        exit(EXIT_FAILURE);
    }
    // Create the q_not_empty conditional variable
    if(pthread_cond_init(&(pool->q_not_empty), NULL) != 0) {
        perror("pthread_cond_init() failed\n");
        pthread_cond_destroy(&(pool->q_empty));
        pthread_mutex_destroy(&(pool->qlock));
        free(pool->threads);
        free(pool);
        exit(EXIT_FAILURE);
    }
    pool->shutdown = 0;
    pool->dont_accept = 0;

    // ==================== Initialize threads ====================
    for(int i = 0; i < num_threads_in_pool; i++) {
        if(pthread_create(&(pool->threads[i]), NULL, do_work, pool) != 0) {
            perror("pthread_cond_init() failed\n");
            pool->shutdown = 1;
            pthread_cond_signal(&(pool->q_not_empty));
            pthread_cond_destroy(&(pool->q_not_empty));
            pthread_cond_destroy(&(pool->q_empty));
            pthread_mutex_destroy(&(pool->qlock));
            free(pool->threads);
            free(pool);
            exit(EXIT_FAILURE);
        }
    }

    return pool;
}

void destroy_threadpool(threadpool* destroyme) {
    if(destroyme == NULL) return;
    // Wait for the mutex to unlock
    if(pthread_mutex_lock(&(destroyme->qlock)) != 0) {
        perror("pthread_mutex_lock() failed\n");
        exit(EXIT_FAILURE);
    }
    // Mutex is ours
    destroyme->dont_accept = 1;
    // Wait for the tasks queue to empty
    if(destroyme->qsize > 0) {
        if(pthread_cond_wait(&(destroyme->q_empty), &(destroyme->qlock)) != 0) {
            perror("pthread_cond_wait() failed\n");
            exit(EXIT_FAILURE);
        }
    }
    // Signals the start of the shutdown process
    destroyme->shutdown = 1;
    // Wakes up all the sleeping threads so they can 'see' they need to 'suicide'
    if(pthread_cond_broadcast(&(destroyme->q_not_empty)) != 0) {
        perror("pthread_cond_broadcast() failed\n");
            exit(EXIT_FAILURE);
    }
    // Unlock the mutex so the other threads can finish their work
    if(pthread_mutex_unlock(&(destroyme->qlock)) != 0) {
        perror("pthread_mutex_unlock() failed\n");
            exit(EXIT_FAILURE);
    }
    // Wait for all the threads to 'die'
    for(int i = 0; i < destroyme->num_threads; i++){
        if(pthread_join(destroyme->threads[i], NULL) != 0) {
            perror("pthread_join() failed\n");
            exit(EXIT_FAILURE);
        }
    }
    // Destroy the threadpool itself
    if(destroyme->threads != NULL)
        free(destroyme->threads);
    if(pthread_cond_destroy(&(destroyme->q_empty)) != 0) {
        perror("pthread_cond_destroy() failed\n");
            exit(EXIT_FAILURE);
    }
    if(pthread_cond_destroy(&(destroyme->q_not_empty)) != 0) {
        perror("pthread_cond_destroy() failed\n");
            exit(EXIT_FAILURE);
    }
    if(pthread_mutex_destroy(&(destroyme->qlock)) != 0) {
        perror("pthread_mutex_destroy() failed\n");
            exit(EXIT_FAILURE);
    }
    free(destroyme);
}

void dispatch(threadpool* from_me, dispatch_fn dispatch_to_here, void *arg) {
    if(from_me == NULL) return;
    // Waiting for mutex to free
    if(pthread_mutex_lock(&(from_me->qlock)) != 0) {
        perror("pthread_mutex_lock() failed\n");
        exit(EXIT_FAILURE);
    }
    // Mutex is ours
    int dontAccept = from_me->dont_accept;
    // Free the mutex
    if(pthread_mutex_unlock(&(from_me->qlock)) != 0) {
        perror("pthread_mutex_unlock() failed\n");
        exit(EXIT_FAILURE);
    }
    // Check if we can accept more work
    if(dontAccept == 1) return;
    // Create new work
    work_t* work = (work_t*)malloc(sizeof(work_t));
    if(work == NULL) {
        perror("malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    // Initialize new work
    work->routine = dispatch_to_here;
    work->arg = arg;
    work->next = NULL;

    // Waiting for mutex to free again
    if(pthread_mutex_lock(&(from_me->qlock)) != 0) {
        perror("pthread_mutex_lock() failed\n");
        exit(EXIT_FAILURE);
    }
    // Add the work to the queue
    if(from_me->qhead == NULL) {
        from_me->qhead = work;
    } else {
        from_me->qtail->next = work;
    }
    from_me->qtail = work;
    from_me->qsize += 1;
    // Signal a sleeping thread that there is work to be done
    if(pthread_cond_signal(&(from_me->q_not_empty)) != 0) {
        perror("pthread_cond_signal() failed\n");
        exit(EXIT_FAILURE);
    }
    // Free the mutex
    if(pthread_mutex_unlock(&(from_me->qlock)) != 0) {
        perror("pthread_mutex_unlock() failed\n");
        exit(EXIT_FAILURE);
    }
}

void* do_work(void* p) {
    threadpool* pool = (threadpool*)p;
    while(1) {
        // Wait for mutex to be free
        if(pthread_mutex_lock(&(pool->qlock)) != 0) {
            perror("pthread_mutex_lock() failed\n");
            exit(EXIT_FAILURE);
        }
        // Mutex is ours
        // Checking if we are in the destruction process
        if(pool->shutdown == 1) {
            if(pthread_mutex_unlock(&(pool->qlock)) != 0) {
                perror("pthread_mutex_unlock() failed\n");
                exit(EXIT_FAILURE);
            }
            return NULL;
        }
        // Wait for there to be work
        if(pool->qsize == 0) {
            if(pthread_cond_wait(&(pool->q_not_empty), &(pool->qlock)) != 0) {
                perror("pthread_cond_wait() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        // Work flag triggered
        // Checking if we are in the destruction process (again)
        if(pool->shutdown == 1) {
            if(pthread_mutex_unlock(&(pool->qlock)) != 0) {
                perror("pthread_mutex_unlock() failed\n");
                exit(EXIT_FAILURE);
            }
            return NULL;
        }
        // ========== Do work from the queue ==========
        // Pop the head
        work_t* work = pool->qhead;
        if(work == NULL) {
            if(pthread_mutex_unlock(&pool->qlock) != 0) {
                perror("pthread_mutex_unlock() failed\n");
                exit(EXIT_FAILURE);
            }
            continue;
        }
        pool->qhead = pool->qhead->next;
        pool->qsize -= 1;
        // Check if we need to signal the start of the destruction process
        if(pool->dont_accept == 1 && pool->qsize == 0) {
            if(pthread_cond_signal(&(pool->q_empty)) != 0) {
                perror("pthread_cond_signal() failed\n");
                exit(EXIT_FAILURE);
            }
        }
        // Free the mutex
        if(pthread_mutex_unlock(&pool->qlock) != 0) {
            perror("pthread_mutex_unlock() failed\n");
            exit(EXIT_FAILURE);
        }
        // Do the work (finally)
        (*(work->routine))(work->arg);
        free(work);
    }
}
