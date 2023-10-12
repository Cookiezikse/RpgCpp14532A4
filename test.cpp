#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdlib> 
#include <conio.h>
#include <algorithm>
#include <thread>
#include <chrono>

bool test = false;
int text_speed = 30;

struct slowly_printing_string { std::string data; long int delay; };

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
	// Fonction qui permet de changer l'id d'une capa
	int capa(int number) const {
		switch (number) {
		case 1:
			return capa1;
			break;
		case 2:
			return capa2;
			break;
		case 3:
			return capa3;
			break;
		case 4:
			return capa4;
			break;
		default:
			break;
		}
	}

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
	Attack(const std::string& name, const std::string& type, int damage, int precision, int use) :
		name(name), type(type), damage(damage), precision(precision), use(use) {}

	const std::string& getName() const { return name; }
	const std::string& getType() const { return type; }
	int getDamage() const { return damage; }
	int getPrecision() const { return precision; }
	int getUse() const { return use; }
	int Used() { use -= 1; }

private:
	std::string name;
	std::string type;
	int damage;
	int precision;
	int use;
};

int CalculDamage(const Attack& attack, const Character& Ally, const Character& Enemy) {

	int damage;

	int damage_attack = attack.getDamage();
	int Ally_attack = Ally.getAttack();
	int Enemy_defense = Enemy.getDefense();
	const std::string& type_attack = attack.getType();
	const std::string& type_Enemy = Enemy.getType();

	if (test) {
		std::cout << "Type attack: " << type_attack << " " << " Type enemy: " << type_Enemy << std::endl;
	}

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
			/*
			const char* list[] = {"Fire", "Ground", "Rock"};
			bool contain = false;
			for (int i = 0; i < 3; i++) {
				if (type_attack == list[i]) {
					modifier = 2;
				}
			}
			*/
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

	if (test == true) {
		std::cout << " Attaque : " << Ally_attack << " Damage attaque: " << damage_attack << " Enemy def : " << Enemy_defense << " Modifier : " << modifier << std::endl;
	}
	return damage;
}

int CalculPrecision(const Attack& attack) {

	srand(time(0));
	std::vector<int> list{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int index = rand() % list.size();
	int value = list[index];


	bool can_attack = true;
	int random = rand() % 100; //Bug, random peut rester sur une valeur 
	
	if (test) {
		std::cout << value << std::endl;
	}
	if (value <= (attack.getPrecision() / 10)) {
		return can_attack;
	}
	else {
		return !can_attack;
	}
}

int Attack_turn(const Attack& attack , const Character& P1, Character& P2) {

	if (CalculPrecision(attack) == true) {
		int damage = CalculDamage(attack, P1, P2);
		P2.getAttack();
		P2.takeDamage(damage);
		if (P2.IsAlive()) {
			std::cout << slowly_printing_string{ "Vous avez blesse votre ennemi de ",text_speed } << damage << slowly_printing_string{ " hp",text_speed } << std::endl; /*attack ally enemy*/

		}
	}
	else {
		std::cout << slowly_printing_string{ "L'attaque a rate",text_speed } << std::endl;
	}
	return 0;
}


int main() {

	Attack attack1("Mangeur", "Grass", 10, 90, 15); /*Name type damage precision use*/

	std::vector<Attack> attack_list;
	attack_list.push_back(attack1);

	Character P1("Jaja", 30, 14, 20, 2, 8, 9, 1, "Grass"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */
	Character P2("L'ane Trotro", 100, 10, 50, 0, 0, 0, 0, "Water"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */

	//std::vector<Attack> attacks;
	//attacks.push_back(attack1);

	while (P1.IsAlive() && P2.IsAlive()) {
		std::cout << slowly_printing_string{ "L'ennemie a : ",text_speed } << P2.getHp() << slowly_printing_string{ " hp",text_speed } << std::endl;
		Attack_turn(attack1,P1,P2);
		std::cout << slowly_printing_string{ "Appuyez pour continuer...",text_speed };
		char ch = getchar();
	}
	return 0;
}
// for i in range len(attacks): if character.getCapa1 == attacks.index(attacks[i]): vector (vector player attacks qui stock les pointeurs des attaques) push back attacks[i]