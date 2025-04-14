# ESP32サブプロジェクト（YMF288Duo）

このフォルダは、ESP32を用いたYMF288Duo制御マイコンのコードを格納します。

## 🔧 使用環境

- PlatformIO + VSCode
- ボード: `esp32dev`（ESP32-DevKitC-32E）および `esp32s3-devkitc-1`（※対応追加）
- フレームワーク: Arduino

## 📁 ディレクトリ構成（ESP32 S98Player）

```
esp32/
├── src/
│   ├── main.cpp                 # エントリーポイント：setup()/loop()、初期化とモード遷移制御
│   ├── S98Player.cpp/.h         # S98再生処理クラス：S98Header解析、タイミング管理、再生ロジック
│   ├── SDHelper.cpp/.h          # microSDカード操作ヘルパ：ファイル列挙、読み出しユーティリティ
│   ├── YMF288Driver.cpp/.h      # YMF288アクセス用ドライバ：レジスタI/O、DAC/I2S出力、バス制御（※ESP32-S3対応済）
│   ├── ModeManager.cpp/.h       # 動作モード管理：スタンドアロン／BLE制御の切替ロジック
│   ├── BLEReceiver.cpp/.h       # BLE通信制御：iOSアプリとのBLEパケット受信＆解析
│   ├── OTAUpdate.cpp/.h         # OTAアップデート管理：Wi-Fi経由のファームウェア更新処理
│   ├── CommandParser.cpp/.h     # 制御コマンド解析：BLEやUART経由のJSON/バイナリコマンド対応
│   ├── BufferStream.cpp/.h      # S98やコマンド処理用のリングバッファ/ストリーム実装
│   ├── Logger.cpp/.h            # シリアル出力＆デバッグログ管理（出力レベル切替・タグ付き）
│   ├── RhythmPlayer.cpp/.h      # リズム音源のWAV再生エミュレーション（※ハード非接続時のみ）
│   ├── AdpcmPlayer.cpp/.h       # ADPCM 1音再生：a2p.c変換 + I2S出力
│   └── RhythmController.cpp/.h  # RhythmPlayerとYMF288Driverを自動切替で制御
│
├── include/
│   └── config.h                 # 共通設定：ピンアサイン（ESP32用）
│   └── config_s3.h              # 共通設定：ピンアサイン（ESP32-S3用、※本日追加）
│   └── s98.h                    # s98ファイル解析用の構造体定義
│   └── ymf288.h                 # YMF288ドライバ実装用の構造体定義
│
├── lib/
│   └── （必要に応じて外部ライブラリや自作ライブラリを追加）
│       └── 例：SDfat, ArduinoJson, NimBLE-Arduino 等
│
├── platformio.ini              # PlatformIOビルド設定：esp32dev / esp32s3 両対応に拡張
```

## 🚀 ビルドと書き込み

- ビルド: `PlatformIO: Build`（▷）
- 書き込み: `PlatformIO: Upload`
- モニタ: `PlatformIO: Monitor`

### 🔄 対応ボード別ビルド方法

- 通常ESP32: `pio run -e esp32dev`
- ESP32-S3: `pio run -e esp32s3`

## ✅ 今後追加する予定

- **S98ファイル再生機能**
  - [x] S98ファイルの構造解析（`S98Parser` 実装）
  - [ ] SDカードからのS98ファイル読み込み
  - [ ] タイマ割り込みベースでの再生ループ（WAIT命令対応）

- **YMF288チップ制御**
  - [x] YMF288レジスタ出力の基本制御実装（`Ymf288Driver`）
  - [x] ESP32-S3用に `setDataBus()` を明示GPIO方式に修正
  - [ ] FM / SSG / リズム各音源の再生確認用テストパターン生成

- **BLE経由のリアルタイムコントロール**
  - [ ] BLE通信の受信処理（NimBLE-Arduino）
  - [ ] 制御コマンド（再生/停止/音色変更など）のパース・反映
  - [ ] iPhoneアプリ（PC88iOS）との通信プロトコル確立

- **ソフト再生エミュレーション**
  - [x] `AdpcmPlayer` によるPCM変換＆I2S再生
  - [x] `RhythmPlayer` によるWAV再生機構
  - [x] ハード未接続時は `RhythmController` 経由でソフト再生へ自動切替

---

## 📝 更新履歴

- **2025-04-14**
  - 🎉 ESP32-S3-DevKitC-1 への正式対応（`config_s3.h`, `env:esp32s3` 追加）
  - `Ymf288Driver` の `setDataBus()` をピン定義に従い再実装（S3向け）
  - `RhythmPlayer`, `AdpcmPlayer`, `RhythmController` クラスの新規作成
  - S98コマンドパーサ `S98Parser` を実装、mainループと連携
  - ピンマップ表をMarkdownで整理（日本語文字化け対策）

- **2025-04-13**
  - ディレクトリ構成を詳細化、各ファイルの役割を明記
  - 今後追加する予定を具体的なタスクに分割
  - 初版 README.md 作成
