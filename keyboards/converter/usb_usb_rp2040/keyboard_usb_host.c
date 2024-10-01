// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ch.h"
#include "hal.h"
#include "pio_usb.h"
#include "tusb.h"

#include "debug.h"
#include "config.h"

static virtual_timer_t vt;
static volatile bool core1_started;
static volatile bool shut_down;
static volatile bool core1_entered_ram;

#if !PICO_NO_FLASH
#error "This example must be built to run from SRAM!"
#endif

static void __not_in_flash_func(usb_host_sof_timer_cb)(virtual_timer_t *_vt, void *_) {
    pio_usb_host_frame();
}

void pico_before_flash_operation(void) {
    chVTReset(&vt);

    shut_down = true;
    while (core1_started && !core1_entered_ram) {};

    dprintf("[core1] TUSB shut down\n");
}

void pico_after_flash_operation(void) {
    chVTObjectInit(&vt);
    chVTSetContinuous(&vt, TIME_MS2I(1), usb_host_sof_timer_cb, NULL);

    shut_down = false;

    dprintf("[core1] TUSB (should be) restarted\n");
}

// Continuation of keyboard_pre_init_kb(). Has to be defined in a separate file
// as pico/stdlib.h cannot be included in the same file as ChibiOS.
void __not_in_flash_func(c1_main)(void) {
    chSysWaitSystemState(ch_sys_running);
    chInstanceObjectInit(&ch1, &ch_core1_cfg);
    chSysUnlock();

    // Wait(?)
    chThdSleepMilliseconds(3000);

    // Pico-PIO-USB host stack uses both PIOs and DMA
    hal_lld_peripheral_unreset(RESETS_ALLREG_PIO0);
    hal_lld_peripheral_unreset(RESETS_ALLREG_PIO1);
    hal_lld_peripheral_unreset(RESETS_ALLREG_DMA);

    // Start the USB host task
    pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
    config.pin_dp = USB_DEVICE_DATAPLUS_PIN;
    config.skip_alarm_pool = true;

    tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &config);
    tuh_init(1);

    chVTObjectInit(&vt);
    chVTSetContinuous(&vt, TIME_MS2I(1), usb_host_sof_timer_cb, NULL);

    core1_started = true;
    dprintf("[core1] TUSB started\n");

    // uint32_t cnt = 0;
    while (true) {
        if (shut_down) {
            core1_entered_ram = true;
            // cnt++;

            // if (cnt % 1000000 == 0) {
            //     dprintf("[core1] TUSB shut down, not doing anything\n");
            //     cnt = 0;
            // }

            continue;
        }

        core1_entered_ram = false;

        tuh_task();
        chThdSleepMicroseconds(125);
    }
}
