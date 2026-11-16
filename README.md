# Pump controller on a Blue Pill (STM32F103)

This is the controller for my peristaltic pump. I first built it on an ESP32
(see my ESP32-Pump repo) and I'm now porting it to an STM32F103 in plain C
with the HAL, mainly to learn how to structure firmware properly.

You turn the encoder to move through a small OLED menu and press it to select.
The stepper is driven by a timer PWM, so the main loop never has to wait.

## The device

<p align="center">
  <img src="docs/images/frog_running.png" width="45%" alt="Pump running">
  <img src="docs/images/frog_menu.png" width="45%" alt="Pump main menu">
</p>

*The finished ESP32 version: same mechanics and 3D-printed housing (designed in
Fusion 360 / SolidWorks). The STM32 port uses a 128x64 OLED instead of the
colour TFT shown here.*

## Where it stands

Done so far: display, encoder, motor and the main menu with an info page.
Still missing: the Status and Dispenser screens.

My stepper driver died (I hadn't set the current limit), so the motor code
hasn't run on a real motor yet. I checked the ramp logic by printing the
speed on the OLED. A new DRV8825 is on the way.

## Hardware

- STM32F103C8 "Blue Pill", running at 72 MHz
- SSD1306 OLED, 128x64, over I2C
- Rotary encoder with push button
- Stepper motor with a DRV8825 driver, 1/16 microstepping (3200 steps per turn)

## Pins

| What           | Pin        | Notes                 |
|----------------|------------|-----------------------|
| OLED SCL / SDA | PB6 / PB7  | I2C1                  |
| Encoder CLK    | PA1        | interrupt, pull-up    |
| Encoder DT     | PA4        | input, pull-up        |
| Encoder button | PA5        | input, pull-up        |
| STEP           | PA2        | TIM2 channel 3 (PWM)  |
| DIR            | PB12       | output                |
| EN             | PB13       | output, low = enabled |
| UART (debug)   | PA9 / PA10 | USART1                |

## How the code is split

I split everything into small modules. Each one keeps its variables private
and only offers a few functions in its header.

- `display`: sets up u8g2 and draws a menu line, bold when it's selected
- `encoder`: counts the rotation in an interrupt and turns the button into
  one click per press (edge detection plus a 50 ms debounce)
- `motor`: holds the target speed, ramps up and down, switches the driver on and off
- `menu`: decides which screen is shown and handles the main menu and info page
- `main.c`: just calls `menu_update()` and `motor_update()` in the loop

## A few things I paid attention to

- The ramp doesn't block. `motor_update()` runs every loop but only changes the
  speed every 5 ms, by 1 RPM. From standstill it jumps to 10 RPM, because steppers
  shake at very low speeds.
- Pressing STOP doesn't cut the motor. It slows down first, and only then
  the PWM and the driver are turned off.
- Speed and direction can only be changed while the motor is stopped.
- The OLED is only redrawn when something changed.
- Every feature went through its own branch and pull request before merging.

## Building

Open the project in STM32CubeIDE 1.19. The pin and clock setup is in `PUMP.ioc`.
The display uses [u8g2](https://github.com/olikraus/u8g2) (BSD-2-Clause), which
sits in `Drivers/u8g2`.

## Next

- Status screen (set RPM and direction, start/stop)
- Dispenser screen (RPM, on time, off time)
- Test the motor once the new driver arrives
- Save the settings to flash