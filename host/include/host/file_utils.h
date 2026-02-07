#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <host/common/file_errors.h>
#include <sys/types.h>

#define FILE_UTILS_START 0
#define FILE_UTILS_END 1

/**
 * @brief Open a file in i/o mode
 *
 * @param file_path - Path of the file
 *
 * @return File desc of file, or FILE_E_OPEN
 */
int file_open(const char *file_path);

/**
 * @brief Read a line from a file
 *
 * This function depends on the file's seeker position.
 * Buffer is null-terminated before returning.
 *
 * @param file - File desc to read from
 * @param buf - Buffer to store data
 * @param bytes - # of bytes to read
 *
 * @return Bytes read into buf (0 if empty file), or FILE error
 * @retval FILE_E_INVAL - File was <0 or buffer was NULL
 * @retval FILE_E_READ - Failed to read from file
 */
ssize_t file_read_line(int file, char *buf_out, size_t bytes);

/**
 * @brief Write a line and a '\n' to a file
 *
 * This function depends on the file's seeker position.
 * Pair with file_seek(fd, FILE_UTILS_END) to write a new line.
 * Otherwise, it will rewrite the seeker's current line.
 *
 * @param file - File desc to write to
 * @param buf - String to write (no null-terminator)
 * @param bytes - # of bytes to write
 *
 * @return Bytes written to buffer (without '\n'), or FILE error
 * @retval FILE_E_INVAL - File was <0 or buffer was NULL
 * @retval FILE_E_WRITE - Failed to write data
 */
ssize_t file_write_line(int file, const char *buf, size_t bytes);

/**
 * @brief Set a file seeker to start or end
 *
 * @param file - File desc to set seeker
 * @param pos - FILE_UTILS_START or FILE_UTILS_END
 *
 * @retval FILE_OK - Seeker was set
 * @retval FILE_E_INVAL - File was <0 or invalid pos
 * @retval FILE_E_SEEK - Failed to set seeker
 */
enum file_err file_seek(int file, int pos);

/**
 * @brief Close a file
 *
 * @param file - File desc of file to close
 *
 * @retval FILE_OK - Close was successful
 * @retval FILE_E_CLOSE - Failed to close file
 */
enum file_err file_close(int file);

#endif // FILE_UTILS_H
