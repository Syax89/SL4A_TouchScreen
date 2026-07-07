# Triage EVERY published "Surface*" driver package (pnputil /enum-drivers) without
# decompiling anything: for each one, find its real .inf in the DriverStore and report
# (a) Class/ClassGuid from [Version] -- Class=Extension means "just AddReg, no code",
#     exactly what SurfaceDigitizerHidSpiExtnPackage.inf turned out to be (see
#     05_check_extn_inf.cmd / GROUND_TRUTH.md 15.14) -- skip those, no binary to decompile;
# (b) any line mentioning MSHW0231 (our device), MSHW/ACPI/PNP hardware IDs generally, or
#     "SPB"/"GPIO"/"ACPI" filter mechanisms;
# (c) whether the package folder actually contains a .sys/.dll (if Class != Extension but
#     there's no binary either, still nothing to decompile).
#
# Goal: decompile only the packages that (1) are real code AND (2) plausibly touch our
# device or a generic ACPI/SPB/GPIO mechanism -- not all ~35 blindly.
#
# Run as Administrator:  powershell -ExecutionPolicy Bypass -File 06_triage_all_surface_drivers.ps1 > triage_result.txt

$names = @(
  "surfacebattery.inf","surfacebutton.inf","surfacecfuoverhid.inf","surfacedock2fwupdate.inf",
  "surfacehidfriendlynames.inf","surfacehidminidriver.inf","surfacehotplug.inf",
  "surfaceintegrationdriver.inf","surfacekip.inf","surfacelightsensor.inf","surfacepd.inf",
  "surfacepen0953cfuoverblelcextnpackage.inf","surfacepen0953firmwareupdate.inf",
  "surfacepen217fwupdate.inf","surfacepen217integration.inf","surfacepenblelcaddradaptationdriver.inf",
  "surfacepencfuoverblelc.inf","surfacepowertrackercore.inf","surfacesam.inf",
  "surfaceserialhubdriver.inf","surfaceservicenulldriver.inf","surfacesmf.inf","surfacesmfclient.inf",
  "surfacesmfdisplayclient.inf","surfacesmf_13in.inf","surfacestoragefwupdate.inf",
  "surfacesystemmanagementframeworkdriver.inf","surfacesystemtelemetrydriver.inf","surfacetcon13.inf",
  "surfacetcon13b.inf","surfacetcon13c.inf","surfacetcon13d.inf","surfacetcon15.inf","surfacetcon15b.inf",
  "surfacetcondriver.inf","surfacethermalpolicy.inf","surfacethunderbolt4dockfwupdate.inf",
  "surfacetimealarmacpifilter.inf","surfacetouchfw.inf","surfacetouchpenprocessorupdate.inf",
  "surfacetrackpad.inf","surfaceucmucsihidclient.inf","surfaceuefi.inf","surfaceusb4dockfwupdate.inf",
  "surfacevirtualfunctionenum.inf","surface_tpm.inf","surfaceoempanel.inf","surfacepowermeter.inf"
)

$root = "C:\Windows\System32\DriverStore\FileRepository"

foreach ($name in $names) {
    $dir = Get-ChildItem $root -Directory -Filter "$name*" -ErrorAction SilentlyContinue | Select-Object -First 1
    Write-Host "=================================================================="
    Write-Host "PACKAGE: $name"
    if (-not $dir) {
        Write-Host "  NOT FOUND in DriverStore (not installed on this machine)"
        continue
    }
    Write-Host "  DriverStore dir: $($dir.Name)"

    $realInf = Get-ChildItem $dir.FullName -Filter "*.inf" -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $realInf) {
        Write-Host "  (no .inf found inside?!)"
        continue
    }

    $content = Get-Content $realInf.FullName -ErrorAction SilentlyContinue
    $classLine = $content | Select-String -Pattern '^\s*Class\s*=' | Select-Object -First 1
    $classGuidLine = $content | Select-String -Pattern '^\s*ClassGuid\s*=' | Select-Object -First 1
    Write-Host "  $classLine"
    Write-Host "  $classGuidLine"

    $hwids = $content | Select-String -Pattern 'MSHW|ACPI\\\\|PNP0|SPB|GPIO' | Select-Object -Unique
    if ($hwids) {
        Write-Host "  Hardware-ID / bus lines:"
        $hwids | ForEach-Object { Write-Host "    $_" }
    } else {
        Write-Host "  (no MSHW/ACPI/PNP0/SPB/GPIO lines found)"
    }

    $bins = Get-ChildItem $dir.FullName -Include *.sys,*.dll -Recurse -ErrorAction SilentlyContinue
    if ($bins) {
        Write-Host "  Binaries in package: $($bins.Name -join ', ')"
    } else {
        Write-Host "  Binaries in package: NONE (AddReg/co-installer package only)"
    }
    Write-Host ""
}

Write-Host "=================================================================="
Write-Host "DONE. Look for: Class != Extension AND has a .sys/.dll AND mentions"
Write-Host "MSHW0231/SPB/GPIO -- those are the only candidates worth decompiling."
