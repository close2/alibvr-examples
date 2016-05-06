#define F_CPU 8000000UL

#include <util/delay.h>

#include "ports.h"
#include "adc.h"

// Test program.

// A previous Adc version had problems when reading 3 adc values:
// similar to:
//
// do {
//   <in1>::init
//          adc_8bit
//   <in2>::init
//          adc_8bit
//   <in3>::init
//          adc_8bit
//          turn_off
// }
// ⇒ 1st result was a copy of 3rd

typedef PIN_C5 Input1;
typedef PIN_C4 Input2;
typedef PIN_C3 Input3;

// First define the Adc.
// We want a single conversion (default), whenever we start an adc.
// Use the internal 1.1V voltage as reference.
typedef Adc<_adc::Ref::V1_1> AdcV1_1;

// 255 == 1.1V
const uint8_t V0_5 = (255 * 5) / 11;

typedef PIN_D1 Led1;
typedef PIN_D2 Led2;
typedef PIN_D3 Led3;

__attribute__ ((OS_main)) int main(void) {
  // put Led pins into output mode.
  Led1::DDR = 1;
  Led2::DDR = 1;
  Led3::DDR = 1;
  
  for (;;) {
    // Measure the voltage on Pin Input1.
    AdcV1_1::init<Input1>();
    AdcV1_1::adc_8bit(); // first read after initialization shouldn't be trusted
    auto in1 = AdcV1_1::adc_8bit();
    Led1::PORT = (in1 > V0_5);
    
    AdcV1_1::init<Input2>();
    AdcV1_1::adc_8bit(); // first read after initialization shouldn't be trusted
    auto in2 = AdcV1_1::adc_8bit();
    Led2::PORT = (in2 > V0_5);
    
    AdcV1_1::init<Input3>();
    AdcV1_1::adc_8bit(); // first read after initialization shouldn't be trusted
    auto in3 = AdcV1_1::adc_8bit();
    Led3::PORT = (in3 > V0_5);
    
    AdcV1_1::turn_off();
    
    _delay_ms(100);
  }
  return 0;
}
