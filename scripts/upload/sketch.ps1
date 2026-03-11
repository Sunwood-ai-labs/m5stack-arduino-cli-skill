[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [Parameter(Mandatory = $true)]
    [string]$SketchPath,
    [string]$Port,
    [string]$Fqbn
)

. (Join-Path $PSScriptRoot "..\common\arduino-cli-common.ps1")

$cli = Get-ArduinoCliPath
$resolvedSketchPath = (Resolve-Path $SketchPath).Path

$compileArgs = @("compile")
if ($Fqbn) {
    $compileArgs += @("--fqbn", $Fqbn)
}
$compileArgs += $resolvedSketchPath

if ($PSCmdlet.ShouldProcess($resolvedSketchPath, "Compile sketch")) {
    & $cli @compileArgs
}

$uploadArgs = @("upload")
if ($Port) {
    $uploadArgs += @("-p", $Port)
}
if ($Fqbn) {
    $uploadArgs += @("--fqbn", $Fqbn)
}
$uploadArgs += $resolvedSketchPath

if ($PSCmdlet.ShouldProcess($resolvedSketchPath, "Upload sketch")) {
    & $cli @uploadArgs
}
