/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
#include "status_screen.h"
#include "images.h"

#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/battery.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct layer_status_state
{
  uint8_t index;
  const char *label;
};

static lv_obj_t *screen = NULL;
static lv_obj_t *status_image = NULL;
static lv_obj_t *layer_label = NULL;
static lv_obj_t *battery_label = NULL;

lv_obj_t *zmk_display_status_screen()
{
  LOG_WRN("Creating minimal test screen");

  screen = lv_obj_create(NULL);

  // Layer setup
  layer_label = lv_label_create(screen);
  uint8_t index = zmk_keymap_highest_layer_active();
  const char *label = zmk_keymap_layer_name(index);
  if (label == NULL)
  {
    char text[7] = {};
    sprintf(text, "%i", index);
    lv_label_set_text(layer_label, text);
  }
  else
  {
    char text[13] = {};
    snprintf(text, sizeof(text), "%s", label);
    lv_label_set_text(layer_label, text);
  }
  lv_obj_align(layer_label, LV_ALIGN_TOP_LEFT, 0, 0);

  battery_label = lv_label_create(screen);
  uint8_t charge = zmk_battery_state_of_charge();
  char chargetext[14] = {};
  snprintf(chargetext, sizeof(chargetext), "Battery: %d%%", charge);
  lv_label_set_text(battery_label, chargetext);
  lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, 0, 0);

  // image setup
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

static void set_layer_label(struct layer_status_state state)
{
  if (state.label == NULL)
  {
    char text[7] = {};

    sprintf(text, "%i", state.index);

    lv_label_set_text(layer_label, text);
  }
  else
  {
    char text[13] = {};

    snprintf(text, sizeof(text), "%s", state.label);

    lv_label_set_text(layer_label, text);
  }
}

static void set_battery_label(struct battery_status_state state)
{
  char chargetext[14] = {};
  snprintf(chargetext, sizeof(chargetext), "Battery: %d%%", state.level);
  lv_label_set_text(battery_label, chargetext);
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh)
{
  uint8_t index = zmk_keymap_highest_layer_active();
  return (struct layer_status_state){
      .index = index,
      .label = zmk_keymap_layer_name(index)};
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh)
{
  const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);

  return (struct battery_status_state){
      .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
      .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
  };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, set_layer_label,
                            layer_status_get_state)

ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            set_battery_label, battery_status_get_state)

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */