/* Copyright 2020 IFo Hancroft
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap_us_international.h"
#include "sendstring_us_international.h"
#include "oneshot.h"

#define LA_SYM KC_TRI_LOWER
#define LA_NAV KC_TRI_UPPER
#define LA_MAC_SYM LA_SYM
#define LA_MAC_NAV LA_NAV

// Layer definitions
enum layers {
    BASE,
    NAV,
    SYM,
    NUM,
    MAC_BASE,
    MAC_NAV,
    MAC_SYM,
    MAC_NUM
};

// Custom keycodes for layer keys
enum custom_keycodes {
    KC_NAVSP = SAFE_RANGE,
    KC_MEDEL,
    KC_SYMSP,
    KC_TRI_LOWER,
    KC_TRI_UPPER,

    // Custom oneshot mod implementation with no timers.
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    SW_WIN,  // Switch to next window         (cmd-tab)
    SW_LANG, // Switch to next input language (ctl-spc)
};



// tap dance
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

// Define custom tap dance states
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP
};

const uint16_t PROGMEM combo_enter_left_hand[] = {KC_S, KC_G, COMBO_END};
const uint16_t PROGMEM combo_escape[] = {KC_T, KC_S, COMBO_END};
combo_t key_combos[] = {
    COMBO(combo_enter_left_hand, KC_ENT),
    COMBO(combo_escape, KC_ESC),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ortho_5x15(
        KC_GRAVE, KC_1,    KC_2,    KC_3,    KC_4,          KC_5,    KC_6,    KC_7,    KC_8,   KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_BSPC,
        KC_TAB,   KC_B,    KC_L,    KC_D,    KC_C,          KC_V,    KC_VOLD, KC_VOLU, KC_J,   KC_F,    KC_O,    KC_U,    KC_COMM, KC_LBRC, KC_RBRC,
        KC_ESC,   KC_N,    KC_R,    KC_T,    KC_S,          KC_G,    _______, _______, KC_Y,   KC_H,    KC_A,    KC_E,    KC_I,    KC_SLSH, KC_ENT,
        KC_LSFT,  KC_X,    KC_Q,    KC_M,    KC_W,          KC_Z,    _______, _______, KC_K,   KC_P,    KC_QUOT, KC_SCLN, KC_DOT,  KC_UP,   KC_ENT,
        KC_LCTL,  KC_LGUI, KC_LALT, KC_LALT, OSM(MOD_LSFT), KC_BSPC, LA_NAV,  LA_SYM,  KC_SPC, KC_UNDS, KC_RALT, KC_RGUI, KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [NAV] = LAYOUT_ortho_5x15(
        _______,          LALT(KC_1), LALT(KC_2),   LALT(KC_3),   LALT(KC_4),   LALT(KC_5), LALT(KC_6), LALT(KC_7), LALT(KC_8),       LALT(KC_9),   LALT(KC_0),   _______,            _______,            _______,                _______,
        _______,          LCTL(KC_Z), LCTL(KC_X),   LCTL(KC_INS), LSFT(KC_INS), LCTL(KC_Y), _______,    _______,    LCTL(LSFT(KC_L)), LALT(KC_TAB), LCTL(KC_TAB), LCTL(LSFT(KC_TAB)), LALT(LSFT(KC_TAB)), LCTL(LALT(LSFT(KC_L))), _______,
        LALT(LSFT(KC_Q)), OS_CMD,     OS_ALT,       OS_SHFT,      OS_CTRL,      KC_ESC,     _______,    _______,    KC_ENT,           KC_LEFT,      KC_DOWN,      KC_UP,              KC_RGHT,            KC_DEL,                 LALT(KC_ENT),
        _______,          LALT(KC_R), LALT(KC_SPC), LCTL(KC_A),   LALT(KC_F),   KC_QUES,    _______,    _______,    _______,          KC_HOME,      KC_PGDN,      KC_PGUP,            KC_END,             KC_CAPS,                _______,
        QK_BOOT,          _______,    _______,      _______,      _______,      _______,    _______,    _______,    _______,          QK_REP,       QK_AREP,      _______,            _______,            _______,                _______
    ),

    [SYM] = LAYOUT_ortho_5x15(
        _______, _______,        _______,  _______,  _______,  _______, _______, _______, _______,  _______,  _______, _______,    _______,      _______, _______,
        _______, KC_LCBR,        KC_LBRC,  KC_LPRN,  KC_DQUO,  KC_PIPE, _______, _______, KC_GRAVE, KC_QUOTE, KC_RPRN, KC_RBRC,    KC_RCBR,      _______, _______,
        _______, KC_EXCLAIM,     KC_PLUS,  KC_MINUS, KC_EQUAL, KC_ESC,  _______, _______, KC_ENT,   OS_CTRL,  OS_SHFT, OS_ALT,     OS_CMD,       KC_ENT,  _______,
        _______, KC_KP_ASTERISK, KC_TILDE, KC_SLASH, KC_HASH,  KC_BSLS, _______, _______, KC_DLR,   KC_SPC,   KC_AT,   KC_PERCENT, KC_AMPERSAND, _______, _______,
        _______, _______,        _______,  QK_AREP,  QK_REP,   _______, _______, _______, _______,  _______,  _______, _______,    _______,      _______, QK_BOOT
    ),

    [NUM] = LAYOUT_ortho_5x15(
        _______,      _______, _______, _______,  _______,      _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,      _______, _______, KC_TAB,   LSFT(KC_TAB), _______,    _______, _______, _______, KC_7,    KC_8,    KC_9,    KC_COMM, _______, _______,
        _______,      _______, KC_PLUS, KC_MINUS, KC_EQUAL,     LALT(KC_B), _______, _______, _______, KC_4,    KC_5,    KC_6,    KC_SPC,  _______, _______,
        _______,      _______, _______, KC_UNDS,  KC_0,         LALT(KC_V), _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______, _______,
        DF(MAC_BASE), _______, _______, _______,  _______,      _______,    _______, _______, _______, _______, _______, _______, _______, _______, DF(MAC_BASE)
    ),

    [MAC_BASE] = LAYOUT_ortho_5x15(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,          KC_5,    KC_6,    KC_7,    KC_8,   KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_BSPC,
        KC_TAB,  KC_B,    KC_L,    KC_D,    KC_C,          KC_V,    KC_VOLD, KC_VOLU, KC_J,   KC_F,    KC_O,    KC_U,    KC_COMM, KC_LBRC, KC_RBRC,
        KC_ESC,  KC_N,    KC_R,    KC_T,    KC_S,          KC_G,    _______, _______, KC_Y,   KC_H,    KC_A,    KC_E,    KC_I,    KC_SLSH, KC_ENT,
        KC_LSFT, KC_X,    KC_Q,    KC_M,    KC_W,          KC_Z,    _______, _______, KC_K,   KC_P,    KC_QUOT, KC_SCLN, KC_DOT,  KC_UP,   KC_ENT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_LALT, OSM(MOD_LSFT), KC_BSPC, LA_NAV,  LA_SYM,  KC_SPC, KC_UNDS, KC_RALT, KC_RGUI, KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [MAC_NAV] = LAYOUT_ortho_5x15(
        _______,          LALT(KC_1), LALT(KC_2),   LALT(KC_3), LALT(KC_4), LALT(KC_5), LALT(KC_6), LALT(KC_7), LALT(KC_8),       LALT(KC_9),    LALT(KC_0),   _______,            _______,            _______,                _______,
        _______,          LCMD(KC_Z), LCMD(KC_X),   LCMD(KC_C), LCMD(KC_V), LCMD(KC_Y), _______,    _______,    LCMD(LSFT(KC_L)), LALT(KC_TAB),  LCTL(KC_TAB), LCTL(LSFT(KC_TAB)), LALT(LSFT(KC_TAB)), LCTL(LALT(LSFT(KC_L))), _______,
        LALT(LSFT(KC_Q)), OS_CMD,     OS_ALT,       OS_SHFT,    OS_CTRL,    KC_ESC,     _______,    _______,    KC_ENT,           KC_LEFT,       KC_DOWN,      KC_UP,              KC_RGHT,            KC_DEL,                 LALT(KC_ENT),
        _______,          _______,    LALT(KC_SPC), LCMD(KC_A), LALT(KC_F), KC_QUES,    _______,    _______,    _______,          LCMD(KC_LEFT), KC_PGDN,      KC_PGUP,            LCMD(KC_RGHT),      KC_CAPS,                _______,
        QK_BOOT,          _______,    _______,      _______,    _______,    _______,    _______,    _______,    _______,          QK_REP,        QK_AREP,      _______,            _______,            _______,                _______
    ),

    [MAC_SYM] = LAYOUT_ortho_5x15(
        _______, _______,        _______,  _______,  _______,  _______, _______, _______, _______,  _______,  _______, _______,    _______,      _______, _______,
        _______, KC_LCBR,        KC_LBRC,  KC_LPRN,  KC_DQUO,  KC_PIPE, _______, _______, KC_GRAVE, KC_QUOTE, KC_RPRN, KC_RBRC,    KC_RCBR,      _______, _______,
        _______, KC_EXCLAIM,     KC_PLUS,  KC_MINUS, KC_EQUAL, KC_ESC,  _______, _______, KC_ENT,   OS_CTRL,  OS_SHFT, OS_ALT,     OS_CMD,       KC_ENT,  _______,
        _______, KC_KP_ASTERISK, KC_TILDE, KC_SLASH, KC_HASH,  KC_BSLS, _______, _______, KC_DLR,   KC_SPC,   KC_AT,   KC_PERCENT, KC_AMPERSAND, _______, _______,
        _______, _______,        _______,  QK_AREP,  QK_REP,   _______, _______, _______, _______,  _______,  _______, _______,    _______,      _______, QK_BOOT
    ),

    [MAC_NUM] = LAYOUT_ortho_5x15(
        _______,  _______, _______, _______,  _______,      _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, KC_TAB,   LSFT(KC_TAB), _______,    _______, _______, _______, KC_7,    KC_8,    KC_9,    KC_COMM, _______, _______,
        _______,  _______, KC_PLUS, KC_MINUS, KC_EQUAL,     LALT(KC_B), _______, _______, _______, KC_4,    KC_5,    KC_6,    KC_SPC,  _______, _______,
        _______,  _______, _______, KC_UNDS,  KC_0,         LALT(KC_V), _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______, _______,
        DF(BASE), _______, _______, _______,  _______,      _______,    _______, _______, _______, _______, _______, _______, _______, _______, DF(BASE)
    ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool is_mac_mode = false;  // Start in normal mode by default.
bool tri_lower_pressed = false;  // Whether the KC_TRI_LOWER key is pressed
bool tri_upper_pressed = false;  // Whether the KC_TRI_UPPER key is pressed.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    switch (keycode) {
        // Keep track of whether we're using normal mode or MAC mode
        case DF(BASE):
            if (record->event.pressed) {
                is_mac_mode = false;
            }
            return false;
        case DF(MAC_BASE):
            if (record->event.pressed) {
                is_mac_mode = true;
            }
            return false;

        // Custom TRI_LAYER functionality that will work across the "normal" level and the "MAC" level
        case KC_TRI_LOWER:
            tri_lower_pressed = record->event.pressed;
            break;
        case KC_TRI_UPPER:
            tri_upper_pressed = record->event.pressed;
            break;
    }

    // Determine which layers to activate (custom TRI_LAYER)
    if (tri_lower_pressed && tri_upper_pressed) {
        if (!is_mac_mode) {
            layer_on(NUM);
        } else {
            layer_on(MAC_NUM);
        }
    } else {
        layer_off(NUM);
        layer_off(MAC_NUM);
        if (tri_lower_pressed) {
            layer_on(is_mac_mode ? MAC_SYM : SYM);
        } else {
            layer_off(SYM);
            layer_off(MAC_SYM);
        }
        if (tri_upper_pressed) {
            layer_on(is_mac_mode ? MAC_NAV : NAV);
        } else {
            layer_off(NAV);
            layer_off(MAC_NAV);
        }
    }

    // Handle mode switching in the NUM layer
    if (IS_LAYER_ON(NUM) || IS_LAYER_ON(MAC_NUM)) {
        if (keycode == DF(MAC_BASE) && record->event.pressed) {
            is_mac_mode = true;
            layer_off(NUM);
            layer_on(MAC_BASE);
            return false;
        } else if (keycode == DF(BASE) && record->event.pressed) {
            is_mac_mode = false;
            layer_off(MAC_NUM);
            layer_on(BASE);
            return false;
        }
    }

    return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool shifted = (mods & MOD_MASK_SHIFT);  // Was Shift held?
    bool ctrled = (mods & MOD_MASK_CTRL);  // Was Ctrl held?
    switch (keycode) {
        // Ctrl-Z / Ctrl-Y
        case KC_Y:
            if (ctrled) {
                return C(KC_Z); // Ctrl + Y reverses to Ctrl + Z.
            } else { return KC_TRNS; }
        case KC_Z:
            if (ctrled) {
                return C(KC_Y); // Ctrl + Z reverses to Ctrl + Y.
            } else { return KC_TRNS; }
        // Tab / Shift tab
        case KC_TAB:
            // Browser tab navigation
            if (shifted & ctrled) {
                return C(KC_TAB);     // C-S-Tab reverses to C-Tab
            } else
            if (ctrled) {
                return C(S(KC_TAB));  // C-Tab reverses to C-S-Tab
            }
            // App navigation (Windows)
            if (shifted) {
                return KC_TAB;        // S-Tab reverses to Tab
            } else {
                return S(KC_TAB);     // Tab reverses to S-TAB
            }
        // Ctrl +/-
        case KC_MINUS:
            if (ctrled) {
                return C(KC_EQUAL);  // Ctrl-- reverses to Ctrl-+.
            } else { return KC_TRNS; }
        case KC_EQUAL:
            if (ctrled) {
                return C(KC_MINUS); // Ctrl-+ reverses to Ctrl--.
            } else { return KC_TRNS; }
        // u / Ctrl-R (vim), undo/redo
        case KC_U: return C(KC_R);      // U reverses to Ctrl-R
        case KC_R:
            if (ctrled) {
                return KC_U;  // Ctrl-R reverses to U
            } else { return KC_TRNS; }
    }

    return KC_TRNS;  // Defer to default definitions.
}
