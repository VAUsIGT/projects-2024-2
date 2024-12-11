def generate_string(n):
    # Генерируем строку из языка: + a^(2n) b^(n)
    return ['+'] + ['a'] * (2 * n) + ['b'] * n

# Определяем грамматику в НФХ
grammar = {
    'S': [('P', 'X')],
    'X': [('A', 'X1'), ('A', 'X6')],
    'X1': [('A', 'X2')],
    'X2': [('A', 'X3')],
    'X3': [('A', 'X4')],
    'X4': [('B', 'X5')],
    'X5': [('b',)],
    'X6': [('A', 'X7')],
    'X7': [('X', 'B')],
    'P': [('+',)],
    'A': [('a',)],
    'B': [('b',)]
}

# Соберём множества правил вида V -> t и V -> V1 V2 для удобства
term_productions = {}
var_productions = {}

for left_side, productions in grammar.items():
    for p in productions:
        if len(p) == 1 and p[0] in ['+', 'a', 'b']:  # терминал
            term_productions.setdefault(p[0], set()).add(left_side)
        elif len(p) == 2:
            var_productions.setdefault(p, set()).add(left_side)


def cyk_parse(w):
    n = len(w)
    # П-таблица: P[i][j] = множество нетерминалов, выводящих w[i:j+1]
    P = [[set() for _ in range(n)] for _ in range(n)]

    # Заполняем первый уровень (один символ)
    for i in range(n):
        t = w[i]
        if t in term_productions:
            P[i][i] = P[i][i].union(term_productions[t])

    # Цикл по длине подстроки
    for length in range(2, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            for k in range(i, j):
                for A in P[i][k]:
                    for B in P[k + 1][j]:
                        if (A, B) in var_productions:
                            P[i][j] = P[i][j].union(var_productions[(A, B)])
    return P

# Пример для строки длиной 10 символов (n=3)
w = generate_string(3)  # + a a a a a a b b b
print("Input string:", " ".join(w))

P = cyk_parse(w)
n = len(w)

# Печать CYK-таблицы в табличном виде
print("\nCYK Parse Table:")
cell_width = 20
header = " " * 6 + "".join([f"j={j}".center(cell_width) for j in range(n)])
print(header)
for i in range(n):
    row_str = f"i={i}".ljust(6)
    for j in range(n):
        if j < i:
            row_str += " " * cell_width
        else:
            # Превратим множество в строку
            if P[i][j]:
                cell_content = "{" + ",".join(sorted(P[i][j])) + "}"
            else:
                cell_content = "{}"
            row_str += cell_content.center(cell_width)
    print(row_str)

# Проверяем выводимость
if 'S' in P[0][n - 1]:
    print("\nСтрока выводится из S.")
else:
    print("\nСтрока не выводится из S.")

# Для Эрли
earley_grammar = {
    'S': [['P', 'X']],
    'X': [['A', 'A', 'X', 'B'], ['A', 'A', 'A', 'A', 'B', 'B']],
    'P': [['+']],
    'A': [['a']],
    'B': [['b']]
}

def is_terminal(symbol):
    return symbol in ['+', 'a', 'b']

def earley_parse(words, grammar, start_symbol='S'):
    chart = [set() for _ in range(len(words) + 1)]

    # Инициализируем стартовые правила
    for production in grammar[start_symbol]:
        chart[0].add((start_symbol, tuple(), tuple(production), 0))

    def predict(state_index):
        changed = True
        while changed:
            changed = False
            new_items = set()
            for (lhs, done, todo, origin) in chart[state_index]:
                if len(todo) > 0:
                    next_symbol = todo[0]
                    if next_symbol in grammar and not is_terminal(next_symbol):
                        for prod in grammar[next_symbol]:
                            item = (next_symbol, tuple(), tuple(prod), state_index)
                            if item not in chart[state_index]:
                                new_items.add(item)
            if new_items:
                chart[state_index].update(new_items)
                changed = True

    def complete(state_index):
        changed = True
        while changed:
            changed = False
            new_items = set()
            for (lhs, done, todo, origin) in chart[state_index]:
                if len(todo) == 0:
                    for (LHS, DONE, TODO, ORIGIN) in chart[origin]:
                        if len(TODO) > 0 and TODO[0] == lhs:
                            item = (LHS, DONE + (lhs,), TODO[1:], ORIGIN)
                            if item not in chart[state_index]:
                                new_items.add(item)
            if new_items:
                chart[state_index].update(new_items)
                changed = True

    for i in range(len(words) + 1):
        predict(i)
        complete(i)
        if i < len(words):
            next_word = words[i]
            new_items = set()
            for (lhs, done, todo, origin) in chart[i]:
                if len(todo) > 0 and is_terminal(todo[0]) and todo[0] == next_word:
                    item = (lhs, done + (next_word,), todo[1:], origin)
                    new_items.add(item)
            if new_items:
                chart[i + 1].update(new_items)
                predict(i + 1)
                complete(i + 1)

    for (lhs, done, todo, origin) in chart[len(words)]:
        if lhs == start_symbol and len(todo) == 0 and origin == 0:
            return True, chart
    return False, chart

# Запускаем Эрли-парсер
print("\nInput string:", " ".join(w))
res, chart = earley_parse(w, earley_grammar, 'S')
if res:
    print("Строка выводится.")
else:
    print("Строка не выводится.")

# Печать количества состояний в каждом шаге Earley
print("\nEarley Chart States Count:")
print("Index | States Count")
print("------|-------------")
for i, states in enumerate(chart):
    print(f"{i:5d} | {len(states):>6d}")

print("\nEarley Parsing Chart:")
for i, states in enumerate(chart):
    print(f"i={i}:")
    for (lhs, done, todo, origin) in states:
        done_str = " ".join(done)
        todo_str = " ".join(todo)
        # Позицию можно пометить точкой:
        # Пример: lhs -> done • todo
        print(f"  {lhs} -> {done_str} • {todo_str} (from {origin})")
    print()