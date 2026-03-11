# Diagnosis Playbook

## The Core Interpretation

`arduino-cli board list` does two separate things:

1. Discover the port.
2. Try to identify the board model.

Many M5Stack devices expose only a generic USB-serial bridge to Windows. That means port discovery can be healthy while board identification stays `Unknown`.

## Decision Flow

### 1. Does Windows see a healthy port?

```powershell
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
```

If the target device is present and `Status` is `OK`, the USB transport is usually healthy.

### 2. Can `arduino-cli` find the port?

```powershell
arduino-cli board list
arduino-cli board list --format json
```

If the COM port appears but the board name is missing, treat that as a board identity gap, not an automatic driver failure.

### 3. Is the ESP32 core installed?

```powershell
arduino-cli core update-index
arduino-cli core install esp32:esp32
```

Without the ESP32 core, compile, attach, and upload steps will all be incomplete.

### 4. Can `esptool` talk to the chip?

```powershell
& "C:\Users\<User>\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe" --chip auto --port COM11 chip-id
```

If `esptool` can identify the chip, you have strong proof that the port is real even if the board model remains `Unknown`.

### 5. Attach the intended FQBN explicitly

```powershell
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
```

This bridges the gap between a generic serial bridge and the actual board you intend to use.

## What Not To Do First

- Do not recommend random driver reinstalls if Windows already shows the port as healthy.
- Do not treat `Unknown` by itself as proof of a broken cable or missing driver.
- Do not skip the attach step when the project already has a clear intended board.

## Move On To Development

Once the diagnosis is stable, switch to [Development Support](/guide/development) and use the bundled sample sketch and helper scripts instead of repeating one-off commands by hand.
