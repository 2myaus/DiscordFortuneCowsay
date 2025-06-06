{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = {nixpkgs, ...}: let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = [
        pkgs.gcc
        pkgs.clang-tools
        pkgs.pkg-config
        pkgs.gnumake
        pkgs.bear
        pkgs.dpp
        pkgs.cowsay
        pkgs.fortune
      ];
    };
  };
}
