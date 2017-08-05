Inverted Index 
==============

>[Материал из Википедии — свободной энциклопедии](https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D0%B2%D0%B5%D1%80%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%BD%D1%8B%D0%B9_%D0%B8%D0%BD%D0%B4%D0%B5%D0%BA%D1%81)
>
>Инвертированный индекс (англ. inverted index) — структура данных, в которой для каждого слова коллекции документов в соответствующем списке перечислены все документы в коллекции, в которых оно встретилось. Инвертированный индекс используется для поиска по текстам.


```cpp
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
```