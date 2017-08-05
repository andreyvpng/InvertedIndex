/*
    The MIT License
    Copyright 2017 Andrey Varfolomeev <varf.andrey@hotmail.com>
    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
    to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>


#ifdef _WIN32
    #include <windows.h>
#else
#ifdef __unix__
    #include <sys/types.h>
    #include <dirent.h>
#endif // __unix__
#endif // _WIN32

#ifndef _INVERTED_INDEX_
#define _INVERTED_INDEX_


class StringLocation {
private:
    std::string FileName;   // Имя файла
    std::string lineOfText; // Строка, в которой встретитилось слово
    int lineOfLocation;     // Номер строки
public:
    StringLocation(std::string FileName_, std::string lineOfText_, int lineOfLocation_)
    {
        this->FileName = FileName_;
        this->lineOfText = lineOfText_;
        this->lineOfLocation = lineOfLocation_;
    }


    // Возвращает имя файла
    std::string getFileName ()
    {
        return this->FileName;
    }


    // Возращает номер строки
    int getLineOfLocation ()
    {
        return this->lineOfLocation;
    }


    // Возращает строку, в которой встретилось слово
    std::string getLineOfText ()
    {
        return this->lineOfText;
    }


    // Перегрузка оператора сравнения
    // т.к. StringLocation будем хранить в std::set
    bool operator < (const StringLocation& my_StringLocation) const
    {
        if(this->FileName == my_StringLocation.FileName)
            return this->lineOfLocation < my_StringLocation.lineOfLocation;

        return this->FileName < my_StringLocation.FileName;
    }


    //Перегрузка ввода/вывода
    friend std::ostream& operator << (std::ostream &out, const StringLocation& my_StringLocation)
    {
        out << "File: " << my_StringLocation.FileName << ".\tLine: " << my_StringLocation.lineOfLocation
            << ".\tString: " << my_StringLocation.lineOfText;
        return out;
    }


};

class InvertedIndex {
private:
    std::map<std::string, std::set<StringLocation> > invertedIndex;


    // Проверка соответствия названия файла к маски
    bool CheckFileForMatchingMask(std::string file, std::string mask)
    {
        int i = mask.find("*");
        int r = std::max((int)file.size()-(int)mask.size() + 1, 0);

        if(i != std::string::npos)
            mask.replace(mask.begin() + i,
                        mask.begin() + i + 1,
                        r, '?');

        if(mask.size() != file.size())
            return false;

        for(int i = 0; i < file.size(); i++)
            if(file[i] != mask[i] && mask[i] != '?')
                return false;

        return true;
    }


public:


    // Добавляет информацию о местоположении слова
    void add (const std::string& word, 
              const std::string& FileName, 
              const std::string& lineOfText, 
              const int& lineOfLocation)
    {
        this->invertedIndex[word].insert(StringLocation(FileName, lineOfText, lineOfLocation));
    }
    
    
    // Очистка коллекции
    void clear ()
    {
        invertedIndex.clear();
    }


    // Возвращают информацию о слове
    std::set<StringLocation>& operator[] (const std::string& word) {
          return this->invertedIndex[word];
    }
    std::set<StringLocation> get (const std::string& word)
    {
        return this->invertedIndex[word];
    }


    // Обрабатывает файлы из директории по маске, вызывает функцию parsing_file
    void parsing (std::string NameOfFile)
    {
        #ifdef _WIN32
            char pathToTheFile[2 + NameOfFile.size()] = "./";
            strcat(pathToTheFile, NameOfFile.c_str());
            WIN32_FIND_DATA f;
            HANDLE h = FindFirstFile(pathToTheFile, &f);
            if(h != INVALID_HANDLE_VALUE)
                do
                    this->parsing_file(f.cFileName); 
                while(FindNextFile(h, &f));
            else
                std::cout << "Error opening directory\n";
        #else
        #ifdef __unix__
            DIR *dir = opendir(".");
            if(dir)
            {
                struct dirent *ent;
                while((ent = readdir(dir)) != NULL)
                {
                    if(CheckFileForMatchingMask(ent->d_name, NameOfFile))
                        this->parsing_file(ent->d_name); 
                }
            }
            else
                std::cout << "Error opening directory\n";
        #endif // __unix__
        #endif // _WIN32
    }


    // Обрабатывает файл, добавляет новые слова и строку их местоположения
    void parsing_file (const std::string& NameOfFile)
    {
        std::ifstream fin(NameOfFile);

        if (!fin.is_open()) // если файл не открыт
            std::cout <<  NameOfFile << " can not be opened!\n";
        else
        {
            std::string str, word;
            int count_of_lines = 0;
            while(std::getline(fin, str))
            {
                ++count_of_lines;
                std::istringstream words(str);
                while(words >> word)
                {
                    this->add(word, NameOfFile, str, count_of_lines);
                }
            }
            fin.close();
        }
    }


    // Перегрузка ввода/вывода
    friend std::ostream& operator << (std::ostream &out, const InvertedIndex& InvertedIndex)
    {
        for(std::pair<std::string, std::set<StringLocation> > invertedIndex_ : InvertedIndex.invertedIndex)
        {
            out << invertedIndex_.first << ":\n";

            for(StringLocation strings : invertedIndex_.second)
            {
                out << "\t" << strings << "\n";
            }
        }
        return out;
    }
};


#endif // _INVERTED_INDEX_
