#include <iostream>
#include "InvertedIndex.h"

int main()
{
    InvertedIndex X;

    /*
     *  Метод parsing обрабатывает все файлы соотвествующие заданной маске
     *  Работает под Windows и Linux
     *
     *  Можете написать собственный парсер добавляя информацию о каждом 
     *  слове методом add
     *
     *  X.add(Слово, Имя файла, Строка появления, Номер строки);
     */
    X.parsing("*.txt");

    std::string input;
    std::cin >> input;

    /*
     *  Выводим информацию о всех повлениях
     */
    if(!X[input].empty())
        for(StringLocation info_of_location: X[input])
            std::cout << info_of_location << "\n";
    else
        std::cout << "Not found";
        
    return 0;
}