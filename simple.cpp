#include <cstdio>
#include <libcore/platform/syscall.hpp>
#include <libcore/utility/build_info.hpp>
#include <libcore/utility/log.hpp>
#include <libcore/utility/time/time.hpp>
#include <liblpc40xx/peripherals/gpio.hpp>
#include <liblpc40xx/peripherals/uart.hpp>
#include <liblpc40xx/platform/startup.hpp>

sjsu::Uart & serial_port = sjsu::lpc40xx::GetUart<0>();

int main()
{
  // Step 1. Initialize clocks, peripheral power, and system timers.
  sjsu::lpc40xx::InitializePlatform();

  // Step 2. Create the peripherals needed for the project.
  sjsu::Gpio & led = sjsu::lpc40xx::GetGpio<2, 3>();

  // Step 3. Configure and initialize peripherals
  serial_port.settings.baud_rate = 115200;
  serial_port.Initialize();
  led.Initialize();

  // Step 4. Use peripherals
  led.SetAsOutput();

  // Step 5. (OPTIONAL) Add a serial port to the newlib manager in order to
  sjsu::SysCallManager::Get().AddSerial(serial_port);

  // Step 6. Start Application
  sjsu::log::Print("Starting Application...\n");

  int counter     = 0;
  auto delay_time = 500ms;

  while (1)
  {
    led.SetHigh();
    sjsu::Delay(delay_time);

    led.SetLow();
    sjsu::Delay(delay_time);

    counter++;

    sjsu::log::Print("Counter = {}!\n", counter);
  }

  return 0;
}
