# Определяем правила грамматики
grammar_G = {
    #Грейбах
    'S': ['2 S_1'],
    'S_1': ['* S_2'],
    'S_2': ['2 S_3'],
    'S_3': ['* S_4'],
    'S_4': ['2 C S_5'],
    'S_5': ['- S_6'],
    'S_6': ['1 S_7'],
    'S_7': ['- S_8'],
    'S_8': ['1'],
    'C': ['* D C M O', ''],  # ε-переход
    'D': ['2'],
    'M': ['-'],
    'O': ['1']
}
grammar_H = {
    #Хомского
    'S': ['2 S_2 G_2'],
    'G_2': ['C S_3'],
    'S_2': ['Y Y'],
    'S_3': ['Z Z'],
    'C': ['Y C_2', ''],  # пустое это ε
    'C_2': ['C Z'],
    'X': ['2'],
    'Y': ['A_1 A_2'],
    'Z': ['B_1 B_2'],
    'A_1': ['*'],
    'A_2': ['2'],
    'B_1': ['-'],
    'B_2': ['1']
}
grammar = {}

def choose_grammar(option):
    """Выбирает грамматику на основе параметра option."""
    global grammar
    if option == 'G':
        grammar = grammar_G
    elif option == 'H':
        grammar = grammar_H
    else:
        raise ValueError("Неверный выбор грамматики. Используйте 'G' или 'H'.")

def expand_non_terminal(symbol, ct):
    """Разворачиваем нетерминальный символ с учётом текущего значения ct."""
    if symbol in grammar:
        if symbol == "C":
            # Выбираем продукцию на основе значения ct
            if ct > 0:
                production = grammar[symbol][0]
            else:
                production = grammar[symbol][1]
            return production.split(), max(ct - 1, 0)
        else:
            # Для остальных нетерминалов всегда берем первую продукцию
            production = grammar[symbol][0]
        return production.split(), ct
    else:
        # Если символ терминальный, возвращаем его как есть
        return [symbol], ct

def generate_string(start_symbol, ct):
    """Генерируем строку, начиная с указанного стартового символа."""
    stack = [start_symbol]
    result = []

    while stack:
        current = stack.pop()  # Используем стек для управления порядком
        expanded, ct = expand_non_terminal(current, ct)

        # Обрабатываем развернутые символы в обратном порядке
        for item in reversed(expanded):
            if item in grammar:
                stack.append(item)  # Добавляем нетерминалы в стек
            else:
                result.append(item)  # Терминальные символы сразу в результат

    return ''.join(result)

# Ввод пользователя для выбора грамматики
grammar_choice = input("Выберите грамматику ('G' для Грейбах, 'H' для Хомского): ").strip().upper()
choose_grammar(grammar_choice)
# Примеры вызова
print(generate_string('S', 0))  # Ожидается: 2*2*2-1-1
print(generate_string('S', 5))  # Ожидается: 2*2*2*2-1-1-1
print(generate_string('S', 10))  # Ожидается: 2*2*2*2*2-1-1-1-1
