#ifndef simplex_h_
#define simplex_h_

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <iomanip>
#define MAX_COUNT numberOfConstraints + numberOfVariables
using namespace std;

class Simplex{
    private:
    int numberOfVariables, numberOfConstraints;
    // To store the coefficients of variables of constraints
    vector <vector <float>> tableau;
    
    // To store the constants of constraints (R.H.S.)
    vector <float> b;
    
    // For objective function (Maximization function)
    vector <float> c;
    
    // For the calculation for min Ratio test
    vector <float> Cb;
    
    //Value of Z
    vector <float> z;

    // Theta (min ratio test)
    vector <float> theta;

    // For printing the table
    vector <string> variables;
    vector <string> slackVars;
    vector <string> updatedVariables;
    // For relative profits
    vector <float> cj_zj;
    float maximum;
    bool isUnbounded;

    public:
    Simplex();  // Constructor, accepts input of the LPP.
    ~Simplex(); // Desturctor
    void printTable();  // Prints the table
    bool allNegative(); // Checks if all Cj - Zj is non-positive.
    void updateZ();     // Updates Z after iteration
    int calculateTheta(int incomingVariableIndex);  // Calculate Theta
    void calculateCj_Zj();  // Calculate and update value of Cj - Zj after each iteration
    void updatebWithTheta(int outgoingVarIndex);    // Updates the value of b with pivotal theta value
    void solve();       // Driver function
};

Simplex::Simplex()  {
        isUnbounded = false;
        maximum = INT_MIN;
        cout<<"Enter number of variables in Z: ";
        cin>>numberOfVariables;
        cout<<"Enter number of constraint inequations in Z: ";
        cin>>numberOfConstraints;

        tableau.resize(numberOfConstraints, vector <float>(MAX_COUNT, 0.0) );
        b.resize(numberOfConstraints, 0.0);
        c.resize(MAX_COUNT, 0.0); // variables will be x1 and x2 and so on. We need to consider Slack elements as well.
        Cb.resize(numberOfConstraints, 0.0);
        z.resize(MAX_COUNT + 1, 0.0);   // Last element of Z i.e. Z[MAX_COUNT] will contain the profit at that iteration.
        cj_zj.resize(MAX_COUNT, 0.0);
        theta.resize(numberOfConstraints, 0);

        // Set the variables for dynamic use
        variables.resize(numberOfVariables, "x");
        slackVars.resize(numberOfConstraints, "s");
        updatedVariables.resize(numberOfConstraints, "s");
        for(int i = 0; i< numberOfVariables;i++){
            variables[i]+=to_string(i+1);
        }
        for(int i = 0; i< numberOfConstraints;i++){
            slackVars[i]+=to_string(i+1);
            updatedVariables[i]+=to_string(i+1);
        }

        // Accept coefficients of Z (Which is the maximization objective function)
        cout<<"Enter the coefficients of Z (Objective Function): "<<endl;
        for(int i = 0;i<numberOfVariables; i++){
            cout<<"\tEnter coefficient of x"<<i+1<<" : ";
            cin>>c[i];
        }
        for(int i = numberOfVariables; i<MAX_COUNT; i++){
            c[i] = 0;
        }
        
        // Accept the constraints in the <= form for Maximization
        int count = 0, temp = 0;
        bool isGreaterConstraint = false;
        for(int i = 0; i<numberOfConstraints;i++){
            cout<<"Enter 1 if constraint "<<count+1<<" is in the form <= (i.e. Variables less than the constant) Else enter 0 :: ";
            cin>>temp;
            int j;
            for(j = 0; j < numberOfVariables; j++){
                cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                cin>>tableau[i][j];
                if(temp == 0){
                    tableau[i][j] *= (-1);
                }
            }
            for(j = numberOfVariables; j < MAX_COUNT; j++){
                if(j == numberOfVariables + count){
                    tableau[i][j] = 1;
                }
                else{
                    tableau[i][j] = 0;
                }
            }
            cout<<"Enter the constant of the constraint "<<count+1<<" : ";
            cin>>b[i];
            if(temp == 0){
                b[i] *= (-1);
            }
            count++;
        }
        // Calculate Cj - Zj
        for(int i = 0; i < MAX_COUNT ; i++){
            cj_zj[i] = c[i] - z[i];
        }
        cout<<"The LPP is now as follows :: "<<endl;
        printTable();
}

void Simplex::printTable(){
    cout<<"\tcj";
    for(auto i : c){
        cout<<"\t"<<i;
    }
    cout<<endl;
    int count = 1;
    // Table headers
    cout<<"Cb\t"<<"Basis\t";
    for(auto i: variables){
        cout<<i<<"\t";
    }
    for(auto i: slackVars){
        cout<<i<<"\t";
    }
    cout<<"b\t"<<"theta"<<endl;

    // Print tableau
    for(int i = 0; i < numberOfConstraints; i++){
        cout<<Cb[i]<<"\t";
        cout<<updatedVariables[i]<<"\t";
        for(int j = 0; j < numberOfVariables + numberOfConstraints; j++){
            cout<<setprecision(3)<<tableau[i][j]<<"\t";
        }
        cout<<b[i]<<"\t"<<theta[i]<<endl;
        count++;
    }
    cout<<"\tz\t";
    for(auto i: z){
        cout<<i<<"\t";
    }
    cout<<endl;
    cout<<"\tcj-zj\t";
    for(auto i: cj_zj){
        cout<<i<<"\t";
    }
    cout<<endl;
}

bool Simplex::allNegative(){
    bool allNeg = true;
    for(int i = 0; i < MAX_COUNT; i++){
        if(cj_zj[i] > 0){
            allNeg = false;
            return allNeg;
        }
        else{
            allNeg = true;
        }
    }
    return allNeg;
}

int Simplex::calculateTheta(int incomingVariableIndex){
    int minTheta = INT_MAX;
    int indexOfMintheta = -1;
    // Traverse as per row
    for(int i = 0; i < numberOfConstraints; i++){
        theta[i] = b[i]/tableau[i][incomingVariableIndex];
    }
    for(int i = 0; i < numberOfConstraints; i++){
        if(theta[i] < minTheta){
            indexOfMintheta = i;
            minTheta = theta[i];
        }
    }
    return indexOfMintheta;
}

void Simplex::updateZ(){
    for(int i = 0; i < MAX_COUNT; i++){ //traverse by column
        float temp = 0.0;
        for(int j = 0; j < numberOfConstraints; j++){   // traverse by row
            temp += Cb[j] * tableau[j][i];
        }
        z[i] = temp;
    }
    // Max profit at that stage
    float maxProfit = 0.0;
    for(int i = 0; i < numberOfConstraints; i++){
        maxProfit += Cb[i] * b[i];
    }
    z[MAX_COUNT] = maxProfit;
}

void Simplex::calculateCj_Zj(){
    for(int i = 0 ; i < MAX_COUNT; i++){
        cj_zj[i] = c[i] - z[i];
    }
    cout<<"Cj - Zj updated"<<endl;
}

void Simplex::updatebWithTheta(int outgoingVarIndex){
    b[outgoingVarIndex] = theta[outgoingVarIndex];
}

void Simplex::solve(){
    int iteration = 1;
    while(!allNegative()){
        cout<<"\n\nIteration    ::\t\t"<<iteration<<endl<<endl;
        float maxProfit = -999999.99;
        int incomingVarIndex = -1; // Entering variable, points to the tableau column
        int outgoingVarIndex = -1;  // points to the tableau row
        // Calculate Cj -Zj (relative profit)
        for(int i = 0; i<MAX_COUNT; i++){
            cj_zj[i] = c[i] - z[i];
            if(cj_zj[i] > maxProfit){
                maxProfit = cj_zj[i];
                incomingVarIndex = i;
            }              
        }
        // Now, incomingVarIndex will point to the incoming variable.
        outgoingVarIndex = calculateTheta(incomingVarIndex);    // outgoingVarIndex will point to the outgoing variable.

        cout<<variables[incomingVarIndex]<<" is the incoming variable!!"<<endl;
        cout<<updatedVariables[outgoingVarIndex]<<" is the outgoing variable!!"<<endl;

        // Update the variables as per the incoming and outgoing
        cout<<"Updating the variables"<<endl;
        if(incomingVarIndex < numberOfVariables){
            updatedVariables[outgoingVarIndex] = variables[incomingVarIndex];
        }
        else{
            updatedVariables[outgoingVarIndex] = slackVars[incomingVarIndex];
        }
        // To update the Cb
        cout<<"Updating Cb"<<endl;
        Cb[outgoingVarIndex] = c[incomingVarIndex];
        vector <float> temp;
        temp.resize(MAX_COUNT, 0.0);
        for(int i = 0 ; i < MAX_COUNT; i++){
            temp[i] = tableau[outgoingVarIndex][i] / tableau[outgoingVarIndex][incomingVarIndex];
        }
        for(int i = 0; i < MAX_COUNT; i++){
            tableau[outgoingVarIndex][i] = temp[i];
        }
        temp.clear();
        cout<<"outgoing variable index is "<<outgoingVarIndex<<endl;
        updatebWithTheta(outgoingVarIndex);
        // Make remaining rows of the column as 0
        cout<<"Updating the tableau"<<endl;
        for(int i = 0; i < numberOfConstraints; i++){
            float coefficient = (float) tableau[i][incomingVarIndex];
            if(i != outgoingVarIndex){
                cout<<"Performing "<<updatedVariables[i]<<" - "<<coefficient<<"*"<<updatedVariables[outgoingVarIndex]<<endl;
                for(int j = 0 ; j < MAX_COUNT; j++){
                    tableau[i][j] -= coefficient * tableau[outgoingVarIndex][j];
                }
                b[i] -= coefficient * b[outgoingVarIndex];
            }
        }
        updateZ();
        calculateCj_Zj();
        cout<<"\nTable at the end of Iteration "<<iteration<<" is  :: \n"<<endl;
        printTable();
        iteration++;
    }
    cout<<"\n\nOptimal solution reached as all Cj - Zj are <= 0!!!"<<endl<<endl;
    map <string, float> FinalSolution;
    for(int i = 0 ; i < updatedVariables.size(); i++){
        if(updatedVariables[i].find("x") != string::npos){
            FinalSolution.insert(pair<string, float> (updatedVariables[i], b[i]));
        }
    }
    FinalSolution.insert(pair<string, int> (string("Max Profit"), z[MAX_COUNT]));
    cout<<"FINAL SOLUTION :: "<<endl;
    cout<<"|------------------|"<<endl;
    for(auto i: FinalSolution){
        cout<<"|"<<setw(12)<<left<<i.first<<"|"<<setw(5)<<i.second<<"|"<<endl;
        cout<<"|------------------|"<<endl;
    }
    cout<<"|------------------|"<<endl;
}

#endif