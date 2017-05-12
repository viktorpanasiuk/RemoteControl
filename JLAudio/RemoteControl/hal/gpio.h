#ifndef GPIO_H_
#define GPIO_H_

/* GPIO direction as input */
#define GPIO_DIR_I(DDRx, DDxn) DDRx &= ~_BV(DDxn)

/* GPIO direction as output */
#define GPIO_DIR_O(DDRx, DDxn) DDRx |= _BV(DDxn)

/* GPIO level is low */
#define GPIO_LVL_0(PORTx, Pxn) PORTx &= ~_BV(Pxn)

/* GPIO level is high */
#define GPIO_LVL_1(PORTx, Pxn) PORTx |= _BV(Pxn)

#endif /* GPIO_H_ */