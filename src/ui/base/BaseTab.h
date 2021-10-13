#ifndef G14CONTROLPP_BASETAB_H
#define G14CONTROLPP_BASETAB_H

#include <QWidget>

class BaseTab {

public:
    virtual ~BaseTab() { }
    virtual void setSelected(bool selected) = 0;
};

#endif //G14CONTROLPP_BASETAB_H
