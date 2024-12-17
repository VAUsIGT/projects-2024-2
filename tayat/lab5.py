def tokenize(expr):
    """
    Преобразует входную строку в список токенов (числа, операторы, скобки).
    Токены могут быть:
    - Целые числа (например, '42')
    - Операторы: 'add', 'mul', 'div', 'mod'
    - Скобки: '(', ')'
    """
    tokens = []
    i = 0
    length = len(expr)
    while i < length:
        ch = expr[i]

        # Пропускаем пробелы
        if ch.isspace():
            i += 1
            continue

        # Если скобки
        if ch == '(' or ch == ')':
            tokens.append(ch)
            i += 1
            continue

        # Если цифра - собираем число
        if ch.isdigit():
            start = i
            while i < length and expr[i].isdigit():
                i += 1
            tokens.append(expr[start:i])
            continue

        # Если это буква, пытаемся прочитать оператор
        if ch.isalpha():
            start = i
            while i < length and expr[i].isalpha():
                i += 1
            op = expr[start:i]
            if op in ('add', 'mul', 'div', 'mod'):
                tokens.append(op)
            else:
                # Неизвестный токен
                return None
            continue

        # Если встретился непонятный символ
        return None

    return tokens


def precedence(op):
    """
    Задаёт приоритеты операторов.
    Меньшее число - ниже приоритет.
    Высокий приоритет у mul, div, mod
    Ниже - у add
    """
    if op == 'add':
        return 1
    elif op in ('mul', 'div', 'mod'):
        return 2
    return 0


def shunting_yard(tokens):
    """
    Реализация алгоритма Дейкстры для преобразования инфиксной нотации в ОПН (RPN).
    """
    output_queue = []
    operator_stack = []

    for token in tokens:
        if token.isdigit():
            # Операнд
            output_queue.append(token)
        elif token in ('add', 'mul', 'div', 'mod'):
            # Оператор
            while operator_stack and operator_stack[-1] != '(' and \
                    precedence(operator_stack[-1]) >= precedence(token):
                output_queue.append(operator_stack.pop())
            operator_stack.append(token)
        elif token == '(':
            operator_stack.append(token)
        elif token == ')':
            # Вынимаем из стека в очередь пока не найдем '(' или не кончатся операторы
            while operator_stack and operator_stack[-1] != '(':
                output_queue.append(operator_stack.pop())
            if not operator_stack:
                # Нет соответствующей открывающей скобки - ошибка
                return None
            operator_stack.pop()  # Удаляем '('
        else:
            # Неизвестный токен
            return None

    # Выталкиваем оставшиеся операторы
    while operator_stack:
        top = operator_stack.pop()
        if top == '(':
            # Открывающая скобка без закрывающей - ошибка
            return None
        output_queue.append(top)

    return output_queue


def evaluate_rpn(rpn):
    """
    Вычисляет значение выражения в обратной польской нотации.
    """
    stack = []
    for token in rpn:
        if token.isdigit():
            stack.append(int(token))
        else:
            # Оператор
            if len(stack) < 2:
                # Недостаточно операндов
                raise ValueError("Синтаксическая ошибка (недостаточно операндов)")
            b = stack.pop()
            a = stack.pop()

            if token == 'add':
                res = a + b
            elif token == 'mul':
                res = a * b
            elif token == 'div':
                # Проверим деление на ноль
                if b == 0:
                    raise ZeroDivisionError("Деление на ноль")
                res = a // b
            elif token == 'mod':
                # Проверим деление на ноль
                if b == 0:
                    raise ZeroDivisionError("Деление на ноль")
                res = a % b
            else:
                # Неизвестный оператор
                raise ValueError("Неизвестный оператор")

            stack.append(res)

    if len(stack) != 1:
        # Если в конце вычислений не один элемент на стеке, значит ошибка в выражении
        raise ValueError("Синтаксическая ошибка (лишние операнды/операторы)")

    return stack[0]


def main():
    expr = input("Введите выражение: ").strip()

    tokens = tokenize(expr)
    if tokens is None:
        print("Синтаксическая ошибка")
        return

    rpn = shunting_yard(tokens)
    if rpn is None:
        print("Синтаксическая ошибка")
        return

    # Выведем ОПН
    print("ОПН:", " ".join(rpn))

    try:
        result = evaluate_rpn(rpn)
        print("Результат:", result)
    except (ValueError, ZeroDivisionError):
        print("Синтаксическая ошибка")


if __name__ == "__main__":
    main()
