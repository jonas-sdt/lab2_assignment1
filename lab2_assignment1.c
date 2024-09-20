//*****************************************************************************
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"
// #include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// #include "driverlib/interrupt.h"
// #include "driverlib/pin_map.h"
// #include "inc/tm4c129encpdt.h"

//***********************************************************************
//                       Configurations
//***********************************************************************
// Configure the UART.
void ConfigureUART(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//                      Main
//*****************************************************************************
int main(void) {
  ConfigureUART();

  float pwm_word;
  uint32_t systemClock;
  systemClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                   16000);

  pwm_word = systemClock / 200;
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
  SysCtlPeripheralDisable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralReset(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
  GPIOPinConfigure(GPIO_PF2_M0PWM2);

  UARTprintf("%d\n", 1);

  PWMGenConfigure(PWM0_BASE, PWM_GEN_1,
                  PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC |
                      PWM_GEN_MODE_DBG_RUN);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, pwm_word);

  while (1) {
    // 1: Read the input from the user
    UARTprintf("\nEnter the duty cycle (0-100): ");
    char str_buffer[3] = {' ', ' ', ' '};

    char current_char;

    // Wait until user send duty cycle
    while (!ROM_UARTCharsAvail(UART0_BASE)) {
    }

    // Wait a few ms for all character to be received
    for (volatile int k = 0; k < 100000; k++) {
    }

    for (int i = 0; i < 3 && ROM_UARTCharsAvail(UART0_BASE); i++) {

      current_char = UARTgetc();
      if (current_char == '\r') {
        // UARTprintf("Found carriage return!\n");

        break;
      } else if ((int)current_char >= 48 && (int)current_char <= 57) {
        str_buffer[i] = current_char;
        // UARTprintf("Found number!\n");
      } else {
        // UARTprintf("Invalid char!: %c\n", current_char);
        i--;
        // NOTE: invalid input - ignore
      }
    }

    // clear UART buffer
    while (ROM_UARTCharsAvail(UART0_BASE)) {
      current_char = UARTgetc();
    }

    int digit = 0;
    uint8_t duty_cycle = 0;
    for (int i = 2; i >= 0; i--) {
      if ((int)str_buffer[i] >= 48 && (int)str_buffer[i] <= 57) {
        int multiplier = 1;
        for (int k = 0; k < digit; k++) {
          multiplier *= 10;
        }
        duty_cycle += multiplier * ((int)str_buffer[i] - 48);
        digit++;
      }
    }

    if (digit == 0) {
      continue;
    }

    UARTprintf("duty cylce d=%d\n", duty_cycle);

    if (duty_cycle > 100 || duty_cycle < 0) {
      UARTprintf("duty cycle must be between 0 and 100\n");
      continue;
    }

    // 2: change value of led

    if (duty_cycle == 0) {
      // TODO: turn off led
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    } else if (duty_cycle == 100) {
      // TODO: turn on led
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    } else if (duty_cycle > 0 && duty_cycle < 100) {
      // TODO: change the duty cycle
      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,
                       pwm_word / 1000); // NOTE: set pulse width here
      PWMGenEnable(PWM0_BASE, PWM_GEN_1);
      PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
    } else {
      // NOTE: invalid input - ignore
      // TODO: print error
    }
  }
}
