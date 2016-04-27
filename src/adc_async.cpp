#define F_CPU 8000000UL

#include <util/delay.h>

#include "ports.h"
#include "adc.h"
#include "irqs.h"

typedef PIN_D6 Led;

struct DisplayTmp {
  static uint8_t is_enabled() { return true; }
  
  static void adc_complete(const uint8_t result) {
    const auto mvFor25Degree = 314;
    const uint8_t adcFor25Degree = mvFor25Degree * (255 / 1100);
    Led::PORT = result > adcFor25Degree;
  };
};
  



// First define the Adc.
// We want continuous conversion.
// We will then update an output pin in our adc IRQ handler routine,
typedef Adc<_adc::Ref::V1_1, _adc::Input::Temperature, _adc::Mode::FreeRunning, DisplayTmp> AdcTmp;
#define NEW_ADC AdcTmp
#include REGISTER_ADC


__attribute__ ((OS_main)) int main(void) {
  // put Led pin into output mode.
  Led::DDR = 1;
  AdcTmp::init();
  sei();
  
  AdcTmp::start_adc_8bit();
  
  for (;;) {
  }
  return 0;
}

#define USE_ONLY_DEFINED_IRQS
#include REGISTER_IRQS
