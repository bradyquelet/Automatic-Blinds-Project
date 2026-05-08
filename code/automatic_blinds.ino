#include <Adafruit_NeoPixel.h>  
int Sensor_PIN = A0;
int INPUT1 = 8;
int INPUT2 = 9;
int ENABLE = 10;
int BlindState = 0;
int SensorValue = 0;

int MOTION_PIN = 7;
int LED_PIN = 6;

int threshold = 200;  // Blinds roll back up below threshold

#define NUM_LEDS 60

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); 

void setup() {   
  pinMode(Sensor_PIN, INPUT);
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(MOTION_PIN, INPUT);

  Serial.begin(9600);

  strip.begin();
  strip.show(); // all OFF

  digitalWrite(ENABLE, HIGH);

  delay(3000); 
}

void loop() {

  // read Sensor
  SensorValue = analogRead(Sensor_PIN);
  Serial.println(SensorValue);                          // Gives ADC converted brightness of photoresistor

  if (SensorValue > 1000 && BlindState == 0) {
    // Close blinds
    digitalWrite(ENABLE, HIGH);
    digitalWrite(INPUT1, LOW);
    digitalWrite(INPUT2, HIGH);

    delay(5000);

    digitalWrite(ENABLE, LOW);

    BlindState = 1;
    Serial.println(BlindState);
  }

  else if (SensorValue < threshold && BlindState == 1) { 
    // Open blinds
    digitalWrite(ENABLE, HIGH);
    digitalWrite(INPUT1, HIGH);
    digitalWrite(INPUT2, LOW);

    delay(5000);

    digitalWrite(ENABLE, LOW);

    BlindState = 0;
  }

  else {
    digitalWrite(ENABLE, LOW);
  }

  //  Motion detection with LED strip
  if (BlindState == 1) {
    int motion = digitalRead(MOTION_PIN);

    Serial.print("Motion: ");
    Serial.println(motion);

    if (motion == HIGH) {
      // Warm white-ish color
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 180, 100));
      }
      strip.show();
    } else {
      strip.clear();
      strip.show();
    }

  } else {
    strip.clear();
    strip.show();
  }
}
