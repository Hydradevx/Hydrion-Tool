#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <map>
#include <string>

using namespace std;

extern map<string, function<void()>> commands;
extern map<int, string> descriptions;

void ResgisterCommand(const std::string &name,
                      const std::function<void()> &func);
void LoadCommands();

#endif // COMMANDS_HPP