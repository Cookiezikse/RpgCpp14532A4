#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdlib> 
#include <conio.h>

bool test = false;

class Character {
public:
    Character(const std::string& name, int hp, int defense ,int attack ,int capa1, int capa2, int capa3, int capa4 , const std::string& type)
        : name(name), hp(hp), defense(defense), attack(attack), capa1(capa1), capa2(capa2), capa3(capa3), capa4(capa4), type(type) {}

	const std::string& getName() const { return name; }
	const std::string& getType() const { return type; }
	int getHp() const { return hp; }
	int getDefense() const { return defense; }
	int getAttack() const { return attack; }
	bool IsAlive() const { return hp > 0; }

	void takeDamage(int damage) {
		hp -= damage;
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

int CalculDamage(const Attack& attack, const Character& Ally, const Character& Enemy){

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
	else if (type_attack != type_Enemy){

		if (type_Enemy == "Fire") {
			if (type_attack == "Water") {
				modifier = 2;
			}
			if (type_attack == "Grass") {
				modifier = 0.5;
			}
		}
		else if (type_Enemy == "Water") {
			if (type_attack == "Fire") {
				modifier = 0.5;
			}
			if (type_attack == "Grass") {
				modifier = 2;
			}
		}
		else if (type_Enemy == "Grass") {
			if (type_attack == "Fire") {
				modifier = 2;
			}
			if (type_attack == "Water") {
				modifier = 0.5;
			}
		}
		else if (type_Enemy == "Normal"){
			modifier = 1;
		}
	}

	damage = (0.5 * Ally_attack * (damage_attack/Enemy_defense) * modifier) + 1;
	
	if (test == true) {
		std::cout << " Attaque : " << Ally_attack << " Damage attaque: " << damage_attack << " Enemy def : " << Enemy_defense << " Modifier : " << modifier << std::endl;
	}
	return damage;
}

int CalculPrecision(const Attack& attack) {

	srand(time(0));
	std::vector<int> list{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int index = rand() % list.size();
	int value = list[index];


	bool can_attack = true;
	int random = rand() % 100; //Bug, random peut rester sur une valeur 
	std::cout << value << std::endl;
	if (value <= (attack.getPrecision()/10)) {
		return can_attack;
	}
	else {
		return !can_attack;
	}
}

int main() {

	Attack attack1("Mangeur","Grass",10,90,15); /*Name type damage precision use*/

	Character P1("Jaja", 30, 14, 20, 2, 8, 9, 1,"Grass"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */
	Character P2("L'ane Trotro", 100, 10, 50, 0, 0, 0, 0, "Water"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */

	//std::vector<Attack> attacks;
	//attacks.push_back(attack1);

	while (P1.IsAlive() && P2.IsAlive()) {
		bool *can_attack = new bool;
		*can_attack = CalculPrecision(attack1);
		if (*can_attack == true) {
			std::cout << P2.getHp();
			int damage = CalculDamage(attack1, P1, P2);
			std::cout << "Vous avez blesse votre ennemi de " << damage << " hp" << std::endl; /*attack ally enemy*/
			P2.takeDamage(damage);
			std::cout << P2.getHp() << std::endl;
		}
		else {
			std::cout << "L'attaque a raté ! " << std::endl;
		}
		std::cout << "Appuyez pour continuer..." << std::endl;
		char ch = getchar();

		delete can_attack;
	}
	return 0;
}
// for i in range len(attacks): if character.getCapa1 == attacks.index(attacks[i]): vector (vector player attacks qui stock les pointeurs des attaques) push back attacks[i]