#pragma once

#include "command.h"
#include <string>

command handleUCI(std::string lastCmd, command c = command{Commands::None});
