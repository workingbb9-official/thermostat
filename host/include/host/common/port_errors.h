#ifndef PORT_ERRORS_H
#define PORT_ERRORS_H

// PORT errors run from -39 to -30
enum port_err {
    PORT_OK = 0,
    PORT_E_INVAL = -39,
    PORT_E_OPEN,
    PORT_E_CONFIG,
    PORT_E_READ,
    PORT_E_WRITE,
    PORT_E_CLOSE = -34
};

#endif // PORT_ERRORS_H
