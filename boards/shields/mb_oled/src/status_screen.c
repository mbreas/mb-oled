/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include "status_screen.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

lv_style_t global_style;

static lv_obj_t *layer_label = NULL;

static void update_layer_display(void)
{
  LOG_WRN("Updating layer display");
  if (layer_label == NULL)
  {
    LOG_WRN("Layer label is NULL, cannot update display");
    return;
  }

  lv_label_set_text(layer_label, "Hello");
  LOG_WRN("Layer label is not NULL, updated display");
}

lv_obj_t *zmk_display_status_screen()
{
  lv_obj_t *screen;

  screen = lv_obj_create(NULL);

  lv_style_init(&global_style);
  lv_style_set_bg_color(&global_style, lv_color_white());
  lv_style_set_bg_opa(&global_style, LV_OPA_COVER);
  lv_style_set_text_color(&global_style, lv_color_black());
  lv_style_set_text_font(&global_style, &lv_font_unscii_8);
  lv_style_set_text_letter_space(&global_style, 1);
  lv_style_set_text_line_space(&g lobal_style, 1);
  lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

  layer_label = lv_label_create(screen);
  if (layer_label == NULL)
  {
    LOG_ERR("Failed to create layer label");
    return NULL;
  }

  lv_obj_set_style_bg_color(layer_label, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_text_color(layer_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(layer_label, LV_ALIGN_TOP_LEFT, 0, 0);

  update_layer_display();
  LOG_WRN("Status screen created");
  return screen;
}