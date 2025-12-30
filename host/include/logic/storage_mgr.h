#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

/*
 * @brief Opens all storage files
 *
 * @return Error code (0 for success, 1 for failure)
 */
int storage_mgr_init(void);

/*
 * @brief Writes data to the temperature file
 * @param Float to write into the file
 *
 * @return Error code (0 for success, 1 for failure)
 */
int storage_mgr_write_temp(float data);

/*
 * @brief Reads one log from temperature file
 * @param Float buffer to store data in
 *
 * @return 0 for success, -1 for param error, -2 for syserror
 */
int storage_mgr_read_temp(float *buffer, int line);

/*
 * @brief Closes all storage files
 *
 * @return Error code (0 for success, 1 for failure)
 */
int storage_mgr_close(void);

#endif // STORAGE_MGR_H
