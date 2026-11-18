
#include <iostream>
#include <string>
#include "PharmacyManager.h"

using namespace std;

int main()
{
    PharmacyManager manager;
    string choice;

    manager.loadMedicines();
    manager.loadSales();

    do
    {
        cout << "\n========== PHARMACY SYSTEM ==========\n";
        cout << "1. Show all medicines\n";
        cout << "2. Add medicine batch\n";
        cout << "3. Remove medicine batch\n";
        cout << "4. Close medicine batch\n";
        cout << "5. Check stock\n";
        cout << "6. Sell medicine\n";
        cout << "7. Show sales history\n";
        cout << "8. Check expiration dates\n";
        cout << "0. Exit\n";

        cout << "\nChoose an option: ";
        getline(cin, choice);

        if (choice.length() != 1)
        {
            cout << "Invalid option! Try again.\n";
        }
        else
        {
            switch (choice[0])
            {
            case '1':
                manager.showMedicines();
                break;

            case '2':
            {
                string name, manufacturer, batch, expirationDate;
                string priceText, quantityText, type;
                double price;
                int quantity;

                cout << "\n========== ADD MEDICINE ==========\n";

                cout << "Medicine name: ";
                getline(cin, name);

                cout << "Manufacturer: ";
                getline(cin, manufacturer);

                cout << "Batch number: ";
                getline(cin, batch);

                cout << "Price: ";
                getline(cin, priceText);

                cout << "Quantity: ";
                getline(cin, quantityText);

                cout << "Expiration date (DD.MM.YYYY): ";
                getline(cin, expirationDate);

                cout << "\nMedicine type:\n";
                cout << "1. OTC (without prescription)\n";
                cout << "2. Prescription (requires prescription)\n";
                cout << "Choose type: ";
                getline(cin, type);

                try
                {
                    price = stod(priceText);
                    quantity = stoi(quantityText);
                }
                catch (...)
                {
                    cout << "Invalid price or quantity!\n";
                    break;
                }

                if (type == "1")
                {
                    manager.addMedicine(
                        new OTC_Medicine(
                            name, manufacturer, batch,
                            price, quantity, expirationDate));
                }
                else if (type == "2")
                {
                    string code, doctor;

                    cout << "Prescription code: ";
                    getline(cin, code);

                    cout << "Doctor name: ";
                    getline(cin, doctor);

                    manager.addMedicine(
                        new PrescriptionMedicine(
                            name, manufacturer, batch,
                            price, quantity, expirationDate,
                            code, doctor));
                }
                else
                {
                    cout << "Invalid medicine type!\n";
                }

                break;
            }

            case '3':
            {
                string batch;

                cout << "\nEnter batch number to remove: ";
                getline(cin, batch);

                manager.removeMedicine(batch);

                break;
            }

            case '4':
            {
                string batch;

                cout << "\nEnter batch number to close: ";
                getline(cin, batch);

                manager.closeBatch(batch);

                break;
            }

            case '5':
            {
                string batch;

                cout << "\nEnter batch number: ";
                getline(cin, batch);

                manager.checkStock(batch);

                break;
            }

            case '6':
            {
                string batch, quantityText;
                string prescriptionCode, doctor;

                cout << "\nEnter batch number: ";
                getline(cin, batch);

                cout << "Enter quantity to sell: ";
                getline(cin, quantityText);

                cout << "Prescription code (leave empty for OTC): ";
                getline(cin, prescriptionCode);

                cout << "Doctor name (leave empty for OTC): ";
                getline(cin, doctor);

                try
                {
                    int quantity = stoi(quantityText);

                    manager.sellMedicine(
                        batch, quantity, prescriptionCode, doctor);
                }
                catch (...)
                {
                    cout << "Invalid quantity!\n";
                }

                break;
            }

            case '7':
                manager.showSalesHistory();
                break;

            case '8':
                manager.checkExpirationDates();
                break;

            case '0':
                manager.saveMedicines();
                manager.saveSales();

                cout << "Exiting pharmacy system...\n";
                break;

            default:
                cout << "Invalid option! Try again.\n";
                break;
            }
        }

        if (choice != "0")
        {
            cout << "\nPress ENTER to return to the main menu...";
            getline(cin, choice);
        }

    } while (choice != "0");

    return 0;
}
