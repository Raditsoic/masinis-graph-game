#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <algorithm>

using namespace std;

class NPC;

class Character {
     private:
        int level;
        int expCap;
        int exp;
        string name;
        float maxHealth;
        float currentHealth;
        float magicPower;
        float attackPower;
        float current_location;
        float speed;

    public:
        string basicName;
        int skillCooldown;
        string skillName;
        int ultimateCooldown;
        string ultimateName;

        Character(const string& name, float health, float ap, float mp, float speed, string basic, string skill, string ultimate) : name(name), maxHealth(health), currentHealth(health), attackPower(ap), magicPower(mp), speed(speed), 
        basicName(basic), skillName(skill), ultimateName(ultimate), current_location(0), exp(0), level(1), expCap(20), skillCooldown(0), ultimateCooldown(0) {}

        virtual ~Character() {}

        virtual void attack(NPC &enemy) = 0;

        virtual void addAttributes(int level) = 0;

        void move(int new_location) {
            current_location = new_location;
        }

        int getLocation() {
            return current_location;
        }

        int getLevel() {
            return level;
        }

        float getMaxHealth() {
            return maxHealth;
        }

        float getCurrentHealth() {
            return currentHealth;
        }

        float getSpeed() {
            return speed;
        }

        float getMagicPower() {
            return magicPower;
        }

        float getAttackPower() {
            return attackPower;
        }

        void setCurrentHealth(float hp) {
            currentHealth = hp;
        }

        void setMaxHealth(float hp) {
            maxHealth = hp;
        }

        void setAttackPower(float ap) {
            attackPower = ap;
        }

        void setMagicPower(float mp) {
            magicPower = mp;
        }

        void setSpeed(float sp) {
            speed = sp;
        }

        bool isAlive() {
            return currentHealth > 0;
        }

        string getName() {
            return name;
        }

        void setEXP(int e) {
            exp += e;
        }

        void checkEXP() {
            int prevLevel = level;
            while(true) {
                if (exp >= expCap) {
                    level += 1;
                    exp = exp - expCap;
                    expCap += 20;
                } else {
                    break;
                }
            }
            cout << name << " has leveled up to level " << level << "\n-----------------------\n";

            if (prevLevel != level) {
                addAttributes(level - prevLevel);
            } 
        }
};

class NPC {
    private:
        string name;
        float health;
        float power;
        float level;
        int exp_give;
        int location;
        float speed;
        bool deadState;

    public:
        NPC(const string& n, float h, float p, int l, int exp_give, float loc) : name(n), health(h), power(p), level(l), exp_give(exp_give), location(loc), deadState(false) {} 

        float getHealth() {
            return health;
        }

        float getSpeed() {
            return speed;
        }

        float getEXP() {
            return exp_give;
        }

        float getAttack() {
            return power;
        }

        int getLevel() {
            return level;
        }

        bool getDeadState() {
            return deadState;
        }

        void setHealth(float x) {
            health = x;
        }

        void setDead() {
            deadState = true;
        }

        string getName() {
            return name;
        }

        bool isAlive() {
            return health > 0;
        }

        void attack(Character &player) {
            float damage = getAttack();
            player.setCurrentHealth(player.getCurrentHealth() - damage);
            cout << "\n" << getName() << " has attacked " << player.getName() << " by " << damage << " damage" << endl;
        }       
};

class Mage : public Character {
    public:

        Mage(const string& name) : Character(name, 90, 5, 20, 20, "Bonk!", "Slapur Slaps!", "Kuchiyose: Mahoraga") {}

        void attack(NPC& enemy) override {
            bool skillOnCooldown = (skillCooldown > 0);
            bool ultimateOnCooldown = (ultimateCooldown > 0);

            cout << "Attack Ability:\n1. "<< basicName << "\n2. " << skillName;
            if (skillOnCooldown) {
                cout << "(" << skillCooldown << ")";
            }
            cout << "\n3. " << ultimateName;
            if (ultimateOnCooldown) {
                cout << "(" << ultimateCooldown << ")";
            }

            int skill;
            while (true) {
                cout << "\nChoose Ability (1/2/3): ";
                cin >> skill;

                if (skill == 1) {
                    float damage = getAttackPower();
                    enemy.setHealth(enemy.getHealth() - damage);
                    cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << basicName << endl;
                    break;
                } else if (skill == 2) {
                    if (skillOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getMagicPower() * (1 + 0.3);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << skillName << endl;
                        skillCooldown = 2;
                    }
                    break;
                } else if (skill == 3) {
                    if (ultimateOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getMagicPower() * (1 + 1);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " to " << enemy.getName() << " with " << ultimateName << endl;
                        ultimateCooldown = 4;
                    }
                    break;
                } else {
                    cout << "Invalid choice. Please choose 1, 2, or 3." << endl;
                }
            }

            if (skillOnCooldown) {
                skillCooldown--;
            }
            if (ultimateOnCooldown) {
                ultimateCooldown--;
            }
        }

        void addAttributes(int level) {
            setMaxHealth(getMaxHealth() + level * 7);
            setCurrentHealth(getMaxHealth());
            setAttackPower(getAttackPower() + level * 0.3);
            setMagicPower(getMagicPower() + level * 4);
            setSpeed(getSpeed() + level * 2);
        }
};

class Sniper : public Character {
    public:
        Sniper(const string& n) : Character(n, 95, 22, 13, 17, "Dor!", "Showstopper", "Satanael: Sinfull Shell") {}

        void attack(NPC& enemy) override {
            bool skillOnCooldown = (skillCooldown > 0);
            bool ultimateOnCooldown = (ultimateCooldown > 0);

            cout << "Attack Ability:\n1. " << basicName << "\n2. " << skillName;
            if (skillOnCooldown) {
                cout << "(" << skillCooldown << ")";
            }
            cout << "\n3. " << ultimateName;
            if (ultimateOnCooldown) {
                cout << "(" << ultimateCooldown << ")";
            }

            int skill;
            while (true) {
                cout << "\nChoose Ability (1/2/3): ";
                cin >> skill;

                if (skill == 1) {
                    float damage = getAttackPower();
                    enemy.setHealth(enemy.getHealth() - damage);
                    cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << basicName << endl;
                    break;
                } else if (skill == 2) {
                    if (skillOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getAttackPower() * (1.3);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << skillName << endl;
                        skillCooldown = 2;
                    }
                    break;
                } else if (skill == 3) {
                    if (ultimateOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getAttackPower() * (1 + 1) + getMagicPower() * (0.12);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " to " << enemy.getName() << " with " << ultimateName << endl;
                        ultimateCooldown = 4;
                    }
                    break;
                } else {
                    cout << "Invalid choice. Please choose 1, 2, or 3." << endl;
                }
            }

            if (skillOnCooldown) {
                skillCooldown--;
            }
            if (ultimateOnCooldown) {
                ultimateCooldown--;
            }
        }

        void addAttributes(int level) {
            setMaxHealth(getMaxHealth() + level * 6);
            setCurrentHealth(getMaxHealth());
            setAttackPower(getAttackPower() + level * 3);
            setMagicPower(getMagicPower() + level * 1);
            setSpeed(getSpeed() + level * 1);
        }
};

class Rogue : public Character {
    public:
        Rogue(const string& n) : Character(n, 75, 27, 12, 25, "Steal", "Hassou Tobi", "Storm that is Approaching") {}

        void attack(NPC& enemy) override {
            bool skillOnCooldown = (skillCooldown > 0);
            bool ultimateOnCooldown = (ultimateCooldown > 0);

            cout << "Attack Ability:\n1. " << basicName << "\n2. " << skillName;
            if (skillOnCooldown) {
                cout << "(" << skillCooldown << ")";
            }
            cout << "\n3. " << ultimateName;
            if (ultimateOnCooldown) {
                cout << "(" << ultimateCooldown << ")";
            }

            int skill;
            while (true) {
                cout << "\nChoose Ability (1/2/3): ";
                cin >> skill;

                if (skill == 1) {
                    float damage = getAttackPower();
                    enemy.setHealth(enemy.getHealth() - damage);
                    cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << basicName << endl;
                    break;
                } else if (skill == 2) {
                    if (skillOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getAttackPower() * (1 + 0.3);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " damage to " << enemy.getName() << " with " << skillName << endl;
                        skillCooldown = 2;
                    }
                    break;
                } else if (skill == 3) {
                    if (ultimateOnCooldown) {
                        cout << "The ability is still in cooldown, choose another ability." << endl;
                        continue;
                    } else {
                        float damage = getAttackPower() * (1 + 0.5) + getMagicPower() * (0.5);
                        enemy.setHealth(enemy.getHealth() - damage);
                        cout << "\n" << getName() << " deals " << damage << " to " << enemy.getName() << " with " << ultimateName << endl;
                        ultimateCooldown = 4;
                    }
                    break;
                } else {
                    cout << "Invalid choice. Please choose 1, 2, or 3." << endl;
                }
            }

            if (skillOnCooldown) {
                skillCooldown--;
            }
            if (ultimateOnCooldown) {
                ultimateCooldown--;
            }
        }

        void addAttributes(int level) {
            setMaxHealth(getMaxHealth() + level * 5);
            setCurrentHealth(getMaxHealth());
            setAttackPower(getAttackPower() + level * 5);
            setMagicPower(getMagicPower() + level * 1.3);
            setSpeed(getSpeed() + level * 2);
        }
};

class TurnBasedGame {
    public:
        TurnBasedGame(Character &p, NPC &e) : player(p), enemy(e), PlayerTurn(false) {}

        void run() {
            if (player.getSpeed() > enemy.getSpeed()) {
                PlayerTurn = true;
            } else {
                PlayerTurn = false;
            }

            while(player.isAlive() && enemy.isAlive()) {
                displayState();
                Sleep(500);
                if (PlayerTurn) {
                    player.attack(enemy);
                    PlayerTurn = !PlayerTurn;
                } else {
                    enemy.attack(player);
                    PlayerTurn = !PlayerTurn;
                }
            }
            cout << endl;
            displayResult();
        }

    private:
        Character &player;
        NPC &enemy;
        bool PlayerTurn; 

        void displayState() {
            cout << endl << "-----------------------" << endl;
            cout << player.getName() << "'s health: " << player.getCurrentHealth() << "(" << player.getLevel() << ") | " << enemy.getName() << "'s health: " << enemy.getHealth() << " (" << enemy.getLevel() << ")" << endl;
            cout << "-----------------------" << endl;
        }

        void displayResult() {
            if (player.isAlive())
            {
                int EXPget = enemy.getEXP();
                cout << "-----------------------\nYou have defeated " << enemy.getName() << ", you get " << EXPget << " EXP" << endl;
                enemy.setDead();
                player.setEXP(EXPget);
                player.checkEXP();
            } else {
                cout << "You have been defeated, Game Over!" << endl;
            }
        }
};

class Map {
    public:
        Map(Character *character) : player(character) {}

        void add_node(int node_id, NPC *npc = nullptr, string name = nullptr) {
            nodes[node_id] = npc;
            node_names[node_id] = name; 
        }

        void add_edge(int node_a, int node_b) {
            edges[node_a].push_back(node_b);
            edges[node_b].push_back(node_a);
        }

        vector<int> get_neighbors(int node_id) {
            auto it = edges.find(node_id);
            if (it != edges.end()) {
                return it->second;
            } else {
                return {};
            }
        }

        string get_node_name(int node_id) {
            auto it = node_names.find(node_id);
            if (it != node_names.end()) {
                return it->second;
            } else {
                return "Unknown Node";
            }
        }

        void move_player(int current_node, int target_node) {
            
            auto it = nodes.find(target_node);
            cout << "\n-----------------------" << endl;
            cout << "You travels to  " << get_node_name(target_node);
            player->move(target_node);
            if (player->getLocation() == 16) {
                cout << "\nCongratulations! You just beat the Game!" << endl;
                delete player;
                return;
            }
            if (it != nodes.end()) {
                NPC* npc = it->second;
                if (npc && !npc->getDeadState()) {
                        TurnBasedGame combat(*player, *npc);
                        cout << " and encounters a  " << npc->getName() << ", Commencing Battle!!";
                        combat.run();   
                } else {
                    cout  << ", but no enemies were found." << endl;
                    cout << "-----------------------" << endl;
                }
            } else {
                cout << ", no enemies were found." << endl;
                cout << "-----------------------" << endl;
            }
        }

    private:
        unordered_map<int, NPC*> nodes;
        unordered_map<int, vector<int>> edges;
        unordered_map<int, string> node_names;
        Character *player;
            
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    cout << "---------- Welcome to Masinis Final Project ----------" << endl;
    Sleep(2000);

    string name; cout << "Input your name here: "; cin >> name;

    Character* player = nullptr;

    while(1) {
         string role; cout << "Select which role you want to play (Mage/Sniper/Rogue): "; cin >> role;
         if (role == "Mage") {
            player = new Mage(name);
            break;
        } else if (role == "Sniper") {
            player = new Sniper(name);
            break;
        } else if (role == "Rogue")  {
            player = new Rogue(name);
            break;
        } else {
             cout << "Invalid role. Please enter Mage, Sniper, or Rogue." << endl;
        }
        
    }

    Map gameMap(player);
    gameMap.add_node(0, nullptr, "Mondstadt");
    gameMap.add_node(1, new NPC("Azdaha", 50, 10, 1, 25, 1), "Liyue");
    gameMap.add_node(2, new NPC("Raiden Shogun", 120, 30, 5, 100, 2), "Inazuma");
    gameMap.add_node(3, new NPC("Oray Kobra", 50, 7, 2, 30, 3), "Chasm");
    gameMap.add_node(4, nullptr, "Sumeru");
    gameMap.add_node(5, nullptr, "Fontaine");
    gameMap.add_node(6, nullptr, "Natlan");
    gameMap.add_node(7, new NPC("Dansleif", 70, 15, 7, 80, 7), "Khaenriah");
    gameMap.add_node(8, new NPC("Tsaritsa", 50, 25, 10, 150, 8), "Snezhnaya");
    gameMap.add_node(9, new NPC("Unknown God", 50, 19, 11, 230, 9), "Celestia");
    gameMap.add_node(10, nullptr, "Astral Express");
    gameMap.add_node(11, nullptr, "Hertha Space Station");
    gameMap.add_node(12, new NPC("Cocolia", 50, 10, 13, 150, 12), "Belobog");
    gameMap.add_node(13, new NPC("Tenpenny", 30, 15, 9, 170, 13), "San Fierro");
    gameMap.add_node(14, nullptr, "Las Venturas");
    gameMap.add_node(15, new NPC("Big Smoke", 50, 5, 15, 10, 15), "Los Santos");
    gameMap.add_node(16, nullptr, "Groove Street");

    gameMap.add_edge(0, 1);
    gameMap.add_edge(1, 2);
    gameMap.add_edge(1, 3);
    gameMap.add_edge(1, 4);
    gameMap.add_edge(3, 4);
    gameMap.add_edge(4, 5);
    gameMap.add_edge(5, 6);
    gameMap.add_edge(5, 9);
    gameMap.add_edge(6, 7);
    gameMap.add_edge(6, 8);
    gameMap.add_edge(9, 10);
    gameMap.add_edge(10, 11);
    gameMap.add_edge(10, 12);
    gameMap.add_edge(10, 13);
    gameMap.add_edge(11, 12);
    gameMap.add_edge(13, 14);
    gameMap.add_edge(13, 15);
    gameMap.add_edge(13, 16);


    while(player->isAlive() && player->getLocation() != 16) {
        int move;
        
        cout << "\n-----------------------\n" << "You are in " << gameMap.get_node_name(player->getLocation()) << "." << endl <<  "Masinis Express can travel to:" << endl;
        vector<int> neighbors = gameMap.get_neighbors(player->getLocation());
        for (int i : neighbors) {
            cout << i << ". " << gameMap.get_node_name(i) << endl;
        }
        cout << "Where do you want to go? ";
        bool valid_move = false;
        while(!valid_move) {
            cin >> move;
            if (find(neighbors.begin(), neighbors.end(), move) != neighbors.end()) {
                gameMap.move_player(player->getLocation(), move);
                valid_move = true;
            } else {
                cout << "Invalid location to go to." << endl;
            }
        }
        
    }
    return 0;
}