# ESP32サブプロジェクト（YMF288Duo）

このフォルダは、ESP32を用いたYMF288Duo制御マイコンのコードを格納します。

## 🔧 使用環境

- PlatformIO + VSCode
- ボード: `esp32dev`（ESP32-DevKitC-32E）
- フレームワーク: Arduino

## 📁 ディレクトリ構成

```
esp32/
├── src/         # メインコード
├── lib/         # 自作ライブラリ（例：S98Player, YMF288Driver）
├── include/     # ヘッダファイル（必要に応じて）
├── platformio.ini
└── README.md
```

## 🚀 ビルドと書き込み

- ビルド: `PlatformIO: Build`（▷）
- 書き込み: `PlatformIO: Upload`
- モニタ: `PlatformIO: Monitor`

## ✅ 今後追加する予定

- SDカードからのS98再生
- YMF288制御用I2S出力
- iPhone / PCからのリアルタイムコントロール対応
