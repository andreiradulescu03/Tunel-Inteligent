#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Pini pentru senzori si LED-uri
#define SENSOR_ENTRY_1 5
#define SENSOR_EXIT_1 6
#define LED_WHITE_1 24 // LED alb pentru sensul 1
#define SENSOR_ENTRY_2 36
#define SENSOR_EXIT_2 37
#define LED_WHITE_2 25 // LED alb pentru sensul 2
#define LED_GREEN 7
#define LED_YELLOW 8
#define LED_RED 9
#define POTENTIOMETER_GAS A0
#define POTENTIOMETER_SMOKE A1
#define PANIC_BUTTON 10

// Praguri pentru incidente
#define MAX_CARS_PER_LANE 3
#define GAS_THRESHOLD 700
#define SMOKE_THRESHOLD 700

// Variabile globale
volatile int carCountLane1 = 0; // Numar masini pe sensul 1
volatile int carCountLane2 = 0; // Numar masini pe sensul 2
volatile bool gasLeak = false;
volatile bool smokeDetected = false;
volatile bool panicMode = false;
SemaphoreHandle_t sem_carCountLane1;
SemaphoreHandle_t sem_carCountLane2;

// Functie pentru debounce
bool debounce(int pin) {
    if (digitalRead(pin) == HIGH) {
        vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce de 50 ms
        if (digitalRead(pin) == HIGH) {
            return true;
        }
    }
    return false;
}

// Task pentru verificarea intrarii pe sensul 1
void entryCheckTaskLane1(void *pvParameters) {
    while (1) {
        if (debounce(SENSOR_ENTRY_1) && !panicMode) { // Detectam o masina noua la intrare
            if (xSemaphoreTake(sem_carCountLane1, portMAX_DELAY) == pdTRUE) {
                if (carCountLane1 < MAX_CARS_PER_LANE && !gasLeak && !smokeDetected) {
                    carCountLane1++;
                    Serial.print("Intrare detectata pe sensul 1. Numar masini: ");
                    Serial.println(carCountLane1);
                    digitalWrite(LED_GREEN, HIGH); // Aprindem LED-ul verde pentru intrare
                    vTaskDelay(100 / portTICK_PERIOD_MS); // Puls scurt pe LED
                    digitalWrite(LED_GREEN, LOW);
                    digitalWrite(LED_WHITE_1, HIGH); // LED alb ramane aprins
                }
                xSemaphoreGive(sem_carCountLane1);
            }
            vTaskDelay(5000 / portTICK_PERIOD_MS); // Timeout de 5 secunda intre detectari
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Verificare periodica rapida
    }
}

// Task pentru verificarea iesirii pe sensul 1
void exitCheckTaskLane1(void *pvParameters) {
    while (1) {
        if (debounce(SENSOR_EXIT_1)) { // Detectam o masina la iesire
            if (xSemaphoreTake(sem_carCountLane1, portMAX_DELAY) == pdTRUE) {
                if (carCountLane1 > 0) {
                    carCountLane1--;
                    Serial.print("Iesire detectata pe sensul 1. Numar masini: ");
                    Serial.println(carCountLane1);
                    digitalWrite(LED_RED, HIGH); // Aprindem LED-ul rosu pentru iesire
                    vTaskDelay(100 / portTICK_PERIOD_MS); // Puls scurt pe LED
                    digitalWrite(LED_RED, LOW);
                    if (carCountLane1 == 0) {
                        digitalWrite(LED_WHITE_1, LOW); // Stingem LED-ul alb daca nu mai sunt masini
                    }
                }
                xSemaphoreGive(sem_carCountLane1);
            }
            vTaskDelay(3000 / portTICK_PERIOD_MS); // Timeout de 5 secunda intre detectari
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Verificare periodica rapida
    }
}

// Task pentru verificarea intrarii pe sensul 2
void entryCheckTaskLane2(void *pvParameters) {
    while (1) {
        if (debounce(SENSOR_ENTRY_2) && !panicMode) { // Detectam o masina noua la intrare
            if (xSemaphoreTake(sem_carCountLane2, portMAX_DELAY) == pdTRUE) {
                if (carCountLane2 < MAX_CARS_PER_LANE && !gasLeak && !smokeDetected) {
                    carCountLane2++;
                    Serial.print("Intrare detectata pe sensul 2. Numar masini: ");
                    Serial.println(carCountLane2);
                    digitalWrite(LED_GREEN, HIGH); // Aprindem LED-ul verde pentru intrare
                    vTaskDelay(100 / portTICK_PERIOD_MS); // Puls scurt pe LED
                    digitalWrite(LED_GREEN, LOW);
                    digitalWrite(LED_WHITE_2, HIGH); // LED alb ramane aprins
                }
                xSemaphoreGive(sem_carCountLane2);
            }
            vTaskDelay(5000 / portTICK_PERIOD_MS); // Timeout de 5 secunda intre detectari
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Verificare periodica rapida
    }
}

// Task pentru verificarea iesirii pe sensul 2
void exitCheckTaskLane2(void *pvParameters) {
    while (1) {
        if (debounce(SENSOR_EXIT_2)) { // Detectam o masina la iesire
            if (xSemaphoreTake(sem_carCountLane2, portMAX_DELAY) == pdTRUE) {
                if (carCountLane2 > 0) {
                    carCountLane2--;
                    Serial.print("Iesire detectata pe sensul 2. Numar masini: ");
                    Serial.println(carCountLane2);
                    digitalWrite(LED_RED, HIGH); // Aprindem LED-ul rosu pentru iesire
                    vTaskDelay(100 / portTICK_PERIOD_MS); // Puls scurt pe LED
                    digitalWrite(LED_RED, LOW);
                    if (carCountLane2 == 0) {
                        digitalWrite(LED_WHITE_2, LOW); // Stingem LED-ul alb daca nu mai sunt masini
                    }
                }
                xSemaphoreGive(sem_carCountLane2);
            }
            vTaskDelay(3000 / portTICK_PERIOD_MS); // Timeout de 5 secunda intre detectari
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Verificare periodica rapida
    }
}

// Task pentru monitorizarea incidentelor
void incidentMonitoring(void *pvParameters) {
    while (1) {
        int gasValue = analogRead(POTENTIOMETER_GAS);
        int smokeValue = analogRead(POTENTIOMETER_SMOKE);

        gasLeak = gasValue > GAS_THRESHOLD;
        smokeDetected = smokeValue > SMOKE_THRESHOLD;

        if (gasLeak) {
            Serial.println("Scurgere de gaze detectata!");
        }
        if (smokeDetected) {
            Serial.println("Fum detectat!");
        }

        if (gasLeak || smokeDetected || carCountLane1 >= MAX_CARS_PER_LANE || carCountLane2 >= MAX_CARS_PER_LANE || panicMode) {
            digitalWrite(LED_YELLOW, HIGH); // Aprindem LED-ul galben pentru alerta
        } else {
            digitalWrite(LED_YELLOW, LOW); // Stingem LED-ul galben daca nu mai este alerta
        }

        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}

// Task pentru monitorizarea butonului de panica
void panicButtonTask(void *pvParameters) {
    pinMode(PANIC_BUTTON, INPUT_PULLUP);
    bool lastState = digitalRead(PANIC_BUTTON);

    while (1) {
        bool currentState = digitalRead(PANIC_BUTTON);

        if (currentState == LOW && lastState == HIGH) {
            vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce
            if (digitalRead(PANIC_BUTTON) == LOW) {
                panicMode = !panicMode;
                if (panicMode) {
                    Serial.println("ALERTA: Buton de panica activat!");
                } else {
                    Serial.println("ALERTA: Buton de panica dezactivat!");
                }
            }
        }

        lastState = currentState;
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}

// Task pentru afisare
void displayTask(void *pvParameters) {
    while (1) {
        Serial.print("Numar masini pe sensul 1: ");
        Serial.println(carCountLane1);
        Serial.print("Numar masini pe sensul 2: ");
        Serial.println(carCountLane2);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    // Configurare monitor serial
    Serial.begin(9600);
    Serial.println("Sistem de monitorizare a tunelului pornit.");

    // Configurare pini
    pinMode(SENSOR_ENTRY_1, INPUT);
    pinMode(SENSOR_EXIT_1, INPUT);
    pinMode(LED_WHITE_1, OUTPUT);
    pinMode(SENSOR_ENTRY_2, INPUT);
    pinMode(SENSOR_EXIT_2, INPUT);
    pinMode(LED_WHITE_2, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(POTENTIOMETER_GAS, INPUT);
    pinMode(POTENTIOMETER_SMOKE, INPUT);

    // Creare semafoare pentru contoare
    sem_carCountLane1 = xSemaphoreCreateMutex();
    sem_carCountLane2 = xSemaphoreCreateMutex();

    // Creare task-uri FreeRTOS
    xTaskCreate(entryCheckTaskLane1, "Entry Check Task Lane 1", 128, NULL, 1, NULL);
    xTaskCreate(exitCheckTaskLane1, "Exit Check Task Lane 1", 128, NULL, 1, NULL);
    xTaskCreate(entryCheckTaskLane2, "Entry Check Task Lane 2", 128, NULL, 1, NULL);
    xTaskCreate(exitCheckTaskLane2, "Exit Check Task Lane 2", 128, NULL, 1, NULL);
    xTaskCreate(incidentMonitoring, "Incident Monitoring", 128, NULL, 1, NULL);
    xTaskCreate(displayTask, "Display Task", 128, NULL, 1, NULL);
    xTaskCreate(panicButtonTask, "Panic Button Task", 128, NULL, 1, NULL);
}

void loop() {
}
