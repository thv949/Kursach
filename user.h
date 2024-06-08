#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    static User* getInstance();

    void setPersonID(int id);
    int getPersonID() const;

    void setRole(const QString& role);
    QString getRole() const;

    void setUnit(const QString& unit);
    QString getUnit() const;

private:
    User() = default;
    static User* instance;
    int personID;
    QString role;
    QString unit;
};

#endif // USER_H
