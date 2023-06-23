
#include <iostream>
#include <functional>


using namespace std;


/* C++11 : compile time polymorphism using function obj*/
class AdvRobot{
public:
    //using function obj which is a callable obj that return void with ref to Robot
    using Search = function<void(AdvRobot &)>; 
    using Attack = function<void(AdvRobot &)>;
    using Defend = function<void(AdvRobot &)>; 
    
    /* initialize strategy from user by type of search attack & defend */
    AdvRobot(Search search, Attack attack, Defend defend):
        m_search{search}, m_attack{attack}, m_defend{defend} {}

    // activate robots by users
    void search() { m_search(*this);}  // Search(AdvRobot &)
    void attack() { m_attack(*this);}  // Attack(AdvRobot &)
    void defend() { m_defend(*this);}  // Defend(AdvRobot &)

private:
    // declare all as function obj so you can call them as : m_search(*this)
    Search m_search; 
    Attack m_attack;
    Defend m_defend;
};

// functor objects
class AdvLinear {
public:
    void operator()(AdvRobot &r) {cout << "search linear\n";}
};

class AdvSpiral {
public:
    void operator()(AdvRobot &r) {cout << "search spiral\n";}
};

class AdvPinch {
public:
    void operator()(AdvRobot &r) {cout << "attack pinch\n";}
};

class AdvPunch {
public:
    void operator()(AdvRobot &r) {cout << "attack punch\n";}
};

class AdvRun {
public:
    void operator()(AdvRobot &r) {cout << "defend run\n";}
};

void go(AdvRobot &robot) {
    robot.search() ;
    robot.attack();
    robot.defend();
}

int main() {

    AdvSpiral adv_spiral{}; // value initialization in case there is no default constructor 
    AdvLinear adv_linear{};
    AdvRun adv_run{};
    AdvPinch adv_pinch{};
    AdvPunch adv_punch{};

    AdvRobot robot3(adv_linear, adv_punch, adv_run);
    AdvRobot robot4(adv_spiral, adv_pinch, adv_run);
    AdvRobot robot5([](AdvRobot &r){cout << "search direct\n";}, 
                    [](AdvRobot &r){cout << "attack push\n";},
                    [](AdvRobot &r){cout << "defend hide\n";});

    cout << " **\n";
    go(robot3);
    cout << " **\n";
    go(robot4);
    cout << " **\n";
    go(robot5);

    return 0;
}


