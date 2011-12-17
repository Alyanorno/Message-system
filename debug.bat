cl /EHsc /Zi /MD main.cpp logistics.cpp logics.cpp physics.cpp graphics.cpp SDL/lib/SDLmain.lib SDL/lib/SDL.lib opengl32.lib /link/SUBSYSTEM:WINDOWS
devenv /debugexe main.exe
