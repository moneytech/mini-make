#ifndef MAKE_LISTENER_H
#define MAKE_LISTENER_H

struct make_assignment_stmt;
struct make_command;
struct make_include_stmt;
struct make_string;

struct make_listener {
  /** @brief Used for passing custom data to the
   * callback functions. */
  void *user_data;
  /** @brief Executed when an assignment statement
   * is found. */
  int (*on_assignment_stmt)(void *user_data,
                            const struct make_assignment_stmt *assignment_stmt);
  /** @brief Executed when an include statement
   * is found. */
  int (*on_include_stmt)(void *user_data,
                         const struct make_include_stmt *include_stmt);
  /** @brief Executed right before a rule is parsed. */
  int (*on_rule_start)(void *user_data);
  /** @brief Executed right after a rule is parsed. */
  int (*on_rule_finish)(void *user_data);
  /** @brief Executed when a target is parsed. */
  int (*on_target)(void *user_data,
                   const struct make_string *target);
  /** @brief Executed when a prerequisite is parsed. */
  int (*on_prerequisite)(void *user_data,
                         const struct make_string *prerequisite);
  /** @brief Executed when a command is parsed. */
  int (*on_command)(void *user_data,
                    const struct make_command *command);
  /** @brief Executed when an unexpected character is found. */
  void (*on_unexpected_char)(void *user_data, char c);
  /** @brief Executed when a rule is missing a ':' separator. */
  void (*on_missing_separator)(void *user_data);
  /** @brief Executed when the end of file is reached unexpectedly. */
  void (*on_unexpected_eof)(void *user_data);
};

#endif /* MAKE_LISTENER_H */
