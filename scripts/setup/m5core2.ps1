[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [string]$SketchPath,
    [string]$Port = "COM11",
    [string]$Fqbn = "esp32:esp32:m5stack_core2",
    [switch]$SkipLibraries
)

. (Join-Path $PSScriptRoot "..\common\arduino-cli-common.ps1")

$cli = Get-ArduinoCliPath
Write-Host "Using arduino-cli:" $cli

Ensure-Esp32PackageUrl -CliPath $cli

if ($PSCmdlet.ShouldProcess("Arduino CLI", "Update indexes and install esp32:esp32")) {
    & $cli core update-index
    & $cli core install esp32:esp32
}

if (-not $SkipLibraries -and $PSCmdlet.ShouldProcess("Arduino CLI", "Install M5GFX and M5Unified")) {
    & $cli lib install M5GFX
    & $cli lib install M5Unified
}

if ($SketchPath) {
    $resolvedSketchPath = (Resolve-Path $SketchPath).Path
    if ($PSCmdlet.ShouldProcess($resolvedSketchPath, "Attach $Fqbn on $Port")) {
        & $cli board attach -p $Port -b $Fqbn $resolvedSketchPath
    }
}

& $cli board list
