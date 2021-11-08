#ifndef G14CONTROLPP_FANCURVE_H
#define G14CONTROLPP_FANCURVE_H

class FanCurve {

public:
    FanCurve(unsigned char *temp, unsigned char *speed);
    FanCurve();

    const unsigned char *getTemp() const;

    const unsigned char *getSpeed() const;

    static FanCurve fromAcpiData(unsigned char *data);

    void toAcpiData(unsigned char *result);

private:
    unsigned char temp[8];
    unsigned char speed[8];
};


#endif //G14CONTROLPP_FANCURVE_H
