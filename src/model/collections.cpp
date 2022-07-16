#include "ArmouryCratePlan.h"
#include "CpuProfile.h"
#include "FansProfile.h"
#include "HwProfile.h"
#include "PowerProfile.h"

namespace ArCrate
{
const QStringList Plan::Names =
        QStringList({
                        tr("Windows/Performance")
                        , tr("Turbo/Manual")
                        , tr("Silent")
                    });

const QList<Plan> Plan::List =
        QList<Plan>({
                        Plan(Code::PerformanceWindows)
                        , Plan(Code::TurboManual)
                        , Plan(Code::Silent)
                    });
}

namespace Ryzen
{
const QList<Mode> Mode::Modes =
        QList<Mode>({
                        Mode(Id::Performance)
                        , Mode(Id::PowerSaving)
                        , Mode(Id::Default)});

const QStringList Mode::Names =
        QStringList({
                        tr("Performance")
                        , tr("Power saving")
                        , tr("Default")});

const QString Profile::DefaultName = tr("<Default>");
const QString Profile::CurrentName = tr("<Current>");

const Profile Profile::Default = Profile();
const Profile Profile::Current = Profile(Type::Current);
}

namespace Fans {
const Profile Profile::Default = Profile();

const QString Profile::DefaultFansProfileName = tr("<Default>");
const QString Profile::CurrentFansProfileName = tr("<Current>");
}

const HwProfile HwProfile::Default = HwProfile(ArCrate::Plan(), Fans::Profile::Default, Ryzen::Profile::Default);

const PowerProfile PowerProfile::Default = PowerProfile();
const QString PowerProfile::DefaultName = tr("<Default>");
