#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <sstream>

bool test = false;

// Forward declaration of the Attack class
class Attack;

int text_speed = 10;

struct slowly_printing_string {
    std::string data;
    long int delay;
};

std::ostream& operator<<(std::ostream& out, const slowly_printing_string& s) {
    for (const auto& c : s.data) {
        out << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(s.delay));
    }
    return out;
}

class Character {
public:
    Character(const std::string& name, int hp, int defense, int attack, int capa1, int capa2, int capa3, int capa4, const std::string& type)
        : name(name), hp(hp), defense(defense), attack(attack), capa1(capa1), capa2(capa2), capa3(capa3), capa4(capa4), type(type) {}

    const std::string& getName() const { return name; }
    const std::string& getType() const { return type; }
    int getHp() const { return hp; }
    int getDefense() const { return defense; }
    int getAttack() const { return attack; }
    bool IsAlive() const { return hp > 0; }

    void takeDamage(int damage_all) {
        hp -= damage_all;
        if (hp <= 0) {
            hp = 0;
            std::cout << name << " is dead!" << std::endl;
        }
    }

    int getCapa1() const { return capa1; }
    int getCapa2() const { return capa2; }
    int getCapa3() const { return capa3; }
    int getCapa4() const { return capa4; }

    // Function to retrieve an attack ID
    int capa(int number) const {
        if (number >= 1 && number <= 4) {
            switch (number) {
            case 1:
                return capa1;
            case 2:
                return capa2;
            case 3:
                return capa3;
            case 4:
                return capa4;
            }
        }
        // Return a default value in case of an invalid number
        return 0;
    }

    void ShowMoves(const std::shared_ptr<Attack>& attack) {
        capacitePrint(*attack);
    }

    void capacitePrint(const Attack& attack);

private:
    std::string name;
    std::string type;
    int hp;
    int defense;
    int attack;
    int capa1;
    int capa2;
    int capa3;
    int capa4;
};

class Attack {
public:
    Attack(const std::string& name, const std::string& type, int damage, int precision, int use)
        : name(name), type(type), damage(damage), precision(precision), use(use) {}

    const std::string& getName() const { return name; }
    const std::string& getType() const { return type; }
    int getDamage() const { return damage; }
    int getPrecision() const { return precision; }
    int getUse() const { return use; }
    void Used() { use -= 1; }

private:
    std::string name;
    std::string type;
    int damage;
    int precision;
    int use;
};

void Character::capacitePrint(const Attack& attack) {
    std::cout << "Name: " << attack.getName()
        << ", Damage: " << attack.getDamage()
        << ", Type: " << attack.getType()
        << ", Remaining Uses: " << attack.getUse() << std::endl;
}

void ReadCharacterDataFromFile(Character& character, const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the character file." << std::endl;
        return;
    }

    std::string name, type;
    int hp, defense, attack, capa1, capa2, capa3, capa4;
    if (file >> name >> hp >> defense >> attack >> capa1 >> capa2 >> capa3 >> capa4 >> type) {
        Character newCharacter(name, hp, defense, attack, capa1, capa2, capa3, capa4, type);
        character = newCharacter;
    }

    file.close();
}

void ReadAttackDataFromFile(std::vector<std::shared_ptr<Attack>>& attack_list) {
    std::ifstream file("attacks.txt");

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the attack file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, type;
        int damage, precision, use;
        char comma;

        if (iss >> std::quoted(name) >> comma >> std::quoted(type) >> comma >> damage >> comma >> precision >> comma >> use) {
            auto attack = std::make_shared<Attack>(name, type, damage, precision, use);
            attack_list.push_back(attack);
            if (test)
            std::cout << "Read Attack: Name=" << name << ", Type=" << type << ", Damage=" << damage << ", Precision=" << precision << ", Use=" << use << std::endl;
        }
        else {
            std::cerr << "Error reading line: " << line << std::endl;
        }
    }

    file.close();
}

int CalculDamage(const Attack& attack, const Character& Ally, const Character& Enemy) {
    int damage = 0;
    int damage_attack = attack.getDamage();
    int Ally_attack = Ally.getAttack();
    int Enemy_defense = Enemy.getDefense();
    const std::string& type_attack = attack.getType();
    const std::string& type_Enemy = Enemy.getType();

    int modifier = 0;

    if (type_attack == type_Enemy) {
        modifier = 0.5;
        if (type_Enemy == "Normal") {
            modifier = 1;
        }
    }
    else if (type_attack != type_Enemy) {
        if (type_Enemy == "Fire") {
            if (type_attack == "Water" || type_attack == "Ground" || type_attack == "Rock") {
                modifier = 2;
            }
            if (type_attack == "Grass" || type_attack == "Ice" || type_attack == "Steel" || type_attack == "Fairy") {
                modifier = 0.5;
            }
        }
        else if (type_Enemy == "Water") {
            if (type_attack == "Fire" || type_attack == "Ice" || type_attack == "Steel") {
                modifier = 0.5;
            }
            if (type_attack == "Grass" || type_attack == "Electr") {
                modifier = 2;
            }
        }
        else if (type_Enemy == "Grass") {
            if (type_attack == "Fire" || type_attack == "Ice" || type_attack == "Poison" || type_attack == "Flying" || type_attack == "Bug") {
                modifier = 2;
            }
            if (type_attack == "Water" || type_attack == "Electr" || type_attack == "Ground") {
                modifier = 0.5;
            }
        }
        else if (type_Enemy == "Ice") {
            if (type_attack == "Fire" || type_attack == "Fight" || type_attack == "Steel" || type_attack == "Ground") {
                modifier = 2;
            }
            else {
                modifier = 1;
            }
        }
        else if (type_Enemy == "Normal") {
            modifier = 1;
        }
    }
    else {
        modifier = 1;
    }

    damage = (0.5 * Ally_attack * (damage_attack / Enemy_defense) * modifier) + 1;
    return damage;
}

bool CalculPrecision(Attack& attack) {
    std::cout << "Name: " << attack.getName()
        << ", Damage: " << attack.getDamage()
        << ", Type: " << attack.getType()
        << ", Remaining Uses: " << attack.getUse() << std::endl;
    int random = std::rand() % 10 + 1; // Random number between 1 and 10
    attack.Used();
    if (random <= attack.getPrecision()) {
        return true;
    }
    else {
        return false;
    }
}

int Attack_turn(const std::shared_ptr<Attack>& attack, const Character& Attacker, Character& Defenser, bool player_turn) {
    if (CalculPrecision(*attack)) {
        int damage = CalculDamage(*attack, Attacker, Defenser);
        Defenser.takeDamage(damage);
        if (Defenser.IsAlive()) {
            std::cout << slowly_printing_string{ "L'attaque a blesse de ", text_speed } << damage << slowly_printing_string{ " hp", text_speed } << std::endl;
        }
    }
    else {
        std::cout << slowly_printing_string{ "L'attaque a rate", text_speed } << std::endl;
    }
    return 0;
}

int main() {
    std::vector<std::shared_ptr<Attack>> attack_list;
    ReadAttackDataFromFile(attack_list);

    Character P1("Jaja", 500, 9999, 20, 2, 4, 5, 20, "Grass");
    Character P2("L'ane Trotro", 100, 10, 50, 1, 2, 3, 4, "Water");

    while (P1.IsAlive() && P2.IsAlive()) {
        for (int i = 0; i < 4; i++) {
            P1.ShowMoves(attack_list[P1.capa(i)]);
        }
        int number;
        std::cout << "Choose an attack (1-4): ";
        std::cin >> number;
        bool player_turn = true;
        std::cout << "L'ennemi is of type: " << P2.getType() << std::endl;
        if (number >= 1 && number <= 4) {
            // Check if the selected ability is available (use > 0)
            if (P1.capa(number) > 0) {
                Attack_turn(attack_list[P1.capa(number - 1)], P1, P2, player_turn); // Subtract 1 to convert to a 0-based index
                char ch = getchar();
                player_turn = false;

                if (P1.IsAlive() && P2.IsAlive()) {
                    int enemyAttackNumber = std::rand() % attack_list.size();
                    // Check if the selected ability is available (use > 0)
                    while (attack_list[enemyAttackNumber]->getUse() <= 0) {
                        enemyAttackNumber = std::rand() % attack_list.size();
                    }
                    Attack_turn(attack_list[enemyAttackNumber], P2, P1, player_turn);
                    std::cout << "Vous avez : " << P1.getHp() << " hp" << std::endl;
                    char ch = getchar();
                }
            }
            else {
                std::cout << "Attaque invalide. Cette attaque est épuisée." << std::endl;
            }
        }
        else {
            std::cout << "Attaque invalide. Choisissez un nombre entre 1 et 4." << std::endl;
        }
    }

    return 0;
}
