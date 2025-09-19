// 05_practice_2.ino
const int LED_PIN = 7;

const int LED_ON  = LOW;
const int LED_OFF = HIGH;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF); // 꺼진 채 시작.
}

void loop() {
  // 처음 1초 켜기
  digitalWrite(LED_PIN, LED_ON);
  delay(1000);

  // 다음 1초 동안 5회 깜빡이기 (100 ms OFF + 100 ms ON × 5회)
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, LED_OFF);  delay(100);
    digitalWrite(LED_PIN, LED_ON); delay(100);
  }

  // 루프 종료(끄기)
  digitalWrite(LED_PIN, LED_OFF);
  while (1) {}
}
