# Максимальная возрастающая подпоследовательность с максимальной суммой
Программа на любом языке. Дан одномерный массив случайных целых чисел. Найти максимальную непрерывную возрастающую последовательность с наибольшей суммой элементов.

# Сборка
> make

# Запуск
> ./dp \<path\>

# Описание решения
Для решения поставленно задачи был использован подход динамического программирования. Создаётся временный массив DP, в котором хранится частичная сумма подпоследовательности, пока сами элементы являются возрастающими. </br>
Вывод ответа происходит в консоле. Так же для удобства был сгенерирован файл последовательности `something.txt`.
