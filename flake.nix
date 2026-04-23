{
  description = "C++ development environment and build config.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        pname = "stone-path";
        version = "0.0.0";
        src = ./.;
        nativeBuildInputs = with pkgs; [
          # https://nixos.org/manual/nixpkgs/stable/#cmake
          # Automatically changes the configure phase to use cmake.
          cmake
        ];
        buildInputs = [ ];
        cmakeFlags = [ ];
      };

      devShells.${system}.default = pkgs.mkShell rec {
        packages = with pkgs; [
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath packages;
      };
    };
}
