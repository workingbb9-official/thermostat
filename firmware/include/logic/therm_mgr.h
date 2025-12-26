#ifndef THERM_MGR_H
#define THERM_MGR_H

/*
 * @brief Initialize thermistor for use
 *
 */
void therm_mgr_init(void);

/*
 * @brief Calculate temperature
 *
 * @return Current temperature in celsius
 */
float therm_mgr_get_temp(void);

#endif // THERM_MGR_H
