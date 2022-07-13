#pragma once

#include "../Command/Command.hpp"

class Cli {
public:
    Cli &command(std::string name, std::string description, std::string example, std::vector<Flag> commandFlag, function action);

    void parse(int argc, char **argv);
    static void printHelp(std::map<std::string, Command> &commands);
    static std::string checkIsRequiredFlag(std::vector<std::string> &inputFlags, std::map<std::string, Flag> &commandFlags);
    static bool flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &flag);

private:
    std::map<std::string, Command> commands = {std::make_pair("help", Command("help", "Выведет справочную информацию и подскажет всевозможные команды", "", {},
                                                                              [this](flagsType &parsedFlags) { printHelp(this->commands); }))};
};
