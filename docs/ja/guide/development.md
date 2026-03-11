# 開発支援

## 目的

このガイドは、最初のセットアップだけでなく、サンプルスケッチ、補助スクリプト、ライブラリ導入、継続的な Arduino CLI 開発フローまで支援したいときに使います。

## 同梱サンプルスケッチ

標準のスタータースケッチはここです。

```text
examples/m5core2/hello/hello.ino
```

このスケッチでは次を確認できます。

- `M5Unified` の初期化
- M5Core2 画面への文字表示
- シリアル起動メッセージ
- ボタン A を使った簡単な状態切り替え

## 補助スクリプト

### セットアップ補助

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

このスクリプトは次を行います。

- `arduino-cli` を探す
- ESP32 のパッケージ URL を確認する
- ESP32 コアを導入する
- `M5GFX` と `M5Unified` を導入する
- スケッチにボードとポートを `attach` する

### 書き込み補助

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

このスクリプトはコンパイルとアップロードをまとめて行います。

## 開発中に `arduino-cli` を直接使う例

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
```

まだ `attach` していないなら、毎回 FQBN を明示します。

```powershell
& $cli compile --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
& $cli upload -p COM11 --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
```

## 推奨する開発ループ

1. 新しいスケッチでは最初にセットアップ補助を流す
2. スケッチを編集する
3. こまめにコンパイルする
4. ビルドが通ったら書き込む
5. `sketch.yaml` をスケッチと一緒に残す

## ユーザーに残すべきもの

Codex が開発支援をするなら、最後に次を残します。

- 動くスケッチのパス
- `attach` 済みのボード情報
- 実際に使った CLI コマンド
- 必要だったライブラリ導入コマンド

こうしておくと、次回以降も IDE を開かずに再現できます。

## 拡張するとき

今後さらにボードやサンプルを増やすときは、[リポジトリ構成](/ja/guide/structure) を基準に追加してください。
