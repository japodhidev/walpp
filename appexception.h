#ifndef APPEXCEPTION_H
#define APPEXCEPTION_H

#include <QException>


class AppException: public QException {
public:
    std::string errorMessage;
    explicit AppException(std::string &message);
    void raise() const override { throw *this; }
    AppException *clone() const override { return new AppException(*this); }
};


#endif // APPEXCEPTION_H
