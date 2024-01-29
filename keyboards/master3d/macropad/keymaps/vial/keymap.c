// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum keycodes {
    LAYER_UP = SAFE_RANGE,
    LAYER_DOWN,
};

#define KC_LUP LAYER_UP
#define KC_LDN LAYER_DOWN

#define LAYER_CYCLE_START 0
#define LAYER_CYCLE_END   3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ A │ B │ C │ D │
     * ├───┼───┼───┼───┤
     * │ E │ F │ G │ H │
     * ├───┼───┼───┼───┤
     * │SPC│LUP│LDN│XXX│
     * └───┴───┴───┴───┘
     */
    [0] = LAYOUT(
        KC_F1,    KC_F2,    KC_F3,    KC_F4,
        KC_F5,    KC_F6,    KC_F7,    KC_F8,
        KC_ESC,   KC_LUP,  KC_LDN
    ),

    [1] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,
        KC_5,    KC_6,    KC_7,    KC_8,
        KC_LSFT, KC_LUP,  KC_LDN
    ),

    [2] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,
        KC_E,    KC_F,    KC_G,    KC_H,
        KC_LSFT, KC_LUP,  KC_LDN
    ),
    
    [3] = LAYOUT(
        RGB_M_P,    RGB_M_B,    RGB_M_R,    RGB_M_SW,
        RGB_M_SN,   RGB_M_K,    RGB_M_X,    RGB_M_T,
        QK_BOOT,    KC_LUP,  KC_LDN
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LUP:
    case KC_LDN:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) { 
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return false;
      }

      if (keycode == KC_LDN) {
        uint8_t prev_layer = current_layer - 1;
        if (prev_layer < LAYER_CYCLE_START) {
            prev_layer = LAYER_CYCLE_END;
        }
        layer_move(prev_layer);
      } else if (keycode == KC_LUP) {
        uint8_t next_layer = current_layer + 1;
        if (next_layer > LAYER_CYCLE_END) {
            next_layer = LAYER_CYCLE_START;
        }
        layer_move(next_layer);
      }
      return false;

    // Process other keycodes normally
    default:
      return true;
  }
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
    [2] =   { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_SPD, RGB_SPI)  },
    [3] =   { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),          ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
    //                  Encoder 1                                     Encoder 2
};
#endif