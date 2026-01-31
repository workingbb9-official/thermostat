#ifndef STATE_LOGIN_H
#define STATE_LOGIN_H

#include <host/common/tsys_errors.h>

/**
 * // WIP //
 * 
 * Desc: Record a login by storing in file
 *
 * Return:
 *      TSYS_OK: Login recorded
 *      TSYS_E_FILE: Failed to write to file
 */
enum tsys_err login_record_login(void);

#endif // STATE_LOGIN_H
