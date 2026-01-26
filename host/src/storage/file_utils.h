#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

#define START 0
#define END 1

/**
 * @brief Open file in i/o mode
 * @param *file_path  Path to file
 *
 * @return Fd of file, -1 for syserror 
 */
int file_open(const char *file_path);

/**
 * @brief Read a line from a file
 * @param Fd from file_open(), buffer to write to, sizeof(buffer), target line
 *
 * This function depends on the file's seeker position
 * Pair with file_seek(fd, START) for intended functionality
 * 
 * Read into garbage buffer until target line is reached
 * Read bytes from the target line into the target buffer
 *
 * @return Bytes read, 0 for EOF, -1 for invalid param (small buf), -2 for syserror
 */
ssize_t file_read_line(int fd, char *buffer, size_t buffer_size, int line);

/**
 * @brief Write a line to a file followed by \n, based on seeker
 * @param Fd from file_open(), text to write, strlen(text)
 *
 * This function depends on the file's seeker position
 * Pair with file_seek(fd, END) to write a new line
 * Otherwise it will rewrite the seeker's current line 
 *
 * @return Bytes wrote, -1 for invalid param, -2 for syserror
 */
ssize_t file_write_line(int fd, const char *text, size_t text_len);

/**
 * @brief Set a file's seeker to the start or end of file
 * @param Fd from open(), position to move to (see defines)
 * 
 * This function determines how write_line() and read_line() run
 * Calling this with END will allow for writing to the end of the file
 * Calling this with START will allow for proper use of read_line()
 *
 * @return 0 for success, -1 for invalid param
 */
int file_seek(int fd, int position);

/*
 * @brief Closes file
 * @param Fd of file to close
 *
 * @return 0 for success, -1 for syserror
 */
int file_close(int fd);

#endif // UTILS_H
