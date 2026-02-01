#ifndef STATE_LOGIN_H
#define STATE_LOGIN_H

#include <host/common/tsys_errors.h>

/**
 * // WIP //
 * 
 * Desc: Record a login by storing in file
 *
 * Params:
 *      login_fd: File desc of login file
 *
 * Return:
 *      TSYS_OK: Login recorded
 *      TSYS_E_FILE: Failed to write to file
 */
enum tsys_err login_record_login(int login_fd);

#endif // STATE_LOGIN_H
