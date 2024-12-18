# LL(1)-таблица разбора
parse_table = {
    'D': {'(': ['(', 'L', ')', 'M']},
    'L': {'a': ['a', "L'"], '(': ['D', "L'"]},
    "L'": {',': [',', 'L'], ')': ['ε']},
    'M': {'i': ['i'], 'j': ['j']}
}

# Стековый синтаксический анализатор
def ll1_parser(input_string):
    stack = ['$', 'D']
    input_string += '$'  # нужна обработка строки '(a)i' по действиям ------------------------------------------------------
    pointer = 0

    print("Стек\t\tВход\t\tДействие")
    while stack:
        top = stack.pop()
        current = input_string[pointer]

        if top == current:  # Совпадение терминала
            pointer += 1
            action = "Совпадение"
        elif top in parse_table and current in parse_table[top]:
            production = parse_table[top][current]
            if production != ['ε']:
                stack.extend(reversed(production))
            action = f"{top} -> {' '.join(production)}"
        else:
            raise SyntaxError(f"Ошибка разбора: символ '{current}'")

        print(f"{''.join(stack)}\t\t{input_string[pointer:]}\t\t{action}")

    print("Строка успешно разобрана!")

# Пример вызова
ll1_parser("(a)i")
