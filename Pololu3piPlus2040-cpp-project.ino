#include <Pololu3piPlus2040.h>
#include <PololuMenu.h>

OLED display;
Buzzer buzzer;
LineSensors lineSensors;
Motors motors;
ButtonA buttonA;
ButtonB buttonB;
ButtonC buttonC;
RGBLEDs leds;
PololuMenu menu;

// --- TUNING KNOBS ---
float Kp = 0.06;  // Increased for sharper turning at high speeds
float Kd = 1.8;   // Increased to stop the back end from swinging out
int16_t lastError = 0;
uint16_t threshold = 500;

int16_t baseSpeed = 255;   // Speed for curves (leaves room to steer)
int16_t brakeFactor = 14;  // Lower = Stronger braking on sharp turns

uint16_t calibrationSpeed = 120;
int8_t mode;

// --- FUNCTIONS ---
void selectFollow() { mode = 1; }
void selectMaze() { mode = 2; }
void selectWoble() { mode = 3; }
void selectSpin() { mode = 4; }
void selectMode() {
  display.clear();
  display.print("Select");
  display.gotoXY(0, 1);
  display.print("mode");
  delay(1000);
  static const PololuMenuItem items[] = {
    { "Line", selectFollow },
    { "Maze", selectMaze },
    { "Woble", selectWoble },
    { "Spin", selectSpin },
  };
  menu.setItems(items, 4);
  menu.setDisplay(display);
  menu.setBuzzer(buzzer);
  menu.setButtons(buttonA, buttonB, buttonC);
  while (!menu.select());
}
void loadCustomCharacters() {
  static const char levels[] = { 0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63 };
  display.loadCustomCharacter(levels + 0, 0);
  display.loadCustomCharacter(levels + 1, 1);
  display.loadCustomCharacter(levels + 2, 2);
  display.loadCustomCharacter(levels + 3, 3);
  display.loadCustomCharacter(levels + 4, 4);
  display.loadCustomCharacter(levels + 5, 5);
  display.loadCustomCharacter(levels + 6, 6);
}
void printBar(uint8_t height) {
  if (height > 8) { height = 8; }
  const char barChars[] = { ' ', 0, 1, 2, 3, 4, 5, 6, (char)255 };
  display.print(barChars[height]);
}
void calibrateSensors() {
  display.clear();
  display.print("Calib...");
  delay(1000);
  for (uint16_t i = 0; i < 80; i++) {
    if (i > 20 && i <= 60) motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
    else motors.setSpeeds(calibrationSpeed, -calibrationSpeed);
    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}
void showReading(uint16_t position) {
  display.gotoXY(0, 0);
  display.print(position);
  display.print("    ");
  display.gotoXY(0, 1);
  for (uint8_t i = 0; i < 5; i++) {
    uint8_t barHeight = map(lineSensors.calibratedSensorValues[i], 0, 1000, 0, 8);
    printBar(barHeight);
  }
}
void showReadings() {
  display.clear();
  while (!buttonB.getSingleDebouncedPress()) {
    showReading(lineSensors.readLineBlack());
    delay(50);
  }
}

// --- CORE PROGRAM ---
void setup() {
  // load screen chars
  loadCustomCharacters();

  // select Mode screen
  selectMode();
  display.clear();

  // calibrate screen
  if (mode != 4) {
    display.print("Press B");
    display.gotoXY(0, 1);
    display.print("to calib");
    while (!buttonB.getSingleDebouncedPress());
    calibrateSensors();
    showReadings();
    display.clear();
  }

  // start screen
  display.print("Go!");
  buzzer.play("L16 cdegreg4");
  while (buzzer.isPlaying());
  display.clear();

  // play tetris music
  buzzer.play("v20 t140 o5 l8 e4 <b >c d4 c <b a4 a >c e4 d c <b4. >c d4 e4 c4 <a a4 >r4 d4 f a4 g f e4. c e4 d <c b4 b >c d4 e4 c4 <a a4 l8 e4 <b >c d4 c <b a4 a >c e4 d c <b4. >c d4 e4 c4 <a a4 >r4 d4 f a4 g f e4. c e4 d <c b4 b >c d4 e4 c4 <a a4 l8 e4 <b >c d4 c <b a4 a >c e4 d c <b4. >c d4 e4 c4 <a a4 >r4 d4 f a4 g f e4. c e4 d <c b4 b >c d4 e4 c4 <a a4 l8 e4 <b >c d4 c <b a4 a >c e4 d c <b4. >c d4 e4 c4 <a a4 >r4 d4 f a4 g f e4. c e4 d <c b4 b >c d4 e4 c4 <a a4");
}

void loop() {
  // get position and error
  int16_t position = lineSensors.readLineBlack();
  int16_t error = position - 2000;

  if (mode == 1) { // --- MODE 1: TURBO ---
    // calculate turn size
    float turn = (error * Kp) + (error - lastError) * Kd;
    // save error for next turn
    lastError = error;

    // calculate speed
    int16_t currentTarget = baseSpeed - (abs(error) / brakeFactor);
    for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(0, 0, 255)); }
    leds.show();

    // apply calculated speed and safety constrain to stay within -255 to 255
    int16_t leftSpeed = constrain(currentTarget + (int16_t)turn, -255, 255);
    int16_t rightSpeed = constrain(currentTarget - (int16_t)turn, -255, 255);

    // set motor speed
    motors.setSpeeds(leftSpeed, rightSpeed);
    delay(1.5);
  } else if (mode == 2) { // --- MODE 2: MAZE ---
    // [Insert Labyrinth logic here]
  } else if (mode == 3) { // --- MODE 3: WOBLE ---
    if (position < 1500) {
      motors.setSpeeds(-50, 200);
    } else if (position < 2000) {
      motors.setSpeeds(255, 255);
    } else {
      motors.setSpeeds(200, -50);
    }
  } else if (mode == 4) { // --- MODE 4: Spin / Party Mode ---
    display.clear();
    display.print("PARTY!");

    while (true) {
      motors.setSpeeds(255, -255);

      for (int8_t i = 0; i < 100; i++) {
        // Strobe RGB
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(0, 0, 255)); }
        leds.show();
        delay(50);
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(0, 255, 255)); }
        leds.show();
        delay(50);
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(0, 255, 0)); }
        leds.show();
        delay(50);
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(255, 255, 0)); }
        leds.show();
        delay(50);
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(255, 0, 0)); }
        leds.show();
        delay(50);
        for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(255, 0, 255)); }
        leds.show();
        delay(50);

        // Safety: Press Button A to stop the party
        if (buttonA.isPressed()) {
          motors.setSpeeds(0, 0);
          for (uint8_t i = 0; i < 6; i++) { leds.set(i, RGB(0, 0, 0)); }
          leds.show();
          for(;;);
        }
      }

      motors.setSpeeds(-255, 255);
    }
  }
}