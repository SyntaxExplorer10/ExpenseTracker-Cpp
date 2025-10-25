#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <fstream>
#include <algorithm>
using namespace std;

class Expense {
private:
    string date, note, category;
    float amount;

public:
    Expense(float amt, string cat, string dat, string nt) {
        amount = amt;
        category = cat;
        date = dat;
        note = nt;
    }

    // Getter functions
    float getAmount() const { return amount; }
    string getCategory() const { return category; }
    string getDate() const { return date; }
    string getNote() const { return note; }

    // Display one expense
    void display(int index) const {
        cout << "Expense #" << index << endl;
        cout << "Amount   : " << amount << endl;
        cout << "Category : " << category << endl;
        cout << "Date     : " << date << endl;
        cout << "Note     : " << note << endl;
        cout << "--------------------------" << endl;
    }
};

// Save expenses to a file
void saveToFile(const vector<Expense>& expenses) {
    ofstream outFile("expenses.txt");
    if (!outFile) {
        cout << "Error saving file!" << endl;
        return;
    }

    for (auto &ex : expenses) {
        outFile << ex.getAmount() << ","
                << ex.getCategory() << ","
                << ex.getDate() << ","
                << ex.getNote() << "\n";
    }
    outFile.close();
}

// Load expenses from file
void loadFromFile(vector<Expense>& expenses) {
    ifstream inFile("expenses.txt");
    if (!inFile) return;

    string line;
    while (getline(inFile, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos)
            continue;

        float amt = stof(line.substr(0, pos1));
        string cat = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string dat = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string nt = line.substr(pos3 + 1);

        expenses.emplace_back(amt, cat, dat, nt);
    }

    inFile.close();
}

int main() {
    vector<Expense> expenses;
    int choice;

    loadFromFile(expenses); // Load saved data

    do {
        cout << "\n===== Expense Tracker =====" << endl;
        cout << "1. Add Expense" << endl;
        cout << "2. Display All Expenses" << endl;
        cout << "3. Show Summary" << endl;
        cout << "4. Delete Expense" << endl;
        cout << "5. Sort Expenses" << endl;
        cout << "6. Search" <<endl;
        cout << "7. Clear All" <<endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                float amt;
                string date, note, category;
                int Cho;

                cout << "\nEnter Amount: ";
                cin >> amt;

                cout << "Enter Date (DD/MM/YYYY): ";
                cin >> date;

                cout << "Enter Note: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, note);

                cout << "\nChoose Category:\n";
                cout << "1. Food\n2. Travel\n3. Shopping\n4. Medical\n5. Others\n";
                cout << "Enter Choice: ";
                cin >> Cho;

                switch (Cho) {
                    case 1: category = "Food"; break;
                    case 2: category = "Travel"; break;
                    case 3: category = "Shopping"; break;
                    case 4: category = "Medical"; break;
                    default: category = "Others"; break;
                }

                Expense e(amt, category, date, note);
                expenses.push_back(e);
                cout << "\n✅ Expense Added Successfully!\n";
                saveToFile(expenses);
                break;
            }

            case 2:
                if (expenses.empty()) {
                    cout << "\nNo expenses to display!\n";
                } else {
                    cout << "\n--- All Expenses ---\n";
                    int index = 1;
                    for(auto &ex : expenses)
                        ex.display(index++);
                }
                break;

            case 3: {
                if (expenses.empty()) {
                    cout << "\nNo expenses recorded yet!\n";
                    break;
                }

                float total = 0;
                map<string, float> categoryTotal;

                for (auto &ex : expenses) {
                    total += ex.getAmount();
                    categoryTotal[ex.getCategory()] += ex.getAmount();
                }

                cout << "\n--- Expense Summary ---\n";
                cout << "Total Expense: " << total << endl;
                cout << "\nCategory-wise Breakdown:\n";
                for (auto &pair : categoryTotal)
                    cout << pair.first << ": " << pair.second << endl;
                break;
            }

            case 4: { // Delete Expense
                if (expenses.empty()) {
                    cout << "\nNo expenses to delete!\n";
                    break;
                }

                int delIndex;
                cout << "\nEnter the expense number to delete: ";
                cin >> delIndex;

                if (delIndex > 0 && delIndex <= (int)expenses.size()) {
                    expenses.erase(expenses.begin() + (delIndex - 1));
                    cout << "Expense deleted successfully!\n";
                    saveToFile(expenses);
                } else {
                    cout << "Invalid number!\n";
                }
                break;
            }

            case 5: { // Sort
                if (expenses.empty()) {
                    cout << "\nNo expenses to sort!\n";
                    break;
                }

                int sortChoice;
                cout << "\nSort by:\n1. Amount\n2. Date\nEnter choice: ";
                cin >> sortChoice;

                if (sortChoice == 1) {
                    sort(expenses.begin(), expenses.end(), [](const Expense &a, const Expense &b) {
                        return a.getAmount() < b.getAmount();
                    });
                    cout << "Expenses sorted by amount!\n";
                } else if (sortChoice == 2) {
                    sort(expenses.begin(), expenses.end(), [](const Expense &a, const Expense &b) {
                        return a.getDate() < b.getDate();
                    });
                    cout << "Expenses sorted by date!\n";
                } else {
                    cout << "Invalid choice!\n";
                }

                saveToFile(expenses);
                break;
            }

            case 6:
                { // Search / Filter
                if (expenses.empty()) { cout << "\nNo expenses to search!\n"; break; }
                string key;
                cout << "\nEnter keyword (category/date/note): ";
                cin.ignore();
                getline(cin, key);

                bool found = false;
                int i = 1;
                for (auto &ex : expenses) {
                    if (ex.getCategory().find(key) != string::npos ||
                        ex.getDate().find(key) != string::npos ||
                        ex.getNote().find(key) != string::npos) {
                        ex.display(i);
                        found = true;
                    }
                    i++;
                }
                if (!found) cout << "\nNo results found for \"" << key << "\".\n";
                break;
            }
            case 7:{ // Clear all
                char confirm;
                cout << "\nThis will erase all data. Confirm (y/n)? ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    expenses.clear();
                    saveToFile(expenses);
                    cout << "\n All data cleared!\n";
                } else cout << "\nCancelled.\n";
                break;
            }
            case 8:
                cout << "\nSaving and exiting... Goodbye!\n";
                saveToFile(expenses);
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while(choice != 8);

    return 0;
}
