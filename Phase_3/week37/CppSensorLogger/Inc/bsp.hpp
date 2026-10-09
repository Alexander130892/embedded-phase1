#pragma once

// Board support: pin muxing and other board-specific wiring.
// Call once from main, after clock setup and before any driver init().
void bsp_init();