#include "user.h"

User* User::instance = nullptr;

User* User::getInstance() {
    if (!instance) {
        instance = new User();
    }
    return instance;
}

void User::setPersonID(int id) {
    personID = id;
}

int User::getPersonID() const {
    return personID;
}

void User::setRole(const QString& role) {
    this->role = role;
}

QString User::getRole() const {
    return role;
}

void User::setUnit(const QString& unit) {
    this->unit = unit;
}

QString User::getUnit() const {
    return unit;
}
