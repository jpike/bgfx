CALL D:\tools\Emscripten\emsdk\emsdk_env.bat

REM em++ -v

IF NOT EXIST "build" MKDIR "build"
IF NOT EXIST "build/emscripten" MKDIR "build/emscripten"

REM -x c++ tells the compiler to treat the input files as C++.
em++ -x c++ bgfx.project -I "bx/include" -I "bx/3rdparty" -I "bimg/3rdparty" -I "bimg/3rdparty/astc-codec" -I "bimg/include" -I "bimg/3rdparty/astc-codec/include" -I "bgfx/include" -s ALLOW_MEMORY_GROWTH=1 -o build/emscripten/bgfx.bc

ECHO Done.
