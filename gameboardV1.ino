#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include "Point2D.h"
#include "Ball.h"
#include "PlayerBar.h"
#include "Brick.h"


hw_timer_t *Timer0_Cfg = NULL;
hw_timer_t *Timer1_Cfg = NULL;

bool buzzer_on = false;
int t_start_buzzer = 0;
int t_stop_buzzer = 0;
int encoder_pos = SCREEN_WIDTH/2;
int t_encA = 0;
int t_encB = 0;

int score = 0;
int hiscore = 0;

void buttonISR1();
void buttonISR2();
void IRAM_ATTR EncA_ISR();
void IRAM_ATTR Timer0_ISR();
void Timer1_ISR();
void inline timerSetup();
void inline buzzerTimerSetup(int freq);
void buzzerStart(int freq);
void stopGame();
void restartGame();


Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

PlayerBar myPlayerBar(&display);
Ball myBall(&display, &buzzerStart, &stopGame);
std::vector<Brick*> bricks;

// ------------------------- Setup --------------------------------- //
void setup() {
  noInterrupts();
  // LED
  pinMode(MY_LED_BUILTIN, OUTPUT);
  digitalWrite(MY_LED_BUILTIN, LOW);

  // BUTTON
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonISR2,
    RISING
  );
  pinMode(ENC_BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(
    digitalPinToInterrupt(ENC_BUTTON_PIN),
    buttonISR1,
    RISING
  );

  // ENCODER
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(ENC_A),
    EncA_ISR,
    RISING
  );

  // BUZZER
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  //buzzerTimerSetup(8000);
  timerSetup();
  screenSetup();

  // Generate bricks
  int num_bricks_total = NUM_BRICKS_V*NUM_BRICKS_H;
  for (int i = 0; i<num_bricks_total ; i++) {
    auto b = new Brick(i, &display);
    bricks.push_back(b);
  }

  interrupts();
}


// ------------------------- Visuals --------------------------------- //
void loop() {
  display.clearDisplay();

  for (auto b : bricks) {
    if (b->isDead()) continue;
    b->draw();
  }
  myPlayerBar.draw();
  myBall.draw();

  static const char num[] = {'0','1','2','3','4','5','6','7','8','9'};
  display.drawChar(0, 36, num[(score/100)], SSD1306_INVERSE, SSD1306_BLACK, 1);
  display.drawChar(10, 36, num[(score/10)%10], SSD1306_INVERSE, SSD1306_BLACK, 1);
  display.drawChar(20, 36, num[score%10], SSD1306_INVERSE, SSD1306_BLACK, 1);

  display.display();
}
// ------------------------------------------------------------------- //



// ------------------------- Game --------------------------------- //
void gameLoop() {
  myPlayerBar.updatePos(encoder_pos);
  static bool collided = false;
  static Point2D ballDir = Point2D(0,0);

  // bricks
  for (auto b : bricks) {
    if (b->isDead()) {
     continue;
    }
    collided = b->checkCollide(&myBall, &ballDir);
    if (collided) {
      score++;
      b->takeDmg();
      
      ballDir = Point2D(ballDir.x * myBall.getVel().x, ballDir.y * myBall.getVel().y);
      myBall.bounce(ballDir);
      buzzerStart(2000+score*100);
    }
  }

  // bar
  collided = myPlayerBar.checkCollide(&myBall, &ballDir);
  if (collided) {
    myBall.bounce(ballDir);
    buzzerStart(1000);
  }

  myBall.updatePos();
}
// ---------------------------------------------------------------- //





void screenSetup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 initialization failed!");
    while (true);
  }

  display.clearDisplay();
}

// Starts the buzzer timer
void buzzerStart(int freq) {
  buzzerTimerSetup(freq);
  // Start buzzer
  t_start_buzzer = millis();
  t_stop_buzzer = t_start_buzzer + 100;
  buzzer_on = true;
}

// Encoder button
void buttonISR1() {
  buzzerStart(8000);
}

// SMD button
void buttonISR2() {
  restartGame();
}

// BUZZER
// flips the GPIO high and low according to the buzzer timer's set frequency to produce a specific sound
bool b = false;
void IRAM_ATTR Timer0_ISR() {
  noInterrupts();
  if (buzzer_on == false) return;
  auto t_now = millis();
  if (t_now >= t_stop_buzzer) {
    buzzer_on = false;
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }
  b ^= 1;
  digitalWrite(BUZZER_PIN, b);
  interrupts();
}

// PHYSICS
void Timer1_ISR() {
  noInterrupts();
  gameLoop();
  interrupts();
}

void inline buzzerTimerSetup(int buzz_freq) {
  noInterrupts();

  // Buzzer timer
  const int timer_freq = 1000000;
  int alarm_freq = timer_freq/buzz_freq;
  timerAlarm(Timer0_Cfg, alarm_freq, true, 0);

  interrupts();
}

void inline timerSetup() {
  noInterrupts();

  // // Buzzer timer
  const int timer_freq = 1000000;
  const int buzz_freq = 8000;
  int alarm_freq = timer_freq/buzz_freq;
  Timer0_Cfg = timerBegin(timer_freq); // Hz
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR);
  timerAlarm(Timer0_Cfg, alarm_freq, true, 0);

  // Game physics timer
  alarm_freq = timer_freq/TARGET_FPS;
  Timer1_Cfg = timerBegin(timer_freq); // Hz
  timerAttachInterrupt(Timer1_Cfg, &Timer1_ISR);
  timerAlarm(Timer1_Cfg, alarm_freq, true, 0);

  interrupts();
}

void IRAM_ATTR EncA_ISR() {
  if (micros() - t_encA < MIN_T_ENC) return;

  // Rising edge of A just triggered ISR
  noInterrupts();
  if (digitalRead(ENC_B)) {
    // B is HIGH
    //Serial.println("CCW");
    encoder_pos -= SPINMULT;
  } else {
    // B is LOW
    //Serial.println("CW");
    encoder_pos += SPINMULT;
  }

  auto rightBound = myPlayerBar.getWidth();
  if (encoder_pos >= SCREEN_WIDTH - rightBound) encoder_pos = SCREEN_WIDTH - rightBound;
  if (encoder_pos <= 0) encoder_pos = 0;
  t_encA = micros();
  interrupts();
}

void restartGame() {
  score = 0;
  for (auto b : bricks) {
    b->restart();
  }
  myBall.restart();
}

void stopGame() {
  myBall.disable();
}




