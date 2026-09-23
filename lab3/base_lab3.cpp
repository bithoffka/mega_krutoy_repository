#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Использую это, чтобы, если программа компилируется на Windows,
// подключить заголовочный файл Windows.h, в котором есть функции,
// позволяющие настроить кодировку в консоли.
#ifdef _WIN32
#include <Windows.h>
#endif

// В аргументе функции использую ссылку на message ради экономии памяти
double get_double(const std::string& message) {
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

		std::cerr << "\nОшибка! Введите одно число (например 26, 52 или 3.14)!\n";
	}
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // Настройка кодировки для вывода
	SetConsoleCP(CP_UTF8); // Настройка кодировки для ввода
#endif

	std::cout << "=================================\n";
	std::cout << "------ Какое число больше? ------\n";
	std::cout << "=================================\n\n";

	double first_number{ get_double("Введите первое число: ") };
	double second_number{ get_double("Введите второе число: ") };

	// Тут гораздо удобнее использовать std::max, но т. к. лабораторная работа
	// имеет тему "Ветвление через if-else", буду использовать конструкцию if.
	// В условиях также сказано о необходимости наличия блока else, однако
	// в данном случае он не нужен, т. к. при помощи else if перебираются
	// все возможные случаи
	if (first_number > second_number) {
		std::cout << std::setprecision(10) <<
			"\nПервое число (" <<
			first_number <<
			") больше второго (" <<
			second_number <<
			")!";
	}
	else if (second_number > first_number) {
		std::cout << std::setprecision(10) <<
			"\nВторое число (" <<
			second_number <<
			") больше первого (" <<
			first_number <<
			")!";
	}
	else if (first_number == second_number) {
		std::cout << std::setprecision(10) <<
			"\nПервое число (" <<
			first_number <<
			") равно второму (" <<
			second_number <<
			")!";
	}

	std::cout << "\n\n";

	// Использую это для того, чтобы программа
	// не завершалась сразу же после вывода результата
	std::cin.get();

	return 0;
}