{
  description = "QMK firmware build for user/id75 with callum_mods keymap";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        pname = "qmk-firmware-id75";
        version = "unstable";

        src = pkgs.fetchFromGitHub {
          owner = "qmk";
          repo = "qmk_firmware";
          rev = "master";
          sha256 = pkgs.lib.fakeSha256; # Replace with actual hash
          fetchSubmodules = true;
        };

        nativeBuildInputs = with pkgs; [
          qmk
          python3
          gcc-arm-embedded
          dfu-util
          avrdude
        ];

        buildPhase = ''
          export HOME=$TMPDIR
          qmk setup -y
          qmk compile -kb user/id75 -km callum_mods
        '';

        installPhase = ''
          mkdir -p $out
          find . -name "*.hex" -o -name "*.bin" -o -name "*.uf2" | while read file; do
            cp "$file" $out/
          done
        '';
      };

      devShells.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          qmk
          python3
          gcc-arm-embedded
          dfu-util
          avrdude
        ];

        shellHook = ''
          echo "QMK development environment loaded"
          echo "Run: qmk compile -kb user/id75 -km callum_mods"
        '';
      };
    });
}
