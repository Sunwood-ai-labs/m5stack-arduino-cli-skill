# Repository Structure

## Goal

Use this structure so the repository can keep growing as you add more sample sketches, board-specific helpers, and reusable automation.

## Layout

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

## Conventions

- Put board-specific sketches under `examples/<board>/<sample>/`.
- Keep `sketch.yaml` next to each sample sketch so the board attachment travels with the example.
- Put shared PowerShell helpers under `scripts/common/`.
- Put board-specific setup flows under `scripts/setup/`.
- Put generic compile or upload flows under `scripts/upload/`.
- Keep short top-level wrapper scripts for the commands users will run most often.

## Current Example

- `examples/m5core2/hello/`
- `scripts/setup/m5core2.ps1`
- `scripts/upload/sketch.ps1`
- wrappers:
  - `scripts/setup-m5core2.ps1`
  - `scripts/upload-m5core2.ps1`

## How To Extend

When you add a new board or sample:

1. Add the sketch under the board folder.
2. Add or reuse a setup script under `scripts/setup/`.
3. Reuse `scripts/upload/sketch.ps1` unless the board needs a special upload flow.
4. Document the new sample in the README and docs pages.
