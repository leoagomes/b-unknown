emcmake cmake -S . -B build-web
cmake --build build-web

if ($args[0] -eq "run") {
    Set-Location build-web
    emrun BUnknown.html
}
