#ifndef KEYCODE_H_
#define KEYCODE_H_

#define KEYCODE_TO_ASCII(keycode) (keycode & 0x00ff)
#define KEYCODE_TO_KEYID(keycode) (keycode & 0xff00)
#define IS_SAME_KEYID(keycode1, keycode2) (KEYCODE_TO_KEYID(keycode1) == KEYCODE_TO_KEYID(keycode2))

#define KEY_0 0x0b30
#define KEY_1 0x0231
#define KEY_2 0x0332
#define KEY_3 0x0433
#define KEY_4 0x0534
#define KEY_5 0x0635
#define KEY_6 0x0736
#define KEY_7 0x0837
#define KEY_8 0x0938
#define KEY_9 0x0a39

#define KEY_A 0x1e61
#define KEY_B 0x3062
#define KEY_C 0x2e63
#define KEY_D 0x2064
#define KEY_E 0x1265
#define KEY_F 0x2166
#define KEY_G 0x2267
#define KEY_H 0x2368
#define KEY_I 0x1769
#define KEY_J 0x246a
#define KEY_K 0x256b
#define KEY_L 0x266c
#define KEY_M 0x326d
#define KEY_N 0x316e
#define KEY_O 0x186f
#define KEY_P 0x1970
#define KEY_Q 0x1071
#define KEY_R 0x1372
#define KEY_S 0x1f73
#define KEY_T 0x1474
#define KEY_U 0x1675
#define KEY_V 0x2f76
#define KEY_W 0x1177
#define KEY_X 0x2d78
#define KEY_Y 0x1579
#define KEY_Z 0x2c7a

#define KEY_ESCAPE 0x011b
#define KEY_GRAVE_ACCENT 0x2960
#define KEY_MINUS 0x0c2d
#define KEY_EQUAL 0x0d3d
#define KEY_BACKSPACE 0x0e08
#define KEY_TAB 0x0f09
#define KEY_LEFT_BRACKET 0x1a5b
#define KEY_RIGHT_BRACKET 0x1b5d
#define KEY_BACKSLASH 0x2b5c
#define KEY_SEMICOLON 0x273b
#define KEY_APOSTROPHE 0x2827
#define KEY_ENTER 0x1c0d
#define KEY_COMMA 0x332c
#define KEY_PERIOD 0x342e
#define KEY_SLASH 0x352f
#define KEY_SPACE 0x3920

#define KEY_KP_ASTERISK 0x372a
#define KEY_KP_MINUS 0x4a2d
#define KEY_KP_PLUS 0x4e2b
#define KEY_KP_1 0x4f31
#define KEY_KP_2 0x5032
#define KEY_KP_3 0x5133
#define KEY_KP_4 0x4b34
#define KEY_KP_5 0x4c35
#define KEY_KP_6 0x4d36
#define KEY_KP_7 0x4737
#define KEY_KP_8 0x4838
#define KEY_KP_9 0x4939
#define KEY_KP_0 0x5230
#define KEY_KP_PERIOD 0x532e

/* The following keycodes share a keyID with a previously
   defined key, but the ouput may differ (e.g. because of shift)*/
#define KEY_PAGE_UP 0x4900
#define KEY_PAGE_DOWN 0x5100
#define KEY_ARROW_UP 0x4800
#define KEY_ARROW_LEFT 0x4b00
#define KEY_ARROW_DOWN 0x5000
#define KEY_ARROW_RIGHT 0x4d00
#define KEY_HOME 0x4700
#define KEY_END 0x4f00
#define KEY_INSERT 0x5200
#define KEY_DELETE 0x5300

#define KEY_TILDE 0x297e
#define KEY_UNDERSCORE 0x0c5f
#define KEY_PLUS 0x0d2b
#define KEY_LEFT_CURLY_BRACKET 0x1a7b
#define KEY_RIGHT_CURLY_BRACKET 0x1b7d
#define KEY_PIPE 0x287c
#define KEY_COLON 0x273a
#define KEY_QUOTE 0x2822
#define KEY_LEFT_CARET 0x333c
#define KEY_RIGHT_CARET 0x343e
#define KEY_QUESTION_MARK 0x353f
#define KEY_EXCLAMATION 0x0221
#define KEY_AT 0x0340
#define KEY_POUND 0x0423
#define KEY_DOLLAR 0x0524
#define KEY_MODULUS 0x0625
#define KEY_UP_CARET 0x075e
#define KEY_AMPERSAND 0x0826
#define KEY_ASTERISK 0x092a
#define KEY_LEFT_PARENTH 0x0a28
#define KEY_RIGHT_PARENTH 0x0b29

#endif /* KEYCODE_H_ */
