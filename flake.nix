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

      raygui-style = builtins.fetchurl {
        url = "https://github.com/raysan5/raygui/raw/refs/heads/master/styles/genesis/style_genesis.rgs";
        sha256 = "sha256:1zbh24hlzy8cnqg8qm9b4gjp8zxf7dpgwrqw76jh27386dbab8h3";
      };

      raylib-cpp = pkgs.fetchFromGitHub {
        owner = "RobLoach";
        repo = "raylib-cpp";
        rev = "v5.5.1";
        sha256 = "sha256-+ieOrXetRE9gPhfnnkiI0YUOD/m+pOJmzjcQJPOTkLo=";
      };

      commonPackages = with pkgs; [
        raylib
        raygui

        # https://nixos.org/manual/nixpkgs/stable/#cmake
        # Automatically changes the configure phase to use cmake.
        cmake
        ninja_1_11

        libGL
        # X11 dependencies
        xorg.libX11
        xorg.libX11.dev
        xorg.libXcursor
        xorg.libXi
        xorg.libXinerama
        xorg.libXrandr
        # Audio dependencies
        alsa-lib
      ];

      envVars = {
        RAYLIB_CPP_PATH = "${raylib-cpp}";
      };
    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation (
        {
          pname = "stone-path-pattern";
          version = "0.0.0";
          src = ./.;
          nativeBuildInputs =
            with pkgs;
            [
            ]
            ++ commonPackages;
          buildInputs = [ ];
          cmakeFlags = [ ];
          postInstall = ''
            # Found $PWD by first figuring out that cmake's intermediary files are on /build
            #  (https://nixos.org/manual/nixpkgs/stable/#cmake-variables-controlling)
            #  then reading /build/env-vars and finding the environment variable with the final
            #  place for the intermediary files.
            cp -r $PWD/compile_commands.json $out
            # Supposedly the following solution/repo could also be used and be more versatile but
            #  too much effort in this case:
            #  https://discourse.nixos.org/t/using-nix-infrastructure-to-reliably-generate-compile-commands-json/21961
            #  https://github.com/danielbarter/mini_compile_commands
          '';
        }
        // envVars
      );

      devShells.${system}.default = pkgs.mkShell (
        rec {
          packages =
            with pkgs;
            [
            ]
            ++ commonPackages;
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath packages;
          shellHook = ''
            ln -sf result/compile_commands.json ./compile_commands.json
            mkdir -p ./assets
            ln -sf ${raygui-style} ./assets/style.rgs

            echo
            echo 'Run "nix build" to generate the "compile_commands.json" used by the LSP (clang).'
            echo 'Run "nix run" to run the project.'
            echo
          '';
        }
        // envVars
      );
    };
}
