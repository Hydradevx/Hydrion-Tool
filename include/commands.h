#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <string>

void displayPage(const std::vector<std::string>& commands, int page, int pageSize);
void executeCommand(const std::vector<std::string>& commands, int index);

#endif