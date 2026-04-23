# Procedural stone path pattern generation

This repo contains my attempts at creating a generator for a decent looking stone path pattern.

I'll be using Nix for setup, C++ as the language, and [raylib](https://www.raylib.com) for the
graphics things. All of these were chosen just because I'm interested in them and I want to gain
some experience with them.

For my configuration, I'll be using a [flake.nix](./flake.nix) for both the development environment
(`nix develop`) and for the building (`nix build`). Nix can also be used for running with `nix run`.

I'm already used to using Nix for a development environment but I've never used it for building and
I'm still unsure if it's better than running regular build tools inside the development environment.

## Links

### Development environment and building configuration

[nix develop - Nix 2.28.6 Reference Manual](https://nix.dev/manual/nix/2.28/command-ref/new-cli/nix3-develop.html)

[nix build - Nix 2.28.6 Reference Manual](https://nix.dev/manual/nix/2.28/command-ref/new-cli/nix3-build.html)

[nix run - Nix 2.28.6 Reference Manual](https://nix.dev/manual/nix/2.28/command-ref/new-cli/nix3-run.html)

[Development Environments on NixOS | NixOS & Flakes Book](https://nixos-and-flakes.thiscute.world/development/intro)

[Build a package using Nix - Zero to Nix from Determinate Systems](https://zero-to-nix.com/start/nix-build/)

#### Raylib

[Raylib development environment in Nix/NixOS · GitHub Gist](https://gist.github.com/gabrieldlima/0bf82c01ce54976ffc6f900639c36f51)

#### Nix and C++ interaction

[C - Official NixOS Wiki](https://wiki.nixos.org/wiki/C)

[C++ with Nix in 2023, Part 1: Developer Shells](https://nixcademy.com/posts/cpp-with-nix-in-2023-part-1-shell/)

[Setting up a C++ project environment with nix | Jacek Galowicz](https://galowicz.de/blog/tutorial_nix_cpp_setup/)

[nix-templates/cpp · MordragT/nix-templates](https://github.com/MordragT/nix-templates/tree/322aef20cabb300e05692a444b172ff8c421f524/cpp)

[dev-templates/c-cpp · the-nix-way/dev-templates](https://github.com/the-nix-way/dev-templates/tree/aa21c6a8e9f39866c99be606a7da5d6dafc1db6c/c-cpp)
