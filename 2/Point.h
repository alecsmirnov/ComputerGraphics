#ifndef POINT_H
#define POINT_H

template <typename T>
struct Point {
	Point() = default;
	Point(T x, T y) { this->x = x; this->y = y; }

	class {		// Setter/Getter в стиле C# для поля x (обращение к полю через .)
	public:
		// Установить значение x типа T
		T& operator=(const T& x) { return this->x = x; }
		// Взять значение x типа T
		operator T() const { return x; }
	private:
		T x;
	} x;

	class {		// Setter/Getter в стиле C# для поля y (обращение к полю через .)
	public:
		// Установить значение y типа T
		T& operator=(const T& y) { return this->y = y; }
		// Взять значение y типа T
		operator T() const { return y; }
	private:
		T y;
	} y;
};

#endif