#ifndef SESSION_H
#define SESSION_H

#include <host/common/tsys_errors.h>

/**
 * Desc: Find timestamp of login and store
 *
 * Params:
 *      session_fd: File desc of session logging file
 *
 * Return:
 *      TSYS_OK: Login recorded
 *      TSYS_E_INVAL: session_fd was invalid
 *      TSYS_E_FILE: Failed to write to file
 */
enum tsys_err session_record_login(int session_fd);

/**
 * Desc: Find timestamp of logout and store
 *
 * Params:
 *      session_fd: File desc of session logging file
 *
 * Return:
 *      TSYS_OK: Logout recorded
 *      TSYS_E_INVAL: session_fd was invalid
 *      TSYS_E_FILE: Failed to write to file
 */
enum tsys_err session_record_logout(int session_fd);

#endif // SESSION_H
