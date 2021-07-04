#include <libcore/platform/newlib.hpp>
#include <libcore/utility/build_info.hpp>
#include <libcore/utility/log.hpp>
#include <libcore/utility/time/time.hpp>
#include <liblpc40xx/peripherals/gpio.hpp>
#include <liblpc40xx/peripherals/uart.hpp>
#include <libcore/platform/startup.hpp>
#include <libstm32f10x/peripherals/gpio.hpp>

sjsu::Uart & uart0 = sjsu::lpc40xx::GetUart<0>();

void TestExceptions()
{
  sjsu::log::Print("Go!\n");

  uart0.Write("GO!\n");
  throw 5;
}

sjsu::StaticNewlib<4> newlib;

int log_storage(int, const char * buffer, int length)
{
  uart0.Write(std::span<const std::byte>(
      reinterpret_cast<const std::byte *>(buffer), length));
  return length;
}

struct _reent r = { 0, nullptr, reinterpret_cast<FILE *>(1), nullptr };
struct _reent * _impure_ptr = &r;

extern "C" void __cxa_pure_virtual()
{
  // put your error handling here
}

extern "C" void __cxa_atexit() {
  // uart0.Write("__cxa_atexit!\n");
}

int main()
{
  sjsu::InitializePlatform();
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

  uart0.Write("Hello World!\n");

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
    uart0.Write("COUNTER!\n");

    if (counter == 3)
    {
      sjsu::log::Print("Starting exceptions!\n");
      try
      {
        TestExceptions();
      }
      catch (...)
      {
        uart0.Write("Exception!\n");
        sjsu::log::Print("Exception!\n");
      }
      delay_time = 200ms;
    }
  }

  return 0;
}
