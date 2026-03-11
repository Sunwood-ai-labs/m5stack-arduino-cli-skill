---
layout: home

hero:
  name: "M5Stack Arduino CLI Skill"
  text: "Windows で M5Stack を迷わずセットアップして書き込み、開発する"
  tagline: "`Unknown` 表示から正しい COM ポート確認、FQBN 設定、書き込み、開発フローまでを一貫して扱う Codex スキルの日本語ドキュメントです。"
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
      text: "開発支援"
      link: "/ja/guide/development"
    - theme: alt
      text: "English"
      link: "/"

features:
  - title: "`Unknown` を正しく扱う"
    details: "COM ポート検出とボード自動識別を分けて考え、正常な機器をドライバ不良と誤認しないようにします。"
  - title: "Windows 前提で切り分ける"
    details: "`Get-PnpDevice`、`arduino-cli`、`esptool` を順に使い、CH9102 や CP210x ベースの M5Stack を確実に扱います。"
  - title: "セットアップから開発まで支援する"
    details: "最初の書き込みだけで終わらず、PowerShell 補助スクリプトとサンプルスケッチで継続的な CLI 開発につなげます。"
---

## このサイトで扱うこと

このドキュメントは README を補完するブラウズ用ガイドです。`Unknown` からセットアップ完了までの流れに加え、サンプルスケッチを使った開発の始め方までをまとめています。

## 主な導線

- 最短で動かしたいなら [クイックスタート](/ja/guide/quickstart)
- 切り分けを順に追いたいなら [診断プレイブック](/ja/guide/diagnosis)
- サンプルや補助スクリプトを使って開発まで進めたいなら [開発支援](/ja/guide/development)
- 英語版を見るなら [English docs](/)
