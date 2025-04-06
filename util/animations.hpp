#include "./colors.hpp"
#define DISP_BUILD std::array<std::string, 3>{BRIGHT_YELLOW_FG "building ...", "✅ build finished successfully" COLOR_END "\n", "❌ build failed!" COLOR_END "\n"}
#define DISP_LOAD std::array<std::string, 3>{BRIGHT_YELLOW_FG "Running ...", "✅ binary exited with exited code " COLOR_END, "❌ failed to load binary!"  COLOR_END "\n"}


#define FR_EXPLODE  std::vector<std::string>{"|", "/", "_", "😖", "💥"}
#define FR_BOXX std::vector<std::string>{"◷", "◶", "◵", "◴"};
