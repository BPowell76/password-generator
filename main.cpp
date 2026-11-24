#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

unsigned int char_array_index = 0;

char rand_character(const unsigned int ITERATIONS) {
    std::srand(std::random_device()());
    const int& rand_num = std::rand();

    // Determine the character to return by totaling up the clamped character values and dividing by the number of iterations
    unsigned int char_value_sum = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        char_value_sum += std::clamp((rand_num % 128) + 4, 33, 126);
    }
    return char(char_value_sum / ITERATIONS);
}

bool check_password_uniqueness(std::string password) {
    std::vector<char> password_char;
    char_array_index = 0;
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

    bool needs_reprocessing = true;

    if (reprocess_characters.empty()) {
        needs_reprocessing = false;
    }

    return needs_reprocessing;
}

int main() {
    // Generate random password
    std::string password;
    for (int i = 0; i < 32; i++) {
        password += rand_character(100);
    }

    std::cout << "Password Length: " << password.length() << std::endl;
    std::cout << "Generated Password: " << password << std::endl;

    // Get unique character count
    bool needs_reprocessing = check_password_uniqueness(password);

    std::cout << "Unique Characters: " << char_array_index << std::endl;
    std::cout << "Password Needs Reprocessing: " << needs_reprocessing << std::endl;

    return 0;
}