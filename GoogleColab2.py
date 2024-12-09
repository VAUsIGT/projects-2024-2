from num2words import num2words


def read_int(question, not_a_number_message='Это не число'):
    """
    question - вопрос для пользователя
    not_a_number_message - сообщение в случае, если не число
    """
    while True:
        try:
            number = int(input(question))
            return number
        except ValueError:
            print(not_a_number_message)


def read_from_list(question, options, not_in_list_value='Значение не из списка'):
    """
    question - вопрос для пользователя
    options - варианты ответов
    not_in_list_value - сообщение в случае, если значение не из списка
    """
    str_option = ', '.join(options)
    full_question = f"{question} ({str_option}): "
    while True:
        answer = input(full_question).strip().lower()
        if answer not in options:
            print(not_in_list_value)
        else:
            return answer


def main():
    # Запрашиваем число у пользователя
    number = read_int("Введите число: ")

    # Запрашиваем язык для перевода
    language = read_from_list(
        "Выберите язык перевода",
        ['ru', 'en'],
        "Выберите либо 'ru', либо 'en'."
    )

    # Переводим число в строковое представление
    word_representation = num2words(number, lang=language)

    # Выводим результат
    print(f"Число {number} в строковом представлении ({language}): {word_representation}")


main()
