#ifndef G14CONTROLPP_FANCURVE_H
#define G14CONTROLPP_FANCURVE_H

#include <QMetaType>

class FanCurve {

public:
    FanCurve(unsigned char *temp, unsigned char *speed);
    FanCurve();

    const unsigned char *getTemp() const;

    const unsigned char *getSpeed() const;

    static FanCurve fromAcpiData(unsigned char *data);

    void toAcpiData(unsigned char *result);

    bool operator==(FanCurve const& rhs) const;

private:
    unsigned char temp[8];
    unsigned char speed[8];
};

Q_DECLARE_METATYPE(FanCurve)


#endif //G14CONTROLPP_FANCURVE_H
