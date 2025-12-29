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
 * @brief Closes all storage files
 *
 * @return Error code (0 for success, 1 for failure)
 */
int storage_mgr_close(void);

#endif // STORAGE_MGR_H
