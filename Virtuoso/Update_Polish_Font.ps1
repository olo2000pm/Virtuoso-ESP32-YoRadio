param([switch]$Pause)

$ErrorActionPreference = "Stop"

$source = Join-Path $PSScriptRoot "PL\\glcdfont.c"
if (-not (Test-Path $source)) {
  Write-Host "Nie znaleziono pliku źródłowego: $source" -ForegroundColor Red
  if ($Pause) { Read-Host "Naciśnij Enter, aby zakończyć" }
  exit 1
}

$candidates = @(
  (Join-Path $env:USERPROFILE "OneDrive\\Dokumenty\\Arduino\\libraries\\Adafruit_GFX_Library\\glcdfont.c"),
  (Join-Path $env:USERPROFILE "Documents\\Arduino\\libraries\\Adafruit_GFX_Library\\glcdfont.c")
)

$destination = $candidates | Where-Object { Test-Path (Split-Path $_ -Parent) } | Select-Object -First 1
if (-not $destination) {
  Write-Host "Nie znaleziono biblioteki Adafruit_GFX_Library w folderze Arduino\\libraries." -ForegroundColor Red
  if ($Pause) { Read-Host "Naciśnij Enter, aby zakończyć" }
  exit 2
}

try {
  Copy-Item $source $destination -Force
  $srcHash = (Get-FileHash $source -Algorithm SHA256).Hash
  $dstHash = (Get-FileHash $destination -Algorithm SHA256).Hash
  if ($srcHash -ne $dstHash) {
    throw "Hash pliku docelowego nie zgadza się z plikiem źródłowym."
  }
  Write-Host "Zaktualizowano glcdfont.c w: $destination" -ForegroundColor Green
} catch {
  Write-Host "Błąd aktualizacji glcdfont.c: $($_.Exception.Message)" -ForegroundColor Red
  if ($Pause) { Read-Host "Naciśnij Enter, aby zakończyć" }
  exit 3
}

if ($Pause) { Read-Host "Naciśnij Enter, aby zakończyć" }
