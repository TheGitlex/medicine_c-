#pragma once

#include "Medicine.h"

class PrescriptionMedicine : public Medicine
{
private:
    string prescriptionCode;
    string doctor;

public:
    PrescriptionMedicine(string n, string m, string p, double pr, int q,
                         string exp, string code, string doc)
        : Medicine(n, m, p, pr, q, exp)
    {
        prescriptionCode = code;
        doctor = doc;
    }

    PrescriptionMedicine() : Medicine()
    {
        prescriptionCode = "";
        doctor = "";
    }

    double calcPrice() override
    {
        return getPrice();
    }

    string getPrescriptionCode()
    {
        return prescriptionCode;
    }

    string getDoctor()
    {
        return doctor;
    }

    void setPrescriptionCode(string code)
    {
        if (!code.empty())
            prescriptionCode = code;
    }

    void setDoctor(string doc)
    {
        if (!doc.empty())
            doctor = doc;
    }
};