import random

# список слов для загадывания
word_list = ['раз', 'два', 'три']
def get_word():
    """
    выбирает слово случайным образом из списка
    """
    word = random.choice(word_list)
    return word.upper()
def display_hangman(tries):
    """
    рисует виселицу
    tries - количество неправильных попыток
    """
    stages = [  # финальное сотояние: голова, туловище, обе руки, обе ноги
                """
                   --------
                   |      |
                   |      O
                   |     \\\|/
                   |      |
                   |     / \\
                   -
                """,
                # голова, туловище, обе руки, нога
                """
                   --------
                   |      |
                   |      O
                   |     \\\|/
                   |      |
                   |     /
                   -
                """,
                # голова, туловище, обе руки
                """
                   --------
                   |      |
                   |      O
                   |     \\\|/
                   |      |
                   |
                   -
                """,
                # голова, туловище, одна рука
                """
                   --------
                   |      |
                   |      O
                   |     \\\|
                   |      |
                   |
                   -
                """,
                # голова, туловище
                """
                   --------
                   |      |
                   |      O
                   |      |
                   |      |
                   |
                   -
                """,
                # голова
                """
                   --------
                   |      |
                   |      O
                   |
                   |
                   |
                   -
                """,
                # начальное состояние
                """
                   --------
                   |      |
                   |
                   |
                   |
                   |
                   -
                """
    ]
    return stages[tries]

def play(word):
    """
    Логика игры
    """
    word_completion = "_" * len(word)  # маска для слова
    guessed = False  # флаг угадал или нет
    guessed_letters = []  # список угаданных букв
    guessed_words = []  # список угаданных слов
    tries = 6  # количество неправильных ответов
    print("Поиграем в виселицу!")
    print(display_hangman(tries))
    print(word_completion)
    print("\n")

    while not guessed and tries > 0:
        guess = input("Введите букву или слово целиком: ").upper()
        if len(guess) == 1 and guess.isalpha():
            if guess in guessed_letters:
                print(f"Вы уже называли букву {guess}.")
            elif guess not in word:
                print(f"Буквы {guess} нет в слове.")
                tries -= 1
                guessed_letters.append(guess)
            else:
                print(f"Отлично! Буква {guess} есть в слове.")
                guessed_letters.append(guess)
                word_as_list = list(word_completion)
                indices = [i for i, letter in enumerate(word) if letter == guess]
                for index in indices:
                    word_as_list[index] = guess
                word_completion = "".join(word_as_list)
                if "_" not in word_completion:
                    guessed = True
        elif len(guess) == len(word) and guess.isalpha():
            if guess in guessed_words:
                print(f"Вы уже называли слово {guess}.")
            elif guess != word:
                print(f"Слово {guess} неверно.")
                tries -= 1
                guessed_words.append(guess)
            else:
                guessed = True
                word_completion = word
        else:
            print("Некорректный ввод. Попробуйте снова.")

        print(display_hangman(tries))
        print(word_completion)
        print("\n")

    if guessed:
        print(f"Поздравляем, вы угадали слово {word}! Вы победили!")
    else:
        print(f"Вы не угадали слово. Это было слово {word}. Попробуйте снова!")

def main():
    word = get_word()
    play(word)
    while input("Хотите сыграть еще раз? (да/нет): ").lower() == "да":
        word = get_word()
        play(word)

main()