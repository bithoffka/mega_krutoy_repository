#include <algorithm>
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

// В аргументе функции использую ссылки ради экономии памяти
double get_double(const std::string& message, const std::string& error_message) {
	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		std::cout << message;

		// Использую строковую переменную и std::getline(), чтобы получить от
		// пользователя полную строку, а не до первого пробела
		std::string value_from_user_str{};
		std::getline(std::cin, value_from_user_str);

		// Использую строковый поток, чтобы корректно обработать введённые данные
		std::stringstream custom_stream{ value_from_user_str };
		double value_from_user{};

		/* В нижеописанной конструкции if проверяется 2 условия:

		1. Успешность записи значения, находящегося в потоке, в value_from_user.
		Если пользователь введёт "abc", выражение вернёт false, т. к. такое значение
		нельзя записать в переменную типа double.

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

// Структура, которая позволяет логически объединить параметры табуляции в один объект
struct TabulationValues {
	double start, end, step;
};

// Функция, которая обрабатывает получение значений для табуляции
TabulationValues get_function_tabulation_values() {
	TabulationValues params{};

	// Использую строковые константы, т. к. этот текст не меняется и при этом используется несколько раз
	const std::string A_ERROR{ "Ошибка! Введите одно число, соответствующее значению A (началу)!" };
	const std::string B_ERROR{ "Ошибка! Введите одно число, соответствующее значению B (концу)!\nПри этом B должно быть обязательно больше A!" };
	const std::string H_ERROR{ "Ошибка! Введите одно число, соответствующее значению H (шагу)!\nПри этом H должно быть обязательно положительным!" };

	// Для ввода значения A нет цикла, т. к. для него нет никаких дополнительных условий
	params.start = get_double("Введите значение A: ", A_ERROR);

	while (true) {
		params.end = get_double("Введите значение B: ", B_ERROR);

		if (params.start < params.end) {
			break;
		}

		std::cerr << "\n" << B_ERROR << "\n";
	}

	while (true) {
		params.step = get_double("Введите значение H: ", H_ERROR);

		if (params.step > 0) {
			break;
		}

		std::cerr << "\n" << H_ERROR << "\n";
	}

	return params;
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // Настройка кодировки для вывода
	SetConsoleCP(CP_UTF8); // Настройка кодировки для ввода
#endif

	const double EPSILON{ 1e-9 };

	std::cout << "==================================\n";
	std::cout << "--- Табуляция функции косинуса ---\n";
	std::cout << "==================================\n\n";

	std::cout << "Необходимо установить значения, чтобы начать табуляцию функции!\n\n";
	std::cout << "A - начальное значение\n";
	std::cout << "B - конечное значение\n";
	std::cout << "H - шаг табуляции\n\n";

	TabulationValues params{ get_function_tabulation_values() };

	// У косинуса минимально возможное значение это -1, а
	// максимально возможное это 1, значит нужно установить
	// эти значения наоборот
	double min_value{ 1 };
	double max_value{ -1 };

	// Далее в коде производится настройка точности. Это нужно, чтобы даже
	// при аномально малых и больших числах таблица не разваливалась.
	// В данном случае precision - это сколько будет знаков после запятой, а
	// width - это общая ширина. Стандартная точность - 4
	short precision{ 4 };

	// Если шаг меньше 1, то стандартной точности может не хватить
	if (params.step < 1) {
		precision = (std::max)(
			static_cast<short>(4), // Гарантия того, что точность будет как минимум 4
			static_cast<short>(std::ceil(-std::log10(params.step))) // log10 от шага позволяет узнать количество нулей после запятой
			);
	}

	// Тут производится динамическое вычисление ширины колонки таблицы:
	// Гарантируется минимальная ширина в 18 символов. Если ширины не хватает, то
	// беру вычисленную точность и ещё 15 символов про запас (на целую часть,
	// минус и точку)
	short width{ static_cast<short>((std::max)(18, precision + 15)) };

	std::cout << std::fixed << std::setprecision(precision);

	// Шапка для таблицы
	std::cout <<
		"\n" <<
		std::left <<
		std::setw(width) <<
		"x" <<
		" | " <<
		std::setw(width) <<
		"f(x)" <<
		"\n";

	// Разделительная линия
	// Длина вычисляется так, чтобы накрыть таблицу полностью:
	// Ширина двух колонок + 3 символа на разделитель " | "
	std::cout << std::string(width * 2 + 3, '-') << "\n";

	// Цикл, который перебирает значения, считает результат и выводит его на экран
	// Использую EPSILON, чтобы избежать потери точности при хранении дробных чисел
	for (double x{ params.start }; x <= params.end + EPSILON; x += params.step) {
		double current_value{ std::cos(x) };

		min_value = (std::min)(min_value, current_value);
		max_value = (std::max)(max_value, current_value);

		std::cout <<
			std::left <<
			std::setw(width) <<
			x <<
			" | " <<
			std::setw(width) <<
			current_value <<
			"\n";
	}

	std::cout << "\nМинимальное значение: " << min_value;
	std::cout << "\nМаксимальное значение: " << max_value;

	std::cout << "\n\n";

	// Использую это для того, чтобы программа
	// не завершалась сразу же после вывода результата
	std::cin.get();

	return 0;
}