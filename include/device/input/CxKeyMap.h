#ifndef CX_DEVICE_INPUT_CXKEYMAP_H
#define CX_DEVICE_INPUT_CXKEYMAP_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxKeyMap.h
 * Code for maintaining the current key state for keyboard input.
 *
 * @author Catlin Zilinski
 * @date Jan 1, 2016
 */

#include "core/Cx.h"

/**
 * Mapping for the keyboard input keys.
 * We use the standard US layout and map the keys according
 * to the USB HID 1.12v2 specs.
 * 
 * We've decided to use #defines instead of enums as it makes
 * certain things easier and the capital style is more commonly
 * used / recognized.
 */
#define CX_KEY_UNKNOWN -1

#define CX_KEY_A 4
#define CX_KEY_B 5
#define CX_KEY_C 6
#define CX_KEY_D 7
#define CX_KEY_E 8
#define CX_KEY_F 9
#define CX_KEY_G 10
#define CX_KEY_H 11
#define CX_KEY_I 12
#define CX_KEY_J 13
#define CX_KEY_K 14
#define CX_KEY_L 15
#define CX_KEY_M 16
#define CX_KEY_N 17
#define CX_KEY_O 18
#define CX_KEY_P 19
#define CX_KEY_Q 20
#define CX_KEY_R 21
#define CX_KEY_S 22
#define CX_KEY_T 23
#define CX_KEY_U 24
#define CX_KEY_V 25
#define CX_KEY_W 26
#define CX_KEY_X 27
#define CX_KEY_Y 28
#define CX_KEY_Z 29

#define CX_KEY_1 30
#define CX_KEY_2 31
#define CX_KEY_3 32
#define CX_KEY_4 33
#define CX_KEY_5 34
#define CX_KEY_6 35
#define CX_KEY_7 36
#define CX_KEY_8 37
#define CX_KEY_9 38
#define CX_KEY_0 39

#define CX_KEY_ENTER 40
#define CX_KEY_ESC 41
#define CX_KEY_BACKSPACE 42
#define CX_KEY_TAB 43
#define CX_KEY_SPACE 44

#define CX_KEY_MINUS 45
#define CX_KEY_EQUAL 46
#define CX_KEY_LEFT_BRACKET 47
#define CX_KEY_RIGHT_BRACKET 48
#define CX_KEY_BACKSLASH 49

/* Keyboard Non-US # and ~ */
#define CX_KEY_WORLD_1 50

#define CX_KEY_SEMICOLON 51
#define CX_KEY_APOSTROPHE 52
#define CX_KEY_GRAVE_ACCENT 53
#define CX_KEY_COMMA 54
#define CX_KEY_PERIOD 55
#define CX_KEY_SLASH 56

#define CX_KEY_CAPSLOCK 57

#define CX_KEY_F1 58
#define CX_KEY_F2 59
#define CX_KEY_F3 60
#define CX_KEY_F4 61
#define CX_KEY_F5 62
#define CX_KEY_F6 63
#define CX_KEY_F7 64
#define CX_KEY_F8 65
#define CX_KEY_F9 66
#define CX_KEY_F10 67
#define CX_KEY_F11 68
#define CX_KEY_F12 69

#define CX_KEY_PRINT_SCREEN 70
#define CX_KEY_SCROLL_LOCK 71
#define CX_KEY_PAUSE 72

#define CX_KEY_INSERT 73
#define CX_KEY_HOME 74
#define CX_KEY_PAGE_UP 75
#define CX_KEY_DEL 76
#define CX_KEY_END 77
#define CX_KEY_PAGE_DOWN 78

#define CX_KEY_RIGHT 79
#define CX_KEY_LEFT 80
#define CX_KEY_DOWN 81
#define CX_KEY_UP 82

#define CX_KEY_NUM_LOCK 83
#define CX_KEY_KP_DIVIDE 84
#define CX_KEY_KP_MULTIPLY 85
#define CX_KEY_KP_MINUS 86
#define CX_KEY_KP_PLUS 87
#define CX_KEY_KP_ENTER 88
#define CX_KEY_KP_1 89
#define CX_KEY_KP_2 90
#define CX_KEY_KP_3 91
#define CX_KEY_KP_4 92
#define CX_KEY_KP_5 93
#define CX_KEY_KP_6 94
#define CX_KEY_KP_7 95
#define CX_KEY_KP_8 96
#define CX_KEY_KP_9 97
#define CX_KEY_KP_0 98
#define CX_KEY_KP_PERIOD 99

/* Non-US \ and | */
#define CX_KEY_WORLD_2 100

/* Acutally Application Key but reffered to as menu key. */
#define CX_KEY_MENU 101

#define CX_KEY_POWER 102
#define CX_KEY_KP_EQUAL 103

#define CX_KEY_F13 104
#define CX_KEY_F14 105
#define CX_KEY_F15 106
#define CX_KEY_F16 107
#define CX_KEY_F17 108
#define CX_KEY_F18 109
#define CX_KEY_F19 110
#define CX_KEY_F20 111
#define CX_KEY_F21 112
#define CX_KEY_F22 113
#define CX_KEY_F23 114
#define CX_KEY_F24 115

/* After here, diverge from USB HID numbering to save space and skip non-used things */
#define CX_KEY_LEFT_CTRL 116
#define CX_KEY_LEFT_SHIFT 117
#define CX_KEY_LEFT_ALT 118
#define CX_KEY_LEFT_SUPER 119

#define CX_KEY_RIGHT_CTRL 120
#define CX_KEY_RIGHT_SHIFT 121
#define CX_KEY_RIGHT_ALT 122
#define CX_KEY_RIGHT_SUPER 123

#define CX_KEY_LAST CX_KEY_RIGHT_SUPER


/**
 * Define the modifier key flags for the keyboard modifier states.
 */
#define CX_MOD_CTRL 0x01
#define CX_MOD_SHIFT 0x02
#define CX_MOD_ALT 0x04
#define CX_MOD_SUPER 0x08

/**
 * The possible states for the keys
 */
#define CX_KEY_RELEASE 0
#define CX_KEY_PRESS 1
#define CX_KEY_REPEAT 2

#endif // CX_DEVICE_INPUT_CXKEYMAP_H
