// OOPS (PPF) Mini-Project 3: Shape Area & Perimeter Calculator
// Concepts: inheritance, virtual functions, overriding, runtime polymorphism,
//           classes/objects, constructors, encapsulation.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>
using namespace std;

const double PI = 3.14159265358979323846;

// ---------- Abstract base class ----------
class Shape {
protected:
    string name;
public:
    explicit Shape(const string& n) : name(n) {}
    virtual ~Shape() {}                       // virtual destructor for safe delete
    virtual double area() const = 0;          // overridden by each shape
    virtual double perimeter() const = 0;
    virtual void display() const {            // common display, uses virtual calls
        cout << fixed << setprecision(2)
             << "  Shape     : " << name << "\n"
             << "  Area      : " << area() << "\n"
             << "  Perimeter : " << perimeter() << "\n";
    }
    string getName() const { return name; }
};

// ---------- Circle ----------
class Circle : public Shape {
    double radius;                            // private: encapsulation
public:
    explicit Circle(double r) : Shape("Circle"), radius(r) {}
    double area() const override      { return PI * radius * radius; }
    double perimeter() const override { return 2 * PI * radius; }
};

// ---------- Rectangle ----------
class Rectangle : public Shape {
    double length, width;
public:
    Rectangle(double l, double w) : Shape("Rectangle"), length(l), width(w) {}
    double area() const override      { return length * width; }
    double perimeter() const override { return 2 * (length + width); }
};

// ---------- Triangle (given three sides) ----------
class Triangle : public Shape {
    double a, b, c;
public:
    Triangle(double x, double y, double z) : Shape("Triangle"), a(x), b(y), c(z) {}
    double area() const override {            // Heron's formula
        double s = (a + b + c) / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }
    double perimeter() const override { return a + b + c; }
    static bool isValid(double x, double y, double z) {
        return x + y > z && x + z > y && y + z > x;
    }
};

// ---------- Input helpers ----------
double readPositive(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v && v > 0 && isfinite(v)) return v;
        if (cin.eof()) { cout << "\nInput ended.\n"; exit(1); }
        cout << "  Invalid input. Enter a number greater than 0.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readChoice(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) return v;
        if (cin.eof()) { cout << "\nInput ended.\n"; exit(1); }
        cout << "  Invalid choice. Enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void printMenu() {
    cout << "\n===== Shape Area & Perimeter Calculator =====\n"
         << " 1. Circle\n 2. Rectangle\n 3. Triangle\n"
         << " 4. Show all shapes entered (polymorphism demo)\n"
         << " 5. Exit\n";
}

int main() {
    vector<unique_ptr<Shape>> shapes;         // base-class pointers -> runtime polymorphism
    while (true) {
        printMenu();
        int choice = readChoice("Enter choice: ");
        Shape* current = nullptr;             // base-class pointer

        if (choice == 1) {
            double r = readPositive("  Radius: ");
            shapes.push_back(make_unique<Circle>(r));
            current = shapes.back().get();
        } else if (choice == 2) {
            double l = readPositive("  Length: ");
            double w = readPositive("  Width : ");
            shapes.push_back(make_unique<Rectangle>(l, w));
            current = shapes.back().get();
        } else if (choice == 3) {
            double a = readPositive("  Side a: ");
            double b = readPositive("  Side b: ");
            double c = readPositive("  Side c: ");
            if (!Triangle::isValid(a, b, c)) {
                cout << "  Invalid triangle: sum of any two sides must exceed the third.\n";
                continue;
            }
            shapes.push_back(make_unique<Triangle>(a, b, c));
            current = shapes.back().get();
        } else if (choice == 4) {
            if (shapes.empty()) cout << "  No shapes entered yet.\n";
            int i = 1;
            for (const auto& s : shapes) {    // same call, different behaviour per object
                cout << "\n[" << i++ << "]\n";
                s->display();
            }
            continue;
        } else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "  Invalid choice. Pick 1-5.\n";
            continue;
        }
        cout << "\n";
        current->display();                   // runtime polymorphism via Shape*
    }
    return 0;
}