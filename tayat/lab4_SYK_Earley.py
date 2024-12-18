# Грамматика из 1 кода:
# S -> P X
# X -> A X1 | A X6
# X1 -> A X2
# X2 -> A X3
# X3 -> A X4
# X4 -> B X5
# X5 -> b
# X6 -> A X7
# X7 -> X B
# P -> +
# A -> a
# B -> b

# Генерация строки: + a^(2n) b^(n)
def generate_string(n):
    return ['+'] + ['a'] * (2 * n) + ['b'] * n


###########################################
# Код CYK (из 2 кода, но адаптирован под нашу грамматику)
###########################################

class Grammar:
    def __init__(self):
        self.rules = {}

    def add_rule(self, non_terminal, production):
        if non_terminal not in self.rules:
            self.rules[non_terminal] = []
        self.rules[non_terminal].append(production)


class CYK_Parser:
    def __init__(self, grammar):
        self.grammar = grammar

    def parse(self, string):
        n = len(string)
        table = [[set() for _ in range(n)] for _ in range(n)]
        backtrace = [[{} for _ in range(n)] for _ in range(n)]

        # Заполняем первый слой таблицы терминалами
        for i, symbol in enumerate(string):
            for non_terminal, productions in self.grammar.rules.items():
                for production in productions:
                    # Если продукция - это терминал и совпадает со символом строки
                    if len(production) == 1 and production[0] == symbol:
                        table[i][i].add(non_terminal)
                        backtrace[i][i][non_terminal] = (production, None, None)

        # CYK: перебираем по длине подстроки
        for length in range(2, n + 1):
            for start in range(n - length + 1):
                end = start + length - 1
                for split in range(start, end):
                    # Проверяем все нетерминалы и их продукции
                    for A, productions in self.grammar.rules.items():
                        for production in productions:
                            if len(production) == 2:
                                B, C = production[0], production[1]
                                if B in table[start][split] and C in table[split + 1][end]:
                                    table[start][end].add(A)
                                    backtrace[start][end][A] = (production, (start, split), (split + 1, end))

        start_symbol = 'S'
        accepted = start_symbol in table[0][n - 1]
        return accepted, table, backtrace

    def print_parse_steps(self, string, table):
        n = len(string)
        print("Шаги CYK разбора:")
        print("Строка:", "".join(string))
        print("Таблица:")
        # Выведем таблицу в виде уровней
        print("i/j\t", "\t".join(map(str, range(1, n + 1))))
        for level in range(n, 0, -1):
            row_str = []
            for start in range(n - level + 1):
                cell = ",".join(sorted(table[start][start + level - 1]))
                row_str.append(cell if cell else "_")
            print(f"{level}\t" + "\t".join(row_str))

    def generate_parse_trace(self, i, j, symbol, backtrace):
        if symbol not in backtrace[i][j]:
            return []
        rule, left, right = backtrace[i][j][symbol]
        steps = []

        if len(rule) == 1:
            steps.append(f"{symbol} -> {rule[0]}")
        else:
            B, C = rule
            steps.append(f"{symbol} -> {B}{C}")
            if left is not None:
                steps.extend(self.generate_parse_trace(left[0], left[1], B, backtrace))
            if right is not None:
                steps.extend(self.generate_parse_trace(right[0], right[1], C, backtrace))

        return steps


###########################################
# Код Earley (из 3 кода, адаптирован под нашу грамматику)
###########################################

# Грамматика для Earley в формате:
# нетерминал: [список продукций]
grammar_earley = {
    'S': [['P', 'X']],
    'X': [['A', 'X1'], ['A', 'X6']],
    'X1': [['A', 'X2']],
    'X2': [['A', 'X3']],
    'X3': [['A', 'X4']],
    'X4': [['B', 'X5']],
    'X5': [['b']],
    'X6': [['A', 'X7']],
    'X7': [['X', 'B']],
    'P': [['+']],
    'A': [['a']],
    'B': [['b']]
}


def is_terminal(symbol):
    return symbol in ['+', 'a', 'b']


class State:
    def __init__(self, name, production, dot, start, end):
        self.name = name
        self.production = production
        self.dot = dot
        self.start = start
        self.end = end

    def next_symbol(self):
        return self.production[self.dot] if self.dot < len(self.production) else None

    def is_complete(self):
        return self.dot >= len(self.production)

    def __eq__(self, other):
        return (self.name, self.dot, self.start, self.end, self.production) == \
            (other.name, other.dot, other.start, other.end, other.production)

    def __hash__(self):
        return hash((self.name, self.dot, self.start, self.end, tuple(self.production)))

    def get_rule(self):
        prod = list(self.production)
        prod.insert(self.dot, '•')
        return ' '.join(prod)

    def __repr__(self):
        return f"{self.name} → {self.get_rule()}, [{self.start}, {self.end}]"


def earley_parser(tokens, grammar):
    n = len(tokens)
    chart = [set() for _ in range(n + 1)]
    # Добавляем начальное состояние: S' -> S
    chart[0].add(State("S'", ['S'], 0, 0, 0))

    for i in range(n + 1):
        updated = True
        while updated:
            updated = False
            new_states = set()
            for state in chart[i]:
                if state.is_complete():
                    # Complete
                    for st in chart[state.start]:
                        if not st.is_complete() and st.next_symbol() == state.name:
                            new_state = State(st.name, st.production, st.dot + 1, st.start, i)
                            if new_state not in chart[i]:
                                new_states.add(new_state)
                else:
                    next_sym = state.next_symbol()
                    if next_sym in grammar and not is_terminal(next_sym):
                        # Predict
                        for prod in grammar[next_sym]:
                            new_state = State(next_sym, prod, 0, i, i)
                            if new_state not in chart[i]:
                                new_states.add(new_state)
                    elif i < n and next_sym == tokens[i]:
                        # Scan
                        new_state = State(state.name, state.production, state.dot + 1, state.start, i + 1)
                        if new_state not in chart[i + 1]:
                            chart[i + 1].add(new_state)
            if new_states:
                old_count = len(chart[i])
                chart[i].update(new_states)
                if len(chart[i]) > old_count:
                    updated = True

    return chart


def format_chart(chart, tokens):
    output_lines = []
    header = ['', 'Символ', 'Состояние', 'Правило', '[Начало, Конец]']
    output_lines.append('\t'.join(header))
    for i, states in enumerate(chart):
        token = tokens[i] if i < len(tokens) else ''
        first_line = True
        sorted_states = sorted(states, key=lambda s: (s.name, s.start, s.end, s.production, s.dot))
        for state in sorted_states:
            index_str = str(i) if first_line else ''
            token_str = token if first_line else ''
            output_lines.append(
                f"{index_str}\t{token_str}\t{state.name}\t{state.get_rule()}\t[{state.start}, {state.end}]"
            )
            first_line = False
        if first_line:
            output_lines.append(f"{i}\t{token}\t\t\t")
    return output_lines


###########################################
# Используем оба парсера на строке из грамматики
###########################################

# Создаём грамматику для CYK
cyk_grammar = Grammar()
cyk_grammar.add_rule("S", ("P", "X"))
cyk_grammar.add_rule("P", ('+',))
cyk_grammar.add_rule("X", ("A", "X1"))
cyk_grammar.add_rule("X", ("A", "X6"))
cyk_grammar.add_rule("X1", ("A", "X2"))
cyk_grammar.add_rule("X2", ("A", "X3"))
cyk_grammar.add_rule("X3", ("A", "X4"))
cyk_grammar.add_rule("X4", ("B", "X5"))
cyk_grammar.add_rule("X5", ('b',))
cyk_grammar.add_rule("X6", ("A", "X7"))
cyk_grammar.add_rule("X7", ("X", "B"))
cyk_grammar.add_rule("A", ('a',))
cyk_grammar.add_rule("B", ('b',))

lengths = [3, 33]  # Проверим для n=3 и n=5, можно изменить
for n in lengths:
    w = generate_string(n)
    print("\n=============================")
    print(f"Проверяем строку: {''.join(w)}")
    print("=============================\n")

    # CYK парсер
    parser = CYK_Parser(cyk_grammar)
    accepted, table, backtrace = parser.parse(w)
    parser.print_parse_steps(w, table)
    print("CYK: Строка принята." if accepted else "CYK: Строка не принята.")

    # Earley парсер
    earley_chart = earley_parser(w, grammar_earley)
    formatted_chart = format_chart(earley_chart, w)
    print("\nEarley Chart:")
    for line in formatted_chart:
        print(line)
    accepted_earley = any(
        state.name == "S'" and state.is_complete() and state.start == 0 and state.end == len(w) for state in
        earley_chart[-1])
    print("Earley: Строка принята." if accepted_earley else "Earley: Строка не принята.")
