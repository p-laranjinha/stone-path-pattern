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

#### Nix and C++ interaction

[Raylib development environment in Nix/NixOS · GitHub Gist](https://gist.github.com/gabrieldlima/0bf82c01ce54976ffc6f900639c36f51)

[C - Official NixOS Wiki](https://wiki.nixos.org/wiki/C)

[C++ with Nix in 2023, Part 1: Developer Shells](https://nixcademy.com/posts/cpp-with-nix-in-2023-part-1-shell/)

[Setting up a C++ project environment with nix | Jacek Galowicz](https://galowicz.de/blog/tutorial_nix_cpp_setup/)

[nix-templates/cpp · MordragT/nix-templates](https://github.com/MordragT/nix-templates/tree/322aef20cabb300e05692a444b172ff8c421f524/cpp)

[dev-templates/c-cpp · the-nix-way/dev-templates](https://github.com/the-nix-way/dev-templates/tree/aa21c6a8e9f39866c99be606a7da5d6dafc1db6c/c-cpp)

#### C++

[How to use clangd C/C++ LSP in any project · GitHub Gist](https://gist.github.com/Strus/042a92a00070a943053006bf46912ae9)

[raylib/projects/CMake/CMakeLists.txt · raysan5/raylib](https://github.com/raysan5/raylib/blob/e9caf5a9811a9c1a126398795915b13ef488618e/projects/CMake/CMakeLists.txt)

[SasLuca/raylib-cmake-template](https://github.com/SasLuca/raylib-cmake-template)

### Stone path generation

[Dual Mesh Node - Blender 5.1 Manual](https://docs.blender.org/manual/en/latest/modeling/geometry_nodes/mesh/operations/dual_mesh.html)

[dual_mesh/add_dual_mesh.py · BorisTheBrave/dual_mesh](https://github.com/BorisTheBrave/dual_mesh/blob/2664191f85ea65444fc5e8d18f96d85b9f2735a4/add_dual_mesh.py)

[Procedural stone path with geometry nodes : r/blender](https://www.reddit.com/r/blender/comments/utrfym/procedural_stone_path_with_geometry_nodes/)

[Procedural stone path with geometry nodes, the second one : r/blender](https://www.reddit.com/r/blender/comments/uuvush/procedural_stone_path_with_geometry_nodes_the/)

[How to Make Procedural Cobblestone In Blender! | Geo Nodes Tutorial - YouTube](https://www.youtube.com/watch?v=zO_2gAogtZo)

[Turning ANYTHING into Rocks - Blender Geometry Nodes - YouTube](https://www.youtube.com/watch?v=QA13LPCZ3dM)

[Voronoi diagram - Wikipedia](https://en.wikipedia.org/wiki/Voronoi_diagram)

[newbie: Whats the best method to draw 2d irregular shapes with curved edges? : r/raylib](https://www.reddit.com/r/raylib/comments/165vwux/newbie_whats_the_best_method_to_draw_2d_irregular/)

[Aperiodic tiling - Wikipedia](https://en.wikipedia.org/wiki/Aperiodic_tiling)

[Triangle fan - Wikipedia](https://en.wikipedia.org/wiki/Triangle_fan)

[Triangle strip - Wikipedia](https://en.wikipedia.org/wiki/Triangle_strip)

[Polygon triangulation - Wikipedia](https://en.wikipedia.org/wiki/Polygon_triangulation)

[ivanfratric/polypartition: Tiny Polygon Partitioning and Triangulation Library](https://github.com/ivanfratric/polypartition)

### Other

[Pakz001/Raylib-Examples](https://github.com/Pakz001/Raylib-Examples)

[raylib/examples · raysan5/raylib](https://github.com/raysan5/raylib/tree/master/examples)
