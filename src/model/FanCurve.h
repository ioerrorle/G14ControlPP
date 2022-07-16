#ifndef G14CONTROLPP_FANCURVE_H
#define G14CONTROLPP_FANCURVE_H

#include <QMetaType>

namespace Fans
{

struct Curve {

public:
    uchar temp[8];
    uchar speed[8];

    Curve(unsigned char *temp, unsigned char *speed) {
        memcpy(this->temp, temp, 8);
        memcpy(this->speed, speed, 8);
    };

    Curve() {
        memset(temp, 0, 8);
        memset(speed, 0, 8);
    }

    void toAcpiData(uchar *result) const {
        memcpy(result, this->temp, 8);
        memcpy(&result[8], this->speed, 8);
    };

    Curve& operator =(const uchar *data) {
        memcpy(temp, data, 8);
        memcpy(speed, &data[8], 8);
        return *this;
    }

    bool operator==(Curve const& rhs) const {
        int result = memcmp(this->temp, rhs.temp, 8);
        if (result != 0) {
            return false;
        }
        result = memcmp(this->speed, rhs.speed, 8);
        if (result != 0) {
            return false;
        }
        return true;
    }
};

}

Q_DECLARE_METATYPE(Fans::Curve)


#endif //G14CONTROLPP_FANCURVE_H
