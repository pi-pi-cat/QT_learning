#include "experson.h"

ExPerson::ExPerson(QString name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

int ExPerson::getAge()
{
    return m_age;
}

void ExPerson::setAge(int value)
{
    if (m_age != value) {
        m_age = value;
        emit ageChanged(value);  // 发送属性改变信号
    }
}

void ExPerson::incAge()
{
    setAge(m_age + 1);  // 使用setAge以确保发送信号
} 