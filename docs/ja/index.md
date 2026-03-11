---
layout: home

hero:
  name: "M5Stack Arduino CLI Skill"
  text: "Windows 上の M5Stack と Arduino CLI を推測なしで診断する"
  tagline: "ボードが `Unknown` と表示されても、正しいポート確認と FQBN 設定までたどり着けるようにする Codex スキルの日本語ドキュメントです。"
  image:
    src: /icon.svg
    alt: "M5Stack Arduino CLI Skill icon"
  actions:
    - theme: brand
      text: "クイックスタート"
      link: "/ja/guide/quickstart"
    - theme: alt
      text: "診断プレイブック"
      link: "/ja/guide/diagnosis"
    - theme: alt
      text: "English"
      link: "/"

features:
  - title: "`Unknown` を正しく解釈する"
    details: "COM ポート検出とボード識別を分けて考え、正常な接続を誤って故障扱いしないようにします。"
  - title: "Windows 前提で診断する"
    details: "CH9102 や CP210x を含む M5Stack デバイス向けに、`Get-PnpDevice`、`arduino-cli`、`esptool` の使い順を整理しています。"
  - title: "正しい FQBN に着地する"
    details: "汎用 USB-シリアルブリッジとして見える状態から、`esp32:esp32:m5stack_core2` のような明示的なボード設定へつなげます。"
---

## このサイトで扱う内容

このドキュメントは、リポジトリの `README` を補完する browsable なガイドです。主眼は、
「`Unknown` と出る」状態から「正しいポートと FQBN でコンパイル・アップロードできる」
状態までの判断を再現可能にすることです。

## 主要導線

- 最短で使い始めるなら [クイックスタート](/ja/guide/quickstart)
- 判断の流れをしっかり追うなら [診断プレイブック](/ja/guide/diagnosis)
- 英語版を見たいときは [English docs](/)
