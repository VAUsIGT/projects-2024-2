def tokenize(input_str):
    # Разобьём строку по пробелам на токены. Предполагается, что токены - это: i, :, integer, real
    # Например: "i : integer" -> ["i", ":", "integer"]
    tokens = input_str.strip().split()
    tokens.append('$')  # Добавляем символ конца ввода
    return tokens

# Правила грамматики (нумерация как выше):
# (1) S' → D
# (2) D → L i : T
# (3) L → L i
# (4) L → ε
# (5) T → integer
# (6) T → real
rules = {
    1: ("S'", ["D"]),
    2: ("D", ["L", "i", ":", "T"]),
    3: ("L", ["L", "i"]),
    4: ("L", []),
    5: ("T", ["integer"]),
    6: ("T", ["real"])
}

# Длина правых частей можно получить из rules
def get_rule_info(n):
    left, right = rules[n]
    return (left, len(right))

# Таблицы Action и Goto из предыдущего шага
# Формат:
# Action[(state, symbol)] = ("shift", next_state) | ("reduce", rule_number) | ("accept", None)
# Goto[(state, nonterminal)] = next_state

Action = {
    (0, 'i'): ("reduce", 4),    # L→ε при i
    (1, '$'): ("accept", None),
    (2, 'i'): ("shift", 3),
    (3, ':'): ("shift", 4),
    (3, 'i'): ("reduce", 3),    # L→L i при i
    (3, '$'): ("reduce", 3),    # L→L i при $
    (4, 'integer'): ("shift", 5),
    (4, 'real'): ("shift", 6),
    (5, '$'): ("reduce", 5),    # T→integer
    (6, '$'): ("reduce", 6),    # T→real
    (7, '$'): ("reduce", 2)     # D→L i : T
}

Goto = {
    (0, 'D'): 1,
    (0, 'L'): 2,
    (4, 'T'): 7
}

def print_stack(stack):
    # Стек будет содержать чередование состояния и символов.
    # Предполагается структура стека как список: [state0, symbol1, state1, symbol2, state2 ...]
    # В начале только [0]
    # Для удобства просто печатаем как есть.
    return ' '.join(map(str, stack))

def parse(input_str):
    tokens = tokenize(input_str)
    # Инициализация стека
    # Стек будет содержать состояния и символы вперемешку. Начинаем с [0]
    stack = [0]
    index = 0

    print("Старт анализатора")
    print("Стек:", print_stack(stack))
    print("Вход:", tokens)
    print()

    while True:
        state = stack[-1]  # последнее в стеке - состояние
        if index < len(tokens):
            a = tokens[index]
        else:
            a = '$'  # если вдруг вышли за пределы, считаем конец ввода

        action = Action.get((state, a), None)
        if action is None:
            print(f"Синтаксическая ошибка: нет действия для (state={state}, symbol='{a}')")
            return "Ошибка"

        act_type, param = action

        if act_type == "shift":
            # Сдвиг: кладём символ и состояние
            print(f"Действие: Shift на символ '{a}', переход в состояние {param}")
            stack.append(a)
            stack.append(param)
            index += 1
            print("Стек:", print_stack(stack))
            if index < len(tokens):
                print("Следующий символ:", tokens[index])
            else:
                print("Достигнут конец ввода")
            print()

        elif act_type == "reduce":
            rule_number = param
            left, rlen = get_rule_info(rule_number)
            print(f"Действие: Reduce по правилу {rule_number} ({left} → {' '.join(rules[rule_number][1]) or 'ε'})")

            # Снимаем 2*rlen элементов (каждая пара: символ, состояние) для правой части
            # Структура стека: [s0, X1, s1, X2, s2, ...] где s - состояния, X - символы (терминалы/нетерминалы)
            # При свёртке удаляем rlen раз (символ, состояние)
            for _ in range(rlen):
                stack.pop()  # состояние
                stack.pop()  # символ

            # Теперь на вершине стека состояние, из него делаем переход по нетерминалу left
            s = stack[-1]
            goto_state = Goto.get((s, left), None)
            if goto_state is None:
                print(f"Синтаксическая ошибка: нет перехода по нетерминалу {left} из состояния {s}")
                return "Ошибка"

            # Добавляем левую часть и состояние
            stack.append(left)
            stack.append(goto_state)
            print("Стек после свёртки:", print_stack(stack))
            print()

            # Если свёртка дала нам правило S'→D (правило 1), возможно уже accept на следующем шаге.
            # Но accept проверяем только при действии accept.


        elif act_type == "accept":
            print("Анализ завершён успешно (Accept)")
            print(f"Входная строка: '{input_str}'")
            print("Строка соответствует заданной грамматике.")
            return "Строка принадлежит языку"

        else:
            print("Неизвестный тип действия!")
            return "Ошибка"

# Пример использования:
# Ввод: "i : integer"
# Должен быть принят, так как соответствует D→L i : T с L→ε и T→integer.
if __name__ == "__main__":
    input_str = "i : integer"
    result = parse(input_str)
    if result == "Строка принадлежит языку":
        print(f"Входная строка: '{input_str}'")
        print("Строка соответствует заданной грамматике.")
    else:
        print("Строка не соответствует грамматике.")

