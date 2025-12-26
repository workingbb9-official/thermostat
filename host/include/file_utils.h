#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

/**
 * @brief Reads a line from a file
 * @param Fd from open(), buffer to write to, sizeof(buffer), target line
 *
 * @return Bytes read, 0 for EOF, -1 for syserror, -2 for invalid param
 */
ssize_t file_read_line(int fd, char *buffer, size_t buffer_size, int line);

/**
 * @brief Writes a line to a file (O_APPEND for new line), followed by \n
 * @param Fd from open(), text to write, sizeof(text)
 *
 * @return Bytes wrote, -1 for syserr, -2 for invalid param
 */
ssize_t file_write_line(int fd, const char *text, size_t text_size);

#endif // UTILS_H
