#include<cstdlib>
#include<fstream>
#include<cctype>
#include"map.h"
#include"string.h"

int main(void)
{
    tp::map<int, tp::string> dic;

    tp::string str;
    std::cout<<"Input file: ";
    std::cin>>str;
    char *f = str.c_str();
    std::ifstream in(f);
    delete[] f;
    if(!in.is_open())
    {
        std::cout<<"No intput file: "<<str<<std::endl;
        return -1;
    }
    str.clear();

    // Посчитаем слова в файле и их повторение

    while(!in.eof())
    {
        char c;
        in.get(c);
        while(!isspace(c) && !in.eof())
        {
            str += c;
            in.get(c);
        }

        if(str.length())
            dic[str] += 1;
        str.clear();
    }

    dic.print();

    return 0;
}

