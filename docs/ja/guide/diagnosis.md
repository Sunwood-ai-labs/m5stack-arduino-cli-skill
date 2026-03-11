# 診断プレイブック

## まず押さえる解釈

`arduino-cli board list` は次の 2 つを別々に行います。

1. ポートを見つける
2. ボード型番を推定する

M5Stack は Windows から見ると汎用の USB シリアルブリッジとして見えることが多く、その場合はポート検出が正常でもボード名は `Unknown` のままになります。

## 切り分けの流れ

### 1. Windows は正常なポートとして見ているか

```powershell
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
```

対象デバイスが存在し、`Status` が `OK` なら、USB 輸送層は概ね正常です。

### 2. `arduino-cli` はポートを見つけられるか

```powershell
arduino-cli board list
arduino-cli board list --format json
```

COM ポートは見えているのにボード名だけ出ない場合は、まず自動識別の不足として扱います。

### 3. ESP32 コアは入っているか

```powershell
arduino-cli core update-index
arduino-cli core install esp32:esp32
```

ESP32 コアがないと、`attach`、コンパイル、アップロードは揃いません。

### 4. `esptool` で ESP32 と会話できるか

```powershell
& "C:\Users\<User>\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe" --chip auto --port COM11 chip-id
```

ここで ESP32 として応答すれば、`Unknown` でも実機との通信は取れています。

### 5. 想定ボードを明示的に `attach` する

```powershell
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
```

これで汎用 USB シリアルブリッジと、実際に使いたいボード設定の間を橋渡しできます。

## 最初にやらないこと

- Windows が正常にポートを出しているのに、いきなりドライバ再インストールを勧めない
- `Unknown` だけを根拠にケーブル不良やドライバ不足と断定しない
- ボードが分かっているのに `attach` を飛ばさない

## 開発フェーズへ進む

診断が安定したら、[開発支援](/ja/guide/development) に進み、サンプルスケッチと補助スクリプトを使って再現可能な CLI フローへ移ります。
