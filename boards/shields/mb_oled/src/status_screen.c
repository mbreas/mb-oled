/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include "status_screen.h"
#include "images.h"
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

lv_obj_t *zmk_display_status_screen()
{
  LOG_WRN("Creating minimal test screen");

  lv_obj_t *screen = lv_obj_create(NULL);

  // Create a simple label with default styling
  // lv_obj_t *test_label = lv_label_create(screen);
  // lv_label_set_text(test_label, "TEST");
  // lv_obj_center(test_label);

  status_image = lv_img_create(screen);
  if (status_image != NULL)
  {
    lv_img_set_src(status_image, &zeon); // Use your image variable name
    // lv_obj_align(status_image, LV_ALIGN_TOP_RIGHT, -5, 5);  // Top-right corner
    lv_obj_center(status_image);
    LOG_WRN("Status image created and positioned");
  }
  else
  {
    LOG_ERR("Failed to create status image");
  }

  LOG_WRN("Minimal test screen created");
  return screen;
}