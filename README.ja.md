<div align="center">
  <img src="./assets/m5stack-arduino-cli-icon.svg" alt="M5Stack Arduino CLI icon" width="140" height="140">
  <h1>M5Stack Arduino CLI Skill</h1>
  <p><strong>Windows 上で M5Stack を Arduino CLI だけでセットアップ、書き込み、診断、開発支援するための Codex スキルです。</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-Windows_10%2B-0A7E8C?style=flat-square" alt="Windows badge">
    <img src="https://img.shields.io/badge/Tool-Arduino_CLI-1B5E20?style=flat-square" alt="Arduino CLI badge">
    <img src="https://img.shields.io/badge/Target-M5Stack_%2F_ESP32-37474F?style=flat-square" alt="M5Stack ESP32 badge">
    <img src="https://img.shields.io/badge/License-MIT-E65100?style=flat-square" alt="MIT license badge">
  </p>
  <p><a href="./README.md">English</a> | <a href="./README.ja.md">日本語</a></p>
</div>

## ドキュメント

- ドキュメントサイト: [Sunwood-ai-labs.github.io/m5stack-arduino-cli-skill](https://sunwood-ai-labs.github.io/m5stack-arduino-cli-skill/)
- 英語クイックスタート: [`docs/guide/quickstart.md`](./docs/guide/quickstart.md)
- 日本語クイックスタート: [`docs/ja/guide/quickstart.md`](./docs/ja/guide/quickstart.md)

## 概要

このリポジトリは、Windows 上で `arduino-cli` を使って M5Stack を扱うための再利用可能な Codex スキルです。特に次のような場面を狙っています。

- `arduino-cli board list` で `Unknown` と出る
- USB シリアルブリッジが `CH9102` や `CP210x` として見えている
- 正しい ESP32 の FQBN がまだスケッチに紐づいていない
- セットアップから書き込み、開発の初期化まで一気に進めたい

このスキルは単発のトラブルシュートだけでなく、日常的な開発支援も前提にしています。COM ポートの切り分け、ESP32 コア導入、M5 系ライブラリの導入、`board attach`、コンパイル、アップロードまでを CLI ベースで再現可能な流れにまとめています。

## このスキルでできること

- `Unknown` の意味を正しく切り分ける
- Windows 側で COM ポートとドライバ状態を確認する
- `arduino-cli` と Arduino IDE 同梱版 CLI の両方に対応する
- `esp32:esp32:m5stack_core2` などの FQBN を明示的に付与する
- `M5Unified` と `M5GFX` を導入する
- サンプルスケッチを使って最初の書き込み確認を行う
- CLI だけで M5Stack 開発フローを回せる形に整える

## サンプルスクリプトとサンプルスケッチ

セットアップと書き込みには、同梱の PowerShell スクリプトを使えます。

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

最小の確認用スケッチはここにあります。

```text
examples/m5core2/hello/hello.ino
```

SD カード確認用サンプル:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

```text
examples/m5core2/sd_text_write/sd_text_write.ino
```

- microSD のマウント
- テキストファイルの作成と追記
- 画面とシリアルでの読み戻し
- 空き容量と総容量の表示

このスケッチは `M5Unified` を使って画面表示とシリアル出力を行い、ボタン A を押したときの状態変化も確認できます。

透過アニメーション WebP を使った猫アニメの例:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

```text
examples/m5core2/pixel_pet/pixel_pet.ino
```

- 透過アニメーション WebP を M5Core2 用の RGB565 フレーム列へ変換
- 元の縦横比を保ったまま正方形キャンバスに収める
- プレビュー画像とスプライトシートを同時に生成して確認できる
- 元アニメーションをそのまま再生し、ボタン A / B / C で反応だけを切り替える

サンプリングしたフレームから生成したプレビュー:

![猫アニメのプレビュー](./docs/public/examples/pixel_pet/generated_cat_animation_preview.png)

サンプリングしたアニメーション全体のスプライトシート:

![猫アニメのスプライトシート](./docs/public/examples/pixel_pet/generated_cat_animation_sheet.png)

変換アセットを `uv` で再生成する例:

```powershell
uv run .\scripts\generate_sprite_animation.py --input 'D:\path\to\cat.webp' --output .\examples\m5core2\pixel_pet\generated_cat_animation.h --preview .\docs\public\examples\pixel_pet\generated_cat_animation_preview.png --sheet .\docs\public\examples\pixel_pet\generated_cat_animation_sheet.png --size 112 --frame-step 4 --sheet-columns 8
```

## クイックスタート

Codex に明示的にスキルを使わせるなら、たとえばこう依頼できます。

```text
Use $m5stack-arduino-cli to set up my M5Core2 on Windows, attach the correct FQBN, and upload a sample sketch from Arduino CLI.
```

スキルは次の順で進めます。

1. Windows がシリアルデバイスとして認識しているか確認する
2. `arduino-cli` を見つける
3. ESP32 コアを導入する
4. 正しい COM ポートを見つける
5. `Unknown` を自動識別の限界として扱うべきか判定する
6. `M5GFX` と `M5Unified` を必要に応じて導入する
7. スケッチに FQBN とポートを `board attach` する
8. コンパイルとアップロードを行う

## 主なファイル

| パス | 役割 |
| --- | --- |
| [`SKILL.md`](./SKILL.md) | スキル本体。発火条件、実行ルール、参照先 |
| [`agents/openai.yaml`](./agents/openai.yaml) | UI 向けメタデータ |
| [`scripts/setup-m5core2.ps1`](./scripts/setup-m5core2.ps1) | セットアップ、ライブラリ導入、`board attach` を補助 |
| [`scripts/upload-m5core2.ps1`](./scripts/upload-m5core2.ps1) | コンパイルとアップロードを補助 |
| [`scripts/generate_sprite_animation.py`](./scripts/generate_sprite_animation.py) | 透過アニメーション WebP を RGB565 フレーム列と確認用画像へ変換 |
| [`examples/m5core2/hello/hello.ino`](./examples/m5core2/hello/hello.ino) | M5Core2 向けサンプルスケッチ |
| [`examples/m5core2/sd_text_write/sd_text_write.ino`](./examples/m5core2/sd_text_write/sd_text_write.ino) | SD カード書き込み確認用サンプルスケッチ |
| [`examples/m5core2/pixel_pet/pixel_pet.ino`](./examples/m5core2/pixel_pet/pixel_pet.ino) | 透過 WebP ベースの猫アニメ用サンプルスケッチ |
| [`docs/`](./docs/) | 英日対応の VitePress ドキュメント |
| [`references/windows-setup-and-diagnosis.md`](./references/windows-setup-and-diagnosis.md) | Windows / Arduino CLI のセットアップと切り分け |
| [`references/m5-board-notes.md`](./references/m5-board-notes.md) | M5 固有のボードメモ |
| [`references/development-and-examples.md`](./references/development-and-examples.md) | 開発支援とサンプル利用の流れ |

## 今後の拡張方針

今後サンプルを増やしやすいように、構成を次のルールへ寄せました。

- ボード別サンプルは `examples/<board>/<sample>/`
- 共通 PowerShell 処理は `scripts/common/`
- ボード別セットアップは `scripts/setup/`
- 汎用アップロードは `scripts/upload/`
- ユーザーが直接使う短い入口はトップレベルのラッパー

## こんなときに使う

- Windows で M5Stack を CLI 中心で扱いたい
- `arduino-cli board list` の `Unknown` を正しく説明したい
- M5Core2 用の CLI セットアップを自動化したい
- 書き込み確認用のサンプルスケッチが欲しい
- 開発のたびに同じコマンドを組み直したくない

## ライセンス

このリポジトリは [MIT License](./LICENSE) で公開しています。
