// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(evin_effect)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// Combo of RGB_MATRIX_CYCLE_ALL and RGB_MATRIX_SOLID_REACTIVE when a key is pressed
static bool evin_effect(effect_params_t* params) {
 RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 4, 1));
    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint16_t tick = max_tick;
        // Reverse search to find most recent key hit
        for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
            if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                tick = g_last_hit_tracker.tick[j];
                break;
            }
        }

	RGB rgb;
	HSV hsv = CYCLE_ALL_math(rgb_matrix_config.hsv, i, time);

	// If this wasn't the last key pressed, return the CYCLE_ALL scheme
	if (tick == max_tick) {
	    rgb = rgb_matrix_hsv_to_rgb(hsv);
	}
	// Otherwise, return the solid reactive scheme based on the current CYCLE_ALL color
	else {
            uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));
            rgb = rgb_matrix_hsv_to_rgb(SOLID_REACTIVE_math(hsv, offset));
	}
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
