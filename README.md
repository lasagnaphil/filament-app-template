# Filament App Template

This is a project demonstrating how to use filament (https://github.com/google/filament) as a standalone C++ library. 

Although the filament library is packed full of features, it's a bit hard to integrate it inside a standard CMake
project, so I've created this.

## Notes

- The filament library is included in this project as compiled form. Note that I've personally compiled it with Arch 
  Linux (using clang 11.0.1 with libc++ and libc++abi), so this is not the official binary. The reason I've did this 
  was because the official binary had issues with linking with newer versions of glibc.
  (https://github.com/google/filament/issues/2146).
  
- I haven't tested this with other Linux distributions than Arch (Manjaro) yet.

- For a more convenient CMake experience, I've combined all the static libraries to a single fat library
  (``libfilament-full.a``). You can look at the script ``filament_combine.sh`` to tweak this process yourself.
  
- The two included dependencies for making apps easier (filamentapp, filagui), are a bit tweaked from the the ``libs`` 
  folder in the official filament repo. The plan is to further modify this into a more streamlined app-making 
  experience. 
  
## Included dependencies

- filament
- filamentapp
  - libsdl2
  - assimp
  - stb
- filagui
  - imgui
  - implot

## Known issues

- The app doesn't quit properly sometimes (hang while waiting for threads to join...)