#include "commands.hpp"
#include "commands/webhook.hpp"
#include <functional>
#include <map>
#include <string>

using namespace std;

map<string, function<void()>> commands;
map<int, string> descriptions;

int i = 1;

void RegisterCommand(const string &name, const function<void()> &func) {
  commands[name] = func;
  descriptions[i] = name;
  i++;
}

void LoadCommands() {
  RegisterCommand("Store Webhook", storeWebhook);
  RegisterCommand("Spam Webhook", spamWebhook);
  RegisterCommand("Clear Webhooks", clearWebhooks);
  RegisterCommand("List Webhooks", listWebhooks);
  RegisterCommand("Delete Webhook", deleteWebhook);
}
