if ($args[0] -eq "clean") {
    if (Test-Path "build-web") {
        Remove-Item -Recurse -Force "build-web"
        Write-Host "Removed build-web directory."
    } else {
        Write-Host "build-web directory does not exist."
    }
    exit 0
}

emcmake cmake -S . -B build-web
cmake --build build-web

if ($args[0] -eq "run") {
    emrun build-web/BUnknown.html
}
