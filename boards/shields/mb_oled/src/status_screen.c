/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include "status_screen.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

lv_obj_t *zmk_display_status_screen()
{
  LOG_WRN("Creating minimal test screen");

  lv_obj_t *screen = lv_obj_create(NULL);

  // Create a simple label with default styling
  lv_obj_t *test_label = lv_label_create(screen);
  lv_label_set_text(test_label, "TEST");
  lv_obj_center(test_label);

  LOG_WRN("Minimal test screen created");
  return screen;
}