typedef enum {
    RUN_MODE_DETECT,    /* 検知 */
    RUN_MODE_EXCLUSION, /* 排除 */
    RUN_MODE_DELAY      /* 遅延 */
} run_mode; 

struct ncad_task_context {
    run_mode mode;
    long delay_sec;
}; 
