#ifndef mk_state_h
#define mk_state_h

struct mk_state;

struct mk_state* mk_state_create();

void mk_state_destroy(struct mk_state* state);

int mk_state_define(struct mk_state* state,
                    const char* name,
                    const char* value);

const char* mk_state_find(const struct mk_state* state, const char* name);

#ifdef __GNUC__
#define MK_STATE_REPORT_ATTRIBUTES __attribute__((format (printf, 2, 3)))
#else
#define MK_STATE_REPORT_ATTRIBUTES
#endif

#endif /* mk_state_h */
