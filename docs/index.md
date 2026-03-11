---
layout: home

hero:
  name: "M5Stack Arduino CLI Skill"
  text: "Diagnose Windows + Arduino CLI workflows for M5Stack without guesswork"
  tagline: "A bilingual documentation hub for the Codex skill that turns 'Unknown' board reports into a repeatable setup and upload flow."
  image:
    src: /icon.svg
    alt: M5Stack Arduino CLI Skill icon
  actions:
    - theme: brand
      text: "Quick Start"
      link: "/guide/quickstart"
    - theme: alt
      text: "Diagnosis Playbook"
      link: "/guide/diagnosis"
    - theme: alt
      text: "日本語"
      link: "/ja/"

features:
  - title: "Treat `Unknown` correctly"
    details: "Separate COM port discovery from board identification so healthy boards are not mistaken for driver failures."
  - title: "Stay Windows-aware"
    details: "Use `Get-PnpDevice`, `arduino-cli`, and `esptool` in the right order for CH9102 and CP210x based M5Stack devices."
  - title: "Attach the right FQBN"
    details: "Move from a detected serial bridge to an explicit ESP32 board profile such as `esp32:esp32:m5stack_core2`."
---

## What This Site Covers

This documentation complements the repository's `README` files with a browsable guide for
skill users and maintainers. It focuses on the repeatable path from "board shows as
`Unknown`" to "the sketch compiles and uploads with the correct port and FQBN."

## Main Paths

- Start with [Quick Start](/guide/quickstart) when you want the shortest path to a working setup.
- Open [Diagnosis Playbook](/guide/diagnosis) when you need the full decision flow.
- Switch to [Japanese docs](/ja/) if you want the same structure in Japanese.
