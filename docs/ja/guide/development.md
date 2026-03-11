# 開発支援

## 目的

このガイドは、最初のセットアップだけでなく、サンプルスケッチ、補助スクリプト、ライブラリ導入、継続的な Arduino CLI 開発フローまで支援したいときに使います。

## 同梱サンプルスケッチ

標準のスタータースケッチはここです。

```text
examples/m5core2/hello/hello.ino
```

SD カード確認用サンプル:

```text
examples/m5core2/sd_text_write/sd_text_write.ino
```

- microSD のマウント
- テキストファイルの作成と追記
- 画面とシリアルでの読み戻し
- 空き容量と総容量の表示

アニメーション猫サンプル:

```text
examples/m5core2/pixel_pet/pixel_pet.ino
```

- 透過アニメーション WebP を M5Core2 用の RGB565 フレーム列へ変換
- 元の縦横比を保ったまま正方形キャンバスへ収める
- 透過を保ってそのままアニメーション再生する
- 最後から最初へ戻る部分に補間フレームを入れてループを自然にする
- ボタン A / B / C でなでる、食べる、寝る / 起きる反応を切り替える
- 元動画に動きがある前提で、追加の上下移動を入れない

公開用に残しているプレビュー画像:

![猫アニメのプレビュー](/examples/pixel_pet/generated_cat_animation_preview.png)

公開用に残しているスプライトシート:

![猫アニメのスプライトシート](/examples/pixel_pet/generated_cat_animation_sheet.png)

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

SD カード確認用:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

アニメーション猫サンプル用:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

`uv` でアニメーションヘッダと確認用画像を再生成する例:

```powershell
uv run .\scripts\generate_sprite_animation.py --input 'D:\path\to\cat.webp' --output .\examples\m5core2\pixel_pet\generated_cat_animation.h --preview .\docs\public\examples\pixel_pet\generated_cat_animation_preview.png --sheet .\docs\public\examples\pixel_pet\generated_cat_animation_sheet.png --size 112 --frame-step 4 --sheet-columns 8 --loop-blend-frames 3
```

この変換ではフルフレームを使ったまま正方形キャンバスに収め、ループ端に補間フレームを追加しつつ、確認用のプレビュー画像とスプライトシートも同時に出力できます。

このスクリプトはコンパイルとアップロードをまとめて行います。

## 開発中に `arduino-cli` を直接使う例

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
```

SD カード確認用:

```powershell
& $cli compile .\examples\m5core2\sd_text_write
& $cli upload -p COM11 .\examples\m5core2\sd_text_write
```

アニメーション猫サンプル用:

```powershell
& $cli compile .\examples\m5core2\pixel_pet
& $cli upload -p COM11 .\examples\m5core2\pixel_pet
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
