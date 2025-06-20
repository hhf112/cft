#include "./colors.hpp"

#define arrs3 std::array<std::string, 3>
#define vcs std::vector<std::string>


// Display stetements
#define DISP_BUILD arrs3{BRIGHT_YELLOW_FG "building ..." COLOR_END, "build exited normally" COLOR_END "\n", "❌ build failed!" COLOR_END "\n"}
#define DISP_LOAD arrs3{BRIGHT_YELLOW_FG "Running ...", "binary exited normally" COLOR_END "\n", "❌ failed to load binary!"  COLOR_END "\n"}



//Frames
#define FR_EXPLODE  vcs {"|", "/", "_", "😖", "💥"}
#define FR_BOXX vcs {"◷", "◶", "◵", "◴"};


// Interfaces:
// void spinnerBool(std::array<std::string, 3>& display, std::vector<std::string>& frames, int& done) {

