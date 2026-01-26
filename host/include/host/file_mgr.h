#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include <sys/types.h>
#include <host/common/file_errors.h>

/**
 * Desc: Open all files
 *
 * Return:
 *      FILE_OK: All files were opened
 *      FILE_E_OPEN: Failed to open a file
 *
 * Notes:
 *      This module holds data about file paths and descs.
 *      If file paths change, change them here.
 */
enum file_err file_mgr_init(void);

/**
 * Desc: Write temp to the temperature file
 * 
 * Params:
 *      temp: Value to write
 *
 * Return:
 *      FILE_OK: Temp was written
 *      FILE_E_SEEK: Failed to set seeker
 *      FILE_E_WRITE: Failed to write to file
 */
enum file_err file_mgr_write_temp(float data);

/**
 * Desc: Read one line from temperature file
 *
 * Params:
 *      val_out: Buffer to store value
 *      line: Target line to read from
 *
 * Return:
 *      FILE_OK: Value was read and stored
 *      FILE_INVAL: val_out was NULL
 *      FILE_E_SEEK: Failed to set seeker
 *      FILE_E_READ: Failed to read from file
 */
enum file_err file_mgr_read_temp(float *val_out, size_t line);

/**
 * Desc: Close all files
 *
 * Return:
 *      FILE_OK: All files were closed
 *      FILE_E_CLOSE: Failed to close a file
 */
enum file_err file_mgr_close(void);

#endif // STORAGE_MGR_H
