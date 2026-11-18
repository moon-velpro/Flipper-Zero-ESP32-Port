# Build the minimal Cardputer-Adv Chinese firmware without flashing it.
# Requirements: Windows PowerShell, Python, ESP-IDF 5.4.1 and a recursive git clone.
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    $actualBranch = (& git branch --show-current 2>$null)
    if ($LASTEXITCODE -ne 0 -or $actualBranch.Trim() -ne "feature/cardputer-adv-zh-cn-minimal") {
        throw "Please switch to feature/cardputer-adv-zh-cn-minimal before building."
    }

    Write-Host "Cardputer-Adv minimal Chinese firmware build" -ForegroundColor Cyan
    Write-Host "ESP-IDF must be v5.4.1; original board config is unchanged."
    & python winbuild.py check
    if ($LASTEXITCODE -ne 0) { throw "ESP-IDF check failed." }

    & python winbuild.py build --board cardputer_adv
    if ($LASTEXITCODE -ne 0) { throw "ESP-IDF build failed; do not flash." }

    $bin = Join-Path $root "build_cardputer_adv\furi_esp32.bin"
    $sdk = Join-Path $root "build_cardputer_adv\sdkconfig"
    if (!(Test-Path $bin) -or !(Test-Path $sdk)) {
        throw "Build output or generated sdkconfig missing; do not flash."
    }

    $sdkText = Get-Content $sdk -Raw
    if ($sdkText -notmatch '(?m)^CONFIG_ESPTOOLPY_FLASHSIZE="8MB"$') {
        throw "Expected 8MB flash configuration not found."
    }
    if ($sdkText -match '(?m)^CONFIG_SPIRAM=y$') {
        throw "Unexpected PSRAM configuration; do not flash."
    }

    $size = (Get-Item $bin).Length
    $partitionSize = 0x600000
    if ($size -gt $partitionSize) {
        throw "Firmware size $size exceeds original factory partition $partitionSize."
    }

    $hash = (Get-FileHash -Algorithm SHA256 $bin).Hash
    Write-Host "Build succeeded. Firmware: $bin" -ForegroundColor Green
    Write-Host "Size: $size bytes / $partitionSize bytes"
    Write-Host "SHA256: $hash"
    Write-Host "No flashing has been performed."
    Write-Host "Review bootable-original recovery path before any hardware test."
} finally {
    Pop-Location
}
