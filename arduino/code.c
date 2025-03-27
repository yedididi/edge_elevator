/*

최종 프로젝트 edge_elevator_arduino
버튼 + 부저 + RFID + 서보모터

원래 있던 Wifi 예제 +
ESP32 WiFi 클라이언트 테스트
아두이노 우노 + ESP8266 코드를 아두이노 나노 ESP32용으로 변환

*/

#define AP_SSID "embA"
#define AP_PASS "embA1234"
#define SERVER_NAME "10.10.141.72"
#define SERVER_PORT 5000

#define BUTTON_PIN    4
#define BUZZER_PIN    5
#define SERVO_PIN     7
#define RST_PIN       9       // RFID RST
#define SS_PIN        10      // RFID SS

#define CMD_SIZE 50
#define BUTTON_DEBOUNCE_DELAY 1000
#define SERVO_INTERVAL 2000

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>     // ESP32용 WiFi 라이브러리
#include <ESP32Servo.h>  // 서보 라이브러리 포함

int buttonState = 0;  // variable for reading the pushbutton status
unsigned long lastPressTime = 0;  // 마지막 버튼 눌림 시간
unsigned long previousMillis  = 0;  // 마지막 버튼 눌림 시간
unsigned long buttonDebounceDelay = BUTTON_DEBOUNCE_DELAY; // 디바운싱 시간
unsigned long servoInterval = SERVO_INTERVAL; // 디바운싱 시간
bool servoMoving = false;  // 서보가 회전 중인지 여부

char sendBuf[CMD_SIZE];
char tmpBuf[CMD_SIZE];

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
WiFiClient client;  // ESP32용 WiFi 클라이언트
Servo myServo;  // 서보 객체 생성

void setup() 
{
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200); // ESP32는 더 높은 보드레이트 사용
    Serial.println("ESP32 시작...");

    wifi_Setup();
    rfid_setup();
    servo_setup();
}

void loop() 
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > SERVO_INTERVAL && servoMoving == false)
    {
        myServo.write(60);
        delay(10);
    }
    else if(currentMillis - previousMillis < SERVO_INTERVAL && servoMoving == true)
    {
        myServo.write(136);
        delay(10);
        servoMoving = false;
    }

    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH && (millis() - lastPressTime) > buttonDebounceDelay) 
    {
        memset(sendBuf, 0, CMD_SIZE);  // 버퍼 초기화 추가
        sprintf(sendBuf, "BUTTON_PRESSED");
        Serial.println("BUTTON_PRESSED");
        client.print(sendBuf);  // ESP32의 client.write 대신 client.print 사용
        client.flush();    
        lastPressTime = millis();
    }

    // RFID 카드가 리더기 근처에 있는지 확인
    if (!mfrc522.PICC_IsNewCardPresent()) 
    {
        //Serial.println("No new card detected.");
        delay(100);  // 짧게 대기
        return;
    }

    // RFID 카드의 정보를 읽는다
    if (!mfrc522.PICC_ReadCardSerial()) 
    {
        Serial.println("Failed to read card serial.");
        delay(100);  // 짧게 대기
        return;
    }

    tone(BUZZER_PIN, 3000);
    delay(100);       
    noTone(BUZZER_PIN);       
    delay(100);

    myServo.detach();
    myServo.attach(SERVO_PIN);
    previousMillis = currentMillis;
    servoMoving = true;

    memset(sendBuf, 0, CMD_SIZE);  // 버퍼 초기화 추가

    // UID 값 출력
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        sprintf(tmpBuf, "%02X", mfrc522.uid.uidByte[i]);
        // Serial.print(tmpBuf);
        strcat(sendBuf, tmpBuf);
        strcat(sendBuf, " ");
    }
    Serial.println(sendBuf);
    client.print(sendBuf);  // ESP32의 client.write 대신 client.print 사용
    client.flush();  // 버퍼 플러시
    memset(sendBuf, 0, CMD_SIZE);  // 버퍼 크기 수정
    delay(500);
    Serial.println("End of loop.");
}

void rfid_setup() 
{
    SPI.begin();         // Init SPI bus
    mfrc522.PCD_Init();  // Init MFRC522
    delay(10);            // Optional delay. Some board do need more time after init to be ready, see Readme
    mfrc522.PCD_DumpVersionToSerial();   // Show details of PCD - MFRC522 Card Reader details
}

void servo_setup() 
{
    myServo.attach(SERVO_PIN);  // 서보모터를 7번 핀에 연결
    myServo.write(60);    // 서보를 60도 위치로 이동 (초기화)
    delay(10);
}

void wifi_Setup() 
{
    wifi_Init();
    server_Connect();
}

void wifi_Init() 
{
    WiFi.begin(AP_SSID, AP_PASS);
    // WiFi 연결 대기
    while (WiFi.status() != WL_CONNECTED) 
    {
    delay(500);
    Serial.print(".");
    }
}

int server_Connect() 
{
    if (client.connect(SERVER_NAME, SERVER_PORT)) 
    {
        client.print("arduino"); 
        return 1;
    }
    else 
    {
        return 0;
    } 
}