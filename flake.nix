{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  outputs =
    { nixpkgs, ... }:
    let
      inherit (nixpkgs) lib;

      supportedSystems = [
        "aarch64-linux"
        "x86_64-linux"
      ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      getPkgs = system: nixpkgs.legacyPackages.${system};

      getShellDeps =
        pkgs: with pkgs; [
          imagemagick
        ];

      getAmbiledBin =
        pkgs:
        pkgs.stdenv.mkDerivation {
          name = "ambiled";
          src = ./src;
          buildInputs = getShellDeps pkgs;
          installPhase = ''
            mkdir -p $out/bin
            cp ambiled $out/bin/
          '';
        };

      getShell =
        {
          pkgs,
          extras ? [ ],
        }:
        pkgs.mkShell rec {
          buildInputs = (getShellDeps pkgs) ++ extras ++ [ pkgs.bear ];
          CPATH = lib.makeSearchPathOutput "dev" "include" buildInputs;

          shellHook = ''
            gen_compile_commands() {
              bear -- make -C src rebuild
            }

            gen_compile_commands
          '';
        };

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
