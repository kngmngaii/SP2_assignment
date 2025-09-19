#define PIN_LED 13   // LED 핀 정의

unsigned int count = 0;   // 카운터 변수
bool toggle = false;      // LED 상태 (꺼짐/켜짐)

void setup() {
  Serial.begin(9600);     // 시리얼 통신 초기화
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  count++;                        // 카운터 증가
  Serial.print("Count: ");         
  Serial.println(count);

  toggle = !toggle;                // LED 상태 반전
  digitalWrite(PIN_LED, toggle);   // LED 점멸

  delay(1000);                     // 1초 대기
}
