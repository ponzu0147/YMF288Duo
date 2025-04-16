# ESP32 サブプロジェクト（YMF288Duo）

このフォルダは、ESP32 を用いた YMF288Duo 制御マイコンのコードを格納します。

## 🔧 使用環境

- PlatformIO + VSCode
- ボード: `esp32dev`（ESP32-DevKitC-32E）および `esp32s3-devkitc-1`（※対応追加）
- フレームワーク: Arduino

## 📁 ディレクトリ構成（ESP32 S98Player）

```
esp32/
├── src/
│   ├── main.cpp                  # エントリーポイント（✔）
│   ├── S98Parser.cpp             # S98ファイル解析、コマンド抽出処理（✔）
│   ├── S98TimerPlayer.cpp        # タイマ再生処理（✔）
│   ├── SDHelper.cpp              # SDカードファイル列挙・補助ユーティリティ（✔）
│   ├── RhythmManager.cpp         # 複数リズム音源管理（✔）
│   ├── RhythmController.cpp      # RhythmPlayerとハード再生の切替（✔）
│   ├── RhythmPlayer.cpp          # WAVベースソフト再生（✔）
│   ├── RhythmTestRunner.cpp      # リズム再生パターンの検証・テスト実行（✔）
│   ├── WavPlayer.cpp             # WAV 1音再生 + パン制御（✔）
│   ├── AudioWavPlayer.cpp        # 上位互換：同時複数再生対応（✔）
│   ├── AdpcmPlayer.cpp           # ADPCM 1音再生（✔）
│   ├── Ymf288Driver.cpp          # YMF288制御ドライバ（✔）
│   ├── ModeManager.cpp           # モード（スタンドアロン / BLE）切替制御（未実装）
│   ├── BLEReceiver.cpp           # BLE受信処理、NimBLE通信（未実装）
│   ├── OTAUpdate.cpp             # OTAアップデート制御（未実装）
│   ├── CommandParser.cpp         # UART/BLEコマンド受信・解析（未実装）
│   ├── Logger.cpp                # デバッグログ出力（未実装）
│   └── BufferStream.cpp          # コマンド/S98用リングバッファ（未実装）
│
├── include/
│   ├── config.h                  # 共通設定・ピンアサイン（✔）
│   ├── s98.h                     # S98ファイル構造体定義（✔）
│   ├── Ymf288Driver.hpp          # YMF288ドライバ定義（✔）
│   ├── WavPlayer.hpp             # WavPlayer定義（✔）
│   ├── SDHelper.hpp              # SDヘルパ定義（✔）
│   ├── S98Parser.hpp             # S98パーサ定義（✔）
│   ├── S98Player.h               # S98Player定義（✔）
│   ├── S98TimerPlayer.hpp        # S98タイマプレイヤー定義（✔）
│   ├── RhythmPlayer.hpp          # Rhythm再生定義（✔）
│   ├── RhythmManager.hpp         # 複数リズム管理定義（✔）
│   ├── RhythmController.hpp      # 自動切替定義（✔）
│   ├── RhythmTestRunner.hpp      # テスト再生制御定義（✔）
│   ├── AudioWavPlayer.hpp        # 複数同時再生対応定義（✔）
│   ├── AdpcmPlayer.hpp           # ADPCMプレイヤー定義（✔）
│   └── a2p.h                     # ADPCMテーブルヘッダ（✔）
│
├── lib/
│   └── （外部ライブラリまたは自作ライブラリを必要に応じて追加）
│       └── 例：SDfat, ArduinoJson, NimBLE-Arduino など
│
├── platformio.ini              # ビルド設定（✔）
```

## 🚀 ビルドと書き込み

- ビルド: `PlatformIO: Build`
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

## 📍 更新履歴

- **2025-04-16**
  - `WavPlayer` において audio-tools 非依存のI2S再生実装に切り替え
  - `stop()` に `i2s_zero_dma_buffer()` を追加し、再トリガ時のプチノイズを軽減

- **2025-04-14**
  - ESP32-S3-DevKitC-1 への正式対応（`config_s3.h`, `env:esp32s3` 追加）
  - `Ymf288Driver` の `setDataBus()` をピン定義に従い再実装（S3向け）
  - `RhythmPlayer`, `AdpcmPlayer`, `RhythmController` クラスの新規作成
  - S98コマンドパーサ `S98Parser` を実装、mainループと連携

- **2025-04-13**
  - ディレクトリ構成を詳細化、各ファイルの役割を明記
  - 今後追加する予定を具体的なタスクに分割
  - 初版 README.md 作成
