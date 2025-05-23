# Сортировка строк из файла с удалением пустых строк
Написать программу на C. У программы три аргумента командной строки: входной файл, выходной файл, способ сортировки. Отсортировать строки файла указанным способом, удалить пустые строки без видимых символов, вывести результат в выходной файл и на экран.

# Сборка
> make

# Запуск
> ./line_sort \<path\> \<args\>

# Описание решения
Данный исполнительный файл реализует сортировку строк файла по разным паттернам. Перед сортировкой удалаяются все пустые строки и строки с невидимыми символами. </br>
Алгоритм сортировки следующий:
- Токенизация файла. Токен - это одна строка.
- Генерация стека преобразований для каждой строки в зависимости от введённых команд.
- Проход по всем токенам с применением стека преобразований.
- Сортировка с указанным типом (Возрастание и убывание).

Основная идея, что в токене хранится не только сама строка, но и её временная реализация, и числовое значение, по которому будет происходить сортировка. Например при команде `--tlower`, а после `--bylen`, во временную строку запишется строка в нижнем регистре, а по временном числовом значении будет лежать длина строки (длина именно временного представления, если оно есть).</br>
Сама же программа поддерживает следующий набор команд:
```
--bylen Получение длины строки во временное числовое хранение.
--bychar_at <idx> Получение символа из строки по индексу. Само индексирование может быть отрицательным, тогда используется индексирование с конца строки. При выходе за границы, используются крайние (соотвественные края) символы.
--toint Приведение строки к числу (Или к нули, если числа нет).
--letsum Сумма всех букв в строке.
--digsum Сумма всех цифр в строке.
--chsum Сумма всех символов в строке.
--lcount Сумма символов в нижнем регистре.
--ucount Сумма символов в верхнем регистре.
--tlow Приведение строки в нижний регистр.
--tupp Приведение строки в верхний регистр.
--asc Сортировка по возрастанию.
--desc Сортировка по убыванию.
-o Путь для сохранения (Если не указать, будет создан файл типо XXXXXX_<filename>).
```

P.S. Строки, написанные на русском или другом языке из UTF-8 не корректно обрабатываются командами `--tlower` и `--tupper`. Но для русского языка добавлена поддержка кодировки cp1251.
