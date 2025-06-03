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
    SYM,
    MEDIA
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
        KC_ESC,  KC_N, KC_R, KC_T, KC_S, KC_G,_______, _______, KC_Y,    KC_H, KC_A, KC_E, KC_I, KC_SLSH, KC_ENT,
        KC_LSFT, KC_X,    KC_Q,    KC_M,    KC_W,    KC_Z,    _______, _______, KC_K,    KC_P,    KC_QUOT, KC_SCLN, KC_DOT,  KC_RSFT, KC_ENT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_LALT, OSM(MOD_LSFT), KC_BSPC,MO(NAV),MO(SYM),  KC_SPC,KC_UNDS, KC_RALT, KC_RGUI, KC_RCTL, KC_RCTL, _______
    ),

    [NAV] = LAYOUT_ortho_5x15(
        QK_BOOT, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), LALT(KC_5), LALT(KC_6), LALT(KC_7), LALT(KC_8), LALT(KC_9), LALT(KC_0), _______, _______, _______, QK_BOOT,
        _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_Y), _______, _______, LCTL(LSFT(KC_L)), LALT(KC_TAB), LCTL(KC_TAB), LCTL(LSFT(KC_TAB)), LALT(LSFT(KC_TAB)), LCTL(LALT(LSFT(KC_L))), _______,
        LALT(KC_ENT), OSM(MOD_LGUI), OSM(MOD_LALT), OSM(MOD_LSFT), OSM(MOD_LCTL), LALT(KC_B), _______, _______, KC_BSPC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL, _______,
        _______, LALT(KC_R), LALT(KC_D), LCTL(KC_A), LALT(KC_F), LALT(KC_V), _______, _______, CW_TOGG,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_CAPS, _______,
        _______, _______, _______, _______, _______, _______, _______, MO(MEDIA),  _______,  _______,  _______, _______, _______, _______, _______
    ),

    [SYM] = LAYOUT_ortho_5x15(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_LCBR, KC_LBRC,   KC_LPRN, KC_DQUO, KC_GRAVE, _______, _______, _______, KC_QUOTE, KC_RPRN, KC_RBRC, KC_RCBR, _______, _______,
        _______, KC_EXCLAIM, KC_PLUS, KC_MINUS, KC_EQUAL, KC_BSLS, _______, _______, _______, OSM(MOD_LCTL), OSM(MOD_LSFT), OSM(MOD_LALT), OSM(MOD_LGUI), KC_ENT, _______,
        _______, KC_KP_ASTERISK, KC_TILDE,  KC_SLASH, KC_HASH,  KC_PIPE, _______, _______, _______, KC_SPC, KC_AT, KC_PERCENT, KC_AMPERSAND, _______, _______,
        _______, _______, _______, _______, _______, _______, MO(MEDIA),  _______, _______, _______, _______, _______, _______, _______, _______
    )
    // [MEDIA] = LAYOUT__5x15(
    //     _______, _______, _______, _______, _______, RGB_TOG,                   _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______, _______,                   _______, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______,
    //     _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______,                   BL_TOGG, KC_MPRV, KC_MRWD, KC_MFFD, KC_MNXT, _______,
        ///KC_ESC,  KC_N, KC_R, KC_T, KC_S, KC_G,KC_MPRV, KC_MNXT, KC_Y,    KC_H, KC_A, KC_E, KC_I, KC_SLSH, KC_ENT,
    //     _______, _______, _______, _______, _______, _______, _______, _______, OUT_AUTO,KC_MS_BTN1,KC_MS_BTN2,KC_MS_BTN3,KC_MS_BTN4,KC_MS_BTN5,
    //              _______, _______, _______, _______, _______,          KC_MSTP, KC_MPLY, KC_ESC,  _______, _______
    // ),
};

