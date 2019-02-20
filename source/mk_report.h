#ifndef mk_report_h
#define mk_report_h

#include <stddef.h>

struct mk_state;

#ifdef __GNUC__
#define MK_PRINTF_ATTRIBUTE(fmt_pos) __attribute__((format(printf, fmt_pos, fmt_pos + 1)))
#else /* __GNUC__ */
#define MK_PRINTF_ATTRIBUTE(fmt_pos)
#endif /* __GNUC__ */

void mk_report(struct mk_state* state, const char* fmt, ...) MK_PRINTF_ATTRIBUTE(2);

void mk_syntax_report(struct mk_state* state, size_t line, const char* fmt, ...) MK_PRINTF_ATTRIBUTE(3);

#endif /* mk_report_h */
