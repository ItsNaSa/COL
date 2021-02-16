#ifndef simplex_h_
#define simplex_h_

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#define MAX_COUNT numberOfConstraints + numberOfVariables
using namespace std;

class Simplex{
    private:
    int numberOfVariables, numberOfConstraints;
    // To store the coefficient of the constraints
    vector <vector <float>> tableau;
    
    // To store the constants of constraints
    vector <float> b;
    
    // For objective function
    vector <float> c;
    
    // For the calculation min Ratio test
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
    Simplex();
    void printTable();
    bool allNegative();
    void solve();
    void updateZ(int outgoingVarIndex);
    int calculateTheta(int incomingVariableIndex);
    void calculateCj_Zj();
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
        c.resize(numberOfVariables + numberOfConstraints, 0.0); // variables will be x1 and x2 and so on. We need to consider Slack elements as well.
        Cb.resize(numberOfConstraints, 0.0);
        z.resize(MAX_COUNT + 1, 0.0);
        cj_zj.resize(MAX_COUNT, 0.0);
        variables.resize(numberOfVariables, "x");
        slackVars.resize(numberOfConstraints, "s");
        updatedVariables.resize(numberOfConstraints, "s");
        theta.resize(numberOfConstraints, 0);

        for(int i = 0; i< numberOfVariables;i++){
            variables[i]+=to_string(i+1);
        }
        for(int i = 0; i< numberOfConstraints;i++){
            slackVars[i]+=to_string(i+1);
            updatedVariables[i]+=to_string(i+1);
        }

        cout<<"Enter the coefficients of Z (Objective Function): "<<endl;
        for(int i = 0;i<numberOfVariables; i++){
            cout<<"\tEnter coefficient of x"<<i+1<<" : ";
            cin>>c[i];
        }
        for(int i = numberOfVariables; i<MAX_COUNT; i++){
            c[i] = 0;
        }

        int count = 0, temp = 0;
        bool isGreaterConstraint = false;
        for(int i = 0; i<numberOfConstraints;i++){
            cout<<"Enter 1 if constraint "<<count+1<<" is in the form <= (i.e. Variables less than the constant) Else enter 0 ::: "<<endl;
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
            cout<<tableau[i][j]<<"\t";
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
    bool allNeg = false;
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

void Simplex::updateZ(int outgoingVarIndex){
    for(int i = 0; i < MAX_COUNT; i++){
        z[i] = Cb[outgoingVarIndex] * tableau[outgoingVarIndex][i];
    }
    z[MAX_COUNT] = Cb[outgoingVarIndex] * b[outgoingVarIndex];
    cout<<"Z updated"<<endl;
}

void Simplex::calculateCj_Zj(){
    for(int i = 0 ; i < MAX_COUNT; i++){
        cj_zj[i] = c[i] - z[i];
    }
    cout<<"Cj - Zj updated"<<endl;
}

void Simplex::solve(){
    int iteration = 1;
    // while(!allNegative()){
        cout<<"Iteration    ::\t\t"<<iteration<<endl<<endl;
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
        outgoingVarIndex = calculateTheta(incomingVarIndex);// outgoingVarIndex will point to the outgoing variable.

        // cout<<"Maximum profit is :: "<<maxProfit<<endl;
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
        Cb[outgoingVarIndex] = maxProfit;
        vector <float> temp;
        temp.resize(MAX_COUNT, 0.0);
        for(int i = 0 ; i < MAX_COUNT; i++){
            temp[i] = tableau[outgoingVarIndex][i] / tableau[outgoingVarIndex][incomingVarIndex];
        }
        for(int i = 0; i < MAX_COUNT; i++){
            tableau[outgoingVarIndex][i] = temp[i];
        }
        cout<<"outgoing variable index is "<<outgoingVarIndex<<endl;
        // Make remaining rows of the column as 0
        cout<<"Updating the tableau"<<endl;
        for(int i = 0; i < numberOfConstraints; i++){
            int coefficient = tableau[i][incomingVarIndex];
            if(i != outgoingVarIndex){
                for(int j = 0 ; j < MAX_COUNT; j++){
                    tableau[i][j] -= coefficient * tableau[i][j];
                }
            }
        }
        updateZ(outgoingVarIndex);
        calculateCj_Zj();
        printTable();
    // }
    cout<<"\n\nOptimal solution reached as all Cj - Zj are <= 0!!!"<<endl<<endl;
    map <string, float> FinalSolution;
    for(int i = 0 ; i < updatedVariables.size(); i++){
        if(updatedVariables[i].find("x") != string::npos){
            FinalSolution.insert(pair<string, float> (updatedVariables[i], b[i]));
        }
    }
    FinalSolution.insert(pair<string, int> (string("Max Value of Z"), z[MAX_COUNT]));
    cout<<"FINAL SOLUTION   :: "<<endl;
    for(auto i: FinalSolution){
        cout<<i.first<<"\t"<<i.second<<endl;
    }
}

#endif