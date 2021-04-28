#include <libcore/utility/build_info.hpp>
#include <libcore/utility/time/time.hpp>
#include <liblpc40xx/peripherals/gpio.hpp>
#include <libstm32f10x/peripherals/gpio.hpp>

int main()
{
  sjsu::Gpio * led = &sjsu::GetInactive<sjsu::Gpio>();

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
    printf("Unsupported platform!\n");
    return -1;
  }

  led->Initialize();
  led->SetAsOutput();

  while (1)
  {
    led->SetHigh();
    sjsu::Delay(1s);

    led->SetLow();
    sjsu::Delay(1s);
  }

  return 0;
}
