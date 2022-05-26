#include "FanCurve.h"
#include <string.h>

FanCurve::FanCurve(unsigned char *temp, unsigned char *speed) {
    memcpy(this->temp, temp, 8);
    memcpy(this->speed, speed, 8);
}

FanCurve::FanCurve() {

}

const unsigned char *FanCurve::getTemp() const {
    return temp;
}

const unsigned char *FanCurve::getSpeed() const {
    return speed;
}

FanCurve FanCurve::fromAcpiData(unsigned char *data) {
    return FanCurve(data, &data[8]);
}

void FanCurve::toAcpiData(unsigned char *result) {
    memcpy(result, this->temp, 8);
    memcpy(&result[8], this->speed, 8);
}
