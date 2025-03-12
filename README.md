# Лабораторная работа №II.1. Разработка библиотеки для операций над множествами

## Оглавление

- [Вступление](#описание-проекта)
    - [Цели](#цели-работы)
    - [Задачи](#основные-задачи)
    - [Структура](#структура-проекта)
- [Реализация](#реализация)
    - [Список доступных функций](#функции-реализованные-в-библиотеке)
- [Сборка и запуск](#инструкции-по-сборке-и-запуску)
- [Тестирование и анализ](#тестирование)
    - [Тестирование](#тестирование)
    - [Анализ кода](#анализ-кода)
- [Вывод](#вывод)

## Описание проекта
В данном проекте реализуется библиотека для работы с множествами.
В ней реализованы основные операции над множествами.

## Цель работы и постановка задач
### Цели работы:

Изучить методы реализации операций над множествами в языке C, используя
инструменты разработки в ОС Линукс, а также освоить практики тестирования,
автоматического анализа и форматирования кода.

### Основные задачи:
1. Ознакомление с представлением множеств в виде битовых структур и реализация базовых
   операций над множествами, включая включение, объединение, пересечение, разность,
   симметрическую разность и дополнение.

2. Закрепление навыков использования механизмов динамического выделения памяти и
   работы с битовыми массивами в языке C.

3. Освоение инструментов автоматизированной сборки и анализа кода, включая
   использование Makefile, valgrind, clang-format, AddressSanitizer, MemorySanitizer и cppcheck.

4. Выполнение индивидуального задания, связанного с вычислением теоретико-
   множественного выражения, используя разработанную библиотеку.

5. Разработка алгоритма решения индивидуального задания, написание и отладка
   программы, а также тестирование на различных входных данных. Программа должна
   обеспечивать ввод и корректный вывод данных согласно условиям задания. При разработке
   использовать указанные инструменты для анализа кода, форматирования, поиска ошибок.

## Структура проекта
```
/bitset_project
│── src/
│   │── bitset/
│   │   │── bitset.c
│   │   │── bitset.h
|   │── errors/
|   |   │── errors.c
|   |   │── errors.h
│   │── output/
│   │   │── output.c
│   │   │── output.h
│   │── main.c
│── tests/
│   │── tests.c
│── Makefile
│── Maketest
│── README.md
```

### Описание файлов:
- **bitset.h/bitset.c** — реализация функций работы с множествами.
- **errors.h/errors.c** — функции для обработки возможных ошибок.
- **output.h/output.c** — функции для вывода данных.
- **main.c** — основная программа, использующая библиотеку.
- **tests.c** — тестовые функции для библиотеки.
- **Makefile** — автоматизированная сборка проекта.
- **README.md** — описание проекта.

## Реализация

### Функции, реализованные в библиотеке

Доступные функции для работы с множествами:

| Функция                     | Описание                        |
|-----------------------------|---------------------------------|
| `createBitSet()`            | Создание множества              |
| `addBitSetElement()`        | Добавление элемента             |
| `addManyBitSetElements()`   | Добавление нескольких элементов |
| `removeBitSetElement()`     | Удаление элемента               |
| `isBitSetContains()`        | Проверка наличия элемента       |
| `destroyBitSet()`           | Удаление множества              |
| `isBitSetsEqual()`          | Проверка равенства              |
| `isSubset()`                | Проверка подмножества           |
| `isStrictSubset()`          | Проверка строгого подмножества  |
| `getBitSetsUnion()`         | Объединение                     |
| `getBitSetsIntersection()`  | Пересечение                     |
| `getBitSetsDiff()`          | Разность                        |
| `getSymmetricBitSetsDiff()` | Симметричная разность           |
| `getBitSetComplement()`     | Дополнение                      |
| `printBitSet()`             | Вывод множества                 |


## Инструкции по сборке и запуску
### Сборка проекта
Для сборки используйте команду:
```sh
  make
```
### Запуск программы
```sh
  ./bitset-main
```
### Очистка скомпилированных файлов
```sh
  make clean
```
### Пересборка проекта
```sh
  make rebuild
```
## Тестирование
### Запуск тестов
```sh
  make test
```
## Анализ кода

### Проверка утечек памяти
```sh
  make valgrind
```
### Дополнительная проверка
```
  cppcheck --enable=all *.c
```

**Запуск автоформатирования кода:**
```sh
  make clang-format
```

## Вывод
В результате работы была разработана библиотека для работы с множествами.
В библиотеке предоставлены документация и примеры, что облегчает её дальнейшее использование.