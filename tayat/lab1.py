def process_repeats(sequence):
    result = []
    i = 0
    while i < len(sequence):
        if sequence[i] == '^' and i > 0 and sequence[i + 1].isdigit():
            # Найти число после ^ (количество повторений)
            repeat_count = int(sequence[i + 1])
            result.extend(result[-1] * (repeat_count - 1))  # Повторить предыдущий символ
            i += 2  # Пропустить ^ и число
        else:
            result.append(sequence[i])
            i += 1
    return ''.join(result)

def check_bracket_sequence(sequence):
    stack = []
    errors = 0

    # Карта соответствий открывающих и закрывающих скобок
    matching = {')': '(', ']': '['}

    for char in sequence:
        if char in '([':
            stack.append(char)
        elif char in ')]':
            if stack and stack[-1] == matching[char]:
                stack.pop()
            else:
                errors += 1
        # Игнорируем любые символы, не относящиеся к скобкам
        else:
            continue

    # Все оставшиеся открывающие скобки в стеке — это ошибки
    errors += len(stack)

    return errors

# Пример использования
expressions = ["(([]))^2", "(^2[]))", "(^2)^3("]

for expression in expressions:
    expanded_expression = process_repeats(expression)
    errors = check_bracket_sequence(expanded_expression)
    print(f"Исходная строка: {expression}")
    print(f"Развёрнутая строка: {expanded_expression}")
    print(f"Количество ошибок: {errors}")
