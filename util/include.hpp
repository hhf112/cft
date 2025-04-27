#define utch reinterpret_cast<const char*>

#include "colors.hpp"
#include "enums.hpp"
#include "animations.hpp"
#include "FileIterator.hpp"


#include <string>
#include <array>
#include <vector>
std::string lowerCase(std::string& s);

void spinnerBool(std::array<std::string, 3>& display,
             std::vector<std::string>& frames, int& done);


