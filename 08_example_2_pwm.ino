// 08_example_2.ino – 거리(초음파)에 따른 LED 밝기 제어 (25 ms 주기, active-low PWM)

#define PIN_LED   9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL        346.0    // m/s @ 24℃
#define INTERVAL       25       // ms
#define PULSE_DURATION 10       // us
#define _DIST_MIN      100.0    // mm
#define _DIST_MAX      300.0    // mm

#define TIMEOUT  ((unsigned long)((INTERVAL / 2.0) * 1000.0)) // us
#define SCALE    (0.001 * 0.5 * SND_VEL)  // us -> mm

unsigned long last_ms;
float distance_mm = _DIST_MAX;

// float용 map + clamp
static inline float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  if (x < in_min) x = in_min;
  if (x > in_max) x = in_max;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(115200);
  last_ms = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - last_ms < INTERVAL) return;
  last_ms += INTERVAL;  // 드리프트 방지

  // 1) 거리 측정
  distance_mm = USS_measure(PIN_TRIG, PIN_ECHO);

  // 2) PWM 계산(삼각형 곡선)
  //    200 mm에서 최밝기(0), 150/250 mm에서 50%(~128), 100/300 mm에서 최소 밝기(255)
  int pwm = 255; // 기본: 최소 밝기 (active-low)
  bool valid = isfinite(distance_mm) &&
               (distance_mm >= _DIST_MIN) && (distance_mm <= _DIST_MAX);

  if (valid) {
    float dev = fabsf(distance_mm - 200.0f);      // 0~100
    float pwm_f = fmap(dev, 0.0f, 100.0f, 0.0f, 255.0f);
    if (pwm_f < 0)   pwm_f = 0;
    if (pwm_f > 255) pwm_f = 255;
    pwm = (int)(pwm_f + 0.5f);
  } else {
    pwm = 255; // 범위 밖/타임아웃 시 최소 밝기
  }

  // 3) LED 구동 (UNO D9 PWM, active-low라 0=가장 밝음)
  analogWrite(PIN_LED, pwm);

  // 4) Serial Plotter용 출력(탭 구분)
  Serial.print("distance:"); Serial.print(distance_mm); Serial.print('\t');
  Serial.print("pwm:");      Serial.println(pwm);
}

// 초음파 측정 (mm)
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  unsigned long dur = pulseIn(ECHO, HIGH, TIMEOUT); // us(왕복)
  if (dur == 0) return NAN; // 타임아웃 → invalid
  return dur * SCALE;       // mm
}
