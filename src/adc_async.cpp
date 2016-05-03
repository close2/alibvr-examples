#define F_CPU 8000000UL

#include <util/delay.h>

#include "ports.h"
#include "adc.h"
#include "irqs.h"

typedef PIN_D6 Led;

template <typename T>
static void display_temp(const T& result16) {
  const uint8_t result = result16;
  
  const auto mvFor25Degree = 314;
  const uint8_t adcFor25Degree = mvFor25Degree * (255 / 1100);
  
  Led::PORT = result > adcFor25Degree;
}



// First define the Adc.
// Whenever an adc is finished the IRQ will call our display_temp function.
// Because we chose FreeRunning the adc will continously measure the temperature
// and generate IRQ events.
typedef Adc<_adc::Ref::V1_1, _adc::Input::Temperature, _adc::Mode::FreeRunning, display_temp> AdcTemp;
#define NEW_ADC AdcTemp
#include REGISTER_ADC


__attribute__ ((OS_main)) int main(void) {
  // put Led pin into output mode.
  Led::DDR = 1;
  AdcTemp::init();
  sei();
  
  AdcTemp::start_adc_8bit();
  
  for (;;) {
  }
  return 0;
}

#define USE_ONLY_DEFINED_IRQS
#include REGISTER_IRQS
