# Windows Setup And Diagnosis

## Goal

Use Arduino CLI on Windows to set up an M5Stack board, identify its COM port, attach the intended board profile, and upload sketches reliably.

## Checklist

1. Find `arduino-cli`.
2. Confirm the serial device exists in Windows.
3. Install the ESP32 core.
4. Install M5 libraries if needed.
5. Detect the port.
6. Verify the device with `esptool` if board auto-detection is `Unknown`.
7. Attach board + port to the sketch.
8. Compile and upload.

## Typical Windows commands

### Find Arduino CLI

```powershell
where.exe arduino-cli
```

If that fails, try the Arduino IDE bundled binary:

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli version
```

### Inspect ports

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID,Name,Description,PNPDeviceID
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
& $cli board list
& $cli board list --format json
```

### Configure ESP32 support

If the ESP32 package URL is missing, add:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Then run:

```powershell
& $cli core update-index
& $cli core install esp32:esp32
```

### Install common M5 libraries

```powershell
& $cli lib install M5GFX
& $cli lib install M5Unified
```

### Verify an ESP32 serial device with esptool

Use this when `board list` shows `Unknown` but Windows exposes a COM port:

```powershell
& "C:\Users\<User>\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe" --chip auto --port COM11 chip-id
```

Healthy output proves the port is an ESP32-class device even if Arduino CLI cannot map it to a board name.

### Attach board and port to a sketch

```powershell
& $cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
```

This writes `sketch.yaml` and makes later compile/upload commands simpler.

### Compile and upload

```powershell
& $cli compile D:\Prj\M5\VerifyCore2
& $cli upload -p COM11 D:\Prj\M5\VerifyCore2
```

If the sketch is already attached, `compile` can use the stored FQBN automatically.

## Interpret `Unknown` correctly

`arduino-cli board list` has two separate jobs:

1. Discover the port.
2. Map that port to a known board profile.

It is common for step 1 to succeed while step 2 fails. That yields a valid COM port with `Unknown` in the board column.

Treat `Unknown` as a missing board identity match when:

- Windows shows the port with status `OK`
- the port opens
- `esptool` can talk to the device
- `arduino-cli upload` succeeds when FQBN is set manually

Treat `Unknown` as a real connection or driver problem only when one of those checks also fails.

## Driver guidance

Do not jump to driver reinstall first.

If Windows reports a healthy `Ports` device and upload works, the USB-serial driver is already good enough. The remaining issue is board auto-identification, not transport.
