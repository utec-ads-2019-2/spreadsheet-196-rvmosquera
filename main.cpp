#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

const int CANT_ROWS = 1000;
const int CANT_COL = 18279;

using namespace std;

struct node {
    int col = 0;
    int row = 0;
    node *next = nullptr;

    void killSelf() {
        if (next) {
            next->killSelf();
        }

        delete this;
    }
};


bool isCalculated[CANT_ROWS][CANT_COL];
int spreadsheet[CANT_ROWS][CANT_COL];
node formula[CANT_ROWS][CANT_COL];

int calculateCellContent(int row, int col) {
    int sum = 0;

    if (isCalculated[row][col])
        return spreadsheet[row][col];

    auto nodeAux = formula[row][col].next;

    if (nodeAux) {

        while (nodeAux->next != nullptr) {
            auto row2 = nodeAux->row;
            auto col2 = nodeAux->col;

            if (isCalculated[row2][col2]) {
                sum = sum + spreadsheet[row2][col2];
            } else {
                spreadsheet[row2][col2] = calculateCellContent(row2, col2);
                sum = sum + spreadsheet[row2][col2];
            }
            nodeAux = nodeAux->next;
        }

        if (nodeAux) {
            auto row2 = nodeAux->row;
            auto col2 = nodeAux->col;

            if (isCalculated[row2][col2]) {
                sum = sum + spreadsheet[row2][col2];
            } else {
                spreadsheet[row2][col2] = calculateCellContent(row2, col2);
                sum = sum + spreadsheet[row2][col2];
            }
        }

    } else {
        auto row2 = formula[row][col].row;
        auto col2 = formula[row][col].col;

        if (isCalculated[row2][col2]) {
            sum = sum + spreadsheet[row2][col2];
        } else {
            sum = sum + calculateCellContent(row2, col2);
        }
    }

    isCalculated[row][col] = true;

    return sum;
}

void print2(int array[][CANT_COL], int rows, int cols) {
    for (int j = 0; j < rows; j++) {
        for (int k = 0; k < cols; k++) {
            if (!isCalculated[j][k]) {
                array[j][k] = calculateCellContent(j, k);
                cout << array[j][k] << " ";
            } else
                cout << array[j][k] << " ";

        }
        cout << endl;
    }
}

pair<int, int> get_row_and_col(string cell) {
    auto len = cell.length();

    string numbers;
    int col = 0;
    int row = 0;
    int count = 0;
    int ascii = 0;

    for (int i = len - 1; i >= 0; i--) {
        if (isdigit(cell[i])) {
            numbers = cell[i] + numbers;
        } else {
            ascii = (int) cell[i] - 65;
            col = col + ascii + (ascii + 1) * 26 * count;
            count++;
        }
    }
    row = stoi(numbers);
    row--;

    return make_pair(row, col);
}

void getFormula(int j, int k, string cell) {
    string delimiter = "+";
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string operandStr;
    char first = 'Y';

    if (cell[0] == '=')
        cell = cell.substr(1, cell.length());

    formula[j][k].next = nullptr;

    while ((pos_end = cell.find(delimiter, pos_start)) != string::npos) {
        operandStr = cell.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;

        auto cell_operand = get_row_and_col(operandStr);

        node *newNode = new node();
        auto row = cell_operand.first;
        auto col = cell_operand.second;
        newNode->row = row;
        newNode->col = col;

        if (first == 'Y') {
            formula[j][k].next = newNode;
            first = 'N';
        } else {
            auto aux = formula[j][k].next;
            while (aux->next != nullptr) {
                aux = aux->next;
            }
            aux->next = newNode;
        }
    }

    auto cell_operand = get_row_and_col(cell.substr(pos_start));
    auto newNode = new node();
    auto row = cell_operand.first;
    auto col = cell_operand.second;

    newNode->row = row;
    newNode->col = col;

    auto aux = formula[j][k].next;

    if (aux) {
        while (aux->next != nullptr) {
            aux = aux->next;
        }
        aux->next = newNode;
    } else {
        formula[j][k].next = newNode;
    }
}

int main(int argc, char *argv[]) {
    int number;
    int cols, rows;
    string cell;
    string line;

    cin >> number;
    for (int i = 0; i < number; i++) {
        cin >> cols >> rows; // >> line;

        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                cin >> cell;

                formula[j][k].row = j;
                formula[j][k].col = k;

                auto firstChar = cell[0];
                if (isdigit(firstChar)) {
                    spreadsheet[j][k] = atoi(cell.c_str());

                    isCalculated[j][k] = true;
                } else {
                    if (firstChar == '-') //Negative
                    {
                        spreadsheet[j][k] = atoi(cell.c_str());
                        isCalculated[j][k] = true;
                    } else {
                        isCalculated[j][k] = false;
                        getFormula(j, k, cell);
                    }
                }
            }
        }
        print2(spreadsheet, rows, cols);
    }

    return EXIT_SUCCESS;
}
