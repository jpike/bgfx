#include <cstdint>
#include <cstdio>
#include <exception>
#include <bgfx/bgfx.h>
#include <emscripten.h>
#include <SDL/SDL.h>
#undef main

SDL_Window* g_window = nullptr;
//SDL_GLContext g_gl_context = nullptr;

void EmscriptenMainLoopIteration()
{
    uint32_t elapsed_time_in_ms_until_previous_frame = SDL_GetTicks();
    bool quit = false;

    // EVENT HANDLING.
    SDL_Event application_event;
    while (SDL_PollEvent(&application_event) != 0)
    {
        if (SDL_QUIT == application_event.type)
        {
            quit = true;
        }
    }

    // RENDER THE SCREEN.
    // Submit empty primitive to clear screen.
    constexpr bgfx::ViewId DEFAULT_VIEW_ID = 0;
    bgfx::touch(DEFAULT_VIEW_ID);

#define EXAMPLE_00_HELLO_WORLD 1
#if EXAMPLE_00_HELLO_WORLD
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
    const bgfx::Stats* stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
        stats->width,
        stats->height,
        stats->textWidth,
        stats->textHeight);
#endif

    bgfx::frame();

    // ENFORCE A FRAME RATE.
#if MANUAL_FRAME_RATE
    constexpr float FRAMES_PER_SECOND = 60.0f;
    constexpr float SECONDS_PER_FRAME = 1.0f / FRAMES_PER_SECOND;
    constexpr float MILLISECONDS_PER_SECOND = 1000.0f;
    constexpr uint32_t MILLISECONDS_PER_FRAME = static_cast<uint32_t>(SECONDS_PER_FRAME * MILLISECONDS_PER_SECOND);
    uint32_t current_total_milliseconds = SDL_GetTicks();
    uint32_t elapsed_time_in_ms_for_current_frame = (current_total_milliseconds - elapsed_time_in_ms_until_previous_frame);
    if (elapsed_time_in_ms_for_current_frame < MILLISECONDS_PER_FRAME)
    {
        uint32_t remaining_time_in_ms_for_frame = MILLISECONDS_PER_FRAME - elapsed_time_in_ms_for_current_frame;
        SDL_Delay(remaining_time_in_ms_for_frame);
    }
    current_total_milliseconds = SDL_GetTicks();
    //uint32_t frame_time_in_ms = current_total_milliseconds - elapsed_time_in_ms_until_previous_frame;
    //std::printf("Frame Time (ms): %u\n", frame_time_in_ms);
#endif
    elapsed_time_in_ms_until_previous_frame = SDL_GetTicks();
}

int main()
{
    std::printf("Starting program...\n");

    // CATCH ANY EXCEPTIONS.
    // A lot of things like SDL functions can easily fail.  To easily catch generic errors, everything's wrapped
    // in a try block, although it's not clear yet if that will sufficiently catch all errors.
    // If no errors occurred, success is assumed.
    int exit_code = EXIT_SUCCESS;
    try
    {
        // INITIALIZE SDL.
        constexpr uint32_t SDL_SUBSYSTEMS = (SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO);
        constexpr int SDL_SUCCESS_RETURN_CODE = 0;
        SDL_Init(SDL_SUBSYSTEMS);

        // CREATE THE WINDOW.
        constexpr int SCREEN_WIDTH_IN_PIXELS = 700;
        constexpr int SCREEN_HEIGHT_IN_PIXELS = 400;
        constexpr SDL_WindowFlags WINDOW_SETTINGS = static_cast<SDL_WindowFlags>(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        g_window = SDL_CreateWindow(
            "Bgfx Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH_IN_PIXELS,
            SCREEN_HEIGHT_IN_PIXELS,
            WINDOW_SETTINGS);

        //std::printf("After SDL_CreateWindow\n");
        //g_gl_context = SDL_GL_CreateContext(g_window);
        //std::printf("After SDL_GL_CreateContext\n");
        //SDL_GL_MakeCurrent(g_window, g_gl_context);
        //std::printf("After SDL_GL_MakeCurrent\n");

        //constexpr int VERTICAL_SYNC = 1;
        //SDL_GL_SetSwapInterval(VERTICAL_SYNC);

        std::printf("bgfx::init() before\n");
#if 1
        static char dummy_window_selector[] = "#canvas";
        bgfx::Init bgfx_initialization_parameters = {};
        bgfx_initialization_parameters.platformData.nwh = dummy_window_selector;
        //bgfx_initialization_parameters.platformData.context = g_gl_context;
        bgfx_initialization_parameters.resolution.width = SCREEN_WIDTH_IN_PIXELS;
        bgfx_initialization_parameters.resolution.height = SCREEN_HEIGHT_IN_PIXELS;
        bgfx_initialization_parameters.resolution.reset = BGFX_RESET_VSYNC;
        bool result = bgfx::init(bgfx_initialization_parameters);
#endif
        std::printf("bgfx::init() result = %d\n", result);

#if DISPLAY_DEBUG_STATS
        bgfx::setDebug(BGFX_DEBUG_PROFILER | BGFX_DEBUG_STATS | BGFX_DEBUG_TEXT);
#else
        bgfx::setDebug(BGFX_DEBUG_PROFILER | BGFX_DEBUG_TEXT);
#endif

        constexpr bgfx::ViewId DEFAULT_VIEW_ID = 0;
        bgfx::setViewRect(DEFAULT_VIEW_ID, 0, 0, SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS);
        constexpr uint32_t BACKGROUND_COLOR = 0x443355FF;
        constexpr float DEPTH_CLEAR_VALUE = 1.0f;
        constexpr uint8_t STENCIL_CLEAR_VALUE = 0;
        bgfx::setViewClear(DEFAULT_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, BACKGROUND_COLOR, DEPTH_CLEAR_VALUE, STENCIL_CLEAR_VALUE);

        // Submit empty primitve for rendering.
        bgfx::touch(DEFAULT_VIEW_ID);
    }
    catch (const std::exception& exception)
    {
        std::fprintf(stderr, "Standard exception: %s\nSDL error: %s\n", exception.what(), SDL_GetError());
        exit_code = EXIT_FAILURE;
    }
    catch (...)
    {
        std::fprintf(stderr, "Unknown exception.\nSDL error: %s\n", SDL_GetError());
        exit_code = EXIT_FAILURE;
    }

    constexpr int LET_BROWSER_CONTROL_FRAME_RATE = 0;
    constexpr int SIMULATE_INFINITE_LOOP = 1;
    emscripten_set_main_loop(EmscriptenMainLoopIteration, LET_BROWSER_CONTROL_FRAME_RATE, SIMULATE_INFINITE_LOOP);

    bgfx::shutdown();
    if (g_window)
    {
        SDL_DestroyWindow(g_window);
    }
    SDL_Quit();

    std::printf("Ending program...\n");
    return exit_code;
}
