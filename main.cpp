#include <algorithm>
#include <random>
#include <vector>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

//! Minimum password length to generate
constexpr unsigned int MIN_PASSWORD_LENGTH = 8;

//! Maximum password length to generate
constexpr unsigned int MAX_PASSWORD_LENGTH = 32;

//! Number of times a character can appear in the password string
constexpr unsigned int CHARACTER_REPEAT_LIMIT = 3;

/// Generate a random, printable ASCII character
/// @return printable ASCII character
static char rand_character() {
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(33, 126);
    return static_cast<char>(distribution(rd));
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

std::string generate_password(unsigned int length) {
    std::string password;
    for (int i = 0; i < length; i++) {
        password += rand_character();
    }

    std::vector<char> reprocess_characters = get_characters_for_reprocessing(password);

    unsigned int reprocess_iterations = 0;
    while (reprocess_characters.empty() == false) {
        for (unsigned int i = 0; i < reprocess_characters.size(); i++) {
            for (unsigned int j = 0; j < length; j++) {
                if (password[j] == reprocess_characters[i]) {
                    password[j] = rand_character();
                }
            }
        }

        reprocess_characters = get_characters_for_reprocessing(password);
        reprocess_iterations++;
    }

    return password;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget win;
    win.setWindowTitle("Password Generator");

    auto* sliderLabel = new QLabel();
    sliderLabel->setText(QString::fromStdString("Length: 32"));

    auto* lengthSlider = new QSlider(Qt::Horizontal);
    lengthSlider->setRange(MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
    lengthSlider->setValue(32);

    QObject::connect(lengthSlider, &QSlider::valueChanged, [sliderLabel](int value) {
        sliderLabel->setText(QString("Length: %1").arg(value));
    });

    auto* label = new QLabel();
    label->setText(QString("Generated Password"));

    auto* textOutput = new QLineEdit();
    textOutput->setReadOnly(true);
    textOutput->setFixedWidth(300);

    auto* genPasswordButton = new QPushButton();
    genPasswordButton->setText(QString("Generate Password"));

    QObject::connect(genPasswordButton, &QPushButton::clicked, [lengthSlider, textOutput] {
        textOutput->setText(QString::fromStdString(generate_password(lengthSlider->value())));
    });

    auto* layout = new QVBoxLayout(&win);
    layout->addWidget(sliderLabel);
    layout->addWidget(lengthSlider);
    layout->addSpacing(20);
    layout->addWidget(label);
    layout->addWidget(textOutput);
    layout->addSpacing(20);
    layout->addWidget(genPasswordButton);

    win.show();

    return app.exec();
}