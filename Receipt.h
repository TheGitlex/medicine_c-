
#pragma once
#include <string>
#include <iostream>
using namespace std;

class Receipt
{
private:
    string medicineName;
    string batchNumber;
    int quantity;
    double unitPrice;
    double totalPrice;

public:
    Receipt()
    {
        medicineName = "";
        batchNumber = "";
        quantity = 0;
        unitPrice = 0;
        totalPrice = 0;
    }

    Receipt(string name, string batch, int q, double price)
    {
        medicineName = name;
        batchNumber = batch;
        quantity = q;
        unitPrice = price;
        totalPrice = q * price;
    }

    string getMedicineName() { return medicineName; }
    string getBatchNumber() { return batchNumber; }
    int getQuantity() { return quantity; }
    double getUnitPrice() { return unitPrice; }
    double getTotalPrice() { return totalPrice; }

    void setMedicineName(string name)
    {
        if (!name.empty())
            medicineName = name;
    }

    void setBatchNumber(string batch)
    {
        if (!batch.empty())
            batchNumber = batch;
    }

    void setQuantity(int q)
    {
        if (q > 0)
        {
            quantity = q;
            totalPrice = quantity * unitPrice;
        }
    }

    void setUnitPrice(double price)
    {
        if (price >= 0)
        {
            unitPrice = price;
            totalPrice = quantity * unitPrice;
        }
    }
};


inline ostream& operator<<(ostream& os, Receipt& receipt)
{
    os << "\n========== PHARMACY RECEIPT ==========\n";
    os << "Medicine: " << receipt.getMedicineName() << endl;
    os << "Batch: " << receipt.getBatchNumber() << endl;
    os << "Quantity: " << receipt.getQuantity() << endl;
    os << "Unit price: " << receipt.getUnitPrice() << " lv" << endl;
    os << "--------------------------------------\n";
    os << "TOTAL: " << receipt.getTotalPrice() << " lv" << endl;
    os << "======================================\n";

    return os;
}