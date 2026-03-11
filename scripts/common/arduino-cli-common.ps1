Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-ArduinoCliPath {
    $command = Get-Command arduino-cli -ErrorAction SilentlyContinue
    if ($command) {
        return $command.Source
    }

    $bundled = Join-Path $env:LocalAppData "Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
    if (Test-Path $bundled) {
        return $bundled
    }

    throw "arduino-cli was not found on PATH and the Arduino IDE bundled binary was not found."
}

function Ensure-Esp32PackageUrl {
    param(
        [Parameter(Mandatory = $true)]
        [string]$CliPath
    )

    $configPath = Join-Path $env:USERPROFILE ".arduinoIDE\arduino-cli.yaml"
    if (-not (Test-Path $configPath)) {
        & $CliPath config init | Out-Null
    }

    $url = "https://espressif.github.io/arduino-esp32/package_esp32_index.json"
    $text = if (Test-Path $configPath) { Get-Content $configPath -Raw } else { "" }
    if ($text -match [regex]::Escape($url)) {
        return
    }

    if ($text -match "additional_urls:\s*") {
        $updated = $text -replace "(additional_urls:\s*(?:\r?\n))", "`$1        - $url`r`n"
    } elseif ($text -match "board_manager:\s*(?:\r?\n)") {
        $updated = $text -replace "(board_manager:\s*(?:\r?\n))", "`$1    additional_urls:`r`n        - $url`r`n"
    } else {
        $updated = "board_manager:`r`n    additional_urls:`r`n        - $url`r`n$text"
    }

    Set-Content -Path $configPath -Value $updated -Encoding utf8
}
