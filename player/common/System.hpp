#pragma once

#include <dbus/dbus.h>
#include <unistd.h>
#include <stdint.h>
#include <exception>

class DBusException : public std::exception
{
public:
    DBusException(DBusError error);
    const char* what() const noexcept override;

private:
    DBusError m_error;
};

class System
{
public:
    void preventSleep();

private:
    DBusConnection* connectToDbus();
    DBusMessage* createInhibitCall(const char* pid, const char* reason);

    uint32_t sendAndWaitForResponse(DBusConnection* connection, DBusMessage* message);

private:
    DBusError m_error;

};
