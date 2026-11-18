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
    $idfVersionOutput = & python winbuild.py check 2>&1
    $idfCheckExitCode = $LASTEXITCODE
    $idfVersionText = $idfVersionOutput -join "`n"
    Write-Host $idfVersionText
    if ($idfCheckExitCode -ne 0) { throw "ESP-IDF check failed." }
    if ($idfVersionText -notmatch 'ESP-IDF v5\.4\.1(?:\s|$)') {
        throw "Expected exactly ESP-IDF v5.4.1; check the active ESP_IDF_DIR."
    }

    & python winbuild.py build --board cardputer_adv
    if ($LASTEXITCODE -ne 0) { throw "ESP-IDF build failed; do not flash." }

    $buildDir = Join-Path $root "build_cardputer_adv"
    $bin = Join-Path $buildDir "furi_esp32.bin"
    $sdk = Join-Path $buildDir "sdkconfig"
    foreach ($relative in @(
        "furi_esp32.bin",
        "sdkconfig",
        "furi_esp32.map",
        "bootloader\bootloader.bin",
        "partition_table\partition-table.bin"
    )) {
        if (!(Test-Path (Join-Path $buildDir $relative) -PathType Leaf)) {
            throw "Missing build artifact: $relative; do not flash."
        }
    }

    $sdkText = Get-Content $sdk -Raw
    if ($sdkText -notmatch '(?m)^CONFIG_IDF_TARGET="esp32s3"$') {
        throw "Expected ESP32-S3 target not found."
    }
    if ($sdkText -notmatch '(?m)^CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions_singleapp_large\.csv"$') {
        throw "Original factory partition table is not selected; do not flash."
    }
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
