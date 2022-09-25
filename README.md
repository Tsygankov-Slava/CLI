# Command Line Interface (CLI)

[![Build](https://github.com/Tsygankov-Slava/cli/actions/workflows/build.yml/badge.svg)](https://github.com/Tsygankov-Slava/cli/actions/workflows/build.yml)


CLI - это библиотека, которая помогает разработчику в написании консольных приложений. С помощью этого проекта, программист сможет легко наладить общение пользователя с программой через консоль. Плюс этого проекта в том, что разработчику не придётся каждый раз прописывать свою логику CLI, стоит лишь добавить эту библиотеку в свой проект и можно смело использовать её в своей программе.

# Оглавление

- [ Рекомендованные требования инструментов для разработки этого проекта](#рекомендованные-требования-инструментов-для-разработки-этого-проекта)
- [Установка](#установка)
- - [Для пользователя](#для-пользователя)
- - [Для разработчика CLI](#для-разработчика-cli)
- [Описание](#описание)
- [Пример использования](#пример-использования)
- [Лицензия](#лицензия)

# Рекомендованные требования инструментов для разработки этого проекта

> - Версия CMake: 3.20
> - Версия Clang-Tidy: 13.0.0
> - Стандарт C++: 14
> - Версия Python: 3.1 (нужен если Вы сами собираетесь собирать библиотеку из исходников)

[🔝Оглавление](#оглавление)

# Установка

### Для пользователя

Чтобы получить готовую библиотеку нужно :

1. Зайти в Releases и выбрать подходящую версию CLI (Рекомендовано использовать последнюю версию проекта)
2. После следует найти архив с названием `cli-version.zip` (где `version` это версия релиза) и скачать
![](docs/images/download-build-cli.png)
3. Распакуйте загруженный архив и переместите библиотеку `cli.hpp` в ваш проект (библиотека будет находиться в папке `build/`)
4. Подключите библиотеку в свою программу с помощью `#include` 

Подробности смотреть в разделе ["Пример использования"](#пример-использования)

### Для разработчика CLI

Если Вы хотите вносить вклад в этот проект и заниматься разработкой этого CLI, то для этого Вам потребуется:

1. Открыть терминал и клонировать репозиторий

```
$ git clone https://github.com/Tsygankov-Slava/cli.git
```

2. Перейти в папку с проектом

```
$ cd cli 
```

3. Подключить проект googletest, как submodule

```
$ git submodule update --init --recursive   
```

4. Внести желаемые изменения в код и при необходимости добавьте тесты в файл `tests/test`
5. Прогоните код по тестам, для этого запустите файл `test` (Этот файл собирается автоматически вместе со всем проектом)
6. Если при разработке `CLI` Вы добавили новые файлы, нужно будет открыть скрипт питона `build-lib.py` и добавить нужные пути в список `paths` (❗Порядок файлов в списке важен, иначе библиотека просто не скомпилируется во время использования её в коде)
7. Собрать библиотеку для дальнейшего использования её в проекте. Для сборки библиотеки воспользуемся командой `make`, находясь в каталоге `cli/`

```
$ make
```

P.S. Чтобы использовать команду make, убедитесь что у вас установлен `python`. Потому что сборкой библиотеки занимается скрипт написанный на `python`.

8. После выполнения команды `make` создастся директория `build/` в которой будет находиться собранная библиотека `cli.hpp`

P.S. Если вдруг Вам понадобится удалить директорию `build/`, просто выполните команду `make clean`
```
$ make clean
```

[🔝Оглавление](#оглавление)

# Описание

При использовании CLI стоит различать три понятия `команда`, `аргумент` и `флаг`.
Нужно понимать, что пользователь вводит `команду`, когда хочет выполнить какое-то действие, `аргумент`, чтобы передать в `команду` информацию с которой в дальнейшем работать, а `флаг` он вводит для того, чтобы указать, что действие `команды` будет выполняться по каким-то правилам и с какими-то переданными значениями. \
Исходя из вышесказанного, можно вывести одно очень простое правило: 

> ❗ Команда может содержать некое количество флагов и аргументов, но никак иначе.

### Некоторые функции и правила использования:

1. Для того чтобы начать использовать CLI сначала надо создать объект `Cli`:

```c++
auto cli = cli::Cli();
```

> P.S. У объекта можно вызвать метод `setDescriptionMaxWidth` и передать в него желаемый размер поля для описания
> ```c++
> auto cli = cli::Cli();
> cli.setDescriptionMaxWidth(7);
> ```
> ❗ По умолчанию стоит 50 символов.
> 
> Примеры: 
> - 50 символов:
> ```
> Command:
> help                               Show help information.
> ```
> - 10 символов:
> ```c++
> Command:
> help                               Show help  
>                                    information.
> ```
> ❗ При использовании этого метода стоит обращать внимание, что существует реализация переноса слов.  
> Перенос работает по правилу "не оставлять меньше 3 символов до/после дефиса".

2. Чтобы описать `команду` можно использовать один из вариантов ниже: 

```c++
command(name, description, example, {flags}, action, argumentsCount = 0, canContainEmptyArgumentList = false);
```

```c++
command(name, description, example, action, argumentsCount = 0, canContainEmptyArgumentList = false);
```

```c++
command(name, description, {flags}, action, argumentsCount = 0, canContainEmptyArgumentList = false);
```

```c++
command(name, description, action, argumentsCount = 0, canContainEmptyArgumentList = false);
```

| Поле                          | Тип               | Описание                                                                                                    |
|-------------------------------|-------------------|-------------------------------------------------------------------------------------------------------------|
| `name`                        | `string`          | имя команды                                                                                                 |
| `description`                 | `string`          | описание команды                                                                                            |
| `example`                     | `string`          | пример использования команды                                                                                |
| `flags`                       | `Flags`           | задаются флаги                                                                                              |
| `action`                      | `CommandCallback` | имя функции, которая будет выполняться при вызове данной команды                                            |
| `argumentsCount`              | `int`             | кол-во аргументов, которые команда может принять (если указать `-1`, то кол-во аргументов может быть любое) |
| `canContainEmptyArgumentList` | `bool`            | флаг, разрешающий не принимать аргументов (имеет значение если `argumentsCount = -1`)                |
  
>```c++
>❗ Тип Flags - множество объектов класса Flag (как определять Flag показывается ниже)
>```
>``` c++
>❗ Тип CommandCallback ~ std::function<void(std::map<std::string, Flag> &)>
>```

3. Чтобы описать `флаг` нужно использовать следующий синтаксис:

```c++
Flag(name, shortName, description, isRequired, withValue)
```

| Поле          | Тип        | Описание                                                                     |
|---------------|------------|------------------------------------------------------------------------------|
| `name`        | `string`   | полное имя флага                                                             |
| `shortName`   | `string`   | сокращённое имя флага                                                        |
| `description` | `string`   | описание флага                                                               |
| `isRequired`  | `bool`     | логическое значение, устанавливающее обязательный ли флаг или нет            |
| `withValue`   | `bool`     | логическое значение, устанавливающее прилагается ли к флагу аргумент или нет |
 
> P.S. полное имя флага вызывается с двумя дефисами `--`, сокращённое имя флага с одним дефисом `-`. \
> Пример: 
> - полное имя `--flag`  
> - сокращённое имя `-f`

4. В `CLI` доступна функция `convertArgumentsToType`, которая конвертирует переданные аргументы в определённый тип. Такой функционал позволяет пользователю не писать проверки на корректность переданных аргументов вручную.

> Доступные типы для конвертации: `int`, `string`, `bool`

> Синтаксис: `cli::Cli::convertArgumentsToType<type>(parsedArguments); `

> Возвращает: `cli::ArgumentsType` (Аргументы заданного типа)

Для того чтобы использовать данный функционал в функции привязанной к команде следует написать следующее:
```c++
int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды
    /* 
     * Для того чтобы CLI выдало ошибку при передаче неправильного типа,
     * ВАЖНО у функции convertArgumentsToType отловить исключение,
     * для этого обернём данную команду в блок try/catch
     */
    try {
        auto arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments); // конвертируем переданные аргументы parsedArguments в тип string и записываем их в переменную arguments
        // your code ...
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n"; // Выводим сообщение об ошибки (тип аргументов не соответствует заданному)
        return 3; // код с которым завершается программа
    }
    return 0;
}
```


5. В конце нужно обязательно вызвать функцию `parse` с аргументами `argc` и `argv`.

> В данной версии CLI присутствует само-документирование. Существует по умолчанию команда `help`, которая собирает информацию о командах и флагах и выводит её. \
> Также можно посмотреть информации по отдельной команде или определенному множеству команд.
> ``` 
> $ help 
> ```
> ``` 
> $ help command
> ```
> ``` 
> $ help command1 command2
> ```

> Также можно контролировать цвет выводимой информации в консоль. \
> По умолчанию вывод всей информации (документации или ошибок) стоит в цвете, но если запустить выполнение программы с флагом `--nocolor`, то весь вывод будет в стандартном белом цвете.

❗ Для полного понимая того, как использовать CLI в своём коде, см. раздел ["Пример использования"](#пример-использования)

[🔝Оглавление](#оглавление)

# Пример использования

```c++
#include "../build/cli.hpp" // Подключаем нашу библиотеку для использования CLI (путь до библиотеки может отличаться)

int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments);  // Объявляем функцию, которая будет вызывать при вызове команды printArguments
int func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printHello
int func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printName
int func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printFlagsAndArguments
int func5(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printStringArguments
int func6(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printIntArguments
int func7(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments); // Объявляем функцию, которая будет вызывать при вызове команды printBoolArguments

int main(int argc, char **argv) {
    auto cli = cli::Cli();         // Создаём объект класса Cli
    cli.setDescriptionMaxWidth(7); // Задаём ширину описания

    try {
        cli.command("printArguments", "Displays the passed arguments", "$ printArguments file1.txt file2.txt\n>>> Arguments:\n file1.txt\n    file2.txt", func, -1)     // Добавляем команду printArguments
           .command("printTwoArguments", "Displays the passed arguments", func, 2)                                                                                      // Добавляем команду printTwoArguments
           .command("printHello", "Displays the word \"Hello!\".", "$ printHello \n>>> Hello!", func2)                                                                  // Добавляем команду printHello
           .command("printName", "Displays \"Hello [entered name]!\".", "$ printName -n Name\n>>> Hello Name!",
                    {
                        cli::Flag("name", "n", "A flag that accepts a name as input.", true, true),                                                                     // Объявляем флаг "--name" для команды printName
                        cli::Flag("surname", "s", "A flag that accepts a surname for entry.", true, true)                                                               // Объявляем флаг "--surname" для команды printName
                    }, func3)                                                                                                                                           // Добавляем команду printName и указываем флаги name и surname
           .command("printFlagsAndArguments", "Displays the passed flags and arguments", "$ printFlagsAndArguments file1.txt --dir value file2.txt\n>>> Flags:\n"
                                                                                              "    flag -> value\n Arguments:\n    file1.txt\n    file2.txt",
                    {
                        cli::Flag("dir", "d", "A flag that accepts a directory as input.", false, true),                                                                // Объявляем флаг "--dir" для команды printName
                    }, func4, -1)                                                                                                                                       // Добавляем команду printFlagsAndArguments
           .command("printStringArguments", "Displays the passed string arguments", func5, -1)                                                                          // Добавляем команду printStringArguments
           .command("printIntArguments", "Displays the passed int arguments", func6, -1)                                                                                // Добавляем команду printIntArguments
           .command("printBoolArguments", "Displays the passed bool arguments", func7, -1)                                                                              // Добавляем команду printBoolArguments
           .parse(argc, argv);                                                                                                                                          // Обязательно вызываем функцию parse c аргументами argc и argv
    } catch (const std::invalid_argument &error) {                                                                                                                      // Обрабатываем какие-либо ошибки
        std::cout << error.what() << "\n";                                                                                                                              // Обязательно при выводе ошибок поставить символ переноса строки, иначе возможен вывод странных символов
        return 2;                                                                                                                                                       // код завершения программы при ошибке
    }
    return 0;
}


int func(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        std::cout << "Arguments: \n";
        for (auto &arg : arguments) {
            std::cout << arg << "\n";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func2(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printHello
    std::cout << "Hello!\n";
    return 0;
}

int func3(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printName
    std::cout << "Hello "
              << parsedFlags.at("name").value << " "
              << parsedFlags.at("surname").value << "!\n";
    return 0;
}

int func4(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printFlagsAndArguments
    std::cout << "Flags: \n";
    for (auto &flag : parsedFlags) {
        std::cout << flag.first << " -> " << flag.second.value << "\n";
    }
    std::cout << "Arguments: \n";
    for (auto &arg : parsedArguments) {
        std::cout << arg << "\n";
    }
    return 0;
}

int func5(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printStringArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<std::string>(parsedArguments);
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func6(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printIntArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<int>(parsedArguments);
        for (auto &argument : arguments) {
            std::cout << argument << " ";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}

int func7(cli::FlagsType &parsedFlags, cli::ArgumentsType &parsedArguments) { // Определение функции команды printBoolArguments
    try {
        auto arguments = cli::Cli::convertArgumentsToType<bool>(parsedArguments);
        for (auto argument : arguments) {
            std::cout << argument << " ";
        }
    } catch (const std::invalid_argument &error) {
        std::cout << error.what() << "\n";
        return 3;
    }
    return 0;
}
```

### Примеры запуска в терминале:

```
$ ./cli printHello
Hello!
```

```
$ ./cli printName -n Vanya -s Sidorov
Hello Vanya Sidorov!
```

```
$ ./cli help
CLI version 0.2.1

Usage:
   command [flags] [arguments]

Commands:
  help                               Show help 
                                     informa-
                                     tion. 
  printArguments                     Displays 
                                     the passed
                                     arguments
  printFlagsAndArguments             Displays 
                                     the passed
                                     flags and
                                     arguments
    Flags:
      -d, --dir=VALUE                A flag  
                                     that acc-
                                     epts a 
                                     directory
                                     as input.
  printHello                         Displays 
                                     the word
                                     "Hello!".
  printName                          Displays 
                                     "Hello 
                                     [entered
                                     name]!".
    Flags:
      -n, --name=VALUE[REQUIRED]     A flag  
                                     that acc-
                                     epts a 
                                     name as 
                                     input. 
      -s, --surname=VALUE[REQUIRED]  A flag  
                                     that acc-
                                     epts a 
                                     surname
                                     for entry.
  printTwoArguments                  Displays 
                                     the passed
                                     arguments
```

```
$ ./cli -f --nocolor
ERROR: Unknown flag -> "-f"
```

```
$ ./cli printHello -f --nocolor
Hello!
ERROR: An unknown flag has been entered for the command "printName" -> "-f"
```

```
$ ./cli cmd --nocolor
ERROR: Unknown command -> "cmd"
```

```
$ ./cli printName --nocolor
ERROR: Required flag not entered -> "--name" OR "-n"
```

```
$ ./cli printName -n --nocolor
ERROR: Flag "--name" must accept an argument
```

```
$ ./cli --nocolor printArguments
ERROR: Command "printArguments" must contain at least one argument
```

```
$ ./cli --nocolor printTwoArguments a
ERROR: Command "printTwoArguments" must contain 2 arguments
```


```
$ ./cli printFlagsAndArguments file1.txt --dir directory/ file2.txt
Flags: 
dir -> directory/
Arguments: 
file1.txt
file2.txt
```

```
$ ./cli printIntArguments file1.txt file2.txt
ERROR: Command arguments must be int type
::Wrong argument -> "file1.txt" | "file2.txt"
```

```
$ ./cli printBoolArguments a 4
ERROR: Command arguments must be bool type
::Wrong argument -> "a" | "4"
```

```
$ ./cli printIntArguments -2 +102 10
-2 102 10
```

```
$ ./cli printBoolArguments true false 1 0
1 0 1 0
```

```
$ ./cli printStringArguments true 1 file1.txt +-100 -4
true 1 file1.txt +-100 -4
```

[🔝Оглавление](#оглавление)

# Лицензия
Command Line Interface(CLI) распространяется по [лицензии MIT](https://github.com/Tsygankov-Slava/cli/edit/main/LICENSE) от имени Tsygankov Viacheslav.

[🔝Оглавление](#оглавление)
