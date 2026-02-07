#ifndef SESSION_H
#define SESSION_H

#include <host/common/tsys_errors.h>

#define SESSION_PWD_VALID 1
#define SESSION_PWD_INVALID 0

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

/**
 * @brief Validate password against confirmed ones
 * @note TODO: Currently a STUB returning SESSION_PWD_VALID always
 *
 * @param pwd_fd - File desc of password storage file
 * @param pwd_given - Given password to validate
 * @param output - Buffer to hold output (SESSION_PWD_VALID/INVALID)
 *
 * @retval TSYS_OK - Password was checked and output was wrote to
 * @retval TSYS_E_INVAL - pwd_fd was <0, pwd or output was NULL
 * @retval TSYS_E_FILE - Failed to read from file
 */
enum tsys_err session_validate_pwd(
    int pwd_fd,
    const char *pwd,
    int *output);

/**
 * @brief Notify firmware that password was valid
 * @note TODO: Currently a STUB returning TSYS_OK always
 *
 * The data packet will have type AUTH and payload[0] set to
 * SESSION_PWD_VALID. Firmware should recognize this as valid.
 *
 * @retval TSYS_OK - Data packet was sent
 * @retval TSYS_E_PORT - Failed to send packet
 */
enum tsys_err session_send_valid_pwd(void);

/**
 * @brief Notify firmware that password was invalid
 * @note TODO: Currently a STUB returning TSYS_OK always
 *
 * The data packet will have type AUTH and payload[0] set to
 * SESSION_PWD_INVALID. Firmware should recognize this as invalid.
 *
 * @retval TSYS_OK - Data packet was sent
 * @retval TSYS_E_PORT - Failed to send packet
 */
enum tsys_err session_send_invalid_pwd(void);

#endif // SESSION_H
