// 도전과제 2: PWM 함수 구현 (Arduino Uno, 소프트웨어 PWM)

const uint8_t LED_PIN = 7;

// === 과제 요구 함수 ===
static unsigned int PERIOD_US = 1000; // 기본 1 ms (1 kHz)
static uint8_t DUTY_PCT = 0;          // 0 ~ 100 (%)

void set_period(int period_us) {
  if (period_us < 100)   period_us = 100;    // 0.1 ms
  if (period_us > 10000) period_us = 10000;  // 10 ms
  PERIOD_US = (unsigned int)period_us;
}

void set_duty(int duty_pct) {
  if (duty_pct < 0)   duty_pct = 0;
  if (duty_pct > 100) duty_pct = 100;
  DUTY_PCT = (uint8_t)duty_pct;
}

// PWM 한 사이클(주기)만 생성: HIGH 톤, LOW 톤
inline void pwm_one_cycle() {
  unsigned long ton  = (unsigned long)PERIOD_US * DUTY_PCT / 100UL;
  unsigned long toff = (unsigned long)PERIOD_US - ton;

  if (ton > 0) {
    digitalWrite(LED_PIN, HIGH);
    delayMicroseconds((unsigned int)ton);
  }
  if (toff > 0) {
    digitalWrite(LED_PIN, LOW);
    delayMicroseconds((unsigned int)toff);
  }
}

// 0 → 1초: 0% → 100% 선형 상승
// 1 → 2초: 100% → 0% 선형 하강
const unsigned long TRIANGLE_MS = 2000; // 2 s 주기

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // 과제 촬영용, 아래 값 변경하기 : 10000, 1000, 100
  set_period(100); // 10 ms, 1 ms, 0.1 ms
  set_duty(0);
}

void loop() {
  // 현재 삼각파 위상 계산
  unsigned long t = millis() % TRIANGLE_MS;

  // 0 → 100 → 0 을 2초에 걸쳐 선형
  uint8_t duty;
  if (t < TRIANGLE_MS / 2) {
    // 상승 구간: 0 → 100 (0 ~ 1초)
    duty = (uint8_t)( (t * 100UL) / (TRIANGLE_MS / 2) );
  } else {
    // 하강 구간: 100 → 0 (1 ~ 2초)
    unsigned long down = t - (TRIANGLE_MS / 2);
    duty = (uint8_t)( 100UL - (down * 100UL) / (TRIANGLE_MS / 2) );
  }
  set_duty(duty);

  pwm_one_cycle();
}
