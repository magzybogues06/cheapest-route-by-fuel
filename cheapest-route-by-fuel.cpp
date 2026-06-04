#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>
#include <windows.h>
using namespace std;

// Перечисление типов ошибок

enum class ErrorType {
    NoError,                    // ошибок нет
    FileNotExist,               // файл не существует
    GraphNotUndirected,         // граф ориентированный
    HasLoops,                   // присутствует петля
    MultipleEdges,              // кратные рёбра
    VertexCountRange,           // неверное количество вершин
    VertexPriceMissing,         // отсутствует цена
    VertexPriceFormat,          // неверный формат цены
    VertexPriceRange,           // цена вне диапазона
    VertexNameMissing,          // отсутствует название
    VertexNameCase,             // заглавная буква
    VertexNameLength,           // неверная длина названия
    VertexNameChars,            // запрещённые символы
    VertexNameDuplicate,        // дублирование названий
    VertexUnexpectedAttr,       // неожиданный атрибут
    VertexLabelFormat,          // неверный формат label
    StartEndFileNotExist,       // TXT файл не существует
    StartEndFormat,             // неверный формат
    StartEndTooManyCities,      // более 2 городов
    StartEndCase,               // заглавная буква
    StartEndLength,             // неверная длина
    StartEndChars,              // запрещённые символы
    StartEndNamesMatch,         // города совпадают
    StartEndCityNotFound        // город не найден
};

// Класс Error

class Error {
public:
    ErrorType type;             // тип ошибки
    int vertexId;               // ID вершины (для ошибок в DOT)
    string extra;               // дополнительная информация

    Error() : type(ErrorType::NoError), vertexId(-1) {}   
    Error(ErrorType t, int id = -1, string ex = "") : type(t), vertexId(id), extra(ex) {}  

    // оператор сравнения для использования в set
    bool operator<(const Error& other) const {
        if (type != other.type) return (int)type < (int)other.type;
        if (vertexId != other.vertexId) return vertexId < other.vertexId;
        return extra < other.extra;
    }

    // возвращает сообщение об ошибке
    string getMessage() const {
        switch (type) {
        case ErrorType::FileNotExist: return "Ошибка: Файл \"" + extra + "\" не существует.";
        case ErrorType::GraphNotUndirected: return "Ошибка: Граф должен быть неориентированным.";
        case ErrorType::HasLoops: return "Ошибка: Присутствует петля " + to_string(vertexId) + " - " + to_string(vertexId) + ".";
        case ErrorType::MultipleEdges: return "Ошибка: В графе между вершинами присутствуют кратные связи.";
        case ErrorType::VertexCountRange: return "Ошибка: В графе должно быть минимум 2 вершины и максимум 100 вершин.";
        case ErrorType::VertexPriceMissing: return "Ошибка: В вершине " + to_string(vertexId) + " отсутствует цена на бензин.";
        case ErrorType::VertexPriceFormat: return "Ошибка: В вершине " + to_string(vertexId) + " цена на бензин записана не в виде вещественного числа с округлением до сотых после запятой.";
        case ErrorType::VertexPriceRange: return "Ошибка: В вершине " + to_string(vertexId) + " цена на бензин выходит за пределы допустимых значений (50-90).";
        case ErrorType::VertexNameMissing: return "Ошибка: В вершине " + to_string(vertexId) + " отсутствует название города.";
        case ErrorType::VertexNameCase: return "Ошибка: В вершине " + to_string(vertexId) + " название города записано с заглавной латинской буквой.";
        case ErrorType::VertexNameLength: return "Ошибка: В вершине " + to_string(vertexId) + " название города содержит " + extra + " символов.";
        case ErrorType::VertexNameChars: return "Ошибка: В вершине " + to_string(vertexId) + " в названии города встречаются запрещенные символы.";
        case ErrorType::VertexNameDuplicate: return "Ошибка: У вершин совпадают названия.";
        case ErrorType::VertexUnexpectedAttr: return "Ошибка: У вершины " + to_string(vertexId) + " встречается неожиданный атрибут.";
        case ErrorType::VertexLabelFormat: return "Ошибка: В вершине " + to_string(vertexId) + " неверный формат label.";
        case ErrorType::StartEndFileNotExist: return "Ошибка: Файл \"" + extra + "\" не существует.";
        case ErrorType::StartEndFormat: return "Ошибка: Города должны записываться через пробел.";
        case ErrorType::StartEndTooManyCities: return "Ошибка: В файле более 2 городов.";
        case ErrorType::StartEndCase: return "Ошибка: Города должны записываться в нижнем регистре.";
        case ErrorType::StartEndLength: return "Ошибка: Город должен иметь от 3 до 50 символов.";
        case ErrorType::StartEndChars: return "Ошибка: Города должны иметь в названии только буквы.";
        case ErrorType::StartEndNamesMatch: return "Ошибка: Города не должны совпадать по названию.";
        case ErrorType::StartEndCityNotFound: return "Ошибка: Введенного города \"" + extra + "\" нет в графе.";
        default: return "Неизвестная ошибка.";
        }
    }
};

// Структура City

struct City {
    int id;                     // идентификатор города
    string name;                // название города
    double price;               // цена бензина
};

// Класс Граф 
class Graph {
public:
    map<int, City> cities;                  // контейнер городов
    map<int, vector<int>> adj;              // список смежности
    set<pair<int, int>> edges;              // множество рёбер

    vector<int> bestPath;                   // найденный маршрут
    double bestCost;                        // стоимость маршрута
    set<Error> errors;                      // список ошибок

    Graph() : bestCost(0.0) {}              

    // Добавляет город в граф с проверками
    void addCity(int id, string name, double price) {
        bool hasError = false;

        // Проверка на дублирование 
        if (!name.empty()) {
            for (auto& p : cities) {
                if (p.second.name == name) {
                    errors.insert(Error(ErrorType::VertexNameDuplicate, id));
                    hasError = true;
                    break;
                }
            }
        }

        // Проверка на пустое название
        if (name.empty()) {
            errors.insert(Error(ErrorType::VertexNameMissing, id));
            hasError = true;
        }
        else {
            // Проверка длины 
            if (name.length() < 3 || name.length() > 50) {
                errors.insert(Error(ErrorType::VertexNameLength, id, to_string(name.length())));
                hasError = true;
            }

            // Проверка на заглавные буквы
            bool hasUpper = false;
            for (char c : name) {
                if (c >= 'A' && c <= 'Z') {
                    hasUpper = true;
                    break;
                }
            }
            if (hasUpper) {
                errors.insert(Error(ErrorType::VertexNameCase, id));
                hasError = true;
            }

            // Проверка на запрещённые символы
            bool hasBadChars = false;
            for (char c : name) {
                if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                    hasBadChars = true;
                    break;
                }
            }
            if (hasBadChars) {
                errors.insert(Error(ErrorType::VertexNameChars, id));
                hasError = true;
            }
        }

        // Проверка цены 
        if (price < 50.0 || price > 90.0) {
            errors.insert(Error(ErrorType::VertexPriceRange, id));
            hasError = true;
        }

        // Добавляем город всегда
        cities[id] = { id, name, price };
    }

    // Добавляет дорогу между двумя городами с проверками
    void addEdge(int a, int b) {
        // Проверка на петлю
        if (a == b) {
            errors.insert(Error(ErrorType::HasLoops, a));
            return;
        }
        // Проверка на кратное ребро
        if (edges.count({ a, b }) || edges.count({ b, a })) {
            errors.insert(Error(ErrorType::MultipleEdges));
            return;
        }
        // Добавление ребра
        edges.insert({ a, b });
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // Ищет город по названию
    int findCity(string name) {
        for (auto& p : cities) {
            if (p.second.name == name) return p.first;
        }
        return -1;
    }

    // Находит наименее затратный маршрут (Поиск в ширину + подсчёт стоимости)
    bool findPath(int start, int end, vector<int>& path, double& totalCost) {
        // Если начальный и конечный город совпадают
        if (start == end) {
            // Маршрут состоит из одного города
            // Стоимость равна нулю
            path = { start };
            totalCost = 0;
            return true;
        }

        // visited - посещённые вершины
        // parent - предыдущая вершина на пути
        map<int, bool> visited;
        map<int, int> parent;
        queue<int> q;

        // Начинаем с начального города
        q.push(start);
        visited[start] = true;
        parent[start] = -1;

        // Поиск в ширину: пока очередь не пуста
        while (!q.empty()) {
            // Извлечь вершину из очереди
            int u = q.front();
            q.pop();

            // Если достигли конечного города, то выходим
            if (u == end) break;

            // Для каждого соседа текущей вершины
            for (int v : adj[u]) {
                // Если сосед ещё не посещён
                if (!visited[v]) {
                    // Отметить как посещённый
                    visited[v] = true;
                    // Запомнить, откуда пришли
                    parent[v] = u;
                    // Добавить в очередь
                    q.push(v);
                }
            }
        }

        // Если конечный город не достигнут, значит маршрута нет
        if (!visited[end]) return false;

        // Восстановление пути от конца к началу
        path.clear();
        for (int v = end; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        // Развернуть путь (был от конца к началу)
        reverse(path.begin(), path.end());

        // Подсчёт стоимости маршрута
        totalCost = 0;
        // Стоимость = сумма цен бензина во всех городах кроме последнего
        for (size_t i = 0; i < path.size() - 1; i++) {
            totalCost += cities[path[i]].price;
        }

        // Маршрут найден
        return true;
    }
    int getCityCount() { return (int)cities.size(); }   // возвращает количество городов
    bool hasErrors() { return !errors.empty(); }        // проверяет наличие ошибок
};

// Удаляет пробелы в начале и конце строки
string trim(const string& s) {
    // Найти позицию первого непробельного символа
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";   // если все символы пробелы, то вернуть ""
    // Найти позицию последнего непробельного символа
    size_t end = s.find_last_not_of(" \t\n\r");
    // Вернуть подстроку от start до end
    return s.substr(start, end - start + 1);
}

// Проверяет, является ли символ цифрой
bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

// Форматирует цену с двумя знаками после запятой
string formatPrice(double price) {
    // Выделить целую часть
    int intPart = (int)price;
    // Выделить дробную часть (умножить на 100, округлить)
    int fracPart = (int)((price - intPart) * 100 + 0.5);
    string result = to_string(intPart) + ",";
    // Если дробная часть < 10, значит нужно добавить ведущий ноль
    if (fracPart < 10) result += "0";
    result += to_string(fracPart);
    return result;
}


// Функция чтения файла
string readFile(string filename, set<Error>& errors) {
    // Открыть файл
    ifstream file(filename);
    // Если не открылся, то добавить ошибку, вернуть ""
    if (!file.is_open()) {
        errors.insert(Error(ErrorType::FileNotExist, -1, filename));
        return "";
    }
    // Читать построчно, добавлять в строку
    string content, line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    // Закрыть файл, вернуть содержимое
    file.close();
    return content;
}

// Функция для парсинга TXT-файла и извлечения городов 
bool parseTxt(const string& content, string& start, string& end, set<Error>& errors) {
    string trimmed = trim(content);

    // Считаем количество слов (городов)
    int wordCount = 0;
    bool inWord = false;
    for (char c : trimmed) {
        if (c == ' ') {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        }
        else {
            inWord = true;
        }
    }
    if (inWord) wordCount++;

    // Если больше 2 городов — сразу ошибка и выход
    if (wordCount > 2) {
        errors.insert(Error(ErrorType::StartEndTooManyCities));
        return false;
    }

    // Если меньше 2 городов — ошибка формата
    if (wordCount < 2) {
        errors.insert(Error(ErrorType::StartEndFormat));
        return false;
    }

    // Теперь точно 2 города — разделяем
    size_t spacePos = trimmed.find(' ');
    start = trim(trimmed.substr(0, spacePos));
    end = trim(trimmed.substr(spacePos + 1));

    // Проверка начального города
    if (start.empty()) errors.insert(Error(ErrorType::StartEndFormat));
    if (start.length() < 3 || start.length() > 50) errors.insert(Error(ErrorType::StartEndLength));

    bool hasUpper = false;
    bool hasBadChar = false;
    for (char c : start) {
        if (c >= 'A' && c <= 'Z') hasUpper = true;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) hasBadChar = true;
    }
    if (hasUpper) errors.insert(Error(ErrorType::StartEndCase));
    if (hasBadChar) errors.insert(Error(ErrorType::StartEndChars));

    // Проверка конечного города
    if (end.empty()) errors.insert(Error(ErrorType::StartEndFormat));
    if (end.length() < 3 || end.length() > 50) errors.insert(Error(ErrorType::StartEndLength));

    hasUpper = false;
    hasBadChar = false;
    for (char c : end) {
        if (c >= 'A' && c <= 'Z') hasUpper = true;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) hasBadChar = true;
    }
    if (hasUpper) errors.insert(Error(ErrorType::StartEndCase));
    if (hasBadChar) errors.insert(Error(ErrorType::StartEndChars));

    // Проверка на совпадение городов
    if (start == end && !start.empty() && !end.empty()) {
        errors.insert(Error(ErrorType::StartEndNamesMatch));
    }

    return errors.empty();
}

// Функция для парсинга DOT-файла и заполнения графа 
bool parseDot(const vector<string>& lines, Graph& g) {
    for (const string& line : lines) {
        string trimmed = trim(line);
        if (trimmed.empty()) continue;
        if (trimmed == "graph G {") continue;
        if (trimmed == "}") continue;

        // Проверка на ориентированный граф
        if (trimmed.find("digraph") != string::npos) {
            g.errors.insert(Error(ErrorType::GraphNotUndirected));
            return false;
        }

        // Парсинг ребра (1 -- 2)
        if (trimmed.find("--") != string::npos) {
            size_t dashPos = trimmed.find("--");
            string left = trim(trimmed.substr(0, dashPos));
            string right = trim(trimmed.substr(dashPos + 2));
            if (!right.empty() && right.back() == ';') right.pop_back();

            try {
                int a = stoi(left);
                int b = stoi(right);
                g.addEdge(a, b);
            }
            catch (...) {}
            continue;
        }

        // Парсинг вершины (1 [label = "..."])
        if (trimmed.find("[label") != string::npos) {
            // Находим ID вершины
            size_t spacePos = trimmed.find(' ');
            if (spacePos == string::npos) spacePos = trimmed.find('[');
            if (spacePos == string::npos) continue;

            string idStr = trim(trimmed.substr(0, spacePos));
            int id;
            try { id = stoi(idStr); }
            catch (...) { continue; }

            // Проверка на лишние атрибуты
            if (trimmed.find("shape") != string::npos) {
                g.errors.insert(Error(ErrorType::VertexUnexpectedAttr, id));
                g.addCity(id, "unknown", 50.0);
                continue;
            }

            // Находим содержимое в кавычках
            size_t q1 = trimmed.find('"');
            size_t q2 = trimmed.find('"', q1 + 1);
            if (q1 == string::npos || q2 == string::npos) {
                g.errors.insert(Error(ErrorType::VertexLabelFormat, id));
                g.addCity(id, "unknown", 50.0);
                continue;
            }

            string label = trimmed.substr(q1 + 1, q2 - q1 - 1);

            // Подсчитываем количество ";"
            int semicolonCount = 0;
            for (char c : label) {
                if (c == ';') semicolonCount++;
            }

            // Если больше одного ";" — неверный формат label
            if (semicolonCount > 1) {
                g.errors.insert(Error(ErrorType::VertexLabelFormat, id));
                g.addCity(id, "unknown", 50.0);
                continue;
            }

            // Если нет ";" — тоже неверный формат
            if (semicolonCount == 0) {
                g.errors.insert(Error(ErrorType::VertexLabelFormat, id));
                g.addCity(id, "unknown", 50.0);
                continue;
            }

            // Ищем разделитель ";"
            size_t semicolon = label.find(';');

            // Название города (всё до ";")
            string cityName = trim(label.substr(0, semicolon));

            // Часть с ценой (всё после ";")
            string pricePart = trim(label.substr(semicolon + 1));

            // Проверяем, есть ли в pricePart хоть одна цифра
            bool hasDigit = false;
            string priceStr;
            for (size_t i = 0; i < pricePart.length(); i++) {
                char c = pricePart[i];
                if (c >= '0' && c <= '9') {
                    hasDigit = true;
                    priceStr = pricePart.substr(i);
                    break;
                }
            }

            // Если нет цифр — ошибка отсутствия цены
            if (!hasDigit || priceStr.empty()) {
                g.errors.insert(Error(ErrorType::VertexPriceMissing, id));
                g.addCity(id, cityName, 50.0);
                continue;
            }

            // Проверка формата: должна быть запятая и 2 цифры после неё
            size_t commaPos = priceStr.find(',');
            bool formatOk = true;

            if (commaPos == string::npos) {
                formatOk = false;
            }
            else if (commaPos + 3 > priceStr.length()) {
                formatOk = false;
            }
            else if (!isDigit(priceStr[commaPos + 1]) || !isDigit(priceStr[commaPos + 2])) {
                formatOk = false;
            }

            // Проверка, что после двух цифр нет других символов
            if (formatOk && commaPos + 3 < priceStr.length()) {
                char next = priceStr[commaPos + 3];
                if (next != ' ' && next != '\0' && next != '\r' && next != '\n') {
                    formatOk = false;
                }
            }

            if (!formatOk) {
                g.errors.insert(Error(ErrorType::VertexPriceFormat, id));
                g.addCity(id, cityName, 50.0);
                continue;
            }

            // Заменяем запятую на точку
            priceStr[commaPos] = '.';

            double price;
            try {
                price = stod(priceStr);
            }
            catch (...) {
                g.errors.insert(Error(ErrorType::VertexPriceFormat, id));
                price = 50.0;
            }

            // Добавляем город (проверки названия и цены внутри addCity)
            g.addCity(id, cityName, price);
        }
    }

    // Проверка количества вершин
    if (g.getCityCount() < 2 || g.getCityCount() > 100) {
        g.errors.insert(Error(ErrorType::VertexCountRange));
    }

    return !g.hasErrors();
}

// Функция для генерации выходного DOT-файла 
void generateDotFile(string filename, Graph& g, const vector<int>& path, double totalCost, bool hasPath) {
    // Открыть файл
    ofstream f(filename);
    if (!f.is_open()) return;

    // Записать "graph G {", служебную строку
    f << "graph G {" << endl;
    f << "    { rank = source; 0; }" << endl;

    // Если hasPath, то записать стоимость, иначе сообщение об отсутствии
    if (hasPath) {
        f << "    0 [label = \"Стоимость маршрута = " << formatPrice(totalCost)
            << "\", shape=\"box\", color=\"red\"];" << endl << endl;
    }
    else {
        f << "    0 [label = \"Путь между городами отсутствует\", shape=\"box\", color=\"red\"];" << endl << endl;
    }

    // Записать все вершины
    for (auto& p : g.cities) {
        f << "    " << p.first << " [label = \"" << p.second.name << "; цена "
            << formatPrice(p.second.price) << "\"];" << endl;
    }
    f << endl;

    // Создать множество рёбер маршрута
    set<pair<int, int>> pathEdges;
    if (hasPath) {
        for (size_t i = 0; i + 1 < path.size(); i++) {
            pathEdges.insert({ path[i], path[i + 1] });
            pathEdges.insert({ path[i + 1], path[i] });
        }
    }

    // Записать все рёбра (красным если в маршруте)
    for (auto& e : g.edges) {
        if (hasPath && pathEdges.count(e)) {
            f << "    " << e.first << " -- " << e.second << " [color=\"red\"];" << endl;
        }
        else {
            f << "    " << e.first << " -- " << e.second << ";" << endl;
        }
    }

    // Записать "}", закрыть файл
    f << "}" << endl;
    f.close();
}


int main(int argc, char* argv[]) {
    SetConsoleCP(1251);      
    SetConsoleOutputCP(1251);

    // проверка аргументов командной строки
    if (argc < 4) {
        cout << "Использование: program.exe input.dot input.txt output.dot" << endl;
        return 1;
    }

    string dotFile = argv[1];
    string txtFile = argv[2];
    string outFile = argv[3];

    cout << endl << "Поиск наименее затратного маршрута" << endl << endl;

    Graph graph;
    set<Error> txtErrors;
    string startCity, endCity;

    // чтение и парсинг DOT-файла
    string dotContent = readFile(dotFile, graph.errors);
    if (!dotContent.empty()) {
        vector<string> lines;
        stringstream ss(dotContent);
        string line;
        while (getline(ss, line)) lines.push_back(line);
        parseDot(lines, graph);
    }

    // чтение и парсинг TXT-файла
    string txtContent = readFile(txtFile, txtErrors);
    if (!txtContent.empty()) {
        parseTxt(txtContent, startCity, endCity, txtErrors);
    }

    // вывод ошибок DOT-файла
    if (graph.hasErrors()) {
        cout << endl << "Ошибки в DOT файле:" << endl;
        for (auto& e : graph.errors) cout << "  " << e.getMessage() << endl;
    }
    // вывод ошибок TXT-файла
    if (!txtErrors.empty()) {
        cout << endl << "Ошибки в TXT файле:" << endl;
        for (auto& e : txtErrors) cout << "  " << e.getMessage() << endl;
    }
    // если есть ошибки, то завершить программу
    if (graph.hasErrors() || !txtErrors.empty()) {
        cout << endl << "Программа завершена из-за ошибок." << endl;
        return 1;
    }

    // поиск ID начального и конечного города
    int startId = graph.findCity(startCity);
    int endId = graph.findCity(endCity);

    if (startId == -1) {
        cout << Error(ErrorType::StartEndCityNotFound, -1, startCity).getMessage() << endl;
        return 1;
    }
    if (endId == -1) {
        cout << Error(ErrorType::StartEndCityNotFound, -1, endCity).getMessage() << endl;
        return 1;
    }

    // поиск маршрута
    vector<int> path;
    double totalCost;

    if (graph.findPath(startId, endId, path, totalCost)) {
        cout << endl << "Стоимость маршрута = " << formatPrice(totalCost) << endl << endl;
        for (size_t i = 0; i < path.size(); i++) {
            City& c = graph.cities[path[i]];
            cout << c.name << "; цена " << formatPrice(c.price);
            if (i < path.size() - 1) cout << "  ";
        }
        cout << endl << endl;
        generateDotFile(outFile, graph, path, totalCost, true);
        cout << "Результат сохранен в: " << outFile << endl;
    }
    else {
        cout << endl << "Нет маршрута между " << startCity << " и " << endCity << endl << endl;
        for (auto& p : graph.cities) {
            cout << p.second.name << "; цена " << formatPrice(p.second.price) << endl;
        }
        cout << endl;
        generateDotFile(outFile, graph, path, totalCost, false);
        cout << "Результат сохранен в: " << outFile << endl;
    }
    return 0;
}