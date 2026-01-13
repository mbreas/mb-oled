#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/display.h>
#include <zmk/display/status_screen.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/battery.h>

#include <lvgl.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static lv_obj_t *screen;
static lv_obj_t *layer_label;
static lv_obj_t *battery_label;
static lv_obj_t *output_label;

// Widget update functions
void update_layer_status() {
    uint8_t layer = zmk_keymap_highest_layer_active();
    char layer_text[16];
    snprintf(layer_text, sizeof(layer_text), "Layer: %d", layer);
    lv_label_set_text(layer_label, layer_text);
}

void update_battery_status() {
    uint8_t level = zmk_battery_state_of_charge();
    char battery_text[16];
    snprintf(battery_text, sizeof(battery_text), "Bat: %d%%", level);
    lv_label_set_text(battery_label, battery_text);
}

// Event handlers
int custom_status_screen_listener(const zmk_event_t *eh) {
    if (as_zmk_layer_state_changed(eh)) {
        update_layer_status();
        return ZMK_EV_EVENT_BUBBLE;
    }
    
    if (as_zmk_battery_state_changed(eh)) {
        update_battery_status();
        return ZMK_EV_EVENT_BUBBLE;
    }
    
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(custom_status_screen, custom_status_screen_listener);
ZMK_SUBSCRIPTION(custom_status_screen, zmk_layer_state_changed);
ZMK_SUBSCRIPTION(custom_status_screen, zmk_battery_state_changed);

// Initialize the custom screen
int zmk_display_status_screen_init() {
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    
    // Create layer label
    layer_label = lv_label_create(screen);
    lv_obj_align(layer_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_color(layer_label, lv_color_white(), LV_PART_MAIN);
    
    // Create battery label
    battery_label = lv_label_create(screen);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_text_color(battery_label, lv_color_white(), LV_PART_MAIN);
    
    // Create output status label
    output_label = lv_label_create(screen);
    lv_obj_align(output_label, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_color(output_label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(output_label, "USB");
    
    // Initial updates
    update_layer_status();
    update_battery_status();
    
    lv_scr_load(screen);
    
    return 0;
}