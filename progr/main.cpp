#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <SFML/Graphics.hpp>
#include <ctime>  // Для srand()
#include <set>  
using namespace std;


// Класс для представления ориентированного графа
class Graph {
private:
    int V;  // Количество вершин
    vector<list<int>> adjList;  // Список смежности
    vector<vector<int>> adjMatrix;  // Матрица смежности
    vector<pair<int, int>> edgeList;  // Список рёбер
    vector<sf::CircleShape> vertices; //верщины (для визуализации)
    vector<sf::Color> edgeColors;    // Массив цветов рёбер
    vector<sf::Text> vertexNumbers;  // Тексты с номерами вершин
    sf::Font font;

    // Функция для сравнения цветов
    struct ColorComparer {
        bool operator()(const sf::Color& lhs, const sf::Color& rhs) const {
            // Сравниваем по компонентам цвета
            return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
        }
    };
public:
    Graph(int vertices) {
        V = vertices;
        adjList.resize(V + 1);  // Индексация с 1, так что размер списка смежности будет V+1
        adjMatrix.resize(V + 1, vector<int>(V + 1, 0));  // Матрица смежности

        // Загружаем шрифт для вывода текста (путь к файлу шрифта)
        if (!font.loadFromFile("C:/sfml_build/IdealGothic Bold.otf")) {
            cout << "Не удалось загрузить шрифт!" << endl;
            exit(1);  // Завершаем программу, если шрифт не загружен
        }
    }

    // Добавление направленного ребра в граф
    void addEdge(int u, int v) {
        adjList[u].push_back(v);  // Добавление в список смежности

        adjMatrix[u][v] = 1;  // Заполнение матрицы смежности для направленного ребра

        edgeList.push_back(make_pair(u, v));  // Добавление в список рёбер
    }

    // Вывод списка смежности
    void printAdjList() {
        cout << "Список смежности:\n";
        for (int i = 1; i <= V; ++i) {  // Индексация с 1
            cout << i << ": ";
            for (int v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    // Вывод матрицы смежности
    void printAdjMatrix() {
        cout << "Матрица смежности:\n";

        // Печатаем заголовок для столбцов
        cout << "  ";  // Отступ для первой строки с номерами
        for (int i = 1; i <= V; ++i) {
            cout << i << " ";  // Номера вершин в первой строке
        }
        cout << endl;

        // Печатаем строки с номерами вершин и значениями матрицы
        for (int i = 1; i <= V; ++i) {
            cout << i << " ";  // Номер вершины в первом столбце
            for (int j = 1; j <= V; ++j) {
                cout << adjMatrix[i][j] << " ";  // Значение матрицы
            }
            cout << endl;
        }
    }

    // Вывод списка рёбер
    void printEdgeList() {
        cout << "Список рёбер:\n";
        for (auto edge : edgeList) {
            cout << "(" << edge.first << ", " << edge.second << ")\n";
        }
    }

    // Инициализация визуальных компонентов (вершин)
    void initializeVisuals(sf::RenderWindow& window) {
        float angleStep = 360.f / V;  // Угол между вершинами
        float circleRadius = 240.f;  // Радиус для расставления вершин по кругу

        // Позиции вершин
        for (int i = 0; i < V; ++i) {
            sf::CircleShape vertex(30.f);  // Радиус вершины (круг)
            vertex.setFillColor(sf::Color::Black);
            vertex.setOutlineThickness(3.f); //контур
            vertex.setOutlineColor(sf::Color::White);
            float angle = i * angleStep;
            vertex.setPosition(  // позиция цифр
                window.getSize().x / 2 + circleRadius * cos(angle * 3.14159f / 180.f) - 35.f,
                window.getSize().y / 2 + circleRadius * sin(angle * 3.14159f / 180.f) - 35.f
            );
            vertices.push_back(vertex);
            // Текст с номером вершины
            sf::Text vertexText;
            vertexText.setFont(font);
            vertexText.setString(std::to_string(i + 1));  // Номер вершины
            vertexText.setCharacterSize(24);  // Размер шрифта
            vertexText.setFillColor(sf::Color::White);  // Цвет текста
            vertexText.setPosition(
                window.getSize().x / 2 + circleRadius * cos(angle * 3.14159f / 180.f) - 5.f,
                window.getSize().y / 2 + circleRadius * sin(angle * 3.14159f / 180.f) - 10.f
            );

            // Центрируем текст относительно вершины
            vertexText.setOrigin(vertexText.getLocalBounds().width / 2.f, vertexText.getLocalBounds().height / 2.f);

            vertexNumbers.push_back(vertexText);
        }

        edgeColors.clear();
        srand(time(0));

        set<sf::Color, ColorComparer> usedColors;  // Множество для хранения уникальных цветов

        for (size_t i = 0; i < edgeList.size(); ++i) {
            sf::Color newColor;
            // Генерируем уникальный цвет
            do {
                newColor = sf::Color(rand() % 256, rand() % 256, rand() % 256);  // Генерация случайного цвета
            } while (usedColors.find(newColor) != usedColors.end());  // Проверка на уникальность

            // Добавляем новый уникальный цвет в множество
            usedColors.insert(newColor);

            // Добавляем цвет в список рёбер
            edgeColors.push_back(newColor);
        }
    }

    // Метод для рисования рёбер как треугольников
    void drawEdge(sf::RenderWindow& window, sf::Vector2f startPos, sf::Vector2f endPos, sf::Color edgeColor) {
        // Направление ребра от startPos к endPos
        sf::Vector2f direction = endPos - startPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f Unit = direction / length;  // Нормализуем вектор
        sf::Vector2f normal(-Unit.y, Unit.x);


        // Треугольник (стрелка)
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, endPos);  // Конец стрелки (голова)
        triangle.setPoint(1, startPos + normal * 33.f);  // Левый угол стрелки
        triangle.setPoint(2, startPos - normal * 33.f); // Правый угол стрелки


        // Устанавливаем цвет стрелки
        triangle.setFillColor(edgeColor);
        // Рисуем треугольник (стрелку)
        window.draw(triangle);
    }

    // Метод для визуализации графа
    void visualizeGraph(sf::RenderWindow& window) {
        // Рисуем рёбра как стрелки
        for (size_t i = 0; i < edgeList.size(); ++i) {
            int u = edgeList[i].first;
            int v = edgeList[i].second;

            // Получаем координаты вершин
            sf::Vector2f vertexPosU = vertices[u - 1].getPosition() + sf::Vector2f(30.f, 30.f); // Центр вершины u
            sf::Vector2f vertexPosV = vertices[v - 1].getPosition() + sf::Vector2f(30.f, 30.f); // Центр вершины v
            // Рисуем ребро как стрелку, передаем уникальный цвет для каждого ребра
            drawEdge(window, vertexPosU, vertexPosV, edgeColors[i]);
        }
        // Рисуем вершины
        for (size_t i = 0; i < vertexNumbers.size(); ++i) {
            window.draw(vertices[i]);  // Рисуем саму вершину
            window.draw(vertexNumbers[i]);  // Рисуем номер вершины
        }


    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    // Создаем ориентированный граф с 6 вершинами
    Graph g(6);

    // Добавляем рёбра
    g.addEdge(1, 2);
    g.addEdge(1, 6);
    g.addEdge(3, 2);
    g.addEdge(3, 4);
    g.addEdge(4, 1);
    g.addEdge(4, 3);
    g.addEdge(5, 2);
    g.addEdge(5, 6);
    g.addEdge(6, 1);

    // Выводим граф в разных представлениях
    g.printAdjList();
    cout << endl;
    g.printAdjMatrix();
    cout << endl;
    g.printEdgeList();

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML gragh");



    // Инициализируем вершины для визуализации
    g.initializeVisuals(window);

    // Главный цикл окна
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Color(50,50,50));  // Очищаем экран
        g.visualizeGraph(window);  // Рисуем граф
        window.display();  // Отображаем изменения на экране
    }

    return 0;
}