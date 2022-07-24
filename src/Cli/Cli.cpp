#include "Cli.hpp"

cli::Cli &cli::Cli::command(const std::string &name, const std::string &description, const std::string &example, const std::vector<Flag> &commandFlag, const CommandCallback &action) {
    Command cmd = Command(name, description, example, commandFlag, action);
    commands.insert(std::make_pair(name, cmd));
    return *this;
}

std::string cli::Cli::checkIsRequiredFlag(std::map<std::string, Flag> &inputFlags, std::map<std::string, Flag> &commandFlags) {
    for (auto &commandFlag : commandFlags) {
        auto flag = commandFlag.second;
        if (flag.isRequired) {
            if (!(inputFlags.count(flag.name) || inputFlags.count(flag.shortName))) {
                return "\033[31mERROR: Не введён обязательный флаг -> --" + flag.name + " OR -" + flag.shortName;
            }
        }
    }
    return "";
}

std::string cli::Cli::flagInCommand(std::map<std::string, Flag> &commandFlags, std::string &inputFlagName) {
    for (auto &commandFlag : commandFlags) {
        auto flag = commandFlag.second;
        if (flag.name == inputFlagName || flag.shortName == inputFlagName) {
            return flag.name;
        }
    }
    return "";
}

void cli::Cli::parse(int argc, char **argv) {
    std::string cmd;
    std::string message;
    for (int i = 1; i < argc; ++i) {
        cmd = argv[i];
        if (commands.count(cmd)) {
            if (cmd == "help" && argc > 2) {
                std::vector<std::string> enteredCommands;
                for (int j = i + 1; j < argc; ++j) {
                    cmd = argv[j];
                    if (!commands.count(cmd)) {
                        throw std::invalid_argument("\033[31mERROR: Команды " + cmd + " не существует\n");
                    }
                    enteredCommands.push_back(cmd);
                }
                printCmdHelp(enteredCommands, commands);
                break;
            }
            auto commandFlags = commands.at(cmd).commandFlags;
            std::map<std::string, Flag> flags;
            if (!commandFlags.empty()) {
                std::string flag;
                while (i + 1 < argc && !(flag = argv[i + 1]).empty() && flag[0] == '-' && ++i) {
                    std::string inputFlagName = flag;
                    inputFlagName.erase(std::remove(inputFlagName.begin(), inputFlagName.begin() + 2, '-'), inputFlagName.begin() + 2);
                    if ((inputFlagName = flagInCommand(commandFlags, inputFlagName)).empty()) {
                        throw std::invalid_argument("\033[31mERROR: Введён неизвестный флаг для команды \"" + cmd + "\" -> " + flag);
                    }
                    auto commandFlag = commandFlags.at(inputFlagName);
                    if (commandFlag.withValue) {
                        ++i;
                        commandFlag.value = argv[i];
                    }
                    flags.insert({inputFlagName, commandFlag});
                }
                message = checkIsRequiredFlag(flags, commandFlags);
                if (!message.empty()) {
                    throw std::invalid_argument(message);
                }
            }
            auto action = commands.at(cmd).action;
            if (action) {
                action(flags);
            }
        } else {
            if (cmd[0] == '-') {
                throw std::invalid_argument("\033[31mERROR: Неизвестный флаг -> " + cmd);
            } else {
                throw std::invalid_argument("\033[31mERROR: Неизвестная команда -> " + cmd);
            }
        }
    }
}

void cli::Cli::printAllHelp(std::map<std::string, Command> &commands) {
    std::cout << "\33[33mUsage:\n"
                 "\33[0m   command [flags] [arguments]\n\n"
                 "\33[33mCommands:\n";
    std::vector<int> sizes = getCmdSizes(commands);
    const int maxSize = *std::max_element(sizes.begin(), sizes.end());
    int i = 0;
    for (auto &cmd : commands) {
        std::string str = "  " + cmd.first;
        std::cout << "\033[32m" << std::left << std::setw(maxSize + 2) << str;
        std::cout << "\033[m" << cmd.second.description << "\n";
        ++i;
        if (!cmd.second.commandFlags.empty()) {
            std::cout << "\033[33m    Flags:\n";
        }
        for (auto &flag : cmd.second.commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += "[\033[31mREQUIRED\033[32m]";
            }
            std::cout << "\033[32m" << str;
            std::cout << "\033[m" << std::right << std::setw(maxSize - sizes[i] + 2) << "";
            std::cout << flag.second.description << "\n";
            ++i;
        }
    }
    std::cout << "\n";
}
std::vector<int> cli::Cli::getCmdSizes(std::map<std::string, Command> &commands) {
    std::vector<int> actualSize;
    for (auto &cmd : commands) {
        actualSize.push_back((int) cmd.first.size() + 2);
        for (auto &flag : cmd.second.commandFlags) {
            int flagSize = (int) (flag.first.size() + flag.second.shortName.size() + 11);// 19 - number of special characters (spaces, commas, hyphens)
            if (flag.second.withValue) {
                flagSize += 6;// "=VALUE" == 6
            }
            if (flag.second.isRequired) {
                flagSize += 10;// "\033[31mREQUIRED\033[32m" == 24
            }
            actualSize.push_back(flagSize);
        }
    }
    return actualSize;
}
void cli::Cli::printCmdHelp(std::vector<std::string> &commandsName, std::map<std::string, Command> &commands) {
    std::cout << "\33[33mUsage:\n"
                 "\33[0m   command [flags] [arguments]\n\n";
    if (commandsName.size() > 1) {
        std::cout << "\33[33mCommands:\n";
    } else {
        std::cout << "\33[33mCommand:\n";
    }
    std::map<std::string, Command> commandsMap;
    for (auto &cmdName : commandsName) {
        commandsMap.insert({cmdName, commands.at(cmdName)});
    }
    std::vector<int> sizes = getCmdSizes(commandsMap);
    const int maxSize = *std::max_element(sizes.begin(), sizes.end());
    int i = 0;
    for (auto &cmdName : commandsName) {
        auto cmd = commands.at(cmdName);
        std::string str = "  " + cmd.name;
        std::cout << "\033[32m" << std::left << std::setw(maxSize + 2) << str;
        std::cout << "\033[m" << cmd.description << "\n";
        ++i;
        if (!cmd.commandFlags.empty()) {
            std::cout << "\033[33m    Flags:\n";
        }
        for (auto &flag : cmd.commandFlags) {
            str = "      -" + flag.second.shortName + ", --" + flag.first;
            if (flag.second.withValue) {
                str += "=VALUE";
            }
            if (flag.second.isRequired) {
                str += "[\033[31mREQUIRED\033[32m]";
            }
            std::cout << "\033[32m" << str;
            std::cout << std::setw(maxSize - sizes[i] + 2) << "";
            std::cout << "\033[m" << flag.second.description << "\n";
            ++i;
        }
        if (!cmd.example.empty()) {
            std::cout << "\033[33m    Example:\033[0m\n      ";
            for (auto &symbol : cmd.example) {
                std::cout << symbol;
                if (symbol == '\n') {
                    std::cout << "      ";
                }
            }
            std::cout << "\n";
        }
    }
}
