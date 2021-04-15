#include <libstm32f1xx/peripherals/gpio.hpp>
#include <libcore/utility/time/time.hpp>

int main()
{
  auto &led = sjsu::stm32f10x::GetGpio<'B', 0>();
  led.Initialize();
  led.SetAsOutput();

  while (1)
  {
    led.SetHigh();
    sjsu::Delay(1s);

    led.SetLow();
    sjsu::Delay(1s);
  }

  return 0;
}
