#ifndef PADCOORDS_H_
#define PADCOORDS_H_

//////
#define JOYPAD_X0           178
#define JOYPAD_Y0           0
#define JOYPAD_X2           242
#define JOYPAD_Y2           64

// Jump
#define BTN_JUMP_X0         132
#define BTN_JUMP_Y0         0
#define BTN_JUMP_X2         178
#define BTN_JUMP_Y2         46

// Shoot
#define BTN_SHOOT_X0        40
#define BTN_SHOOT_Y0        0
#define BTN_SHOOT_X2        86
#define BTN_SHOOT_Y2        46

// Activate
#define BTN_ACTIVATE_X0     86
#define BTN_ACTIVATE_Y0     0
#define BTN_ACTIVATE_X2     132
#define BTN_ACTIVATE_Y2     46

//
#define JOYPAD_SIZE         (JOYPAD_X2 - JOYPAD_X0)
#define BUTTON_SIZE         BTN_JUMP_Y2

#endif // PADCOORDS_H_
