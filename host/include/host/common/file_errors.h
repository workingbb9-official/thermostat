#ifndef FILE_ERRORS_H
#define FILE_ERRORS_H

// FILE errors run from -49 to -40
enum file_err {
    FILE_OK = 0,
    FILE_E_INVAL = -49,
    FILE_E_OPEN,
    FILE_E_READ,
    FILE_E_WRITE,
    FILE_E_SEEK,
    FILE_E_CLOSE = -44
};

#endif // FILE_ERRORS_H
