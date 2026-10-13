//without prototype :

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Simple NPC class — no Prototype
class NPC {
public:
    string name;
    int health;
    int attack;
    int defense;

    // “Heavy” constructor: every field must be provided
    NPC(const string& name, int health, int attack, int defense) {

        // call database
        // complex calc
        this->name = name;
        this->health = health;
        this->attack = attack;
        this->defense = defense;

        cout << "Creating NPC '" << name << "' [HP:" << health << ", ATK:" 
             << attack << ", DEF:" << defense << "]\n";
    }

    void describe() {
        cout << "  NPC: " << name << " | HP=" << health << " ATK=" << attack
             << " DEF=" << defense << "\n";
    }
};

int main() {
    // Base Alien
    NPC* alien = new NPC("Alien", 30, 5, 2);
    alien->describe();

    // Powerful Alien — must re-pass all stats, easy to make mistakes
    NPC* alien2 = new NPC("Powerful Alien", 30, 5, 5);
    alien2->describe();

    // If you want 100 aliens, you'd repeat this 100 times…

    // cleanup
    delete alien;
    delete alien2;
    return 0;
}






// with prototype:

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Cloneable (aka Prototype) interface
class Cloneable {
public:
    virtual Cloneable* clone() const = 0;
    virtual ~Cloneable() {}
};

class NPC : public Cloneable {
public:
    string name;
    int health;
    int attack;
    int defense;

    NPC(const string& name, int health, int attack, int defense) {
        // call database
        // complex calc
        this->name = name; 
        this->health = health; 
        this->attack = attack; 
        this->defense = defense;
        cout << "Setting up template NPC '" << name << "'\n";
    }

    // copy‐ctor used by clone()
    NPC(const NPC& other) {
        name = other.name;
        health = other.health;
        attack = other.attack;
        defense = other.defense;
        cout << "Cloning NPC '" << name << "'\n";
    }

    // the clone method required by Prototype
    Cloneable* clone() const override {
        return new NPC(*this);
    }

    void describe() {
        cout << "NPC " << name  << " [HP=" << health  << " ATK=" << attack 
             << " DEF=" << defense << "]\n";
    }

    // setters to tweak the clone…
    void setName(const string& n) { 
        name = n;
    }
    void setHealth(int h) { 
        health = h;
    }
    void setAttack(int a) {
         attack = a; 
    }
    void setDefense(int d){ 
        defense = d;
    }
};

int main() {
    // 1) build one “heavy” template
    NPC* alien = new NPC("Alien", 30, 5, 2);

    // 2) quickly clone + tweak as many variants as you like:
    NPC* alienCopied1 = dynamic_cast<NPC*>(alien->clone());
    alienCopied1->describe();

    NPC* alienCopied2 = dynamic_cast<NPC*>(alien->clone());
    alienCopied2->setName("Powerful Alien");
    alienCopied2->setHealth(50);
    alienCopied2->describe();

    // cleanup
    delete alien;
    delete alienCopied1;
    delete alienCopied2;
}

        