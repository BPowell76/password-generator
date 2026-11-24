#include <algorithm>
#include <iostream>
#include <random>

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

int main() {
    // Generate random password
    std::string password;
    for (int i = 0; i < 32; i++) {
        password += rand_character(100);
    }

    // Get unique character count
    char password_char[32];
    unsigned int char_array_index = 0;
    for (unsigned int i = 0; i < 32; i++) {
        if (i == 0) {
            password_char[char_array_index] = password[i];
            char_array_index++;
            continue;
        }

        unsigned int loop_counter = 0;
        bool character_match = false;
        while (loop_counter < char_array_index) {
            if (password[i] == password_char[loop_counter]) {
                character_match = true;
                break;
            }
            loop_counter++;
        }

        if (!character_match) {
            password_char[char_array_index] = password[i];
            char_array_index++;
        }
    }

    std::cout << "Password Length: " << password.length() << std::endl;
    std::cout << "Unique Characters: " << char_array_index << std::endl;
    std::cout << "Generated Password: " << password << std::endl;

    char reprocess_characters[char_array_index];
    unsigned int reprocess_char_index = 0;

    // Get counts of unique characters and add to reprocess queue if count exceeds 3
    for (unsigned int i = 0; i < char_array_index; i++) {
        if (std::ranges::count(password, password_char[i]) > 3) {
            reprocess_characters[reprocess_char_index] = password_char[i];
            reprocess_char_index++;
        }
    }

    std::cout << "Characters to Reprocess: " << reprocess_char_index << std::endl;

    return 0;
}