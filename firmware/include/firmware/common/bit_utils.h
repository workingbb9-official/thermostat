#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define CLR_BIT(REG, BIT) (REG &= ~(1 << BIT))
#define READ_BIT(REG, BIT) (REG & (1 << BIT))

#endif // BIT_UTILS_H
