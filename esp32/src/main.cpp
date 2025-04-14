#include <Arduino.h>
#include <NimBLEDevice.h>

NimBLEServer* pServer = nullptr;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=== BLEアドバタイズ デバッグ開始（setName対応） ===");

    NimBLEDevice::init("YMF288BLE_Test");
    Serial.println("✅ NimBLEDevice 初期化 OK");

    pServer = NimBLEDevice::createServer();
    Serial.println("✅ サーバ作成 OK");

    NimBLEService* pService = pServer->createService("1234");
    Serial.println("✅ サービス作成 OK");

    NimBLECharacteristic* pChar = pService->createCharacteristic(
        "abcd",
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );
    pChar->setValue("Hello BLE!");
    Serial.println("✅ キャラクタリスティック作成 OK");

    pService->start();
    Serial.println("✅ サービス開始 OK");

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("1234");
    pAdvertising->setName("YMF288BLE_Test");  // 明示的に名前をアドバタイズに含める
    pAdvertising->start();
    Serial.println("✅ アドバタイズ開始 OK（iPhoneから見えるはず）");
}

void loop() {
    // BLE処理はバックグラウンドで継続
}
