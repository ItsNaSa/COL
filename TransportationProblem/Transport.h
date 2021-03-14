#ifndef transport_h_
#define transport_h_

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

#define printHorizontal(x)    for(int i = 0; i<x;i++){ cout<<"==========";}
using namespace std;

class transportation {
    protected:
        // no. of rows (no. of plants which produce a good)
        int noOfRows;
        // No. of columns (no. of warehouses which store/utilize a good)
        int noOfColumns;
        // To store the cose of the transportation
        vector <vector <int> > costTable;
        // To store the allocated goods
        vector <vector <int> > allocationTable;
        // To store the value of supply from each plant
        vector <int> supply;
        // Store the value of updated supply after allocation
        vector <int> remainingSupply;
        // To store the value of demand (usage) of each warehouse
        vector <int> demand;
        // Store the value of updated demand after allocation
        vector <int> remainingDemand;
        vector <int> completedSupply, completedDemand;
        bool highSupply, highDemand;
        vector<int> rowDiff, colDiff;
    public:
        // Default constructor to accept the values
        transportation();
        ~transportation();  // default destructor
        transportation(const transportation &t1);   // Copy constructor
        /**
         * @brief Checks if all supplies are exhausted
         * @return True if all supplies are exhausted, else false.
         */ 
        bool allSupplyZero();

        //Functions for least cost method
        // Get minimum cost from cost table and the row and column of that value
        int getMinimumRC(int &row, int &column);
        //Prints allocation table for least cost
        void printAllocationTable();
        
        /**
         * @function leastCost Main function to call to solve least cost method
         * @return First feasible solution cost
         */
        int leastCost();
    
        /**
         * @function getDifferences
         * @brief Finds the difference between min and second min costs per row and per column (Penalty).
         *          If the row/column are exhausted, the difference is set to INT_MIN.
         */
        void getDifferences(vector<int> &rowDiff, vector <int> &colDiff, bool &isMaxRow, int &index);
        // Prints the allocation table for Vogels, with the penalty
        void printAllocationTableVogels();
        // Main function to be called for Vogels Appropriximation method
        int vogels();
};

transportation::transportation(){
    highDemand = highSupply = false;
    // Get the metadata about rows, columns etc.
    cout<<"Enter the no. of rows (No. of plants producing the goods) :: ";
    cin>>noOfRows;
    cout<<"Enter the no. of columns (No. of warehousing using the goods) :: ";
    cin>>noOfColumns;
    costTable.resize(noOfRows, vector <int>(noOfColumns, 0));
    allocationTable.resize(noOfRows, vector <int>(noOfColumns, 0));
    supply.resize(noOfRows, 0);
    remainingSupply.resize(noOfRows, 0);
    demand.resize(noOfColumns, 0);
    remainingDemand.resize(noOfColumns, 0);
    completedDemand.resize(1,-1);
    completedSupply.resize(1, -1);

    int totalSupply=0, totalDemand=0;
    cout<<"Enter the supply vector"<<endl;
    for(int i = 0; i < noOfRows; i++){
        cout<<"Enter supply of plant "<<i+1<<" :: ";
        cin>>supply[i];
        remainingSupply[i] = supply[i];
        totalSupply += supply[i];
    }
    cout<<"Enter the demand vector"<<endl;
    for(int i = 0; i < noOfColumns; i++){
        cout<<"Enter demand of warehouse "<<i+1<<" :: ";
        cin>>demand[i];
        remainingDemand[i] = demand[i];
        totalDemand += demand[i];
    }
    
    for(int i = 0; i < noOfRows; i++){
        for (int j = 0; j < noOfColumns; j++){
            cout<<"Enter the cost of costMatrix["<<i<<"]["<<j<<"] :: ";
            cin>>costTable[i][j];
        }
        
    }
    
    // Decide if the problem is balanced or not. If not, balance.
    int slack = totalSupply - totalDemand;
    if(slack == 0){ // Balanced
        cout<<"Problem is balanced!\nNo changes to rows/columns!"<<endl;
    }
    else if(slack > 0){ // supply > demand ==> add a column
        highSupply = true;
        cout<<"Total supply > total demand => We add a column"<<endl;
        for (auto a: costTable){
            a.push_back(0);
        }
        for(auto a: allocationTable){
            a.push_back(0);
        }
        demand.push_back(slack);
        remainingDemand.push_back(0);
        
        noOfColumns += 1;   // increment no. of columns
        cout<<"Allocate slack to the extra column on FCFS basis!! (0 cost supply)"<<endl;
        for(int i = 0 ; i < noOfRows; i++){
            allocationTable[i][noOfColumns-1] = (slack <= supply[i] ? slack: supply[i]);
            remainingSupply[i] -= (slack <= supply[i] ? slack: supply[i]);
            slack -= supply[i];
            if(slack <= 0)
                break;
        }
    }
    else{   // supply < demand ==> add a row
        slack *= (-1);
        cout<<"Total supply < total demand => We add a row"<<endl;
        highDemand = true;
        costTable.push_back(vector <int>(noOfColumns, 0));
        allocationTable.push_back(vector <int>(noOfColumns, 0));
        noOfRows += 1;
        supply.push_back(slack);
        remainingSupply.push_back(0);
        
        cout<<"Allocate slack to the extra row on FCFS basis (0- cost demand)"<<endl;
        for(int i = 0; i < noOfColumns; i++){
            allocationTable[noOfRows-1][i] = (slack <= demand[i] ? slack: demand[i]);
            remainingDemand[i] -= (slack <= demand[i] ? slack: demand[i]);
            slack -= demand[i];
            if(slack <= 0){
                break;
            }
            
        }
    }
}

transportation::transportation(const transportation& t1){
    this->highDemand = t1.highDemand;
    this->highSupply = t1.highSupply;
    this->noOfColumns = t1.noOfColumns;
    this->noOfRows = t1.noOfRows;

    this->costTable.resize(t1.noOfRows, vector <int>(noOfColumns, 0));
    this->allocationTable.resize(t1.noOfRows, vector <int>(noOfColumns, 0));
    this->supply.resize(t1.noOfRows, 0);
    this->remainingSupply.resize(t1.noOfRows, 0);
    this->demand.resize(t1.noOfColumns, 0);
    this->remainingDemand.resize(t1.noOfColumns, 0);
    this->completedDemand.resize(1, -1);
    this->completedSupply.resize(1, -1);
    
    for(int i = 0; i<t1.costTable.size(); i++){
        for(int j = 0; j < t1.costTable[0].size(); j++){
            this->costTable[i][j] = t1.costTable[i][j];
        }
    }
    for(int i = 0; i<t1.allocationTable.size(); i++){
        for(int j = 0; j < t1.allocationTable[0].size(); j++){
            this->allocationTable[i][j] = t1.allocationTable[i][j];
        }
    }
    for(int i = 0; i < t1.supply.size(); i++){
        this->supply[i] = t1.supply[i];
        this->remainingSupply[i] = t1.remainingSupply[i];
    }
    for(int i = 0; i < t1.demand.size(); i++){
        this->demand[i] = t1.demand[i];
        this->remainingDemand[i] = t1.remainingDemand[i];
    }
}

transportation::~transportation(){
    noOfColumns = noOfRows = 0;
    supply.clear();
    demand.clear();
    costTable.clear();
    allocationTable.clear();
    completedDemand.clear();
    completedSupply.clear();
    remainingDemand.clear();
    remainingSupply.clear();
}

void transportation::printAllocationTable(){
    // Print warehouses names
    cout<<"|";printHorizontal(noOfColumns+2);cout<<"|"<<endl<<"|  Plant |";
    for(int i = 0; i<noOfColumns;i++){
        cout<<setw(4)<<"W"<<i+1<<setw(4)<<" |";
    }
    cout<<setw(8)<<"Supply "<<"|remain|"<<endl<<"|"; printHorizontal(noOfColumns+2);cout<<"|"<<endl;
    for(int i = 0;i<noOfRows;i++){
        cout<<"|"<<setw(4)<<"P"<<i+1<<setw(4)<<"|";
        for(int j = 0; j < noOfColumns;j++){
            string temp(to_string(costTable[i][j])+"[" + to_string(allocationTable[i][j]) + "]");
            cout<<setw(8)<<temp<<"|";
        }
        cout<<setw(5)<<supply[i]<<setw(4)<<"|"<<"  "<<remainingSupply[i]<<endl;
    }
    cout<<"|"; printHorizontal(noOfColumns+2); cout<<"|";
    cout<<endl<<"|"<<setw(8)<<"DEMAND"; cout<<"|";
    for(auto a: demand){
        cout<<setw(10)<<a<<"|";
    }
    cout<<endl<<"|"; printHorizontal(noOfColumns+2); cout<<"|"<<endl<<"|"<<setw(8)<<"Rem.Dem"<<"|";
    for(auto a: remainingDemand){
        cout<<setw(10)<<a<<"|";
    }
    cout<<endl;
}

bool transportation::allSupplyZero(){
    for(auto a: remainingSupply){
        if(a > 0){
            return false;
        }
    }
    return true;
}

int transportation::getMinimumRC(int &row, int &column){
    int min = INT_MAX;
    cout<<"Starting loop"<<endl;
    for(int i = 0; i < costTable.size(); i++){
        
        // If the row index is found in completed part, skip the row
        vector<int>::iterator rowitr = find(completedSupply.begin(), completedSupply.end(), i);
        if(rowitr != completedSupply.end()){
            cout<<"Row "<<i<<" is exhausted, thus skipping!!"<<endl;
            continue;
        }
        for(int j = 0; j < costTable[0].size(); j++){
            vector <int>::iterator colitr = find(completedDemand.begin(), completedDemand.end(), j);
            // If the column index is found in completed columns, skip the col
            if(colitr != completedDemand.end()){
                cout<<"Column "<<j<<" is exhausted, thus skipping!!"<<endl;
                continue;
            }
            if(costTable[i][j] < min){
                row = i;
                column = j;
                min = costTable[i][j];
                cout<<"Min is "<<costTable[i][j]<<" i="<<i<<" j="<<j<<endl;
            }
            else
                continue;
        }
    }
    return min;
}

int transportation::leastCost(){
    int countOfAllocation = 0;
    int minCost = INT_MAX, minRow = 0, minCol = 0;
    int firstFeasibleScore = 0;
    while(!this->allSupplyZero()){
        int row, column;
        int min_cost = this->getMinimumRC(row, column);
        cout<<"\nLeast cost is "<<min_cost<<" at row="<<row<<" and column="<<column<<endl<<endl;
        if(this->remainingSupply[row] <= this->remainingDemand[column]){
            this->allocationTable[row][column] = this->remainingSupply[row];
            this->remainingDemand[column] -= this->remainingSupply[row];
            this->remainingSupply[row] -= this->remainingSupply[row];
            countOfAllocation++;
        }
        else{
            this->allocationTable[row][column] = this->remainingDemand[column];
            this->remainingSupply[row] -= this->remainingDemand[column];
            this->remainingDemand[column] -= this->remainingDemand[column];
            countOfAllocation++;
        }
        if(this->remainingSupply[row] == 0){
            cout<<"Plant "<<row+1<<" is exhausted!"<<endl;
            this->completedSupply.push_back(row);
        }
        if(this->remainingDemand[column] == 0){
            cout<<"Warehouse "<<column+1<<" is full!"<<endl;
            this->completedDemand.push_back(column);
        }
        firstFeasibleScore += allocationTable[row][column] * costTable[row][column];
        this->printAllocationTable();
    }
    // Check if the problem is degenerate or non-degenerate
    if(countOfAllocation != (noOfColumns+noOfRows+1)){
        cout<<"\n\n\t!!!!\tThis is a degenerate problem!\t!!!!"<<endl;
    }
    else{
        cout<<"\n\n\t!!!!\tThis is a non-degenerate problem!\t!!!!"<<endl;
    }
    return firstFeasibleScore;
}

void transportation::printAllocationTableVogels(){
    // Print warehouses names
    cout<<"|";printHorizontal(noOfColumns+2);cout<<"|"<<endl<<"|  Plant |";
    for(int i = 0; i<noOfColumns;i++){
        cout<<setw(4)<<"W"<<i+1<<setw(4)<<" |";
    }
    cout<<setw(8)<<"Supply "<<"|Remain|"<<" Diff |"<<endl<<"|"; printHorizontal(noOfColumns+2);cout<<"|"<<endl;
    for(int i = 0;i<noOfRows;i++){
        cout<<"|"<<setw(4)<<"P"<<i+1<<setw(4)<<"|";
        for(int j = 0; j < noOfColumns;j++){
            string temp(to_string(costTable[i][j])+"[" + to_string(allocationTable[i][j]) + "]");
            cout<<setw(8)<<temp<<"|";
        }
        cout<<setw(5)<<supply[i]<<setw(4)<<"|"<<"  "<<remainingSupply[i]<<" | "<<rowDiff[i]<<endl;
    }
    cout<<"|"; printHorizontal(noOfColumns+2); cout<<"|";
    cout<<endl<<"|"<<setw(8)<<"DEMAND"; cout<<"|";
    for(auto a: demand){
        cout<<setw(10)<<a<<"|";
    }
    cout<<endl<<"|"; printHorizontal(noOfColumns+2); cout<<"|"<<endl<<"|"<<setw(8)<<"Rem.Dem"<<"|";
    for(auto a: remainingDemand){
        cout<<setw(10)<<a<<"|";
    }
    cout<<endl<<"|"; printHorizontal(noOfColumns+2); cout<<"|"; 
    cout<<endl<<"|"<<setw(8)<<"Differ."<<"|";
    for(auto a: colDiff){
        cout<<setw(10)<<a<<"|";
    }
    cout<<endl;
}

void transportation::getDifferences(vector<int> &rowDiff, vector <int> &colDiff, bool &isMaxRow, int &index){
    // 1. Traverse row-wise and get minimum value in all rows
    for(int i = 0; i< costTable.size(); i++){
        int min=INT_MAX, secondMin=INT_MAX, indexOfMin=-1;
        // Get minimum in a row
        // Check if the row is exhausted and to be skipped
        vector<int>:: iterator rowitr = find(completedSupply.begin(), completedSupply.end(), i);
        if(rowitr != completedSupply.end()){
            rowDiff[i] = INT_MIN;   // If needed to be skipped, make penalty negative infinity
            continue;
        }
        for(int j = 0; j < costTable[0].size(); j++){
            // Check if the column is exhausted, and needs to be skipped
            vector<int>::iterator colitr = find(completedDemand.begin(), completedDemand.end(), j);
            if(colitr != completedDemand.end()){
                continue;
            }
            // If not exhausted, consider it for min
            if(costTable[i][j] < min){
                min = costTable[i][j];
                indexOfMin = j;
            }
        }
        // Get second min
        for(int j = 0; j < costTable[0].size(); j++){
            // Again check for exhausted column
            vector<int>::iterator colitr = find(completedDemand.begin(), completedDemand.end(), j);
            if(colitr != completedDemand.end()){
                continue;
            }

            if(j == indexOfMin)
                continue;
            if(costTable[i][j] < secondMin){
                secondMin = costTable[i][j];
            }
        }
        if(secondMin == INT_MAX){
            secondMin = 0;
            min *= -1;
        }
        cout<<"ROW "<<i<<" :: min value = "<<min<<" Second min is "<<secondMin<<endl;
         
        rowDiff[i] = (secondMin - min);
    } // end 1

    // 2. Traverse col-wise and minimum value in all columns
    for(int i = 0; i< costTable[0].size(); i++){
        int min=INT_MAX, secondMin=INT_MAX, indexOfMin=-1;
        // Check if the column is already exhausted, if yes, make the penalty as -ve infinity
        vector<int>::iterator colitr = find(completedDemand.begin(), completedDemand.end(), i);
        if(colitr != completedDemand.end()){
            colDiff[i] = INT_MIN;
            continue;
        }
        // Get minimum in a column
        for(int j = 0; j < costTable.size(); j++){
            // Check if the row is already exhausted, if yes, skip
            vector<int>:: iterator rowitr = find(completedSupply.begin(), completedSupply.end(), j);
            if(rowitr != completedSupply.end()){
                continue;
            }
            if(costTable[j][i] < min){
                min = costTable[j][i];
                indexOfMin = j;
            }
        }
        // Get second min
        for(int j = 0; j < costTable.size(); j++){
            // Again check if the row is exhausted
            vector<int>:: iterator rowitr = find(completedSupply.begin(), completedSupply.end(), j);
            if(rowitr != completedSupply.end()){
                continue;
            }
            if(j == indexOfMin)
                continue;
            if(costTable[j][i] < secondMin){
                secondMin = costTable[j][i];
            }
        }
        if(secondMin == INT_MAX){
            secondMin = 0;
            min *= -1;
        }
        
        colDiff[i] = (secondMin - min);
    } // end 2

    // get max from row and column penalties
    int rowMax = INT_MIN, colMax = INT_MIN;
    int rowIndex, colIndex;
    for(int i = 0; i < rowDiff.size(); i++){
        if(rowDiff[i] > rowMax){
            rowMax = rowDiff[i];
            rowIndex = i;
        }
    }
    for(int i = 0; i < colDiff.size(); i++){
        if(colDiff[i] > colMax){
            colMax = colDiff[i];
            colIndex = i;
        }
    }
    if(rowMax >= colMax){
        isMaxRow = true;
        index = rowIndex;
    }
    else{
        isMaxRow = false;
        index = colIndex;
    }   
}

int transportation::vogels(){
    int countOfAllocation = 0;
    int feasibleScore = 0;
    while(!allSupplyZero()){
        rowDiff.resize(noOfRows, 0);
        colDiff.resize(noOfColumns, 0);
        bool maxIsRow = true;
        int index = -1;
        getDifferences(rowDiff, colDiff, maxIsRow, index);
        // As per the maximum value is in row or column, solve accordingly
        if(maxIsRow){
            // Index will be index of row with max penalty
            int minCostIndex = -1, minCost = INT_MAX; // minCostIndex is the column with minimum cost
            cout<<"Maximum penalty is in row "<<index<<" and value="<<rowDiff[index]<<endl;
            
            // Select the minimum cost for the said row
            for(int i = 0; i < costTable[0].size(); i++){
                // Check if the column is already exhausted
                vector<int>::iterator colitr = find(completedDemand.begin(), completedDemand.end(), i);
                if(colitr != completedDemand.end()){
                    continue;
                }
                // if not, go ahead
                if(costTable[index][i] < minCost){
                    minCost = costTable[index][i];
                    minCostIndex = i;
                }
            }
            // Check if supply is larger or demand
            if(remainingSupply[index] <= remainingDemand[minCostIndex]){
                allocationTable[index][minCostIndex] = remainingSupply[index];
                remainingDemand[minCostIndex] -= remainingSupply[index];
                remainingSupply[index] -= remainingSupply[index];
                countOfAllocation++;
            }
            else{
                allocationTable[index][minCostIndex] = remainingDemand[minCostIndex];
                remainingSupply[index] -= remainingDemand[minCostIndex];
                remainingDemand[minCostIndex] -= remainingDemand[minCostIndex];
                countOfAllocation++;
            }
            if(this->remainingSupply[index] == 0){
                cout<<"Plant "<<index+1<<" is exhausted!"<<endl;
                this->completedSupply.push_back(index);
            }
            if(this->remainingDemand[minCostIndex] == 0){
                cout<<"Warehouse "<<minCostIndex+1<<" is full!"<<endl;
                this->completedDemand.push_back(minCostIndex);
            }
            feasibleScore += costTable[index][minCostIndex] * allocationTable[index][minCostIndex];
        }
        else{ // Maximum penalty is columnwise
            // minCostIndex = index of row with minimum value, minCost is the value of cost
            int minCostIndex = -1, minCost = INT_MAX;

            cout<<"Maximum penalty is in column and value="<<colDiff[index]<<endl;
            // Select the minimum cost for the said column
            for(int i = 0; i < costTable.size(); i++){
               // Check if the row is taken
               vector<int>::iterator rowitr = find(completedSupply.begin(), completedSupply.end(), i);
                if(rowitr != completedSupply.end()){
                    continue;
                }

                if(costTable[i][index] < minCost){
                    minCost = costTable[i][index];
                    minCostIndex = i;
                }
            }
            // Check if supply is larger or demand
            if(remainingSupply[minCostIndex] <= remainingDemand[index]){
                allocationTable[minCostIndex][index] = remainingSupply[minCostIndex];
                remainingDemand[index] -= remainingSupply[minCostIndex];
                remainingSupply[minCostIndex] -= remainingSupply[minCostIndex];
                countOfAllocation++;
            }
            else{
                allocationTable[minCostIndex][index] = remainingDemand[index];
                remainingSupply[minCostIndex] -= remainingDemand[index];
                remainingDemand[index] -= remainingDemand[index];
                countOfAllocation++;
            }
            if(this->remainingSupply[minCostIndex] == 0){
                cout<<"Plant "<<minCostIndex+1<<" is exhausted!"<<endl;
                this->completedSupply.push_back(minCostIndex);
            }
            if(this->remainingDemand[index] == 0){
                cout<<"Warehouse "<<index+1<<" is full!"<<endl;
                this->completedDemand.push_back(index);
            }
            feasibleScore += costTable[minCostIndex][index] * allocationTable[minCostIndex][index];
        }
        this->printAllocationTableVogels();
    }
    // Check if the problem is degenerate or non-degenerate
    cout<<"Final solution for Vogels :: "<<endl;
    printAllocationTableVogels();
    if(countOfAllocation != (noOfColumns+noOfRows+1)){
        cout<<"\n\n\t!!!!\tThis is a degenerate problem!\t!!!!"<<endl;
    }
    else{
        cout<<"\n\n\t!!!!\tThis is a non-degenerate problem!\t!!!!"<<endl;
    }
    return feasibleScore;
}

#endif