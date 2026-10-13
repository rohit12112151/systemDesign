//without mediator

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Each User knows *all* the others directly.
// If you have N users, you wind up wiring N*(N–1)/2 connections,
// and every new feature (mute, private send, logging...) lives in User too.

class User {
private:
    string name;
    vector<User*> peers;
    vector<string> mutedUsers;

public:
    User(const string& n) {
        name = n;
    }

    // must manually connect every pair → N^2 wiring
    void addPeer(User* u) {
        peers.push_back(u);
    }

    // duplication: everyone has its own mute list
    void mute(const string& userToMute) {
        mutedUsers.push_back(userToMute);
    }

    // broadcast to all peers
    void send(const string& msg) {
        cout << "[" << name << " broadcasts]: " << msg << endl;
        for (User* peer : peers) {
            
            // if they have muted me dont send.
            if(!peer->isMuted(name)) {
                peer->receive(name, msg);
            }
        }
    }

    bool isMuted(string userName) {
        for(auto name : mutedUsers) {
            if(name == userName) {
                return true;
            }
        }
        return false;
    }

    // private send - duplicated in every class
    void sendTo(User* target, const string& msg) {
        cout << "[" << name << "→" << target->name << "]: " << msg << endl;
        if(!target->isMuted(name)) {
            target->receive(name, msg);
        }
    }

    void receive(const string& from, const string& msg) {
        cout << "    " << name << " got from " << from << ": " << msg << endl;
    }
};

int main() {
    // create users
    User* user1 = new User("Rohan");
    User* user2 = new User("Neha");
    User* user3 = new User("Mohan");

    // wire up peers (each knows each other) → n*(n-1)/2 connections
    user1->addPeer(user2);   
    user2->addPeer(user1);

    user1->addPeer(user3);   
    user3->addPeer(user1);

    user2->addPeer(user3); 
    user3->addPeer(user2);

    // mute example: Mohan mutes Rohan (Hence Rohan add Mohan to its muted list).
    user1->mute("Mohan");

    // broadcast
    user1->send("Hello everyone!");

    // private
    user3->sendTo(user2, "Hey Neha!");

    // cleanup
    delete user1;
    delete user2;
    delete user3;
    return 0;
}




















//with mediator

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// forward‐declare Colleague so IMediator can refer to it
class Colleague;

// ─────────────── Mediator Interface ───────────────
class IMediator {
public:
    virtual ~IMediator() {}
    virtual void registerColleague(Colleague* c) = 0;
    virtual void send(const string& from, const string& msg) = 0;
    virtual void sendPrivate(const string& from, const string& to, const string& msg) = 0;
};

// ─────────────── Colleague Interface ───────────────
class Colleague {
protected:
    IMediator* mediator;  

public:
    Colleague(IMediator* m) {
        mediator = m;
        mediator->registerColleague(this);
    }
    virtual ~Colleague() {}

    virtual string getName() = 0;
    virtual void send(const string& msg) = 0;
    virtual void sendPrivate(const string& to, const string& msg) = 0;
    virtual void receive(const string& from, const string& msg) = 0;
};

// ─────────────── Concrete Mediator ───────────────
class ChatMediator : public IMediator {
private:
    vector<Colleague*> colleagues;
    vector<pair<string,string>> mutes; // (muter, muted)

public:
    void registerColleague(Colleague* c) override {
        colleagues.push_back(c);
    }

    void mute(const string& who, const string& whom)  {
        mutes.push_back(make_pair(who, whom));
    }

    void send(const string& from, const string& msg) override {
        cout << "[" << from << " broadcasts]: " << msg << "\n";
        for (Colleague* c : colleagues) {
            // Don't send msg to itself.
            if (c->getName() == from) continue;

            bool isMuted = false;
            // Ignore if person is muted
            for (auto& p : mutes) {
                if (from == p.second && c->getName() == p.first) {
                    isMuted = true;
                    break;
                }
            }
            if (!isMuted) {
                c->receive(from, msg);
            }
        }
    }

    void sendPrivate(const string& from, const string& to, const string& msg) override {
        cout << "[" << from << "→" << to << "]: " << msg << "\n";
        for (Colleague* c : colleagues) {
            if (c->getName() == to) {
                for (auto& p : mutes) {

                    //Dont send if muted
                    if (from == p.second && to == p.first) {
                        cout << "\n[Message is muted]\n";
                        return;
                    }
                }
                c->receive(from, msg);
                return;
            }
        }
        cout << "[Mediator] User \"" << to << "\" not found]\n";
    }
};

// ─────────────── Concrete Colleague ───────────────
class User : public Colleague {
private:
    string name;

public:
    User(const string& n, IMediator* m)
      : Colleague(m) {
        name = n;
    }

    string getName() override {
        return name;
    }

    void send(const string& msg) override {
        mediator->send(name, msg);
    }

    void sendPrivate(const string& to, const string& msg) override {
        mediator->sendPrivate(name, to, msg);
    }

    void receive(const string& from, const string& msg) override {
        cout << "    " << name << " got from " << from << ": " << msg << "\n";
    }
};

// ─────────────── Demo ───────────────
int main() {
    ChatMediator* chatRoom = new ChatMediator();

    User* user1 = new User("Rohan", chatRoom);
    User* user2 = new User("Neha",  chatRoom);
    User* user3 = new User("Mohan", chatRoom);

    // Rohan mutes Mohan
    chatRoom->mute("Rohan", "Mohan");

    // broadcast from Rohan
    user1->send("Hello Everyone!");

    // private from Mohan to Neha
    user3->sendPrivate("Neha", "Hey Neha!");

    // cleanup
    delete user1;
    delete user2;
    delete user3;
    delete chatRoom;
    return 0;
}