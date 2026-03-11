---
layout: home

hero:
  name: "M5Stack Arduino CLI Skill"
  text: "Set up, flash, and develop with M5Stack on Windows without guesswork"
  tagline: "A bilingual documentation hub for the Codex skill that turns `Unknown` board reports into a repeatable setup, upload, and development workflow."
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
      text: "Development Support"
      link: "/guide/development"
    - theme: alt
      text: "日本語"
      link: "/ja/"

features:
  - title: "Treat `Unknown` correctly"
    details: "Separate COM port discovery from board identification so healthy boards are not mistaken for driver failures."
  - title: "Stay Windows-aware"
    details: "Use `Get-PnpDevice`, `arduino-cli`, and `esptool` in the right order for CH9102 and CP210x based M5Stack devices."
  - title: "Support setup through development"
    details: "Move from first attach and upload to repeatable CLI-based development with helper scripts and sample sketches."
---

## What This Site Covers

This documentation complements the repository `README` files with a browsable guide for skill users and maintainers. It focuses on the repeatable path from `Unknown` to a working setup, then extends that path into a practical development workflow.

## Main Paths

- Start with [Quick Start](/guide/quickstart) when you want the shortest path to a working setup.
- Open [Diagnosis Playbook](/guide/diagnosis) when you need the full decision flow.
- Use [Development Support](/guide/development) when you want reusable scripts, sample sketches, and a repeatable CLI workflow.
- Switch to [Japanese docs](/ja/) if you want the same structure in Japanese.
