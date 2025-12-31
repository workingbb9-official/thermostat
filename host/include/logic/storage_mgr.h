#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

/*
 * @brief Opens all storage files
 *
 * Holds internal data about paths and fds
 * If storage paths change, change them here
 *
 * @return 0 for success, -1 for syserror 
 */
int storage_mgr_init(void);

/*
 * @brief Writes data to the temperature file
 * @param Float to write into the file
 *
 * Seeks to the end of the file
 * Converts data into string and writes to file
 *
 * @return 0 for success, -1 for syserror
 */
int storage_mgr_write_temp(float data);

/*
 * @brief Reads one log from temperature file
 * @param Float buffer to store data in
 *
 * Seeks to the beginning of file
 * Converts read line into float and stores
 *
 * @return 0 for success, -1 for invalid params, -2 for syserror
 */
int storage_mgr_read_temp(float *buffer, int line);

/*
 * @brief Closes all storage files
 *
 * @return 0 for success, -1 for syserror
 */
int storage_mgr_close(void);

#endif // STORAGE_MGR_H
