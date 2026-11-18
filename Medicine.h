#pragma once
#include <iostream>
#include <string>
using namespace std;

class Medicine
{
private:
    string name;
    string manufacturer;
    string partida;
    double price;
    int quantity;
    string expirationdate;
    bool isClosed;

public:
    Medicine(string n, string m, string p, double pr, int q, string exp)
    {
        name = n;
        manufacturer = m;
        partida = p;
        price = pr;
        quantity = q;
        expirationdate = exp;
        isClosed = false;
    }

    Medicine()
    {
        name = "";
        manufacturer = "";
        partida = "";
        price = 0.0;
        quantity = 0;
        expirationdate = "";
        isClosed = false;
    }

    virtual double calcPrice()
    {
        return price;
    }

    virtual ~Medicine() {}

    // GETTERS
    string getName()
    {
        return name;
    }

    string getManufacturer()
    {
        return manufacturer;
    }

    string getPartida()
    {
        return partida;
    }

    double getPrice()
    {
        return price;
    }

    int getQuantity()
    {
        return quantity;
    }

    string getExpirationDate()
    {
        return expirationdate;
    }

    // SETTERS
    void setName(string n)
    {
        if (!n.empty())
            name = n;
    }

    void setManufacturer(string m)
    {
        if (!m.empty())
            manufacturer = m;
    }

    void setPartida(string p)
    {
        if (!p.empty())
            partida = p;
    }

    void setPrice(double pr)
    {
        if (pr >= 0)
            price = pr;
    }

    void setQuantity(int q)
    {
        if (q >= 0)
            quantity = q;
    }

    void setExpirationDate(string exp)
    {
        if (!exp.empty())
            expirationdate = exp;
    }

    bool getIsClosed()
    {
        return isClosed;
    }

    void setIsClosed(bool closed)
    {
        isClosed = closed;
    }
};


    inline ostream& operator<<(ostream& os, Medicine& medicine)
{
    os << "Name: " << medicine.getName() << endl;
    os << "Manufacturer: " << medicine.getManufacturer() << endl;
    os << "Batch number: " << medicine.getPartida() << endl;
    os << "Price: " << medicine.getPrice() << endl;
    os << "Quantity: " << medicine.getQuantity() << endl;
    os << "Expiration date: " << medicine.getExpirationDate() << endl;
    os << "Closed: " << (medicine.getIsClosed() ? "Yes" : "No") << endl;

    return os;
}