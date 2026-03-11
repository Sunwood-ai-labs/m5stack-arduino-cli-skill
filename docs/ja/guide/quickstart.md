# クイックスタート

## このスキルが解決すること

このスキルは、Windows に M5Stack が接続されているのに `arduino-cli board list` では
`Unknown` と表示される場面や、コンパイルやアップロードの前に正しいボード設定を
Codex に付与してほしい場面に向いています。

## 推奨プロンプト

```text
Use $m5stack-arduino-cli to diagnose why my M5Core2 shows Unknown in arduino-cli board list on Windows and attach the correct FQBN.
```

## 標準ワークフロー

1. Windows がボードをシリアルデバイスとして認識しているか確認する
2. `arduino-cli` を見つける。`PATH` に無ければ Arduino IDE 同梱版も探す
3. ESP32 コアが設定済みか、インストール済みか確認する
4. Windows と `arduino-cli board list` から現在の COM ポートを特定する
5. `Unknown` は、通信自体も失敗していない限り自動識別の限界とみなす
6. 想定 FQBN とポートをスケッチへ明示的にアタッチする
7. その設定でコンパイルとアップロードを実行する

## 重要コマンド

```powershell
where.exe arduino-cli
Get-CimInstance Win32_SerialPort | Select-Object DeviceID,Name,Description,PNPDeviceID
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
arduino-cli board list
arduino-cli core update-index
arduino-cli core install esp32:esp32
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
arduino-cli compile D:\Prj\M5\VerifyCore2
arduino-cli upload -p COM11 D:\Prj\M5\VerifyCore2
```

## 代表的な前提値

- M5Core2 の FQBN: `esp32:esp32:m5stack_core2`
- よくあるブリッジ名: `USB-Enhanced-SERIAL CH9102`, `Silicon Labs CP210x USB to UART Bridge`
- よく使うライブラリ: `M5Unified`, `M5GFX`

## 次のページ

詳しい判断フローは [診断プレイブック](/ja/guide/diagnosis) を参照してください。
