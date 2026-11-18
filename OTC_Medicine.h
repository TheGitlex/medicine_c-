#pragma once

#include "Medicine.h"

class OTC_Medicine : public Medicine
{
public:
    OTC_Medicine(string n, string m, string p, double pr, int q, string exp)
        : Medicine(n, m, p, pr, q, exp) {}

    OTC_Medicine() : Medicine() {}

    double calcPrice() override
    {
        return getPrice() * 0.90;
    }
};