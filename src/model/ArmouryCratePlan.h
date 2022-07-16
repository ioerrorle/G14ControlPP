#ifndef G14CONTROLPP_ARMOURYCRATEPLAN_H
#define G14CONTROLPP_ARMOURYCRATEPLAN_H

#include <QCoreApplication>
#include <QString>

namespace ArCrate
{

struct Plan {
    //Q_DECLARE_TR_FUNCTIONS(Plan);

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

const QStringList Plan::Names = QStringList({
    ("Windows/Performance"),
    ("Turbo/Manual"),
    ("Silent")});

const QList<Plan> Plan::List = QList<Plan>({
    Plan(Code::PerformanceWindows),
    Plan(Code::TurboManual),
    Plan(Code::Silent)});
}


#endif //G14CONTROLPP_ARMOURYCRATEPLAN_H
