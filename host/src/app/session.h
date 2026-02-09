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
 * @brief Compare password to valid ones
 *
 * @param pwd_fd - File desc of password storage file
 * @param pwd_given - Given password to validate
 * @param user - Buffer to hold username, set to '\0' for invalid
 * @param len - Length of user buffer
 *
 * @retval TSYS_OK - Password was checked and result in user
 * @retval TSYS_E_INVAL - pwd_fd was <0, pwd or user was NULL
 * @retval TSYS_E_FILE - Failed to read from file
 */
enum tsys_err session_validate_pwd(
    int pwd_fd,
    const char *pwd,
    char *user,
    int len);

/**
 * @brief Send valid username of user to firmware
 *
 * The data packet will have type AUTH and payload set to represent a
 * string. Firmware should recognize payload as a username.
 *
 * @param user - The username to send
 *
 * @retval TSYS_OK - Data packet was sent
 * @retval TSYS_E_PORT - Failed to send packet
 */
enum tsys_err session_send_valid_pwd(const char *user);

/**
 * @brief Notify firmware that password was invalid
 *
 * The data packet will have type AUTH and payload[0] set to
 * PAYLOAD_NONE. Firmware should recognize this as invalid.
 *
 * @retval TSYS_OK - Data packet was sent
 * @retval TSYS_E_PORT - Failed to send packet
 */
enum tsys_err session_send_invalid_pwd(void);

#endif // SESSION_H
