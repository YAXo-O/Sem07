#ifndef STRING_H
#define STRING_H

#include<iostream>
#include<stdexcept>
#include<algorithm>
#include<cstring>
#include<initializer_list>

namespace tp
{

    template<typename T,typename A = std::allocator<T>>
    class string_base
    {
    private:
        T *data;
        std::size_t len; // Без учёта \0
        std::size_t cap;
    public:
        string_base();
        string_base(const string_base<T, A> &other);
        string_base(const T *str);
        string_base(string_base<T, A> &&other);
        string_base(const std::initializer_list<T> &list);
        // Конструктор по итераторам в конце класса, после объявления итератора
        string_base<T, A> &operator=(const string_base<T, A> other);
        string_base<T, A> &operator=(string_base<T, A> &&other);
        string_base<T, A> &operator=(const T *str);
        string_base<T, A> &operator=(const T ch);
        ~string_base();

        std::size_t size() const;
        std::size_t length() const;
        void resize(std::size_t _s, T fill = ' ');
        size_t capacity() const;
        void reserve(std::size_t n = 0);
        void clear();
        bool empty() const;
        void shring_to_fit();

        T &operator[](std::size_t id);
        T &at(std::size_t id);
        T &back();
        T &front();

        string_base<T, A> &operator+=(const string_base<T, A> &other);
        string_base<T, A> &operator+=(const T *str);
        string_base<T, A> &operator+=(const T);
        string_base<T, A> &append(const string_base<T, A> &other, std::size_t in_id); // append позволяет делать вставку
        string_base<T, A> &append(const T *str, std::size_t in_id); //  в любую часть строки(в том числе и по индексу, превышающему длину)
        string_base<T, A> &push_back(const T ch);
        string_base<T, A> &erase(std::size_t id, std::size_t len = 1);
        void swap(string_base<T, A> &other);
        T pop_back();

        T *c_str();
        void copy(T *s, size_t len, size_t pos = 0) const; // TODO
        string_base<T, A> substr(std::size_t id, std::size_t len);

        string_base<T, A> operator+(const string_base<T, A> &other) const;
        string_base<T, A> operator+(const T *str) const;
        string_base<T, A> operator+(const T ch) const;

        bool operator==(const string_base<T, A> &rgt) const;
        bool operator==(const T *str) const;
        bool operator==(const T ch)const ;
        bool operator!=(const string_base<T, A> &rgt) const;
        bool operator!=(const T *str) const;
        bool operator!=(const T ch) const;
        bool operator>(const string_base<T, A> &rgt) const;
        bool operator>(const T *str) const;
        bool operator>(const T ch) const;
        bool operator<(const string_base<T, A> &rgt) const;
        bool operator<(const T *str) const;
        bool operator<(const T ch) const;
        bool operator>=(const string_base<T, A> &rgt) const;
        bool operator>=(const T *str) const;
        bool operator>=(const T ch) const;
        bool operator<=(const string_base<T, A> &rgt) const;
        bool operator<=(const T *str) const;
        bool operator<=(const T ch) const;

        class str_iterator
        {
        private:
            T *it;
        public:
            str_iterator();
            str_iterator(const str_iterator &other);
            str_iterator(T *p);
            str_iterator &operator=(const str_iterator &other);
            ~str_iterator();

            str_iterator &operator++();
            str_iterator operator++(int);
            str_iterator &operator--();
            str_iterator operator--(int);

            bool operator==(const str_iterator &other) const;
            bool operator!=(const str_iterator &other) const;

            T &operator*();

        };

        str_iterator begin() const;
        str_iterator end() const;

        string_base(const str_iterator &b, const str_iterator &e);
    };

    template<typename T, typename A>
    std::ostream &operator<<(std::ostream &out, string_base<T, A> &str)
    {
        for(size_t i = 0; i < str.length(); i++)
        {
            out<<str[i];
        }

        return out;

    }

    template<typename T, typename A>
    std::istream &operator>>(std::istream &in, string_base<T, A> &str)
    {
        str.clear();
        char c;
        c = in.get();
        while(c != '\n')
        {
            str += c;
            c = in.get();
        }

        return in;
    }

    template<typename T, typename A>
    string_base<T,A>::string_base(): data(NULL), len(0), cap(0)
    {
    }

    template<typename T, typename A>
    string_base<T, A>::string_base(const string_base<T, A> &other): len(other.len), cap(other.cap)
    {
        try
        {
            data = new T[cap];
            std::copy(other.data, other.data + len*sizeof(T), data);
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't copy construct string: "<<e.what()<<std::endl;
            throw;
        }
    }

    template<typename T, typename A>
    string_base<T, A>::string_base(const T *str): len(std::strlen(str)), cap(len)
    {
        try
        {
           data = new T[cap];
           std::copy(str, str + len, data);
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't copy construct: "<<e.what()<<std::endl;
        }

    }

    template<typename T, typename A>
    string_base<T, A>::string_base(string_base<T, A> &&other): data(other.data), len(other.len)
    {
        other.data = NULL;
        other.len = 0;
    }

    template<typename T, typename A>
    string_base<T, A>::string_base(const std::initializer_list<T> &list): data(NULL), len(0), cap(0)
    {
        reserve(list.size());
        for(auto elem : list)
            push_back(elem);
    }

    template<typename T, typename A>
    string_base<T, A>::string_base(const string_base<T, A>::str_iterator &b, const string_base<T, A>::str_iterator &e):
        data(NULL), len(0), cap(0)
    {
        for(auto it = b; it != e; it++)
            push_back(*it);
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator=(string_base<T, A> other)
    {
        swap(other);
        return *this;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator=(string_base<T, A> &&other)
    {
        if(data)
            delete[] data;

        data = other.data;
        len = other.len;

        other.data = NULL;
        other.len = 0;

        return *this;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator=(const T *str)
    {
        if(data)
        {
            delete[] data;
            data = NULL;
        }

        len = strlen(str);
        cap = len;

        try
        {
            data = new T[cap];
            std::copy(str, str + len, data);
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't assign char*: "<<e.what()<<std::endl;
            throw;
        }

        return *this;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator=(const T ch)
    {
        if(len)
            clear();

        try
        {
            cap = len = 1;
            data = new T[cap];
            data[0] = ch;
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't alloc mem for =(char): "<<e.what()<<std::endl;
            throw;
        }

        return *this;
    }

    template<typename T, typename A>
    string_base<T, A>::~string_base()
    {
        if(data)
        {
           delete[] data;

        }
    }

    template<typename T, typename A>
    std::size_t string_base<T, A>::size() const
    {
        return len*sizeof(T);
    }

    template<typename T, typename A>
    std::size_t string_base<T, A>::length() const
    {
        return len;
    }

    template<typename T, typename A>
    void string_base<T, A>::resize(std::size_t _s, T fill)
    {
        if(_s <= len)
        {
            for(size_t i = _s; i < len; i++ )
                data[i].~T();
            len = _s;
            return;
        }

        if(_s > cap)
            reserve(_s);

        for(std::size_t i = len; i < _s; i++)
            data[i] = fill;

        len = _s;
    }

    template<typename T, typename A>
    size_t string_base<T, A>::capacity() const
    {
        return cap;
    }

    template<typename T, typename A>
    void string_base<T, A>::reserve(std::size_t n)
    {
        if(n <= cap && n != 0)
            return;

        cap = (n == 0) ? (cap*2 + 1) : n;
        try
        {
            T *tmp = new T[cap];
            if(data)
            {
                std::copy(data, data + len, tmp);
                delete[] data;
            }
            data = tmp;
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't alloc memory for tempory holder: "<<e.what()<<std::endl;
            throw;
        }
    }

    template<typename T, typename A>
    void string_base<T, A>::clear()
    {
        len = 0;
        cap = 0;
        if(data)
        {
            delete[] data;
            data = NULL;
        }
    }

    template<typename T, typename A>
    bool string_base<T, A>::empty() const
    {
        return len == 0;
    }

    template<typename T, typename A>
    void string_base<T, A>::shring_to_fit()
    {
        if(len != cap)
        {
            try
            {
                T *tmp = new T[len];
                if(data)
                {
                    std::copy(data, data + len, tmp);
                    delete[] data;
                }
                data = tmp;
            }
            catch(std::bad_alloc &e)
            {
                std::cout<<"Can't allocate mem for tmp at shrinking: "<<e.what()<<std::endl;
                throw;
            }
        }
    }

    template<typename T, typename A>
    T &string_base<T, A>::operator[](std::size_t id)
    {
        return data[id];
    }

    template<typename T, typename A>
    T &string_base<T, A>::at(std::size_t id)
    {
        if(id >= len)
        {
            throw std::out_of_range("Access to non-existing string element;");
        }

        return data[id];
    }

    template<typename T, typename A>
    T &string_base<T, A>::back()
    {
        if(!len)
            throw std::out_of_range("Backing out of range element;");

        return data[len-1];
    }

    template<typename T, typename A>
    T &string_base<T, A>::front()
    {
        if(!len)
            throw std::out_of_range("Fronting out of range element;");

        return data[0];
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator+=(const string_base<T, A> &other)
    {
        if(len + other.len > cap)
            reserve(len + other.len);

        std::copy(other.data, other.data + other.len, data + len);
        len += other.len;

        return *this;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator+=(const T *str)
    {
        if(!str)
            return *this;

        size_t sl = strlen(str);

        if(cap < len + sl)
            reserve(len + sl);

        std::copy(str, str + sl, data + len);
        len += sl;

        return *this;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::operator+=(const T ch)
    {
        if(len == cap)
            reserve();
        // Добавление по символу, скорее всего, происходит в каком-нибудь цикле, поэтому
        // Целесобразно сразу выделить побольше памяти, чтобы поменьше её перевыделять
        // В идеале, программист, если ему известно кол-во добавляемых символов,
        // Должен заранее зарезервировать память перед циклом
        // В этом случае выделится ровно столько памяти, сколько нужно

        data[len++] = ch;

        return *this;
    }


    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::append(const string_base<T, A> &other, std::size_t in_id)
    {
        if(in_id >= cap)
        {
            //reserve(in_id + other.len + 1);
            resize(in_id);
            len = cap;
        }
        else if(len + other.len > cap)
            reserve(len + other.len);

        try
        {
         T *tmp = new T[len - in_id];

         std::copy(data + in_id, data + len, tmp);
         std::copy(other.data, other.data + other.len, data + in_id);
         std::copy(tmp, tmp + len - in_id, data + in_id + other.len);

        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't allocate mem for appending: "<<e.what()<<std::endl;
            throw;
        }

        len += other.len;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::append(const T *str, std::size_t in_id)
    {
        size_t slen = strlen(str);
        if(in_id >= cap)
        {
            resize(in_id);
            len = cap;
        }
        else if(len + slen > cap)
            reserve(len + slen);

        try
        {
         T *tmp = new T[len - in_id];

         std::copy(data + in_id, data + len, tmp);
         std::copy(str, str + slen, data + in_id);
         std::copy(tmp, tmp + len - in_id, data + in_id + slen);

        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't allocate mem for appending: "<<e.what()<<std::endl;
            throw;
        }

        len += slen;
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::push_back(const T ch)
    {
        return operator+=(ch);
    }

    template<typename T, typename A>
    string_base<T, A> &string_base<T, A>::erase(std::size_t id, std::size_t len)
    {
        if(id <= 0)
            return *this;
        if(id >= this->len)
            return *this;

        if(id + len >= this->len)
        {
            for(size_t i = id; i < this->len; i++)
                data[i].~T();
            this->len = id;
        }
        else
        {

            std::copy(data + id + len, data + this->len, data + id);
            for(size_t i = id + len; i < this->len; i++)
                data[i].~T();
            this->len -= len;
        }

        return *this;


    }

    template<typename T, typename A>
    void string_base<T, A>::swap(string_base<T, A> &other)
    {
        std::swap(other.len, len);
        std::swap(other.cap, cap);
        std::swap(other.data, data);
    }

    template<typename T, typename A>
    T string_base<T, A>::pop_back()
    {
        if(len == 0)
            throw std::out_of_range("Popping from empty string;");

        len--;
        T res = data[len];
        data[len].~T();
        return res;
    }

    template<typename T, typename A>
    T *string_base<T, A>::c_str()
    {
        try
        {
            T *tmp = new T[len+1];
            std::copy(data, data + len, tmp);
            tmp[len] = '\0';
            return tmp;
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't allocate memory for c_str(): "<<e.what()<<std::endl;
            throw;
        }
    }

    template<typename T, typename A>
    void string_base<T, A>::copy(T *s, size_t len, size_t pos) const
    {
        if((pos + len) > this->len)
            throw std::out_of_range("Out of range copy;");

        std::copy(data + pos, data + pos + len, s);
    }

    template<typename T, typename A>
    string_base<T, A> string_base<T, A>::substr(std::size_t id, std::size_t len)
    {
        if(id >= this->len || (id + len) >= this->len )
            throw std::out_of_range("Out of range substring!");

        try
        {
            string_base<T, A> str;
            std::copy(data + id, data + id + len, str.data);
            return str;
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't allocate mem for substring: "<<e.what()<<std::endl;
            throw;
        }
    }

    template<typename T, typename A>
    string_base<T, A> string_base<T, A>::operator+(const string_base<T, A> &other) const
    {
        string_base<T, A> str;
        str.reserve(len + other.len);
        str += data;
        str += other.data;

        return str;
    }

    template<typename T, typename A>
    string_base<T, A> string_base<T, A>::operator+(const T *str) const
    {
        std::size_t slen = strlen(str);
        string_base<T, A> strng;
        strng.reserve(len + slen);
        strng += data;
        strng += str;

        return strng;
    }

    template<typename T, typename A>
    string_base<T, A> string_base<T, A>::operator+(const T ch) const
    {
        string_base<T, A> str;
        str.reserve(str.len + 1);
        str += data;
        str += ch;

        return str;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator==(const string_base<T, A> &rgt) const
    {
        return !strncmp(data, rgt.data, std::max(len, rgt.len));
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator==(const T *str) const
    {
        return !strncmp(data, str, std::max(len, strlen(str)));
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator==(const T ch) const
    {
        return !strncmp(data, &ch, len);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator!=(const string_base<T, A> &rgt) const
    {
        return !(*this == rgt);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator!=(const T *str) const
    {
        return !(*this == str);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator!=(const T ch) const
    {
        return !(*this == ch);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>(const string_base<T, A> &rgt) const
    {
        return strncmp(data, rgt.data, std::max(len, rgt.len)) > 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>(const T *str) const
    {
        return strncmp(data, str, std::max(len, strlen(str)) > 0);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>(const T ch) const
    {
        return strncmp(data, &ch, len) > 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<(const string_base<T, A> &rgt) const
    {
        return strncmp(data, rgt.data, std::max(len, rgt.len)) < 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<(const T *str) const
    {
        return strncmp(data, str, std::max(len, strlen(str)) < 0);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<(const T ch) const
    {
        return strncmp(data, &ch, len) < 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>=(const string_base<T, A> &rgt) const
    {
        return strncmp(data, rgt.data, std::max(len, rgt.len)) >= 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>=(const T *str) const
    {
        return strncmp(data, str, std::max(len, strlen(str)) >= 0);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator>=(const T ch) const
    {
        return strncmp(data, &ch, len) >= 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<=(const string_base<T, A> &rgt) const
    {
        return strcmp(data, rgt.data, std::max(len, rgt.len)) <= 0;
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<=(const T *str) const
    {
        return strncmp(data, str, std::max(len, strlen(str)) <= 0);
    }

    template<typename T, typename A>
    bool string_base<T, A>::operator<=(const T ch) const
    {
        return strncmp(data, &ch, len) <= 0;
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator string_base<T, A>::begin() const
    {
        return str_iterator(data);
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator string_base<T, A>::end() const
    {
        if(data)
            return str_iterator(data + len);
        else
            return str_iterator(NULL);
    }

    typedef string_base<char> string;

    template<typename T, typename A>
    string_base<T, A>::str_iterator::str_iterator(): it(NULL)
    {
    }

    template<typename T, typename A>
    string_base<T, A>::str_iterator::str_iterator(const string_base<T, A>::str_iterator &other): it(other.it)
    {
    }

    template<typename T, typename A>
    string_base<T, A>::str_iterator::str_iterator(T *p): it(p)
    {
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator &string_base<T, A>::str_iterator::operator=(const string_base<T, A>::str_iterator &other)
    {
        it = other.it;
        return *this;
    }

    template<typename T, typename A>
    string_base<T, A>::str_iterator::~str_iterator()
    {
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator &string_base<T, A>::str_iterator::operator++()
    {
        ++it;
        return *this;
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator &string_base<T, A>::str_iterator::operator--()
    {
        --it;
        return *this;
    }

    template<typename T, typename A>
    T &string_base<T, A>::str_iterator::operator*()
    {
        return *it;
    }

    template<typename T, typename A>
    bool string_base<T, A>::str_iterator::operator==(const string_base<T, A>::str_iterator &other) const
    {
        return (it == other.it);
    }

    template<typename T, typename A>
    bool string_base<T, A>::str_iterator::operator!=(const string_base<T, A>::str_iterator &other) const
    {
        return (it != other.it);
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator string_base<T, A>::str_iterator::operator++(int)
    {
        str_iterator itr(it);
        it++;
        return itr;
    }

    template<typename T, typename A>
    typename string_base<T, A>::str_iterator string_base<T, A>::str_iterator::operator--(int)
    {
        str_iterator itr(it);
        it--;
        return itr;
    }



}

tp::string operator"" _s(const char *str, std::size_t size)
{
    tp::string s;
    for(size_t i = 0 ; i < size; i++)
        s += str[i];

    return s;
}

#endif // STRING_H
