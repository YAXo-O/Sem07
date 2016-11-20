#ifndef MAP_H
#define MAP_H

#include<iostream>
#include<stdexcept>

namespace tp{

    template<typename val, typename key>
    struct data
    {
        val v_data;
        key k_data;
    };

    template<typename val, typename key>
    struct tree_node
    {
        data<val, key> buff;
        tree_node *right = NULL;
        tree_node *left = NULL;
        tree_node *parent = NULL;
    };

    template<typename val, typename key>
    void treeDel(tree_node<val, key> *node)
    {
        if(!node)
            return;
            //throw std::range_error("Empty node");

        if(node->left != NULL)
            treeDel(node->left);
        if(node->right != NULL)
            treeDel(node->right);
        delete node;
    }

    template<typename val, typename key>
    void treeCopy(tree_node<val, key> *n_node, tree_node<val, key> *ref_node)
    {
        if(!n_node)
            return;
            //throw std::range_error("Empty node");

        if(ref_node->left != NULL)
        {
            try
            {
                tree_node<val, key> *tmp = new tree_node<val ,key>;
                treeCopy(tmp, ref_node->left);
                n_node->left = tmp;
            }
            catch(std::bad_alloc &e)
            {
                std::cout<<"Can't allocate mem for tree node: "<<e.what()<<std::endl;
                throw;
            }
        }

        n_node->buff = ref_node->buff;

        if(ref_node->right != NULL)
        {
            try
            {
                tree_node<val, key> *tmp = new tree_node<val ,key>;
                treeCopy(tmp, ref_node->right);
                n_node->right = tmp;
            }
            catch(std::bad_alloc &e)
            {
                std::cout<<"Can't allocate mem for tree node: "<<e.what()<<std::endl;
                throw;
            }
        }

    }

    template<typename val, typename key>
    void treePrint(tree_node<val, key> *node)
    {
        if(!node)
            return;

        if(node->left != NULL)
            treePrint(node->left);

        std::cout<<"--------"<<std::endl;
        std::cout<<"Value: "<<node->buff.v_data<<std::endl;
        std::cout<<"Key: "<<node->buff.k_data<<std::endl;

        if(node->right != NULL)
            treePrint(node->right);
    }

    template<typename val, typename key>
    void treeFind(tree_node<val, key> *node, const key &id, val *(&res))
    {
        if(!node || res)
            return;

        if(node->buff.k_data == id)
        {

            res = &(node->buff.v_data);
            return;
        }

        if(node->left != NULL && node->buff.k_data > id)
            treeFind(node->left, id, res);
        if(node->right != NULL && node->buff.k_data < id)
            treeFind(node->right, id, res);

    }

    template<typename val, typename key>
    void treeErase(tree_node<val, key> *(&node), key id, bool &bGo)
    {
        if(!node || !bGo)
            return;

        if(node->buff.k_data == id)
        {

            tree_node<val, key> *tmp = node->right;
            if(tmp != NULL)
            {
                while(tmp->left != NULL)
                    tmp = tmp->left;

                tmp->left = node->left;

                tree_node<val, key> *td = node;
                node = node->right;
                delete td;
                bGo = false;
                return;
            }
            else
            {
                tree_node<val, key> *td = node;
                node = node->left;
                delete td;
            }
        }

        if(node->left != NULL && id < node->buff.k_data)
            treeErase(node->left, id, bGo);
        if(node->right != NULL && id > node->buff.k_data)
            treeErase(node->right, id, bGo);
    }

    template<typename val, typename key>
    void treeInsert(tree_node<val, key> *(&node), const val &d_value, const key &d_key, bool &bGo)
    {
        if(!node || !bGo)
            return;

        if(node->buff.k_data == d_key)
        {
            node->buff.v_data = d_value;
            bGo = false;
        }

        if(d_key < node->buff.k_data)
        {
            if(node->left != NULL)
                treeInsert(node->left, d_value, d_key, bGo);
            else
            {
                try
                {
                    tree_node<val, key> *n_node = new tree_node<val, key>;
                    n_node->buff.k_data = d_key;
                    n_node->buff.v_data = d_value;
                    n_node->parent = node;
                    node->left = n_node;
                    bGo = false;
                }
                catch(std::bad_alloc &e)
                {
                    std::cout<<"Can't allocate memory for insertion: "<<e.what()<<std::endl;
                    throw;
                }
            }
        }
        if(d_key > node->buff.k_data)
        {
            if(node->right != NULL)
                treeInsert(node->right, d_value, d_key, bGo);
            else
            {
                try
                {
                    tree_node<val, key> *n_node = new tree_node<val, key>;
                    n_node->buff.k_data = d_key;
                    n_node->buff.v_data = d_value;
                    n_node->parent = node;
                    node->right = n_node;
                    bGo = false;
                }
                catch(std::bad_alloc &e)
                {
                    std::cout<<"Can't allocate memory for insertion: "<<e.what()<<std::endl;
                    throw;
                }
            }
        }


    }

    template<typename val, typename key>
    void treeClear(tree_node<val, key> *node)
    {
        if(!node)
            return;

        if(node->left != NULL)
            treeClear(node->left);
        if(node->right != NULL)
            treeClear(node->right);

        delete node;
    }

    template<typename val, typename key>
    void treeCount(tree_node<val, key> *node, size_t &size)
    {
        if(!node)
            return;

        size++;
        if(node->left != NULL)
            treeCount(node->left, size);
        if(node->right != NULL)
            treeCount(node->right, size);

    }

    template<typename val, typename key>
    class map
    {
    private:
        tree_node<val, key> *root;
    public:
        map();
        map(const map &other);
        map &operator=(map other);
        map(const std::initializer_list<data<val, key> > &list);
        map(map &&other);
        map &operator=(map &&other);
        ~map();

        bool empty() const;
        std::size_t size() const;

        val &operator[](const key id);
        val &at(const key id);

        val &insert(const val value, const key id);
        void erase(const key id);
        void swap(map &x);
        void clear();

        std::size_t count(const key id) const;// сделать
        void print();

        //template<typename val1, typename key1, typename val2, typename key2>
        //friend map<decltype(val1 + val2),decltype(key1+key2)> operator+(map<val1, key1> m1, map<val2, key2> m2);

        class map_iterator
        {
        private:
            tree_node<val, key> *ptr;
        public:
            map_iterator();
            map_iterator(const map_iterator &other);
            map_iterator(tree_node<val, key> *p);
            map_iterator &operator=(const map_iterator &other);
            map_iterator &operator=(const tree_node<val, key> *p);
            ~map_iterator();


            map_iterator operator++();
            map_iterator operator++(int);

            bool operator==(const map_iterator &other) const;
            bool operator!=(const map_iterator &other) const;

            data<val, key> &operator*();
        };

        map_iterator begin();
        map_iterator end();

        map(map_iterator fst, map_iterator lst);
    };

    template<typename val, typename key>
    map<val, key>::map():  root(NULL)
    {
    }

    template<typename val, typename key>
    map<val, key>::map(const map &other)
    {
        try
        {
            root = new tree_node<val, key>;
        }
        catch(std::bad_alloc &e)
        {
            std::cout<<"Can't allocate memory for tree root: "<<e.what()<<std::endl;
            throw;
        }

        treeCopy(root, other.root);
    }

    template<typename val, typename key>
    map<val, key>::~map()
    {
        if(root != NULL)
            treeDel(root);

    }

    template<typename val, typename key>
    map<val, key> &map<val, key>::operator=(map other)
    {
        swap(other);
    }


    template<typename val, typename key>
    map<val, key>::map(const std::initializer_list<data<val, key>> &list): root(NULL)
    {
        for(auto elem : list)
            insert(elem.v_data, elem.k_data);
    }

    template<typename val, typename key>
    map<val, key>::map(map<val, key> &&other)
    {
        root = other.root;
        other.root = NULL;
    }

    template<typename val, typename key>
    map<val, key> &map<val, key>::operator=(map<val, key> &&other)
    {
        if(root)
            treeDel(root);

        root = other.root;
        other.root = NULL;

        return *this;
    }

    template<typename val, typename key>
    bool map<val, key>::empty() const
    {
        return  root == NULL;
    }

    template<typename val, typename key>
    std::size_t map<val, key>::size() const
    {
        size_t size = 0;
        if(root)
        {
            treeCount(root, size);
            return size;
        }

        return 0;
    }

    template<typename val, typename key>
    val &map<val, key>::operator[](const key id)
    {
        val *search = NULL;
        treeFind(root, id, search);

        return (search != NULL) ? *search : insert(val(), id);
    }

    template<typename val, typename key>
    val &map<val, key>::at(const key id)
    {
        val *search = NULL;
        treeFind(root, id, search);
        if(!search)
            throw std::range_error("At() - out of range!");

    }

    template<typename val, typename key>
    val &map<val, key>::insert(const val value, const key id)
    {
        bool bGo = true;

        if(root == NULL)
        {
            try
            {
                root = new tree_node<val, key>;
                root->buff.k_data = id;
                root->buff.v_data = value;
                root->parent = NULL;
            }
            catch(std::bad_alloc &e)
            {
                std::cout<<"Can't allocate mem for root: "<<e.what()<<std::endl;
                throw;
            }

            return root->buff.v_data;
        }
        treeInsert(root, value, id, bGo);

        val *res = NULL;
        treeFind(root, id, res);

        return *res;
    }

    template<typename val, typename key>
    void map<val, key>::erase(const key id)
    {
        bool bGo = true;
        treeErase(root, id, bGo);
    }

    template<typename val, typename key>
    void map<val, key>::swap(map &x)
    {
        std::swap(root, x.root);
    }

    template<typename val, typename key>
    void map<val, key>::clear()
    {
        treeClear(root);
        root = NULL;
    }

    template<typename val, typename key>
    void map<val, key>::print()
    {
        std::cout<<"~~~"<<std::endl;
        std::cout<<"size: "<<size()<<std::endl;
        treePrint(root);
        std::cout<<"~~~"<<std::endl;
    }

    template<typename val, typename key>
    typename map<val, key>::map_iterator map<val, key>::begin()
    {
        if(!root)
            return map_iterator();


        tree_node<val, key> *node = root;
        while(node->left != NULL)
            node = node->left;
        map_iterator it(node);
        return it;
    }

    template<typename val, typename key>
    typename map<val, key>::map_iterator map<val, key>::end()
    {
        return map_iterator();
    }

    template<typename val, typename key>
    map<val, key>::map(map::map_iterator fst, map::map_iterator lst)
    {
        for(auto it = fst; it != lst; it++)
            insert((*it).v_data, (*it).k_data);
    }

    template<typename val, typename key>
    map<val, key>::map_iterator::map_iterator(): ptr(NULL)
    {
    }

    template<typename val, typename key>
    map<val, key>::map_iterator::map_iterator(const map<val, key>::map_iterator &other): ptr(other.ptr)
    {
    }

    template<typename val, typename key>
    map<val, key>::map_iterator::map_iterator(tree_node<val, key> *p): ptr(p)
    {
    }

    template<typename val, typename key>
    typename map<val, key>::map_iterator &map<val ,key>::map_iterator::operator=(const map<val, key>::map_iterator &other)
    {
        ptr = other.ptr;
        return *this;
    }

    template<typename val, typename key>
    typename map<val, key>::map_iterator &map<val, key>::map_iterator::operator=(const tree_node<val, key> *p)
    {
        ptr = p;
        return *this;
    }

    template<typename val, typename key>
    map<val, key>::map_iterator::~map_iterator()
    {
    }

    template<typename val, typename key>
    typename map<val, key>::map_iterator map<val, key>::map_iterator::operator++()
    {
        if(!(ptr->right) && (!(ptr->parent) || ptr->parent->buff.k_data < ptr->buff.k_data ))
        {
           ptr = NULL;
           return *this;
        }

        if(ptr)
        {
            if(ptr->right != NULL)
            {
                ptr = ptr->right;
                while(ptr->left != NULL)
                    ptr = ptr->left;
                return *this;
            }
            else
            {
                if(ptr->parent == NULL)
                {
                    ptr = NULL;
                    return *this;
                }
                else
                {
                    key value = ptr->buff.k_data;
                    while(ptr->right == NULL && ptr->buff.k_data <= value)
                    {
                        ptr = ptr->parent;
                        if(ptr == NULL)
                        {
                            return *this;
                        }

                    }
                    return *this;
                }

            }
        }
        return *this;
    }


    template<typename val, typename key>
    typename map<val, key>::map_iterator map<val ,key>::map_iterator::operator++(int)
    {
        map_iterator tmp(ptr);
        operator++();
        return tmp;
    }

    template<typename val, typename key>
    bool map<val, key>::map_iterator::operator==(const map<val, key>::map_iterator &other) const
    {
        return (ptr == other.ptr);
    }

    template<typename val, typename key>
    bool map<val, key>::map_iterator::operator!=(const map<val, key>::map_iterator &other) const
    {
        return ptr != other.ptr;
    }

    template<typename val, typename key>
    data<val, key> &map<val, key>::map_iterator::operator*()
    {
        return ptr->buff;
    }
}

#endif // MAP_H
