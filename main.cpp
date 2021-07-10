#include <libcore/platform/syscall.hpp>
#include <libcore/utility/build_info.hpp>
#include <libcore/utility/log.hpp>
#include <libcore/utility/time/time.hpp>

#include <liblpc40xx/peripherals/gpio.hpp>
#include <liblpc40xx/peripherals/uart.hpp>
#include <liblpc40xx/platform/startup.hpp>

#include <libstm32f10x/peripherals/gpio.hpp>
#include <libstm32f10x/peripherals/uart.hpp>
#include <libstm32f10x/platform/startup.hpp>

void TestExceptions()
{
  sjsu::log::Print("Go!\n");
  throw 5;
}

int main()
{
  sjsu::Gpio * led_ptr         = &sjsu::GetInactive<sjsu::Gpio>();
  sjsu::Uart * serial_port_ptr = &sjsu::GetInactive<sjsu::Uart>();

  if constexpr (sjsu::build::IsPlatform("lpc40xx"))
  {
    sjsu::lpc40xx::InitializePlatform();
    led_ptr         = &sjsu::lpc40xx::GetGpio<2, 3>();
    serial_port_ptr = &sjsu::lpc40xx::GetUart<0>();
  }
  else if constexpr (sjsu::build::IsPlatform("stm32f10x"))
  {
    sjsu::stm32f10x::InitializePlatform();
    led_ptr         = &sjsu::stm32f10x::GetGpio<'A', 1>();
    serial_port_ptr = &sjsu::stm32f10x::GetUart<1, 32>();
  }
  else
  {
    return -1;
  }

  auto & led = *led_ptr;
  auto & serial_port = *serial_port_ptr;

  led.Initialize();
  led.SetAsOutput();

  // Set UART0 baudrate, which is required for printf and scanf to work properly
  serial_port.settings.baud_rate = 115200;
  serial_port.Initialize();

  sjsu::SysCallManager::Get().AddSerial(serial_port);

  int counter     = 0;
  auto delay_time = 1000ms;

  sjsu::log::Print("Starting Application...\n");

  while (1)
  {
    led.SetHigh();
    sjsu::Delay(delay_time);

    led.SetLow();
    sjsu::Delay(delay_time);

    counter++;

    sjsu::log::Print("Counter = {}!\n", counter);

    if (counter == 3)
    {
      sjsu::log::Print("Starting exceptions!\n");
      try
      {
        TestExceptions();
      }
      catch (...)
      {
        sjsu::log::Print("Exception!\n");
        delay_time = 200ms;
      }
    }
  }

  return 0;
}
