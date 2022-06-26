#!/bin/bash
# Bootstraps vcpkg dependencies

set -euo pipefail

#git submodule add https://github.com/microsoft/vcpkg ./vcpkg
# Synchronize the module
git submodule sync
git submodule update --init

