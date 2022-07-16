#ifndef G14CONTROLPP_CPUPROFILE_H
#define G14CONTROLPP_CPUPROFILE_H

#include <QCoreApplication>
#include <QStringList>
#include <QMetaType>

namespace Ryzen
{
struct Mode
{
    //Q_DECLARE_TR_FUNCTIONS("Ryzen");
public:
    static const QList<Mode> Modes;

    enum class Id
    {
        Performance = 0,
        PowerSaving = 1,
        Default = 2
    };

    static const QStringList Names;

    Id mode;

    explicit Mode(Id modeId = Id::Default) {
        this->mode = modeId;
    }

    QString name() const {
        return Names.at(static_cast<int>(mode));
    }

    operator Id() {
        return mode;
    }

    operator int() const {
        return static_cast<int>(mode);
    }

//    explicit operator QString() const {
//        return Names.at(static_cast<int>(mode));
//    }

    Mode& operator =(int mode) {
        this->mode = static_cast<Id>(mode);
        return *this;
    }
};

const QList<Mode> Mode::Modes = QList<Mode>({
                                                Mode(Id::Performance),
                                                Mode(Id::PowerSaving),
                                                Mode(Id::Default)});

const QStringList Mode::Names = QStringList({
                                                ("Performance"),
                                                ("Power saving"),
                                                ("Default")});

struct Profile
{
    //Q_DECLARE_TR_FUNCTIONS(Profile);
public:
    enum class Type {
        Default = 0,
        Current = 1,
        UserDefined = 2
    };

    static const QString DefaultName;
    static const QString CurrentName;

    static const Profile Default;
    static const Profile Current;

    float stapmLimit;
    float stapmTime;
    float slowLimit;
    float slowTime;
    float fastLimit;

private:
    QString m_name;
    Mode m_mode;
    Type m_type = Type::Default;

public:
    explicit Profile(Type type = Type::Default) : m_type(type){ };

    Profile(const QString &name, const QStringList &stringList) {
        if (stringList.size() < 6 || stringList[0] == "default") {
            this->m_type = Type::Default;
            return;
        }

        this->m_name = name;
        stapmLimit = stringList[0].toFloat();
        stapmTime = stringList[1].toFloat();
        slowLimit = stringList[2].toFloat();
        slowTime = stringList[3].toFloat();
        fastLimit = stringList[4].toFloat();
        m_mode = stringList[5].toInt();
        m_type = Type::UserDefined;
    }

    Profile(const QString &name,
            float stapmLimit,
            float stapmTime,
            float slowLimit,
            float slowTime,
            float fastLimit,
            int mode)
        : stapmLimit(stapmLimit)
        , stapmTime(stapmTime)
        , slowLimit(slowLimit)
        , slowTime(slowTime)
        , fastLimit(fastLimit)
        , m_name(name)
        , m_mode(Mode::Modes[mode])
        , m_type(Type::UserDefined) {
    }

    QString name() const {
        if (m_type == Ryzen::Profile::Type::Default)
            return DefaultName;
        if (m_type == Ryzen::Profile::Type::Current)
            return CurrentName;
        return m_name;
    }

    Mode mode() const {
        return m_mode;
    }

    Type type() const {
        return m_type;
    }

    QStringList toQStringList() const {
        QStringList result;
        result << QString::number(stapmLimit)
               << QString::number(stapmTime)
               << QString::number(slowLimit)
               << QString::number(slowTime)
               << QString::number(fastLimit)
               << QString::number(m_mode);
        return result;
    }

    Profile& operator =(const QStringList &stringList) {
        if (stringList.size() < 6 || stringList[0] == "default") {
            this->m_type = Type::Default;
            return *this;
        }

        stapmLimit = stringList[0].toFloat();
        stapmTime = stringList[1].toFloat();
        slowLimit = stringList[2].toFloat();
        slowTime = stringList[3].toFloat();
        fastLimit = stringList[4].toFloat();
        m_mode = stringList[5].toInt();
        m_type = Type::UserDefined;
        return *this;
    }
};

const QString Profile::DefaultName = ("<Default>");
const QString Profile::CurrentName = ("<Current>");

const Profile Profile::Default = Profile();
const Profile Profile::Current = Profile(Type::Current);

}

Q_DECLARE_METATYPE(Ryzen::Profile)

#endif //G14CONTROLPP_CPUPROFILE_H
