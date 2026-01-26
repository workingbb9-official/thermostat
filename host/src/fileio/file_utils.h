#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <sys/types.h>
#include <host/common/file_errors.h>

#define START 0
#define END 1

/**
 * Desc: Open a file in i/o mode
 *
 * Params:
 *      file_path: Path of the file
 *
 * Return:
 *      >0: File desc of the file
 *      FILE_E_OPEN: Failed to open file
 */
int file_open(const char *file_path);

/**
 * Desc: Read a line from a file
 *
 * Params:
 *      file: File desc to read from
 *      buf: Buffer to store data
 *      bytes: # of bytes to read
 *      line: target line to read
 *
 * Return:
 *      >=0: Bytes read into buf (0 means empty file)
 *      FILE_E_INVAL: File was <0 or buffer was NULL
 *      FILE_E_READ: Failed to read from file
 *
 * Notes:
 *      This function depends on the file's seeker position.
 *      Pair with file_seek(fd, START) for intended functionality.
 *      Buffer is null-terminated before returning
 */
ssize_t file_read_line(int file, char *buf_out, size_t bytes, size_t line);

/**
 * Desc: Write a line and a '\n' to a file
 *
 * Params:
 *      file: File desc to write to
 *      buf: Data to write
 *      bytes: # of bytes to write
 *
 * Return:
 *      >0: Bytes written to port
 *      FILE_E_INVAL: File was <0 or buffer was NULL
 *      FILE_E_WRITE: Failed to write data
 * 
 * Notes:
 *      This function depends on the file's seeker position.
 *      Pair with file_seek(fd, END) to write a new line.
 *      Otherwise, it will rewrite the seeker's current line.
 */
ssize_t file_write_line(int file, const char *buf, size_t bytes);

/**
 * Desc: Set a file seeker to start or end
 *
 * Params:
 *      file: File desc to set seeker
 *      pos: START or END of file
 * 
 * Return:
 *      FILE_OK: Seeker was set
 *      FILE_E_INVAL: File was <0 or invalid pos
 *      FILE_E_SEEK: Failed to set seeker
 *
 * Notes: 
 *      This function determines how write_line() and read_line() run.
 *      Calling this with END will allow for writing to the end of the file.
 *      Calling this with START will allow for proper use of read_line().
 */
enum file_err file_seek(int file, int pos);

/**
 * Desc: Close a file
 *
 * Params:
 *      file: File desc of file to close
 *
 * Return:
 *      FILE_OK: Close was successful
 *      FILE_E_CLOSE: Failed to close file
 */
enum file_err file_close(int file);

#endif // FILE_UTILS_H
