# 診断プレイブック

## まず押さえる解釈

`arduino-cli board list` には、別々の 2 段階があります。

1. ポートを見つける
2. ボード種別を推定する

M5Stack では、Windows から見えるのが汎用 USB-シリアルブリッジだけということがよくあります。
そのため、ポート検出は正常でもボード名推定だけが `Unknown` のまま残ることがあります。

## 判断フロー

### 1. Windows が正常なポートとして見ているか

まずデバイス状態を確認します。

```powershell
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
```

対象デバイスが存在し、`Status` が `OK` なら、USB 通信経路は概ね健全です。

### 2. `arduino-cli` がポートを見つけられるか

```powershell
arduino-cli board list
arduino-cli board list --format json
```

COM ポートが見えていてボード名だけ空なら、まずは識別不足と考えます。即座に
ドライバ不良と決めつけないのが重要です。

### 3. ESP32 コアが入っているか

```powershell
arduino-cli core update-index
arduino-cli core install esp32:esp32
```

ESP32 コアが無いと、attach、compile、upload の判断が途中で止まります。

### 4. `esptool` でチップに話せるか

```powershell
& "C:\Users\<User>\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe" --chip auto --port COM11 chip-id
```

ここで ESP32 系チップが応答すれば、ボード名が `Unknown` でもポートの実体はかなり確度高く
証明できます。

### 5. 想定 FQBN を明示的にアタッチする

```powershell
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
```

この手順で、汎用シリアルブリッジとして見えている状態と、実際に使いたいボード設定を接続します。

## 最初にやらない方がよいこと

- Windows が正常表示なのに、いきなりドライバを再インストールしない
- `Unknown` だけを根拠に、ケーブル不良や接続断を断定しない
- ボードが明らかなのに attach を飛ばして先へ進まない

## 関連リファレンス

- [Windows Setup And Diagnosis](https://github.com/Sunwood-ai-labs/m5stack-arduino-cli-skill/blob/main/references/windows-setup-and-diagnosis.md)
- [M5 Board Notes](https://github.com/Sunwood-ai-labs/m5stack-arduino-cli-skill/blob/main/references/m5-board-notes.md)
