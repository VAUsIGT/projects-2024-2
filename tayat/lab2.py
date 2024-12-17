# Определение автоматов (осущесвить предварительный вывод всех операторов, а также указание конечных и начальных)

# Автомат L1
# Состояния: p (начальное), q, r, s
# Переходы:
# p: a->q, b->s
# q: a->r, a->s (недетерминированный переход по a в r и s)
# r: a->s, b->r
# s: b->q
# Конечные: r, s

L1_states = {'p', 'q', 'r', 's'}
L1_initial = 'p'
L1_final = {'r', 's'}
L1_transitions = {
    'p': {'a': ['q'], 'b': ['s']},
    'q': {'a': ['r', 's']},
    'r': {'a': ['s'], 'b': ['r']},
    's': {'b': ['q']}
}

# Автомат L2
# Состояния: q' (начальное), r'
# q': a->r', b->r'
# r': a->q', b->r'
# Конечное: r'
L2_states = {"q'", "r'"}
L2_initial = "q'"
L2_final = {"r'"}
L2_transitions = {
    "q'": {'a': ["r'"], 'b': ["r'"]},
    "r'": {'a': ["q'"], 'b': ["r'"]}
}


def print_transition_table(transitions):
    print("Текущее состояние | Символ | Новое состояние")
    for state in transitions:
        for symbol in transitions[state]:
            for ns in transitions[state][symbol]:
                print(f"{str(state):<18} | {symbol:<6} | {str(ns)}")


def process_string(initial, final, transitions, input_string):
    current_states = {initial}
    print(f"Начальное состояние: {current_states}")

    for symbol in input_string:
        next_states = set()
        for state in current_states:
            if symbol in transitions[state]:
                next_states.update(transitions[state][symbol])
        current_states = next_states
        print(f"Символ: {symbol}, Следующие состояния: {current_states}")

        if not current_states:  # Если нет переходов, строка не принята
            print("Переходов больше нет. Строка не принята.")
            return

    # Проверяем, есть ли конечное состояние среди текущих
    if current_states.intersection(final):
        print("Остановились на конечном состоянии. Строка принята.")
    else:
        print("Конечного состояния нет. Строка не принята.")

def union_automata(L1_init, L1_fin, L1_trans, L2_init, L2_fin, L2_trans):
    # Создаем новое начальное состояние F c e-переходами к L1_init и L2_init             у автомата на входе терминальная строка, дальше указывается последоввательность переходов
    # Создаем новое конечное состояние E c e-переходами из всех конечных L1_fin и L2_fin
    union_trans = {}
    union_final = set()

    union_trans['F'] = {'ε': [L1_init, L2_init]}
    # Копируем переходы L1
    for st in L1_trans:
        union_trans[st] = {}
        for sym, ns_list in L1_trans[st].items():
            union_trans[st][sym] = ns_list[:]

    # Копируем переходы L2
    for st in L2_trans:
        if st not in union_trans:
            union_trans[st] = {}
        for sym, ns_list in L2_trans[st].items():
            union_trans[st][sym] = ns_list[:]

    union_trans['E'] = {}
    for f in L1_fin:
        if 'ε' not in union_trans[f]:
            union_trans[f]['ε'] = []
        union_trans[f]['ε'].append('E')

    for f in L2_fin:
        if 'ε' not in union_trans[f]:
            union_trans[f]['ε'] = []
        union_trans[f]['ε'].append('E')

    union_final.add('E')

    return 'F', union_final, union_trans


def intersection_automata(L1_states, L1_init, L1_fin, L1_trans,
                          L2_states, L2_init, L2_fin, L2_trans):
    inter_trans = {}
    inter_final = set()

    for s1 in L1_states:
        for s2 in L2_states:
            new_state = s1 + s2
            inter_trans[new_state] = {}
            # Символы для пересечения
            sym_common = set(L1_trans[s1].keys()).intersection(L2_trans[s2].keys())
            for sym in sym_common:
                next_states = []
                ns1 = L1_trans[s1][sym]
                ns2 = L2_trans[s2][sym]
                for x in ns1:
                    for y in ns2:
                        next_states.append(x + y)
                if next_states:
                    inter_trans[new_state][sym] = next_states

    inter_initial = L1_init + L2_init
    # Конечные - пары конечных
    for s1f in L1_fin:
        for s2f in L2_fin:
            inter_final.add(s1f + s2f)

    return inter_initial, inter_final, inter_trans


def difference_automata(L1_states, L1_init, L1_fin, L1_trans,
                        L2_states, L2_init, L2_fin, L2_trans):
    # Вычитание:
    # Для L2_final_mod = { "q'" }.
    L2_modified_final = {"q'"}
    diff_trans = {}
    diff_final = set()

    for s1 in L1_states:
        for s2 in L2_states:
            new_state = s1 + s2
            diff_trans[new_state] = {}
            sym_common = set(L1_trans[s1].keys()).intersection(L2_trans[s2].keys())
            for sym in sym_common:
                next_states = []
                ns1 = L1_trans[s1][sym]
                ns2 = L2_trans[s2][sym]
                for x in ns1:
                    for y in ns2:
                        next_states.append(x + y)
                if next_states:
                    diff_trans[new_state][sym] = next_states

    diff_initial = L1_init + L2_init
    for s1f in L1_fin:
        for s2f in L2_modified_final:
            diff_final.add(s1f + s2f)

    return diff_initial, diff_final, diff_trans


def concatenation_automata(L1_states, L1_init, L1_fin, L1_trans,
                           L2_states, L2_init, L2_fin, L2_trans):
    # Конкатенация: Из конечных состояний L1 идут ε-переходы в начальное L2.
    concat_trans = {}
    for st in L1_trans:
        concat_trans[st] = {}
        for sym, ns_list in L1_trans[st].items():
            concat_trans[st][sym] = ns_list[:]

    for st in L2_trans:
        concat_trans[st] = {}
        for sym, ns_list in L2_trans[st].items():
            concat_trans[st][sym] = ns_list[:]

    for f in L1_fin:
        if 'ε' not in concat_trans[f]:
            concat_trans[f]['ε'] = []
        concat_trans[f]['ε'].append(L2_init)

    concat_initial = L1_init
    concat_final = L2_fin

    return concat_initial, concat_final, concat_trans


# Выбор операции
print("Выберите операцию:")
print("1 - объединение")
print("2 - пересечение")
print("3 - вычитание")
print("4 - конкатенация")
operation = input("Введите номер операции: ")

if operation == '1':
    init, final, trans = union_automata(L1_initial, L1_final, L1_transitions,
                                        L2_initial, L2_final, L2_transitions)
    print("Объединение автоматов:")
elif operation == '2':
    init, final, trans = intersection_automata(L1_states, L1_initial, L1_final, L1_transitions,
                                               L2_states, L2_initial, L2_final, L2_transitions)
    print("Пересечение автоматов:")
elif operation == '3':
    init, final, trans = difference_automata(L1_states, L1_initial, L1_final, L1_transitions,
                                             L2_states, L2_initial, L2_final, L2_transitions)
    print("Вычитание автоматов:")
elif operation == '4':
    init, final, trans = concatenation_automata(L1_states, L1_initial, L1_final, L1_transitions,
                                                L2_states, L2_initial, L2_final, L2_transitions)
    print("Конкатенация автоматов:")
else:
    print("Неверная операция. Завершение работы программы.")
    exit()

# Печать результата выбранной операции
print_transition_table(trans)
all_states = set(trans.keys())
print("Все состояния:", all_states)
print("Начальное состояние:", init)
print("Конечные состояния:", final)

# Проверка строки
input_string = input("Введите строку из символов 'a' и 'b': ")
process_string(init, final, trans, input_string)