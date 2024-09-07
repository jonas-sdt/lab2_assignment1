#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"
#include "inc/hw_memmap.h"
#include <stdbool.h>
#include <stdint.h>
// The error routine that is called if the driver library
// encounters an error.
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
  while (1)
    ;
}
#endif

// Main Function
int main(void) {
  unsigned char ucDelta, ucState;

  // Configure the device pins. LAB 1: INTRODUCTION TO EK-TM4C129EXL 22
  PinoutSet(false, false);

  // Initialize the button driver.
  ButtonsInit();

  // Enable the GPIO pin for the LED (PN0).
  // Set the direction as output, and
  // enable the GPIO pin for digital function.
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

  // Loop forever.
  while (1) {
    // Poll the buttons.
    ucState = ButtonsPoll(&ucDelta, 0);
    if (BUTTON_PRESSED(RIGHT_BUTTON, ucState, ucDelta))
      // Turn on D1.
      LEDWrite(CLP_D1, 1);
    if (BUTTON_PRESSED(LEFT_BUTTON, ucState, ucDelta))
      // Turn off D1.
      LEDWrite(CLP_D1, 0);
  }
}
