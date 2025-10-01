#include <iostream>
#include <cctype>
#include <string>
#include <chrono>
#include <random>
#include <vector>

template<typename T>
class BinTree
{
public:
    BinTree() : root(nullptr) {}
    ~BinTree()
    {
        freeTree(root);
        root = nullptr;
    }

    void Clear()
    {
        freeTree(root);
        root = nullptr;
    }

    void insert(T data)
    {
        root = insert(root, data);
    }

    void print()
    {
        if (root == nullptr)
        {
            std::cout << "Дерево пусто" << std::endl;
            return;
        }
        print(root, 0);
    }

    bool search(T data)
    {
        return search(root, data) != nullptr;
    }

    int countEntry(T data)
    {
        return countEntry(root, data);
    }

private:
    template<typename T2>
    class Node
    {
    public:
        T2 data;
        Node<T2>* left;
        Node<T2>* right;
        Node(T2 data) : data(data), left(nullptr), right(nullptr) {}
    };

    Node<T>* insert(Node<T>* root, T data)
    {
        if (root == nullptr)
        {
            return new Node<T>(data);
        }
        //
        if (data == root->data)
        {
            std::cout << "Значение " << data << " уже существует" << std::endl;
            return root;
        }
        //
        if (data < root->data)
            root->left = insert(root->left, data);
        else
            root->right = insert(root->right, data);
        return root;
    }

    Node<T>* search(Node<T>* root, T data)
    {
        if (root == nullptr || root->data == data)
            return root;
        if (data < root->data)
            return search(root->left, data);
        else
            return search(root->right, data);
    }

    int countEntry(Node<T>* root, T data)
    {
        if (root == nullptr)
            return 0;
        int count = (root->data == data) ? 1 : 0;
        count += countEntry(root->left, data);
        count += countEntry(root->right, data);
        return count;
    }

    void freeTree(Node<T>* root)
    {
        if (root == nullptr)
            return;
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }

    void print(Node<T>* root, int l)
    {
        if (root == nullptr)
            return;
        print(root->right, l + 1);
        for (int i = 0; i < l; i++)
            std::cout << "   ";
        std::cout << root->data << std::endl;
        print(root->left, l + 1);
    }

    Node<T>* root;
};

template<typename T>
T failInit()
{
    std::string enter;
    int dot = 0;

    while (true)
    {

    bool valide = true;
    std::getline(std::cin, enter);

    if (enter.size() < 10)
    {
        for (int i = 0; i < enter.size(); i++)
        {
            if (!(i == 0 && enter[0] == '-'))
            {
                if (!std::isdigit(enter[i]) && !(enter[i] == '.' || enter[i] == ','))
                {
                    valide = false;
                    break;
                }else if (enter[i] == '.' || enter[i] == ',')
                {
                    enter[i] = ','; // запятая в русской раскладке .... , в остальных случаях точка
                    ++dot;
                }
                else if (dot > 1)
                {
                    valide = false;
                    break;
                }
            }
        }
    }
    else
    {
        valide = false; // слишком длнная строка
    }
        if(valide) break;

    std::cout << "Некорректный ввод" << std::endl;
    std::cin.clear();
    }
    
    return static_cast<T>(std::stod(enter));
}

void timeBt(BinTree<int>& other, int data)
{
    auto start = std::chrono::high_resolution_clock::now();
    other.search(data);
    auto end = std::chrono::high_resolution_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Время поиска: " << result.count() << " мкс" << std::endl;
}

void testBt()
{
    BinTree<int> bt;
    std::random_device rd;
    std::mt19937 gen(rd());
    int value[5] = { 100, 200, 400, 800, 1600 };

    for (int i = 0; i < 5; i++)
    {
        for (int n = 0; n < value[i]; n++)
        {
            bt.insert(n); 
        }

        std::cout << "Элементов в дереве: " << value[i] << std::endl;
        timeBt(bt, value[i] - 1); 
        bt.Clear(); 
    }
    std::cout << '\n';
    for (int i = 0; i < 5; i++)
    {
        std::vector<int> rndValue;
        std::uniform_int_distribution<int> dist(1, value[i]);
        for (int n = 0; n < value[i]; n++)
        {
            rndValue.push_back(n);
        }
        std::shuffle(rndValue.begin(), rndValue.end(), gen);
        for (int n = 0; n < value[i]; n++)
        {
            bt.insert(rndValue[n]);
        }

        std::cout << "Элементов в случайном дереве: " << value[i] << std::endl;
        timeBt(bt, value[i] - 1); 
        bt.Clear(); 
    }
}


int main()
{
    using Type = int;
    setlocale(LC_ALL, "RUSSIAN");

    //testBt();

    BinTree<Type> bt;
    Type data, search_value;
    bool start = true;

    std::cout << "-1 окончание построения дерева" << std::endl;
    while (start)
    {
        std::cout << "Введите число: ";
        //std::getline(std::cin, data);
        data = failInit<Type>();
        if (data == -1)
        {
            std::cout << "Построение дерева окончено\n" << std::endl;
            start = false;
        }
        else
        {
            bt.insert(data);
        }
    }

    std::cout << "Дерево (повернуто на 90 градусов): " << std::endl;
    bt.print();

    std::cout << "Введите значение для поиска: ";
    //std::getline(std::cin, search_value);
    std::cin >> search_value;
    if (bt.search(search_value))
        std::cout << "Значение " << search_value << " найдено в дереве" << std::endl;
    else
        std::cout << "Значение " << search_value << " не найдено в дереве" << std::endl;

    std::cout << "Введите значение для подсчета вхождений: ";
    std::cin >> search_value;
    int count = bt.countEntry(search_value);
    std::cout << "Значение " << search_value << " встречается " << count << " раз(а)" << std::endl;

    return 0;
}
