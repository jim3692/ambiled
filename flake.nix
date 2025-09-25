{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  outputs =
    { nixpkgs, ... }:
    let
      supportedSystems = [
        "aarch64-linux"
        "x86_64-linux"
      ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      getPkgs = system: nixpkgs.legacyPackages.${system};

      getAmbiledBin =
        pkgs:
        pkgs.runCommand "ambiled"
          {
            buildInputs = [ pkgs.imagemagick ];
          }
          ''
            mkdir -p $out/bin
            cd ${./src}

            export PATH="$PATH:${pkgs.gcc}/bin:${pkgs.imagemagick}/bin"
            cc \
                `MagickWand-config --cflags --cppflags` \
                -O2 -o $out/bin/ambiled common.c analyze-image.c main.c \
                `MagickWand-config --ldflags --libs`
          '';

      getShellDeps =
        pkgs: with pkgs; [
          gcc
          imagemagick
        ];

      getShell =
        {
          pkgs,
          extras ? [ ],
        }:
        pkgs.mkShell { buildInputs = (getShellDeps pkgs) ++ extras; };

      runInDockerXfce =
        pkgs:
        pkgs.writeShellScriptBin "run-in-docker-xfce" ''
          docker run --rm -it \
            -v "/nix/store:/nix/store:ro" \
            -v "$HOME:/host:ro" \
            -v "${getShell { inherit pkgs; }}:/nix-shell:ro" \
            -e "NIX_CFLAGS_COMPILE=$NIX_CFLAGS_COMPILE" \
            -e "NIX_LDFLAGS=$NIX_CFLAGS_COMPILE" \
            -p 6901:6901 \
            accetto/ubuntu-vnc-xfce-g3:24.04
        '';
    in
    {
      packages = forAllSystems (system: rec {
        default = ambiled;
        ambiled = getAmbiledBin (getPkgs system);
      });

      devShells = forAllSystems (system: {
        default =
          let
            pkgs = getPkgs system;
          in
          getShell {
            inherit pkgs;
            extras = [ (runInDockerXfce pkgs) ];
          };
      });
    };
}
