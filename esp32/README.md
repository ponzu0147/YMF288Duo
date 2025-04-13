# ESP32サブプロジェクト（YMF288Duo）

このフォルダは、ESP32を用いたYMF288Duo制御マイコンのコードを格納します。

## 🔧 使用環境

- PlatformIO + VSCode
- ボード: `esp32dev`（ESP32-DevKitC-32E）
- フレームワーク: Arduino

## 📁 ディレクトリ構成（ESP32 S98Player）

```
esp32/
├── src/
│   ├── main.cpp                 # エントリーポイント：setup()/loop()、初期化とモード遷移制御
│   ├── S98Player.cpp/.h         # S98再生処理クラス：S98Header解析、タイミング管理、再生ロジック
│   ├── SDHelper.cpp/.h          # microSDカード操作ヘルパ：ファイル列挙、読み出しユーティリティ
│   ├── YMF288Driver.cpp/.h      # YMF288アクセス用ドライバ：レジスタI/O、DAC/I2S出力、バス制御
│   ├── ModeManager.cpp/.h       # 動作モード管理：スタンドアロン／BLE制御の切替ロジック
│   ├── BLEReceiver.cpp/.h       # BLE通信制御：iOSアプリとのBLEパケット受信＆解析
│   ├── OTAUpdate.cpp/.h         # OTAアップデート管理：Wi-Fi経由のファームウェア更新処理
│   ├── CommandParser.cpp/.h     # 制御コマンド解析：BLEやUART経由のJSON/バイナリコマンド対応
│   ├── BufferStream.cpp/.h      # S98やコマンド処理用のリングバッファ/ストリーム実装
│   └── Logger.cpp/.h            # シリアル出力＆デバッグログ管理（出力レベル切替・タグ付き）
│
├── include/
│   └── config.h                 # 共通設定：ピンアサイン、モード定義、ビルド切替など
│   └── s98.h                    # s98ファイル解析用の構造体定義
│   └── ymf288.h                 # YMF288ドライバ実装用の構造体定義
│
├── lib/
│   └── （必要に応じて外部ライブラリや自作ライブラリを追加）
│       └── 例：SDfat, ArduinoJson, NimBLE-Arduino 等
│
├── platformio.ini              # PlatformIOビルド設定：ボード種別、ライブラリ依存、ビルドフラグなど
```

## 🚀 ビルドと書き込み

- ビルド: `PlatformIO: Build`（▷）
- 書き込み: `PlatformIO: Upload`
- モニタ: `PlatformIO: Monitor`

## ✅ 今後追加する予定

- **S98ファイル再生機能**
  - [ ] SDカードからのS98ファイル読み込み（バイナリ / フォルダ選択）
  - [ ] S98Headerの解析とタイマ設定処理
  - [ ] タイマ割り込みベースでの再生ループ（WAIT命令対応）

- **YMF288チップ制御**
  - [ ] YMF288のレジスタ制御ドライバの整備（バスアクセス）
  - [ ] I2S経由でDACに音声データを送出（BU9480F / PCM5102A 両対応）
  - [ ] FM / SSG / リズム各音源の再生確認用テストパターン生成

- **BLE経由のリアルタイムコントロール**
  - [ ] BLE通信の受信処理（NimBLE-Arduino）
  - [ ] 制御コマンド（再生/停止/音色変更など）のパース・反映
  - [ ] iPhoneアプリ（PC88iOS）との通信プロトコル確立

- **開発支援機能**
  - [ ] シリアルデバッグログのレベル別出力（Loggerモジュール）
  - [ ] モード切替（スタンドアロン / リモート）の動作確認用UI出力

---

## 📝 更新履歴

- **2025-04-13**
  - ディレクトリ構成を詳細化、各ファイルの役割を明記
  - 今後追加する予定を具体的なタスクに分割
  - 初版 README.md 作成
