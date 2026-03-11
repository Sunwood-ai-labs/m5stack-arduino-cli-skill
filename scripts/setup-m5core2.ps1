[CmdletBinding(DefaultParameterSetName = "Default")]
param(
    [string]$SketchPath,
    [string]$Port = "COM11",
    [string]$Fqbn = "esp32:esp32:m5stack_core2",
    [switch]$SkipLibraries,
    [switch]$WhatIf
)

$scriptPath = Join-Path $PSScriptRoot "setup\m5core2.ps1"
$invokeParams = @{
    Port = $Port
    Fqbn = $Fqbn
}
if ($SketchPath) {
    $invokeParams.SketchPath = $SketchPath
}
if ($SkipLibraries) {
    $invokeParams.SkipLibraries = $true
}
if ($WhatIf) {
    $invokeParams.WhatIf = $true
}

& $scriptPath @invokeParams
