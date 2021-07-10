#include <cinttypes>
#include <cstdint>
#include <libcore/platform/syscall.hpp>
#include <libcore/utility/build_info.hpp>
#include <libcore/utility/debug.hpp>
#include <libcore/utility/log.hpp>
#include <libcore/utility/time/time.hpp>
#include <libesp8266/devices/esp8266.hpp>
#include <liblpc40xx/peripherals/uart.hpp>
#include <libstm32f10x/peripherals/uart.hpp>
#include <string_view>

std::string_view host = "www.example.com";
std::string_view get_request_example =
    "GET / HTTP/1.1\r\n"
    "Host: www.example.com\r\n"
    "\r\n";

char log_buffer[1024 * 5];
int position = 0;

sjsu::StaticNewlib<4> newlib;

int log_storage(int, const char * buffer, int length)
{
  strncpy(&log_buffer[position], buffer, sizeof(log_buffer) - position);
  position += length;
}

int main()
{
  newlib.AddWriter(log_storage);
  sjsu::SysCallManager::Set(&newlib);

  sjsu::log::Print("ESP8266 Application Starting...");

  // Phase #1:
  // Define all of the peripheral you plan to use as pointers. Pointers must be
  // used in order to do the next step
  sjsu::Uart * uart = &sjsu::GetInactive<sjsu::Uart>();

  // Phase #2:
  // Use "constexpr if" to instantiate the peripherals for each platform
  if constexpr (sjsu::build::IsPlatform("stm32f10x"))
  {
    sjsu::log::Print("Current Platform STM32F10x...");
    // Giving UART a massive 1kB receive buffer to make we don't lose any data.
    uart = &sjsu::stm32f10x::GetUart<2, 1024>();
  }
  else if constexpr (sjsu::build::IsPlatform("lpc40xx"))
  {
    sjsu::log::Print("Current Platform LPC40xx...");
    uart = &sjsu::lpc40xx::GetUart<3>();
  }
  else
  {
    sjsu::log::Critical("Invalid platform for this application!");
    return -1;
  }

  sjsu::Esp8266 esp(*uart);

  auto & socket = esp.GetInternetSocket();
  auto & wifi   = esp.GetWiFi();

  // NOTE: there is no need to initialize wifi or socket as they both simply
  // initialize and enable the ESP module.
  sjsu::log::Print("Initializing Esp8266 module...");
  esp.Initialize();

  while (true)
  {
    sjsu::log::Print("Connecting to WiFi...");
    if (wifi.ConnectToAccessPoint("KAMMCE-PHONE", "roverteam", 10s))
    {
      break;
    }
    sjsu::log::Info("Failed to connect to WiFi... Retrying ...");
    wifi.DisconnectFromAccessPoint();
  }

  sjsu::log::Print("Connected to WiFi!!");
  sjsu::log::Print("Connecting to server (%s)...", host.data());

  socket.Connect(sjsu::InternetSocket::Protocol::kTCP, host, 80, 5s);

  sjsu::log::Print("Writing to server (%s)...", host.data());

  std::span write_payload(
      reinterpret_cast<const uint8_t *>(get_request_example.data()),
      get_request_example.size());

  socket.Write(write_payload, 5s);

  sjsu::log::Print("Reading back response from server (%s)...", host.data());

  std::array<uint8_t, 1024 * 2> response;
  size_t read_back = socket.Read(response, 10s);

  sjsu::log::Print("Printing Server Response:");
  printf("%.*s\n", read_back, response.data());
  puts("===================================================================");

  socket.Close();

  wifi.DisconnectFromAccessPoint();

  return 0;
}
