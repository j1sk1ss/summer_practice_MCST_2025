# Парсер аргументов командной строки с использованием getopt и getopt_long
Написать парсер аргументов на языке C. Программа принимает опции `-m`, `-c`, `-s`, `-t`, а также `--elbrus=[1c+, 2c+, 2c3, 4c, 8c, 16c]` и произвольное количество строк без префиксов. Пример:
```
prg -tm -s --elbrus=16c file0 --elbrus=2c3 file1
```
Вывод: `Options are correct: t, m, c, elbrus=16c, elbrus=2c3, non-options: file0, file1.`  
При некорректной опции программа сообщает об ошибке.

# Сборка
> make

# Запуск
> ./parser \<args\>

# Описание решения
Реализованное решение представляет из себя два файла. Это `parser.c` и `parser_old.c`. Дело в том, что изначальное я не достаточно верно прочитал поставленную задачу, и проигнорировал встроенные инструменты `getopt` и `getopt_long`, результатом чего вышел файл `parser_old.c`.

nikolaj@nikolajs-MacBook-Pro fourth % ./parser -mcst -mcst -mcst -mcst -mcst -mcst -tscm --elbrus=1c+ elbrus eblrus!!!
./parser -mcst -mcst -mcst -mcst -mcst -mcst -tscm --elbrus=1c+ elbrus eblrusmake!
Options are correct: m, c, s, t, m, c, s, t, m, c, s, t, m, c, s, t, m, c, s, t, m, c, s, t, t, s, c, m, elbrus=1c+, non-options: elbrus, eblrusmake!.
