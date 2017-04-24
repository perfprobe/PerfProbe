#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <endian.h>
#include <assert.h>

#include "events.h"

typedef void (*decode_event_data)(FILE* stream, const struct event_hdr* header, const struct timeval* tv, void* data);
typedef void (*print_event_data)(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const void* data);

/* ==================================== Event Decode and Print Functions ========================= */
static void decode_sync_log_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct sync_log_event* data) {
  fread(&data->magic, 8, 1, stream);
}

static void print_sync_log_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct sync_log_event* data) {
  fprintf(stream, "{\"magic\":\"%s\"}", data->magic);
}

static void decode_missed_count_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct missed_count_event* data) {
  fread(&data->count, 4, 1, stream);
}

static void print_missed_count_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct missed_count_event* data) {
  fprintf(stream, "{\"count\":%u}", data->count);
}

static void decode_general_lock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct general_lock_event* data) {
  fread(&data->lock, 4, 1, stream);
}

static void print_general_lock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct general_lock_event* data) {
  fprintf(stream, "{\"lock\":%u}", data->lock);
}

static void decode_general_notify_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct general_notify_event* data) {
  fread(&data->lock, 4, 1, stream);
  fread(&data->pid, 2, 1, stream);
}

static void print_general_notify_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct general_notify_event* data) {
  fprintf(stream, "{\"lock\":%u,\"pid\":%u}", data->lock, data->pid);
}

static void decode_hotcpu_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct hotcpu_event* data) {
  fread(&data->cpu, 1, 1, stream);\
}

static void print_hotcpu_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct hotcpu_event* data) {
  fprintf(stream, "{\"cpu\":%u}", data->cpu);
}

static void decode_binder_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct binder_event* data) {
  fread(&data->transaction, 4, 1, stream);
}

static void print_binder_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct binder_event* data) {
  fprintf(stream, "{\"trans\":%u}", data->transaction);
}

static void decode_cpufreq_set_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct cpufreq_set_event* data) {
  fread(&data->cpu, 1, 1, stream);
  fread(&data->old_freq, 4, 1, stream);
  fread(&data->new_freq, 4, 1, stream);
}

static void print_cpufreq_set_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct cpufreq_set_event* data) {
  fprintf(stream, "{\"cpu\":%u,\"old_khz\":%u,\"new_khz\":%u}", data->cpu, data->old_freq, data->new_freq);
}

static char* TASK_STATE[] = {"R", "I", "U", "S", "T", NULL, NULL, "D", "W"};

static void decode_context_switch_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct context_switch_event* data) {
  fread(&data->new_pid, 2, 1, stream);
  fread(&data->state, 1, 1, stream);
}

static void print_context_switch_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct context_switch_event* data) {
  fprintf(stream, "{\"old\":%u,\"new\":%u,\"state\":\"%s\"}", header->pid, data->new_pid, TASK_STATE[data->state ? __builtin_ctz(data->state)+1 : 0]);
}

static void decode_wake_lock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct wake_lock_event* data) {
  fread(&data->lock, 4, 1, stream);
  fread(&data->timeout, 4, 1, stream);
}

static void print_wake_lock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct wake_lock_event* data) {
  fprintf(stream, "{\"lock\":%u,\"timeout\":", data->lock);
  if (data->timeout)
    fprintf(stream, "%u", data->timeout);
  else
    fprintf(stream, "null");
  fprintf(stream, "}");
}

static void decode_wake_unlock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct wake_unlock_event* data) {
  fread(&data->lock, 4, 1, stream);
}

static void print_wake_unlock_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct wake_unlock_event* data) {
  fprintf(stream, "{\"lock\":%u}", data->lock);
}

static void decode_fork_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct fork_event* data) {
  fread(&data->pid, 4, 1, stream);
}

static void print_fork_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct fork_event* data) {
  fprintf(stream, "{\"pid\":%u,\"tgid\":%u}", data->pid, data->tgid);
}						   

static void decode_thread_name_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct thread_name_event* data) {
  fread(&data->pid, 2, 1, stream);
  fread(&data->comm, 16, 1, stream);
}

static void print_thread_name_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct thread_name_event* data) {
  printf("{\"pid\":%u,\"name\":\"%s\"}", data->pid, data->comm);
}

static void decode_cpufreq_mod_timer_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct cpufreq_mod_timer_event* data) {
  fread(&data->cpu, 1, 1, stream);
  fread(&data->microseconds, 4, 1, stream);
}

static void print_cpufreq_mod_timer_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct cpufreq_mod_timer_event* data) {
  printf("{\"cpu\":%u,\"microseconds\":%u}", data->cpu, data->microseconds);
}

static void decode_cpufreq_timer_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, struct cpufreq_timer_event* data) {
  fread(&data->cpu, 1, 1, stream);
}

static void print_cpufreq_timer_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, const struct cpufreq_timer_event* data) {
  printf("{\"cpu\":%u}", data->cpu);
}

/* ======================================================================================================== */

#define DECLARE_TYPE_DATA(type_suffix) struct type_suffix##_event shared_##type_suffix##_event

static DECLARE_TYPE_DATA(sync_log);
static DECLARE_TYPE_DATA(missed_count);
static DECLARE_TYPE_DATA(context_switch);
static DECLARE_TYPE_DATA(hotcpu);
static DECLARE_TYPE_DATA(cpufreq_set);
static DECLARE_TYPE_DATA(wake_lock);
static DECLARE_TYPE_DATA(wake_unlock);
static DECLARE_TYPE_DATA(fork);
static DECLARE_TYPE_DATA(thread_name);
static DECLARE_TYPE_DATA(general_lock);
static DECLARE_TYPE_DATA(general_notify);
static DECLARE_TYPE_DATA(binder);
static DECLARE_TYPE_DATA(cpufreq_mod_timer);
static DECLARE_TYPE_DATA(cpufreq_timer);

struct event_type {
  const char* const type_json;
  const decode_event_data decoder;
  const print_event_data printer;
  void* data;
};

#define INIT_TYPE(type, suffix) [EVENT_##type] = {.type_json = #type, \
						  .decoder = (decode_event_data) decode_##suffix##_event,	\
						  .printer = (print_event_data) print_##suffix##_event, \
						  .data = &shared_##suffix##_event}

#define INIT_TYPE_SIMPLE(type) [EVENT_##type] = {.type_json = #type, \
					  .decoder = NULL,    \
					  .printer = NULL,    \
					  .data = NULL}

static struct event_type EVENT_TYPES[256] = {
  INIT_TYPE(SYNC_LOG, sync_log),
  INIT_TYPE(MISSED_COUNT, missed_count),
  INIT_TYPE(CPU_ONLINE, hotcpu),
  INIT_TYPE(CPU_DOWN_PREPARE, hotcpu),
  INIT_TYPE(CPU_DEAD, hotcpu),
  INIT_TYPE(CPUFREQ_SET, cpufreq_set),
  INIT_TYPE(BINDER_PRODUCE_ONEWAY, binder),
  INIT_TYPE(BINDER_PRODUCE_TWOWAY, binder),
  INIT_TYPE(BINDER_PRODUCE_REPLY, binder),
  INIT_TYPE(BINDER_CONSUME, binder),
  INIT_TYPE_SIMPLE(SUSPEND_START),
  INIT_TYPE_SIMPLE(SUSPEND),
  INIT_TYPE_SIMPLE(RESUME),
  INIT_TYPE_SIMPLE(RESUME_FINISH),
  INIT_TYPE(WAKE_LOCK, wake_lock),
  INIT_TYPE(WAKE_UNLOCK, wake_unlock),
  INIT_TYPE(CONTEXT_SWITCH, context_switch),
  INIT_TYPE_SIMPLE(PREEMPT_TICK),
  INIT_TYPE_SIMPLE(PREEMPT_WAKEUP),
  INIT_TYPE_SIMPLE(YIELD),
  INIT_TYPE_SIMPLE(IDLE_START),
  INIT_TYPE_SIMPLE(IDLE_END),
  INIT_TYPE(FORK, fork),
  INIT_TYPE(THREAD_NAME, thread_name),
  INIT_TYPE_SIMPLE(EXIT),
  INIT_TYPE_SIMPLE(DATAGRAM_BLOCK),
  INIT_TYPE_SIMPLE(DATAGRAM_RESUME),
  INIT_TYPE_SIMPLE(STREAM_BLOCK),
  INIT_TYPE_SIMPLE(STREAM_RESUME),
  INIT_TYPE_SIMPLE(SOCK_BLOCK),
  INIT_TYPE_SIMPLE(SOCK_RESUME),
  INIT_TYPE_SIMPLE(IO_BLOCK),
  INIT_TYPE_SIMPLE(IO_RESUME),
  INIT_TYPE(WAITQUEUE_WAIT, general_lock),
  INIT_TYPE(WAITQUEUE_WAKE, general_lock),
  INIT_TYPE(WAITQUEUE_NOTIFY, general_notify),
  INIT_TYPE(MUTEX_LOCK, general_lock),
  INIT_TYPE(MUTEX_WAIT, general_lock),
  INIT_TYPE(MUTEX_WAKE, general_lock),
  INIT_TYPE(MUTEX_NOTIFY, general_notify),
  INIT_TYPE(SEMAPHORE_LOCK, general_lock),
  INIT_TYPE(SEMAPHORE_WAIT, general_lock),
  INIT_TYPE(SEMAPHORE_WAKE, general_lock),
  INIT_TYPE(SEMAPHORE_NOTIFY, general_notify),
  INIT_TYPE(FUTEX_WAIT, general_lock),
  INIT_TYPE(FUTEX_WAKE, general_lock),
  INIT_TYPE(FUTEX_NOTIFY, general_notify),
  INIT_TYPE_SIMPLE(CPUFREQ_BOOST),
  INIT_TYPE_SIMPLE(CPUFREQ_WAKE_UP),
  INIT_TYPE(CPUFREQ_MOD_TIMER, cpufreq_mod_timer),
  INIT_TYPE(CPUFREQ_DEL_TIMER, cpufreq_timer),
  INIT_TYPE(CPUFREQ_TIMER, cpufreq_timer)
};

#define PID_MASK 0x7FFF

static void fprint_json_event(FILE* stream, const struct event_hdr* header, const struct timeval* tv, print_event_data printer, const void* data) {
  static const char* const EVENT_JSON_START = "{\"event\":\"%s\",\"time\":{\"sec\":%u,\"usec\":%u},\"cpu\":%u,\"pid\":%u,\"irq\":%s,\"data\":";
  static const char* const EVENT_JSON_END = "}\n";

  const char* type_name = EVENT_TYPES[header->event_type].type_json;  
  const char* irq = (0x8000 & header->pid) ? "true" : "false";
  
  fprintf(stream, EVENT_JSON_START, type_name, tv->tv_sec, tv->tv_usec, GET_CPU(header), header->pid & PID_MASK, irq);
  if (printer)
    printer(stream, header, tv, data);
  else
    fprintf(stream, "null");
  fprintf(stream, EVENT_JSON_END);
}

inline size_t read_next_header(struct event_hdr* header, FILE* stream) {
  return fread(header, sizeof(*header), 1, stream);
}

// From http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
#define SIGN_EXT(val, b) ((val ^ (1 << (b-1))) - (1 << (b-1)))

/* Only works on little endian arch */
static inline void read_next_timestamp(struct timeval* tv, struct event_hdr* header, FILE* stream, char suspended) {
  static struct timeval prev;

  int sec_len;
  int usec_len;
  tv->tv_sec = 0;
  tv->tv_usec = 0;

  sec_len = GET_SEC_LEN(header);
  usec_len = GET_USEC_LEN(header);

  fread(&tv->tv_sec, 1, sec_len, stream);
  fread(&tv->tv_usec, 1, usec_len, stream);

  tv->tv_sec = SIGN_EXT(tv->tv_sec, sec_len*8);
  tv->tv_usec = SIGN_EXT(tv->tv_usec, usec_len*8);

  if (header->event_type == EVENT_SYNC_LOG) {
    prev = *tv;
  }
  else {
    prev.tv_sec += tv->tv_sec;
    prev.tv_usec += tv->tv_usec;
    *tv = prev;
  }

}

int main() {
  struct event_type* type;
  struct event_hdr header;
  struct timeval timestamp;

  char suspended = 0;

  FILE* istream = stdin;
  FILE* ostream = stdout;

  while (read_next_header(&header, istream) && !feof(istream)) {
    read_next_timestamp(&timestamp, &header, istream, suspended);

    type = &EVENT_TYPES[header.event_type];
    if (!type){
      fprintf(stderr, "Unknown event type received: %u\n", header.event_type);
      exit(1);
    }

    if (type->decoder)
      type->decoder(istream, &header, &timestamp, type->data);

    /* Track if the device is currently suspended or not.  If device
       was already suspended, drop the event, because the event timestamp may be
       incorrect. */
    if (header.event_type == EVENT_SUSPEND)
      suspended = 1;
    else if (header.event_type == EVENT_RESUME)
      suspended = 0;
    else
      if (suspended)
	continue;
    
    fprint_json_event(ostream, &header, &timestamp, type->printer, type->data);
  }

  return 0;
}
