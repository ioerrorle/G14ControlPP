#ifndef G14CONTROLPP_ARMOURYCRATEPLAN_H
#define G14CONTROLPP_ARMOURYCRATEPLAN_H

#include <QCoreApplication>
#include <QString>

namespace ArCrate
{

struct Plan {
    Q_DECLARE_TR_FUNCTIONS(ArCrate::Plan);

public:
    enum class Code {
        PerformanceWindows = 0x00,
        TurboManual = 0x01,
        Silent = 0x02
    };

    static const QStringList Names;
    static const QList<Plan> List;

    Code id;

    explicit Plan(Code id = Code::PerformanceWindows) {
        this->id = id;
    }

    QString name() const {
        return Names[static_cast<int>(id)];
    }

    operator int() const {
        return static_cast<int>(id);
    }

    /*operator QString() const {
        return Names[static_cast<int>(id)];
    }*/
};
}


#endif //G14CONTROLPP_ARMOURYCRATEPLAN_H
