// Arduino pin connected to the sensor's TRIG pin.
const byte trigPin = 2;

// Arduino pin connected to the sensor's ECHO pin.
const byte echoPin = 3;

// LED pins in order from BOTTOM to TOP.
// These pins support PWM, which controls LED brightness.
const byte ledPins[] = {5, 6, 9, 10, 11};

// Number of LEDs in the circuit.
const byte ledCount = 5;

// All LEDs turn off when the object is 50 cm away or farther.
const float farDistance = 50.0;

// All LEDs reach full brightness at 2 cm or closer.
const float nearDistance = 2.0;

// Current combined light level: 0 means all off; 5 means all full.
float lightLevel = 0;

// setup() runs once when the Arduino starts.
void setup() {
  // Set TRIG as an output so it can send a trigger pulse.
  pinMode(trigPin, OUTPUT);

  // Start with the trigger signal LOW.
  digitalWrite(trigPin, LOW);

  // Set ECHO as an input to read the sensor's return pulse.
  pinMode(echoPin, INPUT);

  // Go through every LED, starting with the bottom one.
  for (byte i = 0; i < ledCount; i++) {
    // Set this LED's pin as an output.
    pinMode(ledPins[i], OUTPUT);

    // Start this LED at zero brightness.
    analogWrite(ledPins[i], 0);
  }

  // Start serial communication for viewing distance readings.
  Serial.begin(9600);
}

// loop() repeats continuously while the Arduino is powered.
void loop() {
  // Ensure TRIG is LOW before starting a new measurement.
  digitalWrite(trigPin, LOW);

  // Wait 2 microseconds to establish a clean LOW signal.
  delayMicroseconds(2);

  // Start the trigger pulse.
  digitalWrite(trigPin, HIGH);

  // Keep the trigger HIGH for 10 microseconds.
  delayMicroseconds(10);

  // End the trigger pulse; the sensor now measures the echo.
  digitalWrite(trigPin, LOW);

  // Measure how long ECHO stays HIGH, in microseconds.
  // Stop waiting after 25,000 microseconds if no echo arrives.
  unsigned long duration = pulseIn(echoPin, HIGH, 25000);

  // Default to all lights off if no echo is detected.
  float targetLevel = 0;

  // A duration greater than zero means an echo was received.
  if (duration > 0) {
    // Convert travel time to centimeters.
    // Sound travels about 0.0343 cm per microsecond.
    // Divide by 2 because sound travels to the object and back.
    float distance = duration * 0.0343 / 2.0;

    // Convert distance into a combined light level from 0 to 5.
    // A closer object produces a higher light level.
    targetLevel = (farDistance - distance)
                  / (farDistance - nearDistance) * ledCount;

    // Keep the result between all off and all fully bright.
    targetLevel = constrain(targetLevel, 0.0, (float)ledCount);

    // Print a label before the distance reading.
    Serial.print("Distance: ");

    // Print the measured distance.
    Serial.print(distance);

    // Print the unit and move to the next line.
    Serial.println(" cm");
  } else {
    // Report when the sensor did not receive an echo.
    Serial.println("No echo detected");
  }

  // Move 20% toward the desired light level on each loop.
  // This softens changes and reduces flicker from small variations.
  lightLevel += (targetLevel - lightLevel) * 0.2;

  // Update each LED in order from bottom to top.
  for (byte i = 0; i < ledCount; i++) {
    // Give each LED its own portion of the combined light level.
    // Example: at level 1.5, the bottom LED is full,
    // the second is half bright, and the rest are off.
    float brightness = constrain(lightLevel - i, 0.0, 1.0);

    // Round very small brightness values down to fully off.
    if (brightness < 0.005) {
      brightness = 0;
    }

    // Round values near maximum up to fully on.
    if (brightness > 0.995) {
      brightness = 1;
    }

    // Convert the brightness from 0–1 into PWM values of 0–255.
    int pwmValue = (int)(brightness * 255 + 0.5);

    // Apply the brightness to this LED.
    analogWrite(ledPins[i], pwmValue);
  }

  // Wait 70 milliseconds before taking the next measurement.
  delay(70);
}