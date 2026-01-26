#ifndef TSYS_ERRORS_H
#define TSYS_ERRORS_H

/* TSYS errors run from -69 to -60 */
enum tsys_err {
    TSYS_OK = 0,
    TSYS_E_INVAL = -69,
    TSYS_E_PORT,
    TSYS_E_FILE,
    TSYS_E_ANALYZE,
    TSYS_E_SIGNAL,
    TSYS_E_NET,
    TSYS_E_WEATHER = -63
};

#endif // TSYS_ERRORS_H
