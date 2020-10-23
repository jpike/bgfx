CALL D:\tools\Emscripten\emsdk\emsdk_env.bat

REM em++ -v

IF NOT EXIST "build" MKDIR "build"
IF NOT EXIST "build/emscripten" MKDIR "build/emscripten"

REM -x c++ tells the compiler to treat the input files as C++.
em++ SDL_Example/main_emscripten.cpp build/emscripten/bgfx.bc -I "bgfx/include" -I "SDL_Example/ThirdParty" -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -o build/emscripten/index.html --shell-file SDL_Example/emscripten_shell.html -v

ECHO Done.
