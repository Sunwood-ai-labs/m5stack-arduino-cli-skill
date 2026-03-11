<div align="center">
  <img src="./assets/m5stack-arduino-cli-icon.svg" alt="M5Stack Arduino CLI icon" width="140" height="140">
  <h1>M5Stack Arduino CLI Skill</h1>
  <p><strong>Windows 上で M5Stack を Arduino CLI からセットアップ・診断・運用するための Codex 向けスキルです。</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-Windows_10%2B-0A7E8C?style=flat-square" alt="Windows badge">
    <img src="https://img.shields.io/badge/Tool-Arduino_CLI-1B5E20?style=flat-square" alt="Arduino CLI badge">
    <img src="https://img.shields.io/badge/Target-M5Stack_%2F_ESP32-37474F?style=flat-square" alt="M5Stack ESP32 badge">
    <img src="https://img.shields.io/badge/License-MIT-E65100?style=flat-square" alt="MIT license badge">
  </p>
  <p><a href="./README.md">English</a> | <a href="./README.ja.md">日本語</a></p>
</div>

## ドキュメント

- 閲覧用サイト: [sunwood-ai-labs.github.io/m5stack-arduino-cli-skill](https://sunwood-ai-labs.github.io/m5stack-arduino-cli-skill/)
- 英語版クイックスタート: [`docs/guide/quickstart.md`](./docs/guide/quickstart.md)
- 日本語版クイックスタート: [`docs/ja/guide/quickstart.md`](./docs/ja/guide/quickstart.md)

## 概要

このリポジトリは、Windows 上で `arduino-cli` を使って M5Stack を扱う際の定番トラブルを
Codex が迷わずさばけるようにするための再利用可能なスキルです。特に、
`arduino-cli board list` で `Unknown` と表示されるケースや、USB シリアルブリッジが
汎用デバイスとして見えているケース、適切な ESP32 の FQBN がまだスケッチに紐づいて
いないケースを主対象にしています。

このスキルは「まずデバイスの健全性を証明し、そのうえで正しい COM ポートと FQBN を
結びつける」ことを重視しています。無駄なドライバ再インストールを避けながら、
コンパイルやアップロード成功まで最短で戻すための流れを用意しています。

## このスキルでできること

- `arduino-cli board list` が COM ポートを見つけていても `Unknown` になる理由を説明する
- `CH9102` や `CP210x` のような Windows 上の汎用 USB-シリアルブリッジを前提に診断する
- `arduino-cli`、`esptool`、Windows のデバイス情報を正しい順番で使うよう Codex を誘導する
- M5Core2 の既定 FQBN や `M5GFX`、`M5Unified` といった代表ライブラリを案内する

## クイックスタート

Codex に対して、スキル名を明示して依頼します。

```text
Use $m5stack-arduino-cli to diagnose why my M5Core2 shows Unknown in arduino-cli board list on Windows and attach the correct FQBN.
```

スキルは次の流れで作業を進めます。

1. Windows が対象デバイスをシリアルポートとして認識しているか確認する
2. `arduino-cli` を見つける。`PATH` に無ければ Arduino IDE 同梱版も探す
3. `esp32:esp32` のボードサポートが入っているか確認する
4. Windows ツールと `arduino-cli board list` で正しい COM ポートを特定する
5. `Unknown` は、Windows 側や `esptool` 側も失敗していない限り「自動識別の不足」とみなす
6. コンパイルやアップロードの前に、正しい FQBN とポートをスケッチへ紐づける

## 収録ファイル

| パス | 役割 |
| --- | --- |
| [`SKILL.md`](./SKILL.md) | スキル本体の指示、ルール、標準ワークフロー |
| [`agents/openai.yaml`](./agents/openai.yaml) | 表示名や既定プロンプトなどのエージェント向けメタデータ |
| [`docs/`](./docs/) | ブラウズ可能な英日 VitePress ドキュメント |
| [`references/windows-setup-and-diagnosis.md`](./references/windows-setup-and-diagnosis.md) | Windows コマンド、セットアップ手順、`Unknown` 診断の詳細 |
| [`references/m5-board-notes.md`](./references/m5-board-notes.md) | M5 系ボード特有の事情、ブリッジチップの背景、FQBN 既定値 |

## ワークフローの要点

### `Unknown` を正しく扱う

このスキルは、次の 2 つを明確に分けて扱います。

- ポート検出
- ボード識別

M5Stack 系では、Windows から見える USB 識別子がボード固有ではなく汎用 USB-シリアル
ブリッジ側の情報であることがよくあります。そのため、COM ポートとしては正常でも、
Arduino CLI がボード名を自動判定できず `Unknown` になることがあります。

### 推測より検証を優先する

スキルは闇雲なドライバ再インストールを勧めません。代わりに、Codex に次を確認させます。

- Windows のデバイス状態
- シリアルポートの可視性
- ESP32 パッケージの導入状況
- 必要に応じた `esptool` の疎通確認
- 想定ボードとポートのアタッチ状況

## リポジトリ構成

```text
.
|-- SKILL.md
|-- README.md
|-- README.ja.md
|-- agents/
|   `-- openai.yaml
|-- assets/
|   `-- m5stack-arduino-cli-icon.svg
|-- docs/
|   |-- .vitepress/
|   |-- guide/
|   `-- ja/
`-- references/
    |-- m5-board-notes.md
    `-- windows-setup-and-diagnosis.md
```

## 使いどころ

次のような場面で、このリポジトリをスキルとして組み込むと効果的です。

- Windows 上で M5Stack の初期セットアップをしたい
- `arduino-cli board list` の `Unknown` を正しく説明したい
- `esp32:esp32:m5stack_core2` などの FQBN を適切に設定したい
- スケッチのコンパイルとアップロードを試行錯誤少なめで進めたい

## ライセンス

このリポジトリは [MIT License](./LICENSE) で提供します。
