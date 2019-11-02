#include <Arduino.h>
#include <FreeRTOS.h>

constexpr int PIN_RING = 5;
constexpr int PIN_BUZZER = 22;
constexpr int LEDC_CHANNEL_BUZZER = 0;

TaskHandle_t beepTask;
volatile bool DRAM_ATTR isBeeping = false;

void IRAM_ATTR startBeep() {
  if (!isBeeping) {
    vTaskResume(beepTask);
  }
}

void beepTaskHandler(__attribute__((unused)) void *) {
  pinMode(PIN_BUZZER, OUTPUT);
  ledcAttachPin(PIN_BUZZER, 0);
  while (true) {
    isBeeping = false;
    vTaskSuspend(beepTask);
    Serial.println("BEEP");
    isBeeping = true;
    ledcWriteTone(LEDC_CHANNEL_BUZZER, 220);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ledcWriteTone(LEDC_CHANNEL_BUZZER, 440);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ledcWriteTone(LEDC_CHANNEL_BUZZER, 880);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ledcWriteTone(LEDC_CHANNEL_BUZZER, 0);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RING, INPUT_PULLUP);
  xTaskCreatePinnedToCore(&beepTaskHandler, "Beep", 1024, nullptr, 1, &beepTask, 1);
  attachInterrupt(digitalPinToInterrupt(PIN_RING), startBeep, FALLING);
}

void loop() {
  Serial.println(digitalRead(PIN_RING));
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
