#include <iostream>
#include <iomanip>
#include <string>
#include <list>

const char MAX_LINE_LENGTH = 50;
const char HASH_FUNCTION_COEFFITIENT = 31;
const char INIT_TABLE_SIZE = 10;
const char RESIZE_FACTOR = 2;


bool isNumber(std::string value)
{
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!isdigit(value[i]))
        {
            return false;
        }
    }
    return true;
}

#pragma region LIST

template <typename T>
struct Node 
{
    T data;
    Node* next;

    Node(const T& newData) : data(newData), next(nullptr) {}
};

template <typename T>
class List 
{
private:
    Node<T>* head;

public:
    List() : head(nullptr) {}
    ~List() 
    {
        clear();
    }

    void push_back(const T& data) 
    {
        Node<T>* newNode = new Node<T>(data);
        if (!head) 
        {
            head = newNode;
            return;
        }

        Node<T>* current = head;
        while (current->next) 
        {
            current = current->next;
        }

        current->next = newNode;
    }

    void clear() 
    {
        while (head) 
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void remove(const T& value) 
    {
        Node<T>* current = head;
        Node<T>* previous = nullptr;

        while (current) 
        {
            if (current->data == value) 
            {
                if (previous) 
                {
                    previous->next = current->next;
                    delete current;
                    return;
                }
                else 
                {
                    Node<T>* temp = head;
                    head = head->next;
                    delete temp;
                    return;
                }
            }
            else 
            {
                previous = current;
                current = current->next;
            }
        }
    }

    class Iterator 
    {
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* start) : current(start) {}

        Iterator& operator++() 
        {
            if (current) 
            {
                current = current->next;
            }
            return *this;
        }

        T& operator*() 
        {
            return current->data;
        }

        bool operator!=(const Iterator& other) const 
        {
            return current != other.current;
        }
    };

    Iterator begin() 
    {
        return Iterator(head);
    }

    Iterator end() 
    {
        return Iterator(nullptr);
    }
};

#pragma endregion

typedef List<std::pair<std::string, std::string>> HashList;

#pragma region HASH_TABLE

class HashTable 
{
private:
    int tableSize = 10;
    HashList* table;

    size_t hashFunction(const std::string& input) 
    {
        size_t hashValue = 0;

        for (char ch : input) 
        {
            hashValue = hashValue * 31 + ch;
        }

        return hashValue % tableSize;
    }

public:
    HashTable() : tableSize(INIT_TABLE_SIZE)
    {
        table = new HashList[tableSize];
    }

    void rehash() 
    {
        int newSize = tableSize * RESIZE_FACTOR + 1;

        HashList* newTable = new HashList[newSize];

        for (size_t i = 0; i < tableSize; i++)
        {
            for(const auto& entry : table[i])
            {
                int rehash = hashFunction(entry.first);
                newTable[rehash].push_back(entry);
            }
        }

        delete[] table;
        table = std::move(newTable);
        tableSize = newSize;
    }

    void insert(const std::string& key, const std::string& value) 
    {
        int index = hashFunction(key);
        table[index].push_back(std::make_pair(key, value));
    }

    std::string get(const std::string& key) 
    {
        int index = hashFunction(key);
        for (const auto& entry : table[index])
        {
            if (entry.first == key) 
            {
                return entry.second;
            }
        }
        return "Ключ не найден";
    }

    bool try_remove(const std::string& key) 
    {
        int index = hashFunction(key);
        List<std::pair<std::string, std::string>>& entries = table[index];
        for (const auto& entry : entries) 
        {
            if (entry.first == key) 
            {
                entries.remove(entry);
                return true;
            }
        }
        return false;
    }

    void displayTable() 
    {
        for (int i = 0; i < tableSize; ++i) 
        {
            std::cout  << "Индекс " << std::setw(5) << i << ": ";
            for (const auto& entry : table[i])
            {
                std::cout << "{" << entry.first << ": " << entry.second << "} ";
            }
            std::cout << std::endl;
        }
    }
};

#pragma endregion

void clearConsole() 
{
    system("cls");
}

void printHeader(const std::string& header)
{
    std::cout << "\n----------------------------\n\n";
    std::cout << header;
    std::cout << "\n----------------------------\n\n";
}

int main() {
    setlocale(LC_ALL, "RU");

    HashTable hashTable;
    int choice = -1;
    std::string input;
    std::string key;
    std::string value;

    do 
    {
        clearConsole();

        printHeader(std::string("Меню:\n1. Добавить элемент\n2. Удалить элемент\n3. Вывести таблицу значений\n4. Выполнить рехэширование\n0. Выход\n"));
        std::cout << "Выберите действие: ";

        std::cin >> input;
        if (isNumber(input))
        {
            choice = atoi(input.c_str());
        }
        else
        {
            choice = -1;
        }

        switch (choice) 
        {
        case 1:
            clearConsole();
            printHeader("Добавление элемента\n");
            std::cout << "Введите ключ: ";
            std::cin.get();
            std::getline(std::cin, key);
            std::cout << "Введите значение: ";
            std::getline(std::cin, value);
            hashTable.insert(key, value);
            break;

        case 2:
            clearConsole();
            printHeader("Удаление элемента\n");
            std::cout << "Введите ключ для удаления: ";
            std::cin.get();
            std::getline(std::cin, key);
            if (hashTable.try_remove(key))
            {
                std::cout << "Ключ \"" + key + "\" успешно удален\n";
            }
            else
            {
                std::cout << "Ключ \"" + key + "\" не найден\n";
            }
            break;

        case 3:
            clearConsole();
            printHeader("Вывод таблицы значений\n");
            hashTable.displayTable();
            std::cin.get();
            break;

        case 4:
            clearConsole();
            printHeader("Рехэширование таблицы\n");

            std::cout << "Введите:\nY - выполнить рехэширование\n";
            std::cin >> input;
            if (input == "Y")
            {
                hashTable.rehash();
                std::cout << "Рехэширование было успешно выполнено";
            }
            else
            {
                std::cout << "Неправильный ввод. Рехэширование не было выполнено";
            }

            std::cin.get();
            break;

        case 0:
            std::cout << "Выход из программы.\n";
            break;

        default:
            std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
            std::cin.get();
        }

        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.get();
    } while (choice != 0);

    return 0;
}