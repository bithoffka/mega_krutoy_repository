#include <iostream>
#include <sstream>
#include <string>

// Использую это, чтобы, если программа компилируется на Windows,
// подключить заголовочный файл Windows.h, в котором есть функции,
// позволяющие настроить кодировку в консоли.
#ifdef _WIN32
#include <Windows.h>
#endif

// Использую директиву define, чтобы не писать длинный текст в нескольких местах в коде
#define NON_NEGATIVE_ERROR_MESSAGE "Ошибка! Введите одно неотрицательное число (например 26, 52 или 3.14)!"

// Использую шаблон, чтобы не писать один и тот же код для
// выбора опции в меню и получения величины для конвертации
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

// Функция, возвращающая число типа short (для экономии памяти) в диапазоне от 1 до 3
short handle_choice(const std::string& message, const std::string& error_message) {
	short choice_id{};

	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		choice_id = get_number<short>(message, error_message);

		if (choice_id >= 1 && choice_id <= 3) {
			return choice_id; // Если полученное значение находится в диапазоне от 1 до 3, возвращаем его
		}

		std::cerr << "\n" << error_message << "\n";
	}
}

// Функция, возвращающая неотрицательное число типа double
double get_non_negative_double(const std::string& message, const std::string& error_message) {
	double value_from_user{};

	while (true) { // Цикл позволяет ввести значение заново, если произошла ошибка
		value_from_user = get_number<double>(message, error_message);

		if (value_from_user >= 0) {
			return value_from_user; // Если полученное значение неотрицательно, возвращаем его
		}

		std::cerr << "\n" << error_message << "\n";
	}
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // Настройка кодировки для вывода
	SetConsoleCP(CP_UTF8); // Настройка кодировки для ввода
#endif

	std::cout << "============================\n";
	std::cout << "Конвертер длины в миллиметры\n";
	std::cout << "============================\n\n";

	std::cout << "Выберите, какую величину перевести в миллиметры:\n\n";
	std::cout << "1) Километры\n2) Метры\n3) Сантиметры\n\n";

	short choice_id{ handle_choice("Ваш выбор (1 - 3): ", "Ошибка! Введите число от 1 до 3!")};

	switch (choice_id) { // Использую switch-case, чтобы проверить одну переменную на несколько значений
		case 1: {
			double km{
				get_non_negative_double(
					"Введите длину (км): ",
					NON_NEGATIVE_ERROR_MESSAGE
				)
			};

			std::cout << "\n" << km << " километров = " << km * 1000000.0;
			break;
		}
		case 2: {
			double m{
				get_non_negative_double(
					"Введите длину (м): ",
					NON_NEGATIVE_ERROR_MESSAGE
				)
			};

			std::cout << "\n" << m << " метров = " << m * 1000.0;
			break;
		}
		case 3: {
			double cm{
				get_non_negative_double(
					"Введите длину (см): ",
					NON_NEGATIVE_ERROR_MESSAGE
				)
			};

			std::cout << "\n" << cm << " сантиметров = " << cm * 10.0;
			break;
		}
	}

	std::cout << " миллиметров\n\n";

	// Использую это для того, чтобы программа
	// не завершалась сразу же после вывода результата
	std::cin.get();

	return 0;
}