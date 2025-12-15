#include "Graph.h"
#include "Simple_window.h"
#include <cmath>

using namespace Graph_lib;

// Рекурсивная функция построения дерева Пифагора
void pythagoras_tree(
    Point p,          // левый нижний угол квадрата
    double size,      // длина стороны
    double angle,     // угол поворота
    int depth,        // глубина рекурсии
    Vector_ref<Shape>& shapes
)
{
    if (depth == 0) return;

    // Векторы сторон квадрата
    double dx = size * cos(angle);
    double dy = size * sin(angle);

    double px = -size * sin(angle);
    double py =  size * cos(angle);

    // Вершины квадрата
    Point p1 = p;
    Point p2(p.x + dx, p.y - dy);
    Point p3(p2.x + px, p2.y - py);
    Point p4(p.x + px, p.y - py);

    Polygon* square = new Polygon;
    square->add(p1);
    square->add(p2);
    square->add(p3);
    square->add(p4);

    square->set_color(Color::dark_green);
    square->set_fill_color(Color::green);

    shapes.push_back(square);

    // Точки для новых квадратов
    Point left_base = p4;
    Point right_base = p3;

    double new_size = size * 0.7;

    // Левый и правый квадраты
    pythagoras_tree(left_base, new_size, angle + M_PI / 4, depth - 1, shapes);
    pythagoras_tree(right_base, new_size, angle - M_PI / 4, depth - 1, shapes);
}

int main()
{
    Simple_window win(Point(100, 100), 800, 600, "Pythagoras Tree");

    Vector_ref<Shape> shapes;

    Point start(350, 500);
    double size = 100;
    int depth = 8;

    pythagoras_tree(start, size, 0, depth, shapes);

    for (int i = 0; i < shapes.size(); ++i)
        win.attach(shapes[i]);

    win.wait_for_button();
    return 0;
}
