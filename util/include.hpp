#define utch reinterpret_cast<const char*>

#include "colors.hpp"
#include "enums.hpp"
#include "animations.hpp"


#include <string>
#include <array>
#include <vector>
std::string lowerCase(std::string& s);

//display and frames are to be passed as rvalues!
void spinnerBool(std::array<std::string, 3>& display,
             std::vector<std::string>& frames, int& done);


