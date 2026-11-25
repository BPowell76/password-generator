#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

constexpr unsigned int ITERATIONS = 100;
constexpr unsigned int PASSWORD_LENGTH = 32;

/*
Generate a random, printable ASCII character
 */
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

    std::vector<char> reprocess_characters;
    // Get counts of unique characters and add to reprocess queue if count exceeds 3
    for (unsigned int i = 0; i < char_array_index; i++) {
        if (std::ranges::count(password, password_char[i]) > 3) {
            reprocess_characters.push_back(password_char[i]);
        }
    }

    return reprocess_characters;
}

int main() {
    // Generate random password
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