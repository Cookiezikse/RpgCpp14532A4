#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Character {
public:
    Character(const std::string& name, int hp, int defense ,int attack ,int capa1, int capa2, int capa3, int capa4 , const std::string& type)
        : name(name), hp(hp), defense(defense), attack(attack), capa1(capa1), capa2(capa2), capa3(capa3), capa4(capa4), type(type) {}

	const std::string& getName() const {
		return name;
	}

	const std::string& getType() const {
		return type;
	}

	int getHp() const {
		return hp;
	}

	int getDefense() const {
		return defense;
	}

	int getAttack() const {
		return attack;
	}

	bool IsAlive() const {
		return hp > 0;
	}

	void takeDamage(int damage) {
		hp -= damage;
		if (hp <= 0) {
			hp = 0;
			std::cout << name << " is dead!" << std::endl;
		}
	}

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

	const std::string& getName() const {
		return name;
	}
	const std::string& getType() const {
			return type;
	}
	int getDamage() const {
		return damage;
	}
	int getPrecision() const {
		return precision;
	}
	int getUse() const {
		return use;
	}
	int Used() {
		use -= 1;
	}

private:
	const std::string& name;
	const std::string& type;
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

	int modifier;

	if (type_attack == type_Enemy) {
		modifier = 1;
	}
	else if (type_attack != type_Enemy){

		std::cout << "WARNING : prankex" << std::endl;

		if (type_attack == "Fire" && type_Enemy == "Water") {
			modifier = 0.5;
		}
		else if (type_attack == "Water" && type_Enemy == "Fire") {
			modifier = 2;
		}
	}

	damage = (0.5 * Ally_attack * (damage_attack/Enemy_defense)) + 1;

	return damage;
}


int main() {

	Attack attack1("Mangeur","Grass",20,100,15); /*Name type damage precision use*/

	Character Jaja("Jaja", 30, 14, 20, 0, 0, 0, 0,"Grass"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */
	Character Trotro("L'ane Trotro", 100, 1, 50, 0, 0, 0, 0, "Water"); /* NOM HP DEFENSE ATTACK CAPA1 CAPA2 CAPA3 CAPA4 TYPE */

	std::cout << "Character : " << Jaja.getName() << std::endl;
	std::cout << "Character : " << Trotro.getName() << std::endl;

	CalculDamage(attack1,Jaja,Trotro); /*attack ally enemy*/

	return 0;
}
