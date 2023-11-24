#include <iostream>
#include <functional>
#include <string>
#include <list>

const char MAX_LINE_LENGTH = 50;
const char HASH_FUNCTION_COEFFITIENT = 31;

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
struct Node {
    T data;
    Node* next;

    Node(const T& newData) : data(newData), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;

public:
    // Конструктор
    List() : head(nullptr) {}

    // Деструктор
    ~List() {
        clear();
    }

    // Добавление элемента в конец списка
    void push_back(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = newNode;
            return;
        }

        Node<T>* current = head;
        while (current->next) {
            current = current->next;
        }

        current->next = newNode;
    }

    // Очистка списка
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Удаление первого вхождения элемента из списка
    void remove(const T& value) {
        Node<T>* current = head;
        Node<T>* previous = nullptr;

        while (current) {
            if (current->data == value) {
                // Удаляем элемент из списка
                if (previous) {
                    previous->next = current->next;
                    delete current;
                    return;
                }
                else {
                    // Если удаляется первый элемент
                    Node<T>* temp = head;
                    head = head->next;
                    delete temp;
                    return;
                }
            }
            else {
                // Переходим к следующему элементу
                previous = current;
                current = current->next;
            }
        }
    }

    // Итератор для перебора элементов списка
    class Iterator {
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* start) : current(start) {}

        // Переход к следующему элементу
        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        // Получение значения текущего элемента
        T& operator*() {
            return current->data;
        }

        // Проверка на равенство
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    // Методы для работы с итератором
    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

#pragma endregion

#pragma region HASH_TABLE

class HashTable {
private:
    static const int tableSize = 10;
    List<std::pair<std::string, std::string>> table[tableSize];

    size_t hashFunction(const std::string& input) {
        size_t hashValue = 0;

        for (char ch : input) {
            hashValue = hashValue * 31 + ch;
        }

        return hashValue % tableSize;
    }

public:
    // Вставка элемента в хэш-таблицу
    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);
        table[index].push_back(std::make_pair(key, value));
    }

    // Получение значения по ключу из хэш-таблицы
    std::string get(const std::string& key) {
        int index = hashFunction(key);
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
                return entry.second;
            }
        }
        return "Ключ не найден";
    }

    // Удаление элемента по ключу из хэш-таблицы
    bool try_remove(const std::string& key) {
        int index = hashFunction(key);
        List<std::pair<std::string, std::string>>& entries = table[index];
        for (const auto& entry : entries) {
            if (entry.first == key) {
                entries.remove(entry);
                return true;
            }
        }
        return false;
    }

    // Вывод хэш-таблицы
    void displayTable() {
        for (int i = 0; i < tableSize; ++i) {
            std::cout << "Список " << i << ": ";
            for (const auto& entry : table[i]) {
                std::cout << "{" << entry.first << ": " << entry.second << "} ";
            }
            std::cout << std::endl;
        }
    }

};

#pragma endregion

// Очистка экрана консоли
void clearConsole() {
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

    HashTable myHashTable;
    int choice = -1;
    std::string input;
    std::string key;
    std::string value;

    do {
        clearConsole();

        printHeader(std::string("Меню:\n1. Добавить элемент\n2. Удалить элемент\n3. Вывести таблицу значений\n0. Выход\n"));
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

        switch (choice) {
        case 1:
            clearConsole();
            printHeader("Добавление элемента\n");
            std::cout << "Введите ключ: ";
            std::cin.get();
            std::getline(std::cin, key);
            std::cout << "Введите значение: ";
            std::getline(std::cin, value);
            myHashTable.insert(key, value);
            break;

        case 2:
            clearConsole();
            printHeader("Удаление элемента\n");
            std::cout << "Введите ключ для удаления: ";
            std::cin.get();
            std::getline(std::cin, key);
            if (myHashTable.try_remove(key))
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
            myHashTable.displayTable();
            break;

        case 0:
            std::cout << "Выход из программы.\n";
            break;

        default:
            std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
            std::cin.get();
        }

        // Ожидание ввода перед очисткой
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.get();
    } while (choice != 0);

    return 0;
}