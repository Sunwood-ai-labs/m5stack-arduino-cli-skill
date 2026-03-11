# クイックスタート

## このスキルが解決すること

Windows に M5Stack を接続したのに `arduino-cli board list` で `Unknown` と出るときや、開発を始める前に正しいボード設定・書き込みフローを CLI だけで整えたいときに使います。

## 推奨プロンプト

```text
Use $m5stack-arduino-cli to set up my M5Core2 on Windows, attach the correct FQBN, and upload a sample sketch from Arduino CLI.
```

## 基本フロー

1. Windows がシリアルデバイスとして認識しているか確認する
2. `arduino-cli` を探す
3. ESP32 コアを導入する
4. 現在の COM ポートを確認する
5. `Unknown` を自動識別の限界として扱うべきか判定する
6. `M5GFX` と `M5Unified` を必要に応じて導入する
7. スケッチに FQBN とポートを `board attach` する
8. コンパイルして書き込む

## 重要コマンド

```powershell
where.exe arduino-cli
Get-CimInstance Win32_SerialPort | Select-Object DeviceID,Name,Description,PNPDeviceID
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
arduino-cli board list
arduino-cli core update-index
arduino-cli core install esp32:esp32
arduino-cli lib install M5GFX
arduino-cli lib install M5Unified
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
arduino-cli compile D:\Prj\M5\VerifyCore2
arduino-cli upload -p COM11 D:\Prj\M5\VerifyCore2
```

## 同梱ヘルパー

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2-hello -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2-hello -Port COM11
```

## 既定値

- M5Core2 の FQBN: `esp32:esp32:m5stack_core2`
- よくある USB ブリッジ名: `USB-Enhanced-SERIAL CH9102`, `Silicon Labs CP210x USB to UART Bridge`
- よく使うライブラリ: `M5Unified`, `M5GFX`
- 既定のサンプルスケッチ: `examples/m5core2/hello/hello.ino`

## 次に読む場所

詳しい切り分けは [診断プレイブック](/ja/guide/diagnosis)、サンプルスケッチや補助スクリプトを使った継続開発は [開発支援](/ja/guide/development) を見てください。
