#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <memory>
using namespace std;

class Item
{
public:
    string name;
    Item(const string& n) : name(n) {}
};

class SpecialAbility 
{
public:
    string name;
    double activationProbability; // Probability of activation (0.0 to 1.0)
    SpecialAbility(const string& n, double p) : name(n), activationProbability(p) {}
};

class Player 
{
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int defense;
    unique_ptr<Item> equippedItem; 
    unique_ptr<SpecialAbility> specialAbility; 

public:
    Player(int h, int mD, int rD, int d) : health(h), maxHealth(h), meleeDamage(mD), rangedDamage(rD), defense(d) {}

    void increaseStats(int healthIncrease, int meleeDamageIncrease, int defenseIncrease) 
    {
        maxHealth += healthIncrease;
        health = maxHealth; // Reset health to max
        meleeDamage += meleeDamageIncrease; //increasing damage power
        defense += defenseIncrease; //increasing defence power
    }

    void equipItem(const string& itemName) 
    {
        equippedItem = make_unique<Item>(itemName);
        cout << "Equipped: " << equippedItem->name << endl;
    }

    void setSpecialAbility(const string& abilityName, double probability) 
    {
        specialAbility = make_unique<SpecialAbility>(abilityName, probability);
        cout << "Special Ability Awarded: " << specialAbility->name << endl;
    }

    bool performSpecialAbility() 
    {
        if (specialAbility && (rand() % 100) < (specialAbility->activationProbability * 100)) 
        {
            cout << "Special Ability Activated: " << specialAbility->name << endl;
            return true;
        }
        return false;
    }

    void takeDamage(int damage) 
    {
        int actualDamage = max(0, damage - defense);
        health -= actualDamage;
        if (health < 0)
        {
            health = 0;
        }
    }

    void heal(int amount) 
    {
        health += amount;
        if (health > maxHealth)
        {
            health = maxHealth;
        }
            
    }

    int attack() 
    {
        return meleeDamage;
    }

    int rangedAttack() 
    {
        return rangedDamage;
    }

    int getHealth()  
    {
        return health;
    }
};

class Enemy 
{
private:
    int health;
    int meleeDamage;
    int defense;

public:
    Enemy(int h, int mD, int d) : health(h), meleeDamage(mD), defense(d) {}

    void takeDamage(int damage) 
    {
        int actualDamage = max(0, damage - defense);
        health -= actualDamage;
        if (health < 0)
        {
            health = 0;
        }
    }

    int attack() 
    {
        return meleeDamage;
    }

    int getHealth() 
    {
        return health;
    }
};

void printPlayerStatus(Player& player) 
{
    cout << "\nYour health: " << player.getHealth() << endl;
}

void battle(Player& player, vector<Enemy>& enemies) 
{
    while (player.getHealth() > 0 && !enemies.empty()) 
    {
        printPlayerStatus(player);
        cout << "Choose an action: (1) Attack (2) Heal" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) // Attack
        { 
            bool specialActivated = player.performSpecialAbility();
            int damage = player.attack();
            if (specialActivated) 
            {
                damage *= 2; // Critical hit doubles the damage
            }
            cout << "You attack with " << damage << " damage!" << endl;

            // Apply damage to the first enemy
            enemies[0].takeDamage(damage);
            if (enemies[0].getHealth() <= 0) 
            {
                cout << "Enemy defeated!" << endl;
                enemies.erase(enemies.begin()); // Removing defeated enemy
            }

            // Enemies attack
            if (!enemies.empty()) 
            {
                for (auto& enemy : enemies) 
                {
                    player.takeDamage(enemy.attack());
                    cout << "Enemy attacks! Damage: " << enemy.attack() << endl;
                    if (player.getHealth() <= 0) 
                    {
                        cout << "You have been defeated!" << endl;
                        return;
                    }
                }
            }
        }
        else if (choice == 2) // Heal
        { 
            int healAmount = 20;
            player.heal(healAmount);
            cout << "You heal for " << healAmount << " HP!" << endl;
        }
    }
}

void levelUp(Player& player, int level) 
{
    if (level == 2) 
    {
        player.increaseStats(20, 5, 2);
        player.setSpecialAbility("Critical Hits", 0.10);
        player.equipItem("Sword");
    }
    else if (level == 3) 
    {
        player.increaseStats(30, 5, 3);
        player.setSpecialAbility("Blocker", 0.10);
        player.equipItem("Shield");
    }
    else if (level == 4) 
    {
        player.increaseStats(40, 5, 4);
        player.setSpecialAbility("Life Steal", 0.10);
        player.equipItem("Armour");
    }
    else if (level == 5) 
    {
        player.increaseStats(50, 5, 5);
        player.setSpecialAbility("Ranged Attack", 0.10);
        player.equipItem("Bow");
    }
    else if (level == 6) 
    {
        player.increaseStats(60, 10, 10);
        player.setSpecialAbility("Ultimate Strike", 0.20);
    }
}

void bossBattle(Player& player) 
{
    cout << "Final Boss Fight: Murloc!" << endl;
    Enemy murloc(200, 20, 10);
    vector<Enemy> enemies = { murloc };

    battle(player, enemies);
    if (player.getHealth() > 0) 
    {
        cout << "You have defeated Murloc and saved the village!" << endl;
    }
}

int main() 
{
    srand(static_cast<unsigned>(time(0))); // Seed random number generator

    Player hero(100, 10, 15, 5);

    for (int level = 1; level <= 5; ++level) 
    {
        cout << "\nLevel " << level << endl;

        vector<Enemy> enemies;
        for (int i = 0; i < level; ++i) 
        {
            enemies.emplace_back(rand() % 30 + 10, rand() % 10 + 5, rand() % 5 + 2);
        }

        cout << "You encounter " << enemies.size() << " enemies!" << endl;
        battle(hero, enemies);

        if (hero.getHealth() <= 0) 
        {
            cout << "Game Over!" << endl; // Game over message
            return 0; 
        }

        if (level < 5) 
        {
            levelUp(hero, level);
        }
        else {
            bossBattle(hero);
        }
    }

    cout << "Game Over!" << endl;
}
