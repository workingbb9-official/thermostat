#ifndef SESSION_H
#define SESSION_H

#include <host/common/tsys_errors.h>

/**
 * @brief Find timestamp of login and store
 *
 * @param session_fd - File desc of session logging file
 *
 * @retval TSYS_OK - Login recorded
 * @retval TSYS_E_INVAL - session_fd was invalid
 * @retval TSYS_E_FILE - Failed to write to file
 */
enum tsys_err session_record_login(int session_fd);

/**
 * @brief Find timestamp of logout and store
 *
 * @param session_fd - File desc of session logging file
 *
 * @retval TSYS_OK - Logout recorded
 * @retval TSYS_E_INVAL - session_fd was invalid
 * @retval TSYS_E_FILE - Failed to write to file
 */
enum tsys_err session_record_logout(int session_fd);

#endif // SESSION_H
