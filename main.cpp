#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <QApplication>
#include <QWidget>

//! Number of iterations to generate a password character
constexpr unsigned int ITERATIONS = 100;

//! Password length to generate
constexpr unsigned int PASSWORD_LENGTH = 32;

//! Number of times a character can appear in the password string
constexpr unsigned int CHARACTER_REPEAT_LIMIT = 3;

/// Generate a random, printable ASCII character
/// @return printable ASCII character
static char rand_character() {
    std::srand(std::random_device()());
    const int& rand_num = std::rand();

    // Determine the character to return by totaling up the clamped character values and dividing by the number of iterations
    unsigned int char_value_sum = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        char_value_sum += std::clamp((rand_num % 128) + 4, 33, 126);
    }

    return static_cast<char>(char_value_sum / ITERATIONS);
}

/// Process password string to look for characters that repeat too many times leading to a weaker password
/// @param password The password string to check for duplicate characters
/// @return vector with characters that exceeded the value of CHARACTER_REPEAT_LIMIT
std::vector<char> get_characters_for_reprocessing(std::string password) {
    std::vector<char> password_char;
    unsigned int char_array_index = 0;
    for (unsigned int i = 0; i < password.length(); i++) {
        if (i == 0) {
            password_char.push_back(password[i]);
            continue;
        }

        unsigned int loop_counter = 0;
        bool character_match = false;
        while (loop_counter < password_char.size()) {
            if (password[i] == password_char[loop_counter]) {
                character_match = true;
                break;
            }
            loop_counter++;
        }

        if (!character_match) {
            password_char.push_back(password[i]);
            char_array_index++;
        }
    }

    // Get counts of unique characters and add to reprocess queue if count exceeds CHARACTER_REPEAT_LIMIT
    std::vector<char> reprocess_characters;
    for (unsigned int i = 0; i < char_array_index; i++) {
        if (std::ranges::count(password, password_char[i]) > CHARACTER_REPEAT_LIMIT) {
            reprocess_characters.push_back(password_char[i]);
        }
    }

    return reprocess_characters;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget win;
    win.setWindowTitle("Password Generator");
    win.resize(800, 500);
    win.show();

    return app.exec();
    std::string password;
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        password += rand_character();
    }

    std::vector<char> reprocess_characters = get_characters_for_reprocessing(password);

    unsigned int reprocess_iterations = 0;
    while (reprocess_characters.empty() == false) {
        for (unsigned int i = 0; i < reprocess_characters.size(); i++) {
            for (unsigned int j = 0; j < PASSWORD_LENGTH; j++) {
                if (password[j] == reprocess_characters[i]) {
                    password[j] = rand_character();
                }
            }
        }

        reprocess_characters = get_characters_for_reprocessing(password);
        reprocess_iterations++;
    }

    std::cout << "Password Length: " << password.length() << std::endl;
    std::cout << "Reprocess Iterations: " << reprocess_iterations << std::endl;
    std::cout << "Password: " << password << std::endl;

    return 0;
}