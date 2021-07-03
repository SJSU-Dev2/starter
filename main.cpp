#include <libcore/platform/newlib.hpp>
#include <libcore/utility/build_info.hpp>
#include <libcore/utility/log.hpp>
#include <libcore/utility/time/time.hpp>
#include <liblpc40xx/peripherals/gpio.hpp>
#include <liblpc40xx/peripherals/uart.hpp>
#include <libstm32f10x/peripherals/gpio.hpp>

void TestExceptions()
{
  throw 5;
}

sjsu::StaticNewlib<4> newlib;

sjsu::Uart & uart0 = sjsu::lpc40xx::GetUart<0>();

int log_storage(int, const char * buffer, int length)
{
  uart0.Write(std::span<const std::byte>(
      reinterpret_cast<const std::byte *>(buffer), length));
  return length;
}

int main()
{
  sjsu::Gpio * led = &sjsu::GetInactive<sjsu::Gpio>();

  // Set UART0 baudrate, which is required for printf and scanf to work properly
  uart0.settings.baud_rate = 115200;
  uart0.Initialize();

  newlib.AddWriter(log_storage);
  sjsu::NewlibManager::Set(&newlib);

  if constexpr (sjsu::build::IsPlatform("lpc40xx"))
  {
    led = &sjsu::lpc40xx::GetGpio<2, 3>();
  }
  else if constexpr (sjsu::build::IsPlatform("stm32f10x"))
  {
    led = &sjsu::stm32f10x::GetGpio<'A', 1>();
  }
  else
  {
    sjsu::log::Print("Unsupported platform %d!\n", 41);
    sjsu::log::Print("Unsupported platform {}!\n", 42);
    return -1;
  }

  sjsu::log::Print("Hello World!! {}\n", 42.5f);

  led->Initialize();
  led->SetAsOutput();

  int counter     = 0;
  auto delay_time = 1000ms;

  while (1)
  {
    led->SetHigh();
    sjsu::Delay(delay_time);

    led->SetLow();
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
      }
      delay_time = 200ms;
    }
  }

  return 0;
}
