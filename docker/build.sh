#!/usr/bin/env bash

set -e

mkdir -p .cache && pushd .cache >/dev/null

[ -f priv.pem ] || \
    nix-store --generate-binary-cache-key nix-serve-1 priv.pem pub.pem

derivation=$(nix build --no-link --print-out-paths .#ambiled)
nix store sign --recursive --key-file priv.pem $derivation

echo "derivation: $derivation"

popd >/dev/null

dockerIp=$(ip -4 a s docker0 | head -2 | tail -1 | awk '{ print $2 }' | xargs dirname)

nix-serve --host $dockerIp --port 54123 &
nixServePid=$! ; echo "nixServePid: $nixServePid"

docker build -t jim3692/ambiled \
    --build-arg "NIX_SUBSTITOR=http://$dockerIp:54123" \
    --build-arg "NIX_SIGNING_KEY=$(cat .cache/pub.pem)" \
    --build-arg "NIX_DERIVATION=$derivation" \
    . || true

kill -HUP $nixServePid
