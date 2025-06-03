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

// Layer definitions
enum layers {
    BASE,
    NAV,
    MEDIA,
    SYM,
    DIACRITICS
};

// Custom keycodes for layer keys
enum custom_keycodes {
    KC_NAVSP = SAFE_RANGE,
    KC_MEDEL,
    KC_SYMSP
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

// Function declarations for tap dancing
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ortho_5x15(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_BSPC,
        KC_TAB,  KC_B,    KC_L,    KC_D,    KC_C,    KC_V,  KC_VOLD, KC_VOLU, KC_J,    KC_F,    KC_O,    KC_U,    KC_COMM, KC_LBRC, KC_RBRC,
        // KC_ESC,  LGUI_T(KC_N), LALT_T(KC_R), LSFT_T(KC_T), LCTL_T(KC_S), KC_G,KC_MPRV, KC_MNXT, KC_Y,    RCTL_T(KC_H), RSFT_T(KC_A), RALT_T(KC_E), RGUI_T(KC_I), KC_SLSH, KC_ENT,
        KC_ESC,  KC_N, KC_R, KC_T, KC_S, KC_G,KC_MPRV, KC_MNXT, KC_Y,    KC_H, KC_A, KC_E, KC_I, KC_SLSH, KC_ENT,
        KC_LSFT, KC_X,    KC_Q,    KC_M,    KC_W,    KC_Z,    KC_MSTP, KC_MPLY, KC_K,    KC_P,    KC_QUOT, KC_SCLN, KC_DOT,  KC_RSFT, KC_ENT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_LALT, LT(MEDIA, KC_DEL),LT(NAV, KC_BSPC),LT(DIACRITICS, KC_ENT),  KC_ESC,  LT(SYM, KC_SPC),KC_TAB, KC_RALT, KC_RGUI, KC_RCTL, KC_RCTL, _______
    ),

    [NAV] = LAYOUT_ortho_5x15(
        _______, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), LALT(KC_5), LALT(KC_6), LALT(KC_7), LALT(KC_8), LALT(KC_9), LALT(KC_0), _______, _______, _______, _______,
        _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_Y), _______, _______, _______, LALT(KC_TAB), LCTL(KC_TAB), LCTL(LSFT(KC_TAB)), LALT(LSFT(KC_TAB)), _______, _______,
        _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______, _______, _______, KC_BSPC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL, _______,
        _______, _______, LALT(KC_R), LCTL(KC_A), LALT(KC_F), _______, _______, _______, CW_TOGG,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_CAPS, _______,
        _______, _______, _______, _______, _______, _______, _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______, _______, _______, QK_BOOT
    ),

    // [MEDIA] = LAYOUT_ortho_5x15(
    //     _______, _______, _______, _______, _______, RGB_TOG,                   _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______, _______,                   _______, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______,
    //     _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______,                   BL_TOGG, KC_MPRV, KC_MRWD, KC_MFFD, KC_MNXT, _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, OUT_AUTO,KC_MS_BTN1,KC_MS_BTN2,KC_MS_BTN3,KC_MS_BTN4,KC_MS_BTN5,
    //              _______, _______, _______, _______, _______,          KC_MSTP, KC_MPLY, KC_ESC,  _______, _______
    // ),

    [SYM] = LAYOUT_ortho_5x15(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_EXLM, KC_AT,   KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_ASTR, KC_HASH, KC_LPRN, KC_RPRN, KC_BSLS, _______, _______, _______, KC_LCTL, KC_LSFT, KC_LALT, KC_LGUI, _______, _______,
        _______, _______, KC_GRV,  KC_COMM, KC_DOT,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, KC_SCLN, KC_MINS, KC_EQL,  _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [DIACRITICS] = LAYOUT_ortho_5x15(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, US_DGRV,    _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, US_EACU, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______
    )
};

