#ifndef FDMNGR_TOOL_H_
#define FDMNGR_TOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get flags to a file descriptor
 *
 * @param fd File descriptor
 * @return int the current flags on success, -1 on failure
 */
int fdmngr_tool_get_flags(int fd);

/**
 * @brief Set flags to a file descriptor
 *
 * @param fd File descriptor
 * @param flags Flags to set (e.g., O_NONBLOCK)
 * @return int the new flags on success, -1 on failure
 */
int fdmngr_tool_set_flags(int fd, int flags);

/**
 * @brief Add flags to a file descriptor
 *
 * @param fd File descriptor
 * @param flags Flags to add (e.g., O_NONBLOCK)
 * @return int the new flags on success, -1 on failure
 */
int fdmngr_tool_add_flags(int fd, int flags);

/**
 * @brief Sub flags to a file descriptor
 *
 * @param fd File descriptor
 * @param flags Flags to sub (e.g., O_NONBLOCK)
 * @return int the new flags on success, -1 on failure
 */
int fdmngr_tool_sub_flags(int fd, int flags);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef FDMNGR_TOOL_H_ */
