
#include <inttypes.h>
#include <Arduino.h>

#include <Servo.h>

#include "app_config.h"

uint16_t photores_lower = UINT16_MAX;
uint16_t photores_upper = 0;

Servo servo;

void app_periph_init();

void setup() {
  uint32_t start_time; 
  uint32_t last_toggle;
  uint32_t current_time;
  uint16_t photores_value;
  
  Serial.begin(BAUD_RATE);
  while (!Serial);

  Serial.println("Init periph");
  app_periph_init();

  start_time = millis();
  last_toggle = start_time;
  current_time;
  photores_value;
  while ( ( (current_time = millis()) - start_time) < 10000) {
    photores_value = analogRead(PHOTRES_GPIO_NUM);


    if (photores_value > photores_upper) {
      photores_upper = photores_value;
    }
    if (photores_value < photores_lower) {
      photores_lower = photores_value;
    }
    
    if ( (current_time - last_toggle) > 1000) {
      uint8_t state = digitalRead(LED_GPIO_NUM);
      digitalWrite(LED_GPIO_NUM, !state);
      last_toggle = current_time;
    }
    delay(10);
  }
  digitalWrite(LED_GPIO_NUM, LOW);

  Serial.printf("Photoresistor Range = { lower = %" PRIu16 ", upper = %" PRIu16 "}", photores_lower, photores_upper);
  Serial.println();
  Serial.println("Enter Loop");
}

void loop() {
  uint16_t photores_value;
  uint8_t servo_angle;

  photores_value = analogRead(PHOTRES_GPIO_NUM);
  photores_value = constrain(photores_value, photores_lower, photores_upper);

  // Transform photoresistor value from ADC range to servo range
  servo_angle = ( (photores_value - photores_lower) * (180) ) / (double)( photores_upper - photores_lower ) + photores_lower;

  Serial.printf("photoresisor value = %" PRIu16 ", servo angle = %" PRIu8, photores_value, servo_angle);
  Serial.println();

  servo.write(servo_angle);

}

void app_periph_init() {
  // LED Peripheral
  pinMode(LED_GPIO_NUM, OUTPUT);

  // Photoresistor Peripheral
  analogReadResolution(PHOTORES_READ_RESOLUTION);
  pinMode(PHOTRES_GPIO_NUM, ANALOG);

  // Servo Peripheral
  servo.attach(SERVO_GPIO_NUM);
}