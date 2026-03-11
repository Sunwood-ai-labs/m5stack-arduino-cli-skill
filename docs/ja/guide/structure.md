# リポジトリ構成

## 目的

今後サンプルスケッチや補助スクリプトを増やしても整理が崩れないように、このリポジトリでは役割ごとに配置を分けます。

## 基本レイアウト

```text
examples/
  <board>/
    <sample>/
      <sample>.ino
      sketch.yaml

scripts/
  common/
    *.ps1
  setup/
    <board>.ps1
  upload/
    *.ps1
  <friendly-wrapper>.ps1
```

## ルール

- ボードごとのサンプルは `examples/<board>/<sample>/` に置く
- `sketch.yaml` はサンプルの横に置いて、ボード設定を一緒に持ち運べるようにする
- 共通処理は `scripts/common/` に置く
- ボード別セットアップは `scripts/setup/` に置く
- 汎用のコンパイルやアップロード処理は `scripts/upload/` に置く
- ユーザーが直接叩く入口は、分かりやすいトップレベルラッパーとして残す

## 現在の例

- `examples/m5core2/hello/`
- `scripts/setup/m5core2.ps1`
- `scripts/upload/sketch.ps1`
- 入口ラッパー:
  - `scripts/setup-m5core2.ps1`
  - `scripts/upload-m5core2.ps1`

## 増やし方

新しいボードやサンプルを足すときは次の順で進めます。

1. サンプルスケッチを該当ボード配下へ追加する
2. 必要なら `scripts/setup/` にボード別スクリプトを追加する
3. アップロードはまず `scripts/upload/sketch.ps1` の再利用を検討する
4. README と docs に新しいサンプルを追記する
