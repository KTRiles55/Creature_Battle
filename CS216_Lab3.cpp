#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>


//Maximum total number of creatures that can be read in file. This can be modified by the user.
const int MIN_TOTAL_CREATURES = 5;

const int MAX_TOTAL_CREATURES = 10;

const int MIN_STAT_RANGE = 10;

const int MAX_STAT_RANGE = 90;



class Creatures
{
private:

	std::string ID;

	int health;

	int strength;

public:

	//Mutator functions to set stats.

	void setID(std::string name)
	{
		ID = name;
	}

	void setHealth(int AmountofHealth)
	{
		health = AmountofHealth;
	}

	void setStrength(int hitPower)
	{
		strength = hitPower;
	}

	//Accessor functions to retrieve stats.

	std::string getID() const
	{
		return ID;
	}

	int getHealth() const
	{
		return health;
	}

	int getStrength() const
	{
		return strength;
	}

	int getDamage() const
	{
		return ((rand() % strength) + 1);
	}

	std::string ToString() const;

	std::string setBattleToString() const; 
};

void findCreature(Creatures monsters[]);

void setCreature(Creatures monsters[], std::string new_ID, int new_Health, int new_Strength, int creature_count);

void analyzeNumOfCreatures(int &creature_count);

void showStats(Creatures monsters[]);

void choose_Fighters(Creatures monsters[], Creatures& battle_monster, Creatures& battle_monsterOpponent);

void battle_with_creatures(Creatures& battle_monster, Creatures& battle_monsterOpponent);

void creature_attacks(Creatures& battle_monster, Creatures& battle_monsterOpponent);

void monster_takes_turn(Creatures& attacker, Creatures& defender);

void monster_Opponent_takes_turn(Creatures& attacker, Creatures& defender);

void resetCreatureStats(Creatures monsters[]);

void showMenu();

void continueEachRound();



int main()
{
	srand((unsigned int)time(0));

	std::string monster_alias;
	int menuSelect = 0;

	Creatures monster_list[MAX_TOTAL_CREATURES];
	Creatures chosen_monster;
	Creatures chosen_monster_opponent;

	//reads from file listing creature names.
	findCreature(monster_list);

	//Displays menu to select from by user input.
	while (menuSelect != 3)
	{
		showMenu();
		std::cin >> menuSelect;
		switch (menuSelect)
		{
		case 1:

			//Displays creature stats. 
			showStats(monster_list);
			break;

		case 2:

			//Allows user to choose fighters to battle it out.
			choose_Fighters(monster_list, chosen_monster, chosen_monster_opponent);
			resetCreatureStats(monster_list);
			break;

		case 3:

			//Terminates program.
			std::cout << "\n\nEnding program...\n\n";
			break;

			//If user enters a letter or non-alpha character, this restarts the input stream.
		default:

			std::cin.clear();
			std::cin.ignore(' ', '\n');
			std::cout << "\n\nPlease try typing and entering the correct menu selection number again.\n\n";
		}
	}
	return 0;
}




void findCreature(Creatures monsters[])
{
	std::ifstream read_file;
	int i;
	read_file.open("creatures.txt", std::ios::in);
	if (read_file.fail())
	{
		std::cout << "\n\nThis file cannot be accessed.\n\n";
		exit(EXIT_FAILURE);
	}
	int file_scanner = read_file.peek();

	//checks if file is empty or not.
	if (file_scanner == EOF)
	{
		std::cout << "\n\nThis file contains no data (please add input in file).\n\n";
		exit(EXIT_FAILURE);
	}
	std::string tempID;
	int creature_count = 0;
	while (read_file >> tempID)
	{
		//calculates health and strength for each creature, then sets the stats.
		setCreature(monsters, tempID, MIN_STAT_RANGE + (rand() % MAX_STAT_RANGE), MIN_STAT_RANGE + (rand() % MAX_STAT_RANGE), creature_count);
		creature_count++;
	}
	//Checks whether there are 10 creatures in total.
	analyzeNumOfCreatures(creature_count);
	read_file.close();
}


//Sets the name and stats for creature.
void setCreature(Creatures monsters[], std::string new_ID, int new_Health, int new_Strength, int creature_count)
{
	monsters[creature_count].setID(new_ID);
	monsters[creature_count].setHealth(new_Health);
	monsters[creature_count].setStrength(new_Strength);
}



void showMenu()
{
	std::cout << "\n\n\n(1) View list of creatures and their stats (including damage that a creature can inflict).\n\n"
				 "(2) Battle with two creatures.\n\n"
				 "(3) Exit the program.\n\n\n\n"
				 "Please enter one of the following choices[1-3]: ";
}


//Formats each creature object into a string.
std::string Creatures::ToString() const
{
	std::ostringstream creature_stats;
	creature_stats << std::left << std::setw(25) << getID() << std::setw(20) << getHealth() << std::setw(24) << getStrength() << getDamage();
	return creature_stats.str();
}


//Displays list of creatures.
void showStats(Creatures monsters[])
{
	int i;
	std::cout << "\n\nCreature ID" << std::right << std::setw(19) << "Health" << std::setw(24) << "Hit Strength" << std::setw(28) << "Damage Inflicted\n";
	for (i = 0; i < MAX_TOTAL_CREATURES; ++i)
	{
		std::cout << "\n" << i + 1 << ". " << monsters[i].ToString() << "\n";
	}
}



void choose_Fighters(Creatures monsters[], Creatures& battle_monster, Creatures& battle_monsterOpponent)
{
	int chosen_creature_num;
	int response_confirm = 0;
	while (response_confirm == 0)
	{
		showStats(monsters);
		std::cout << "\n\nPlease select a monster for battle by typing and then entering the number on the left that lists the monster. ";
		std::cin >> chosen_creature_num;
		if ((chosen_creature_num >= 1) && (chosen_creature_num <= MAX_TOTAL_CREATURES))
		{
			battle_monster = monsters[chosen_creature_num - 1];
			response_confirm += 1;
			std::cout << "\n\nYou have selected " << battle_monster.getID() << " to participate in battle!\n";
		}

		else
		{
			std::cin.clear();
			std::cin.ignore(' ', '\n');
			std::cout << "\n\nPlease choose a monster from this list by typing the correct number\n\n\n";
		}
	}

	while (response_confirm == 1)
	{
		showStats(monsters);
		std::cout << "\n\nPlease select a monster as an opponent for battle by typing" 
		" and then entering the number on the left that lists the monster. ";
		std::cin >> chosen_creature_num;
		if ((chosen_creature_num >= 1) && (chosen_creature_num <= MAX_TOTAL_CREATURES))
		{
			battle_monsterOpponent = monsters[chosen_creature_num - 1];
			response_confirm += 1;
			//starts battle process between two chosen creatures.
			battle_with_creatures(battle_monster, battle_monsterOpponent);
		}

		else
		{
			std::cin.clear();
			std::cin.ignore(' ', '\n');
			std::cout << "\n\nPlease choose a monster from this list by typing the correct number.\n\n\n";
		}
	}
}


void battle_with_creatures(Creatures& battle_monster, Creatures& battle_monsterOpponent)
{
	int i = 1;
	continueEachRound();
	std::cout << "\nYou have selected " << battle_monsterOpponent.getID() << " to participate in battle as the opponent!\n\n"
			  << battle_monster.getID() << " is battling against " << battle_monsterOpponent.getID() << "!\n";
	bool startBattle = true;
	while (startBattle)
	{
		if (battle_monster.getHealth() == 0)
		{
			std::cout << "\n\n" << battle_monster.getID() << " is defeated!\n" << battle_monsterOpponent.getID() << " is the winner!\n\n";
			startBattle = false;
		}

		else if (battle_monsterOpponent.getHealth() == 0)
		{
			std::cout << "\n\n" << battle_monsterOpponent.getID() << " is defeated!\n" << battle_monster.getID() << " is the winner!\n\n";
			startBattle = false;
		}

		else
		{
			std::cout << "\n\n\n\nBattle starts now!\n...\nRound " << i << ":\n...\n\n" 
			<< battle_monster.setBattleToString() << "\n\n\n" << battle_monsterOpponent.setBattleToString() << "\n\n\n\n";
			continueEachRound();
			creature_attacks(battle_monster, battle_monsterOpponent);
			i += 1;
		}
	}
}


//Displays GUI for battling monsters and updates the health once damage is inflicted.
std::string Creatures::setBattleToString() const
{
	std::ostringstream battle_stats;
	battle_stats << getID() << "\n" << "Health: " << getHealth() << "\n" << "Hit Strength: " << getStrength() << "\n" << "Damage inflicted: " << getDamage();
	return battle_stats.str();
}



void creature_attacks(Creatures& battle_monster, Creatures& battle_monsterOpponent)
{
	int monster_turn = 1 + (rand() % 2);
	if (monster_turn == 1)
	{
		monster_takes_turn(battle_monster, battle_monsterOpponent);
		monster_Opponent_takes_turn(battle_monsterOpponent, battle_monster);
	}

	else if (monster_turn == 2)
	{
		monster_takes_turn(battle_monsterOpponent, battle_monster);
		monster_Opponent_takes_turn(battle_monster, battle_monsterOpponent);
	}
}



void monster_takes_turn(Creatures& attacker, Creatures& defender)
{
	int health_lost;
	health_lost = 0 + (rand() % attacker.getDamage());
	defender.setHealth(defender.getHealth() - health_lost);

	std::cout << "\n...\n\n" << attacker.setBattleToString() << "\n\n\n" << defender.setBattleToString() << "\n\n...\n\n";
	std::cout << "Now is " << attacker.getID() << "'s turn!\n...\n"
		<< attacker.getID() << " attacks " << defender.getID() << "!\n...\n";

	if (health_lost == 0)
	{
		std::cout << attacker.getID() << " misses!\n...\n";
	}

	std::cout << defender.getID() << " loses " << health_lost << " Health!\n...\n\n";
	if (defender.getHealth() <= 0)
	{
		defender.setHealth(0);
	}
	std::cout << defender.getID() << "'s health is now " << defender.getHealth() << "!\n";
	continueEachRound();
}


void monster_Opponent_takes_turn(Creatures& attacker, Creatures& defender)
{
	if (attacker.getHealth() != 0)
	{
		std::cout << "...\n\n" << attacker.getID() << " is attacking " << defender.getID() << " next!\n\n";
		continueEachRound();
		monster_takes_turn(attacker, defender);
	}
}


void resetCreatureStats(Creatures monsters[])
{
	int i;
	for (i = 0; i < MAX_TOTAL_CREATURES; ++i)
	{
		monsters[i].setHealth(MIN_STAT_RANGE + (rand() % MAX_STAT_RANGE));
		monsters[i].setStrength(MIN_STAT_RANGE + (rand() % MAX_STAT_RANGE));
	}
	std::cout << "\n\nAll Creature stats (health & strength) have been reset.\n\n";
}


void continueEachRound()
{
	std::cout << "\n\n*Press \"ENTER\" to continue...*\n\n";
	std::cin.get();
}



void analyzeNumOfCreatures(int& creature_count)
{
	if ((creature_count < MIN_TOTAL_CREATURES) && (creature_count > 0))
	{
		std::cout << "\n\nThere are not enough creatures.\n"
			"This file must contain between 5-10 creatures.\n"
			"Please input more creatures into the file.\n\n";
		exit(EXIT_FAILURE);
	}

	//If the file reads only input spaces/newlines, but still no creatures.
	else if (creature_count == 0)
	{
		std::cout << "\n\nThis file has no creatures (please input creatures into file).\n\n";
		exit(EXIT_FAILURE);
	}
}
