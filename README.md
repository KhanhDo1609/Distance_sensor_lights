# Distance Sensor Lights

Five LEDs create a proximity-controlled ambient light using an Arduino Uno and an HC-SR04 ultrasonic distance sensor. As an object moves closer, the bottom LED gradually brightens. Once it is fully bright, the next LED starts fading in, continuing up to the top LED. Moving away reverses the sequence.

**Range: 50 cm to 2 cm.** At 50 cm or farther, the target is all off. At 2 cm or closer, the target is all five fully bright. Transitions are smoothed, so changes take a moment to settle. No echo makes the lights fade out. This project responds to distance rather than room brightness.

[Run the Wokwi simulation](https://wokwi.com/projects/479462698364160001) · [Arduino sketch](Distance_sensor_lights/Distance_sensor_lights.ino) · [Demo gallery source](docs/index.html)

## Three video demos

All three supplied MP4 recordings are included unchanged. GitHub's repository file viewer does not play these videos inline. The included gallery has **three embedded video players**, each with a **Download MP4** button.

### Watch online with GitHub Pages

The gallery is ready to publish. To enable it, open [Settings → Pages](https://github.com/KhanhDo1609/Distance_sensor_lights/settings/pages), choose **Deploy from a branch**, select **main** and **/docs**, and click **Save**. After GitHub finishes publishing, the gallery will be available at:

**[Open the three-demo gallery](https://khanhdo1609.github.io/Distance_sensor_lights/)** — requires the Pages setting above.

### Download the original videos

| Demo | Repository file | Download MP4 |
| --- | --- | --- |
| 1 — IMG_0122 | [Demo 1 file](docs/demos/IMG_0122.mp4) | [Download demo 1](https://github.com/KhanhDo1609/Distance_sensor_lights/raw/refs/heads/main/docs/demos/IMG_0122.mp4) |
| 2 — IMG_0123 | [Demo 2 file](docs/demos/IMG_0123.mp4) | [Download demo 2](https://github.com/KhanhDo1609/Distance_sensor_lights/raw/refs/heads/main/docs/demos/IMG_0123.mp4) |
| 3 — IMG_0124 | [Demo 3 file](docs/demos/IMG_0124.mp4) | [Download demo 3](https://github.com/KhanhDo1609/Distance_sensor_lights/raw/refs/heads/main/docs/demos/IMG_0124.mp4) |

To watch immediately without publishing, download or clone this repository and open `docs/index.html` in a browser. Keep the `docs/demos` folder beside it. If your browser cannot play a recording's codec, download it and open it in a local video player.

## Parts

- Arduino Uno and USB cable
- HC-SR04 ultrasonic distance sensor
- Five LEDs
- Five 220 Ω resistors (one per LED, matching the Wokwi diagram)
- Breadboard and jumper wires

## Wiring

| Component | Arduino connection |
| --- | --- |
| HC-SR04 VCC | 5V / breadboard positive rail |
| HC-SR04 GND | GND / breadboard negative rail |
| HC-SR04 TRIG | D2 |
| HC-SR04 ECHO | D3 |
| Bottom LED (1) anode | D5 |
| LED 2 anode | D6 |
| Middle LED (3) anode | D9 |
| LED 4 anode | D10 |
| Top LED (5) anode | D11 |

Connect each LED cathode to GND through its own 220 Ω resistor. The anode is normally the longer lead; the cathode is normally the shorter lead next to the flat edge. Connect the Arduino GND and sensor GND to the same ground rail. All five LED pins support PWM on the Uno.

The exported simulator circuit is saved in [`wokwi/diagram.json`](wokwi/diagram.json). The accompanying `wokwi/sketch.ino` is a copy of the supplied Arduino sketch, so the local simulator files use the documented 50 cm behavior.

## Upload and run

1. Download the repository ZIP using **Code → Download ZIP**, or clone it.
2. Open `Distance_sensor_lights/Distance_sensor_lights.ino` in the Arduino IDE. Keep the sketch inside its matching folder.
3. Connect the Arduino Uno, select **Arduino Uno** under Board, and select its port.
4. Upload the sketch. No extra Arduino libraries are required.
5. Open Serial Monitor at **9600 baud** to see the measured distance.
6. Move an object toward the sensor from beyond 50 cm. Watch the LEDs fill from bottom to top, with each LED fading before the next begins.

You can also open the linked Wokwi project, start the simulation, select the HC-SR04, and adjust its distance. The online project is a separate saved copy; use the included sketch if its saved code differs.

## How the code works

1. Send a 10-microsecond trigger pulse.
2. Measure the echo pulse with `pulseIn()`, allowing up to 25,000 microseconds before timing out.
3. Convert the round-trip time to centimeters: `distance = duration * 0.0343 / 2.0`.
4. Map distance into a combined light level between 0 and 5: `(50 - distance) / (50 - 2) * 5`, clamped to that range.
5. Move the current light level 20% toward the target each loop to soften transitions.
6. Give each LED its part of the light level and use `analogWrite()` with values from 0 to 255 to control brightness. For example, level 1.5 means the bottom LED is fully bright, the second is half bright, and the other three are off.
7. Wait 70 milliseconds before the next measurement.

### Approximate fade ranges

These ranges describe the steady target; smoothing introduces a short delay.

| LED, bottom to top | Fades from off to full as distance decreases |
| --- | --- |
| 1 — D5 | 50 → 40.4 cm |
| 2 — D6 | 40.4 → 30.8 cm |
| 3 — D9 | 30.8 → 21.2 cm |
| 4 — D10 | 21.2 → 11.6 cm |
| 5 — D11 | 11.6 → 2 cm |

## Adjust the behavior

Edit the constants near the top of the sketch:

```cpp
const byte ledPins[] = {5, 6, 9, 10, 11}; // Bottom to top
const float farDistance = 50.0;         // All-off distance in cm
const float nearDistance = 2.0;         // Full-brightness distance in cm
```

Keep `farDistance` greater than `nearDistance`. The smoothing factor is `0.2` in the `lightLevel` update: a smaller value fades more slowly, and a larger value responds faster. If you change the main sketch, update `wokwi/sketch.ino` too.

## Troubleshooting

- **No LEDs:** check LED polarity, the shared ground, and the 5V connection.
- **Wrong order:** check the LED wiring against the bottom-to-top pin array.
- **An LED only switches on/off:** ensure it uses one of the documented PWM pins.
- **No echo:** check TRIG/ECHO wiring and aim a broad, flat object at the sensor.
- **Unsteady readings:** hold the object steady; angled or soft surfaces can give inconsistent echoes. Measurements extremely close to the sensor can be unreliable.

## Files

```text
Distance_sensor_lights/Distance_sensor_lights.ino  Commented Arduino sketch
wokwi/sketch.ino                                 Matching simulator sketch
wokwi/diagram.json                               Exported simulator circuit
docs/index.html                                 Three-player demo gallery
docs/demos/IMG_0122.mp4                           Original demo 1
docs/demos/IMG_0123.mp4                           Original demo 2
docs/demos/IMG_0124.mp4                           Original demo 3
README.md                                       Project guide
```
