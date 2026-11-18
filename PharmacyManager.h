#pragma once

#include "Medicine.h"
#include "OTC_Medicine.h"
#include "PrescriptionMedicine.h"
#include "Receipt.h"

#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class PharmacyManager
{
private:
    vector<Medicine *> medicines;
    vector<string> salesHistory;

    bool isExpired(string date)
    {
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        time_t now = time(0);
        tm *currentDate = localtime(&now);

        int currentDay = currentDate->tm_mday;
        int currentMonth = currentDate->tm_mon + 1;
        int currentYear = currentDate->tm_year + 1900;

        if (year < currentYear ||
            (year == currentYear && month < currentMonth) ||
            (year == currentYear && month == currentMonth && day < currentDay))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isValidDate(string date)
    {
        if (date.length() != 10 || date[2] != '.' || date[5] != '.')
            return false;
        for (int i = 0; i < date.length(); i++)
        {
            if (i != 2 && i != 5 &&
                (date[i] < '0' || date[i] > '9'))
                return false;
        }
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        if (year < 1 || month < 1 || month > 12)
            return false;

        int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};

        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            daysInMonth[1] = 29;

        if (day < 1 || day > daysInMonth[month - 1])
            return false;

        return true;
    }

public:
    PharmacyManager()
    {
    }

    PharmacyManager(vector<string> s)
    {
        salesHistory = s;
    }

    ~PharmacyManager()
    {
        for (int i = 0; i < medicines.size(); i++)
        {
            delete medicines[i];
        }
    }

    void addMedicine(Medicine *medicine)
    {
        if (medicine == nullptr)
        {
            cout << "Invalid medicine!\n";
            return;
        }

        if (medicine->getName().empty() ||
            medicine->getManufacturer().empty() ||
            medicine->getPartida().empty() ||
            medicine->getPrice() < 0 ||
            medicine->getQuantity() < 0 ||
            !isValidDate(medicine->getExpirationDate()))
        {
            cout << "Invalid medicine information!\n";
            delete medicine;
            return;
        }

        PrescriptionMedicine *prescriptionMed =
            dynamic_cast<PrescriptionMedicine *>(medicine);

        if (prescriptionMed != nullptr)
        {
            if (prescriptionMed->getPrescriptionCode().empty() ||
                prescriptionMed->getDoctor().empty())
            {
                cout << "Prescription code and doctor are required!\n";
                delete medicine;
                return;
            }
        }

        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i]->getPartida() == medicine->getPartida())
            {
                cout << "Batch number already exists!\n";
                delete medicine;
                return;
            }
        }

        medicines.push_back(medicine);
        cout << "Medicine added successfully!\n";
    }

    void removeMedicine(string partida)
    {
        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i]->getPartida() == partida)
            {
                medicines.erase(medicines.begin() + i);
                delete medicines[i];

                cout << "Medicine batch with partida " << partida << " removed successfully." << endl;
                return;
            }
        }
        cout << "Medicine batch with partida " << partida << " not found." << endl;
    }

    void closeBatch(string partida)
    {
        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i]->getPartida() == partida)
            {
                if (medicines[i]->getIsClosed())
                {
                    cout << "Medicine batch with partida " << partida << " is already closed." << endl;
                    return;
                }

                medicines[i]->setIsClosed(true);
                cout << "Medicine batch with partida " << partida << " closed successfully." << endl;
                return;
            }
        }
        cout << "Medicine batch with partida " << partida << " not found." << endl;
    }

    void showMedicines()
    {
        if (medicines.empty())
        {
            cout << "No medicines available!\n";
            return;
        }

        for (int i = 0; i < medicines.size(); i++)
        {
            cout << *medicines[i];
            cout << "--------------------------\n";
        }
    }

    void checkStock(string partida)
    {
        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i]->getPartida() == partida)
            {
                cout << "Medicine: " << medicines[i]->getName() << endl;
                cout << "Available quantity: " << medicines[i]->getQuantity() << endl;
                return;
            }
        }
        cout << "Medicine batch with partida " << partida << " not found." << endl;
    }

    void checkExpirationDates()
    {
        bool foundExpired = false;

        for (int i = 0; i < medicines.size(); i++)
        {
            if (isExpired(medicines[i]->getExpirationDate()))
            {
                cout << medicines[i]->getName() << " - EXPIRED!" << endl;
                cout << "Batch: " << medicines[i]->getPartida() << endl;
                cout << "--------------------------\n";

                foundExpired = true;
            }
        }

        if (!foundExpired)
        {
            cout << "No expired medicines found!\n";
        }
    }

    void sellMedicine(string partida, int quantity, string prescriptionCode = "", string doctor = "")
    {
        Medicine *selectedMedicine = nullptr;
        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i]->getPartida() == partida)
            {
                selectedMedicine = medicines[i];
                break;
            }
        }

        if (selectedMedicine == nullptr)
        {
            cout << "Batch not found!\n";
            return;
        }

        if (selectedMedicine->getIsClosed())
        {
            cout << "Batch is closed!\n";
            return;
        }

        if (isExpired(selectedMedicine->getExpirationDate()))
        {
            cout << "Batch is expired!\n";
            return;
        }

        if (quantity <= 0)
        {
            cout << "Invalid quantity!\n";
            return;
        }

        if (selectedMedicine->getQuantity() < quantity)
        {
            cout << "Insufficient stock!\n";
            return;
        }

        PrescriptionMedicine *prescriptionMed = dynamic_cast<PrescriptionMedicine *>(selectedMedicine);

        if (prescriptionMed != nullptr)
        {
            if (prescriptionMed->getPrescriptionCode() != prescriptionCode)
            {
                cout << "Invalid or missing prescription code!\n";
                return;
            }

            if (prescriptionMed->getDoctor() != doctor)
            {
                cout << "Invalid or missing doctor!\n";
                return;
            }
        }

        double unitPrice = selectedMedicine->calcPrice();
        double totalPrice = unitPrice * quantity;

        Receipt receipt(
            selectedMedicine->getName(),
            selectedMedicine->getPartida(),
            quantity,
            unitPrice);

        selectedMedicine->setQuantity(selectedMedicine->getQuantity() - quantity);

        salesHistory.push_back(
            selectedMedicine->getName() + " - Batch: " +
            selectedMedicine->getPartida() + " - " +
            to_string(quantity) + " pcs - " +
            to_string(totalPrice) + " lv");

        cout << "Sale completed successfully!\n";
        cout << receipt;
    }

    void showSalesHistory()
    {
        if (salesHistory.empty())
        {
            cout << "No sales recorded yet!\n";
            return;
        }

        for (int i = 0; i < salesHistory.size(); i++)
        {
            cout << salesHistory[i] << endl;
        }
    }

    void saveMedicines()
    {
        ofstream file("medicines.txt");

        if (!file.is_open())
        {
            cout << "Error opening medicines file!\n";
            return;
        }

        for (int i = 0; i < medicines.size(); i++)
        {
            if (medicines[i] == nullptr)
                continue;

            file << medicines[i]->getName() << "|";
            file << medicines[i]->getManufacturer() << "|";
            file << medicines[i]->getPartida() << "|";
            file << medicines[i]->getPrice() << "|";
            file << medicines[i]->getQuantity() << "|";
            file << medicines[i]->getExpirationDate() << "|";
            file << medicines[i]->getIsClosed() << "|";

            PrescriptionMedicine *prescriptionMed =
                dynamic_cast<PrescriptionMedicine *>(medicines[i]);

            if (prescriptionMed != nullptr)
            {
                file << "Prescription|";
                file << prescriptionMed->getPrescriptionCode() << "|";
                file << prescriptionMed->getDoctor();
            }
            else
            {
                file << "OTC||";
            }

            file << endl;
        }

        file.close();

        if (file.fail())
        {
            cout << "Error saving medicines!\n";
            return;
        }

        cout << "Medicines saved successfully!\n";
    }

    void loadMedicines()
    {
        ifstream file("medicines.txt");

        if (!file.is_open())
        {
            cout << "Medicines file not found! Starting with empty stock.\n";
            return;
        }

        string line;
        int loaded = 0;
        int skipped = 0;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            try
            {
                stringstream ss(line);

                string name, manufacturer, partida;
                string priceText, quantityText, expirationDate;
                string closedText, type, code, doctor;

                if (!getline(ss, name, '|') ||
                    !getline(ss, manufacturer, '|') ||
                    !getline(ss, partida, '|') ||
                    !getline(ss, priceText, '|') ||
                    !getline(ss, quantityText, '|') ||
                    !getline(ss, expirationDate, '|') ||
                    !getline(ss, closedText, '|') ||
                    !getline(ss, type, '|'))
                {
                    skipped++;
                    continue;
                }

                getline(ss, code, '|');
                getline(ss, doctor);

                if (name.empty() || manufacturer.empty() || partida.empty())
                {
                    skipped++;
                    continue;
                }

                // Проверка на цена
                size_t pricePosition;
                double price = stod(priceText, &pricePosition);

                if (pricePosition != priceText.length() ||
                    !isfinite(price) || price < 0)
                {
                    skipped++;
                    continue;
                }

                size_t quantityPosition;
                int quantity = stoi(quantityText, &quantityPosition);

                if (quantityPosition != quantityText.length() || quantity < 0)
                {
                    skipped++;
                    continue;
                }

                // Проверка на затворена партида
                if (closedText != "0" && closedText != "1")
                {
                    skipped++;
                    continue;
                }

                // Проверка на дата
                if (expirationDate.length() != 10 ||
                    expirationDate[2] != '.' ||
                    expirationDate[5] != '.')
                {
                    skipped++;
                    continue;
                }

                bool validDateFormat = true;

                for (int i = 0; i < expirationDate.length(); i++)
                {
                    if (i != 2 && i != 5 &&
                        (expirationDate[i] < '0' || expirationDate[i] > '9'))
                    {
                        validDateFormat = false;
                        break;
                    }
                }

                if (!validDateFormat || !isValidDate(expirationDate))
                {
                    skipped++;
                    continue;
                }

                // Проверка за дублирана партида
                bool duplicate = false;

                for (int i = 0; i < medicines.size(); i++)
                {
                    if (medicines[i]->getPartida() == partida)
                    {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate)
                {
                    skipped++;
                    continue;
                }

                Medicine *medicine = nullptr;

                if (type == "OTC")
                {
                    if (!code.empty() || !doctor.empty())
                    {
                        skipped++;
                        continue;
                    }

                    medicine = new OTC_Medicine(
                        name, manufacturer, partida,
                        price, quantity, expirationDate);
                }
                else if (type == "Prescription")
                {
                    if (code.empty() || doctor.empty())
                    {
                        skipped++;
                        continue;
                    }

                    medicine = new PrescriptionMedicine(
                        name, manufacturer, partida,
                        price, quantity, expirationDate,
                        code, doctor);
                }
                else
                {
                    skipped++;
                    continue;
                }

                medicine->setIsClosed(closedText == "1");
                addMedicine(medicine);

                loaded++;
            }
            catch (...)
            {
                skipped++;
            }
        }

        if (file.bad())
        {
            cout << "Error while reading medicines file!\n";
        }

        file.close();

        cout << "Medicines loaded: " << loaded << endl;
        cout << "Invalid records skipped: " << skipped << endl;
    }

    void saveSales()
    {
        ofstream file("sales.txt");

        if (!file.is_open())
        {
            cout << "Error opening sales file!\n";
            return;
        }

        for (int i = 0; i < salesHistory.size(); i++)
        {
            file << salesHistory[i] << endl;
        }

        file.close();

        if (file.fail())
        {
            cout << "Error saving sales history!\n";
            return;
        }

        cout << "Sales history saved successfully!\n";
    }

    void loadSales()
    {
        ifstream file("sales.txt");

        if (!file.is_open())
        {
            cout << "Sales file not found! Starting with empty history.\n";
            return;
        }

        string line;
        int loaded = 0;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            salesHistory.push_back(line);
            loaded++;
        }

        if (file.bad())
        {
            cout << "Error while reading sales file!\n";
        }

        file.close();

        cout << "Sales loaded: " << loaded << endl;
    }
};