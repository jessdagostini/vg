#include "time-utils.h"

entryPoint * timers = NULL;

long long int entryPointCount = 0;

pthread_rwlock_t lock_time_utils;

// For gbwt_extender
// char regions[10][40] = {"gapless_extender", "for_each_seed", "if_contains", "construct_best_match", "priority_queue_extensions", "move_extensions", "to_right", "to_left", "best_match_move", "best_match_empty"};
// For minimizer_mapper
// char regions[10][40] = {"cluster_seeds", "process_until_threshold_c", "score_extensions", "process_until_threshold_b"};
char regions[10][40] = {"total_extension", "total_main", "total_extension_no_cache"};
// char regions[10][40] = {"process_until_threshold_c", "attempt_rescue", "find_optimal_tail_align", "giraffe_main"};
// char regions[16][40] = {"parse_args", "IO", "find_minimizers", "find_seeds", "cluster_seeds", "cluster_score", "process_until_threshold_c", "extension_score", "discard_processed_cluster_by_score", "process_until_threshold_b", "process_until_threshold_a", "mapq", "annotation", "giraffe_main", "unpaired_alignments"};

void time_utils_dump() {
    // fprintf(stderr, "Entrou dump\n");
    entryPoint *s = NULL;
    entryPoint *tmp = NULL;

    HASH_ITER(hh, timers, s, tmp) {
        fprintf(stderr, "%s, %0.10f, %0.10f, %d\n", regions[s->id_func], s->start, s->end, s->thread);
    }
}

void time_utils_add(double start, double end, int id_func, int thread) {
    if (pthread_rwlock_wrlock(&lock_time_utils) != 0) {
        fprintf(stderr, "Can't get mutex\n");
        exit(-1);
    }

    entryPoint *s = (entryPoint *) malloc(sizeof *s);

    s->start = start;
    s->end = end;
    s->thread = thread;
    s->id_func = id_func;
    s->id = entryPointCount;

    HASH_ADD_INT(timers, id, s);  /* id is the key field */

    entryPointCount++;

    pthread_rwlock_unlock(&lock_time_utils);
    // pthread_mutex_unlock(&incoming_queue_mutex);
}