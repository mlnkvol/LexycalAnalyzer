#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <windows.h> // Для роботи з кольорами

// Структура для зберігання інформації про лексему
struct Lexeme {
    std::string value;
    std::string type;
};

// Функція для класифікації лексем
std::string classifyLexeme(const std::string& lexeme) {
    if (std::regex_match(lexeme, std::regex("^[0-9]+$"))) {
        return "Decimal Number";
    }
    if (std::regex_match(lexeme, std::regex("^0x[0-9A-Fa-f]+$"))) {
        return "Hexadecimal Number";
    }
    if (std::regex_match(lexeme, std::regex("^[0-9]*\\.[0-9]+$"))) {
        return "Floating Point Number";
    }
    if (std::regex_match(lexeme, std::regex("^\".*\"$"))) {
        return "String Literal";
    }
    if (std::regex_match(lexeme, std::regex("^'.*'$"))) {
        return "Character Literal";
    }
    if (std::regex_match(lexeme, std::regex("^#.*"))) {
        return "Preprocessor Directive";
    }
    if (std::regex_match(lexeme, std::regex("^//.*|/\\*.*\\*/$"))) {
        return "Comment";
    }
    if (lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/") {
        return "Operator";
    }
    if (lexeme == ";" || lexeme == "," || lexeme == "(" || lexeme == ")") {
        return "Punctuation";
    }
    if (std::regex_match(lexeme, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$"))) {
        return "Identifier";
    }
    if (std::regex_match(lexeme, std::regex("^\\.[a-zA-Z0-9_]+:$"))) {
        return "Assembly Directive"; // Додано правило для директив
    }
    if (std::regex_match(lexeme, std::regex("^\\.[a-zA-Z0-9_]+$"))) {
        return "Assembly Command"; // Додано правило для команд
    }

    // Додані умови для Variable Assignment
    if (std::regex_match(lexeme, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*=[^=]*$"))) {
        return "Variable Assignment"; // Призначення змінних
    }

    // Додані умови для Labels
    if (std::regex_match(lexeme, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*:$"))) {
        return "Label"; // Мітки
    }

    // Додані умови для міток з розширеннями
    if (std::regex_match(lexeme, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*\\.[a-zA-Z0-9_]*:$"))) {
        return "Label"; // Мітки з розширенням
    }

    // Додані умови для файлів з розширеннями
    if (std::regex_match(lexeme, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*\\.(o|asm)$"))) {
        return "File"; // Файли
    }

    // Додана умова для опцій команд
    if (std::regex_match(lexeme, std::regex("^-[a-zA-Z0-9]+$"))) {
        return "Command Option"; // Опції команд
    }

    if (std::regex_match(lexeme, std::regex("^\\[ebp[+-]?[0-9]+\\]$"))) {
        return "Addressing Mode"; // Адресна арифметика
    }

    return "Unknown";
}



// Функція для встановлення кольору на основі типу лексеми
void setColorByType(const std::string& type) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (type == "Decimal Number") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Яскраво-зелений
    }
    else if (type == "Hexadecimal Number") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Синій
    }
    else if (type == "Floating Point Number") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Яскраво-червоний
    }
    else if (type == "String Literal") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Блакитний
    }
    else if (type == "Character Literal") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Жовтий
    }
    else if (type == "Preprocessor Directive") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE); // Фіолетовий
    }
    else if (type == "Comment") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Блакитний
    }
    else if (type == "Operator") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Червоний
    }
    else if (type == "Punctuation") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED); // Фіолетовий
    }
    else if (type == "Identifier") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Білий
    }
    else if (type == "Assembly Directive") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // Зелений
    }
    else if (type == "Assembly Command") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Яскраво-білий
    }
    else if (type == "Variable Assignment") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN); // Зелений та синій
    }
    else if (type == "Label") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN); // Звичайний жовтий
    }
    else if (type == "Addressing Mode") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); // Яскраво-зелений
    }
    else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Сірий (звичайний текст)
    }
}

// Функція для виведення результатів з кольорами
void printLexemesWithColors(const std::vector<Lexeme>& lexemes) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (const auto& lex : lexemes) {
        setColorByType(lex.type);
        std::cout << "<" << lex.value << ", " << lex.type << ">" << std::endl;
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Повернення до стандартного кольору
}

// Основна функція лексичного аналізу
std::vector<Lexeme> analyzeText(const std::string& text) {
    std::vector<Lexeme> lexemes;
    std::string word;
    for (char c : text) {
        if (isspace(c) || c == ';' || c == ',' || c == '(' || c == ')') {
            if (!word.empty()) {
                lexemes.push_back({ word, classifyLexeme(word) });
                word.clear();
            }
            if (!isspace(c)) { // Додаємо розділові знаки як окремі лексеми
                std::string punctuation(1, c);
                lexemes.push_back({ punctuation, classifyLexeme(punctuation) });
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        lexemes.push_back({ word, classifyLexeme(word) });
    }
    return lexemes;
}

int main() {
    // Налаштування консолі на UTF-8
    system("chcp 65001");

    const std::string inputFilePath = "C:\\Users\\melni\\OneDrive\\Рабочий стол\\KNU\\3rd course\\Системне програмування\\LexycalAnalyzer\\program.asm.txt";
    std::ifstream inputFile(inputFilePath, std::ios::binary); // Відкриття файлу для читання

    if (!inputFile) {
        std::cerr << "Не вдалося відкрити файл!" << std::endl;
        return 1;
    }

    std::string programText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    std::vector<Lexeme> lexemes = analyzeText(programText);
    printLexemesWithColors(lexemes);

    return 0;
}

