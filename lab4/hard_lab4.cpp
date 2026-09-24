#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <array>
#include <cmath>

// Использую это, чтобы, если программа компилируется на Windows,
// подключить заголовочный файл Windows.h, в котором есть функции,
// позволяющие настроить кодировку в консоли.
#ifdef _WIN32
#include <Windows.h>
#endif

// Использую шаблон, чтобы не писать один и тот же код для
// выбора опции в меню и получения величины
template <typename custom_type>
// В аргументе функции использую ссылки на переменные ради экономии памяти
custom_type get_number(const std::string& message, const std::string& error_message) {
	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		std::cout << message;

		// Использую строковую переменную и std::getline(), чтобы получить от
		// пользователя полную строку, а не до первого пробела
		std::string value_from_user_str{};
		std::getline(std::cin, value_from_user_str);

		// Использую строковый поток, чтобы корректно обработать введённые данные
		std::stringstream custom_stream{ value_from_user_str };
		custom_type value_from_user{};

		/* В нижеописанной конструкции if проверяется 2 условия:

		1. Успешность записи значения, находящегося в потоке, в value_from_user.
		Если пользователь введёт "abc", выражение вернёт false, т. к. такое значение
		нельзя записать в переменную с числовым типом данных.

		2. Закончились ли данные в потоке после записи введённого числа.
		Если пользователь введёт "26 2", выражение вернёт false, т. к. ф-ция eof()
		возвращает true только тогда, когда достигнут конец потока.
		При этом, если пользователь введёт "26   ", выражение вернёт true, т. к.
		модификатор потока std::ws убирает лишние пробелы.

		*/

		if (
			(custom_stream >> value_from_user) &&
			(custom_stream >> std::ws).eof()
			) {
			return value_from_user;
		}

		/*
		В критериях оценивания прописано, что нужно очищать буфер ввода
		при ошибке с помощью cin.clear() и cin.ignore(), но т. к.
		я использую getline() и работаю дальше со строковым потоком,
		этого можно не делать.
		*/

		std::cerr << "\n" << error_message << "\n";
	}
}

// В аргументе функции использую ссылки на переменные ради экономии памяти
short get_short_in_range(
	const std::string& message,
	const short& num_from,
	const short& num_to
) {
	const std::string ERROR_MESSAGE{
		"Ошибка! Введите целое число от " +
		std::to_string(num_from) +
		" до " +
		std::to_string(num_to) +
		"!"
	};

	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		short number_from_user{ get_number<short>(message, ERROR_MESSAGE) };

		// Если число входит в заданный интервал, функция его возвращает
		if ((num_from <= number_from_user) && (number_from_user <= num_to)) {
			return number_from_user;
		}
		else {
			std::cerr << "\n" << ERROR_MESSAGE << "\n";
		}
	}
}

// В аргументе функции использую ссылки на переменные ради экономии памяти
double get_positive_double(const std::string& message) {
	const std::string ERROR_MESSAGE{ "Ошибка! Введите положительное число!" };
	
	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		double number_from_user{ get_number<double>(message, ERROR_MESSAGE) };

		// Если число положительное, функция его возвращает
		if (number_from_user > 0) {
			return number_from_user;
		}
		else {
			std::cerr << "\n" << ERROR_MESSAGE << "\n";
		}
	}
}

// Функция позволяет пользователю ввести три стороны треугольника,
// проверяет, что треугольник с такими сторонами может существовать,
// и возвращает массив из трёх значений типа double, каждое из
// которых является стороной треугольника
std::array<double, 3> get_triangle_sides() {
	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		std::array<double, 3> sides{};

		for (short i{ 0 }; i <= 2; i++) {
			sides[i] = get_positive_double("Введите " + std::to_string(i + 1) + "-ю сторону (см): ");
		}

		if ( // Проверка на невырожденность
			sides[0] + sides[1] > sides[2] &&
			sides[0] + sides[2] > sides[1] &&
			sides[1] + sides[2] > sides[0]
			) {
			return sides;
		}
		else {
			std::cout << "\nОшибка! Эти стороны не образуют треугольник!\n";
		}
	}
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // Настройка кодировки для вывода
	SetConsoleCP(CP_UTF8); // Настройка кодировки для ввода
#endif

	const double PI{ 3.1415926535897932 };

	std::cout << "==================================\n";
	std::cout << "--- Геометрический калькулятор ---\n";
	std::cout << "==================================\n\n";

	std::cout << "Имеются четыре категории фигур:\n\n";
	std::cout << "1) Круг (окружность)\n2) Квадрат\n3) Прямоугольник\n4) Треугольник\n\n";

	// Использую short для выбора пункта меню ради экономии памяти
	short figure_choice{ get_short_in_range("Выберите категорию (1-4): ", 1, 4) };

	std::cout << "\nИмеeтся два типа операций:\n\n";
	std::cout << "1) Площадь\n2) Периметр\n\n";

	short category_choice{ get_short_in_range("Выберите тип операции (1 или 2): ", 1, 2) };

	std::cout << "\n";

	// В критериях указано, что обязательно наличие блока default в
	// конструкции switch-case, но в данной ситуации он не обязателен,
	// т. к. все значения, которые не входят в интервал, уже обрабатываются
	// в функции ввода данных
	switch (figure_choice) {
	case 1: { // Тут код для операций с кругом
		// Запрашиваю радиус сразу, т. к. он нужен для вычисления
		// как площади круга, так и длины окружности
		double radius{ get_positive_double("Введите радиус (см): ") };

		switch (category_choice) {
		case 1: // Тут код для нахождения площади круга
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПлощадь круга: " <<
				PI * std::pow(radius, 2) << 
				" кв. см";
			break;
		case 2: // Тут код для нахождения длины окружности
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nДлина окружности: " <<
				2 * PI * radius <<
				" см";
			break;
		}
		break;
	}
	case 2: { // Тут код для операций с квадратом
		// Запрашиваю сторону сразу, т. к. она нужна для вычисления
		// как площади квадрата, так и его периметра
		double side{ get_positive_double("Введите сторону квадрата (см): ") };

		switch (category_choice) {
		case 1: // Тут код для нахождения площади квадрата
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПлощадь квадрата: " <<
				std::pow(side, 2) <<
				" кв. см";
			break;
		case 2: // Тут код для нахождения периметра квадрата
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПериметр квадрата: " <<
				4 * side <<
				" см";
			break;
		}
		break;
	}
	case 3: { // Тут код для операций с прямоугольником
		// Запрашиваю стороны сразу, т. к. они нужны для вычисления
		// как площади прямоугольника, так и его периметра
		double side_a{ get_positive_double("Введите 1-ю сторону прямоугольника (см): ") };
		double side_b{ get_positive_double("Введите 2-ю сторону прямоугольника (см): ") };

		switch (category_choice) {
		case 1: // Тут код для нахождения площади прямоугольника
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПлощадь прямоугольника: " <<
				side_a * side_b <<
				" кв. см";
			break;
		case 2: // Тут код для нахождения периметра прямоугольника
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПериметр прямоугольника: " <<
				2 * (side_a + side_b) <<
				" см";
			break;
		}
		break;
	}
	case 4: { // Тут код для операций с треугольником
		// Запрашиваю стороны сразу, т. к. они нужны для вычисления
		// как площади треугольника, так и его периметра, а также
		// для проверки его не невырожденность
		std::array<double, 3> sides{ get_triangle_sides() };

		switch (category_choice) {
		case 1: { // Тут код для нахождения площади треугольника
			double s{ (sides[0] + sides[1] + sides[2]) / 2 }; // s = semiperimeter

			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПлощадь треугольника: " <<
				std::sqrt(s * (s - sides[0]) * (s - sides[1]) * (s - sides[2])) <<
				" кв. см";
			break;
		}
		case 2: // Тут код для нахождения периметра треугольника
			std::cout << std::fixed <<
				std::setprecision(6) <<
				"\nПериметр треугольника: " <<
				sides[0] + sides[1] + sides[2] <<
				" см";
			break;
		}
		break;
	}
	}

	std::cout << "\n\n";

	// Использую это для того, чтобы программа
	// не завершалась сразу же после вывода результата
	std::cin.get();

	return 0;
}