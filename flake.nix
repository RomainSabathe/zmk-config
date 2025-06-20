{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

    zmk-nix = {
      url = "github:lilyinstarlight/zmk-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    zmk-helpers = {
      url = "github:urob/zmk-helpers";
      flake = false;
    };
  };

  outputs = {
    self,
    nixpkgs,
    zmk-nix,
    zmk-helpers,
  }: let
    forAllSystems = nixpkgs.lib.genAttrs (nixpkgs.lib.attrNames zmk-nix.packages);
  in {
    packages = forAllSystems (system: rec {
      default = firmware;

      firmware = zmk-nix.legacyPackages.${system}.buildSplitKeyboard {
        name = "firmware";

        src = nixpkgs.lib.sourceFilesBySuffices self [".board" ".cmake" ".conf" ".defconfig" ".dts" ".dtsi" ".json" ".keymap" ".overlay" ".shield" ".yml" "_defconfig"];

        board = "nice_nano_v2";
        # shield = "settings_reset";
        shield = "sofle_%PART%";

        zephyrDepsHash = "sha256-reAWLQgGuZQiCiN5yGoVlbN6CT5yxl7lEk2/HbvL2x4=";

        preConfigure = ''
          # Include zmk-helpers
          westBuildFlagsArray+=("-DZMK_EXTRA_MODULES=${zmk-helpers}")
        '';

        meta = {
          description = "ZMK firmware";
          license = nixpkgs.lib.licenses.mit;
          platforms = nixpkgs.lib.platforms.all;
        };
      };

      flash = zmk-nix.packages.${system}.flash.override {inherit firmware;};
      update = zmk-nix.packages.${system}.update;
    });

    devShells = forAllSystems (system: {
      default = zmk-nix.devShells.${system}.default;
    });
  };
}
