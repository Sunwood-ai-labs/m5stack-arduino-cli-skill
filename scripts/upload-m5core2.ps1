[CmdletBinding(DefaultParameterSetName = "Default")]
param(
    [Parameter(Mandatory = $true)]
    [string]$SketchPath,
    [string]$Port = "COM11",
    [string]$Fqbn = "esp32:esp32:m5stack_core2",
    [switch]$WhatIf
)

$scriptPath = Join-Path $PSScriptRoot "upload\sketch.ps1"
$invokeParams = @{
    SketchPath = $SketchPath
    Port = $Port
    Fqbn = $Fqbn
}
if ($WhatIf) {
    $invokeParams.WhatIf = $true
}

& $scriptPath @invokeParams
