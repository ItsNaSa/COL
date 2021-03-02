#ifndef dual_h_
#define dual_h_

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#define MAX_COUNT numberOfConstraints + numberOfVariables
using namespace std;

class Duality {
    private:
    int numberOfVariables, numberOfConstraints;
    int dualVariables, dualConstraints;
    // To store the coefficients of variables of constraints
    vector <vector <float>> tableau, dualTableau;
    
    // To store the constants of constraints (R.H.S.)
    vector <float> b;
    
    // To store the constants of Dual
    vector <float> dualB;
    
    // For objective function (Maximization function)
    vector <float> c;
    // For objective function of Dual
    vector <float> dualC;
    
    vector <string> variables;
    int isMinimization;
    float maximum;
    int countOfEqualTypeConstraints;
    vector <int> indexOfEqualConstraints;
    vector <int> indexOfOppositeSign;
    
    public:
    Duality();
    ~Duality();
    void printLPPTable();
    void convertToDual();
    void printDualTable();
};

Duality::Duality(){
        countOfEqualTypeConstraints = 0;
        cout<<endl<<"Assumption: All the variables are restricted!!!"<<endl<<endl;
        cout<<"Enter 1 if the problem is minimization else enter 0 :: ";
        cin>>isMinimization;
    
        maximum = INT_MIN;
        cout<<"Enter number of variables in Z: ";
        cin>>numberOfVariables;
        cout<<"Enter total number of constraint inequations in Z: ";
        cin>>numberOfConstraints;
        // cout<<"How many of them are equal type constraints :: ";
        // cin>>countOfEqualTypeConstraints;

        tableau.resize(numberOfConstraints, vector <float>(numberOfVariables, 0.0) );
        b.resize(numberOfConstraints, 0.0);
        c.resize(MAX_COUNT, 0.0); // variables will be x1 and x2 and so on. We need to consider Slack elements as well.

        // Set the variables for dynamic use
        variables.resize(numberOfVariables, "x");
        for(int i = 0; i< numberOfVariables;i++){
            variables[i]+=to_string(i+1);
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
        
        // Accept the constraints in the <= form for Maximization and >= for minimization
        int count = 0, temp = 0;
        bool isGreaterConstraint = false;
        for(int i = 0; i<numberOfConstraints;i++){
            cout<<"Enter 1 if constraint "<<count+1<<" is in the form >= (i.e. Variables less than the constant) \nFor = type variable, enter 2 \nElse for <= type enter 0 :: ";
            cin>>temp;
            int j;
            if(isMinimization){
                if(temp == 1){
                    // i.e. if for minimization, constraint is greater than type, well and good
                    for(j = 0; j < numberOfVariables; j++){
                        cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                        cin>>tableau[i][j];
                    }
                }
                else if(temp == 0){
                        indexOfOppositeSign.push_back(i);
                        for(j = 0; j < numberOfVariables; j++){
                            cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                            cin>>tableau[i][j];
                            tableau[i][j] = (-1) * tableau[i][j];
                    }
                }
                else if(temp == 2){
                    indexOfEqualConstraints.push_back(i);
                    for(j = 0; j < numberOfVariables; j++){
                        cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                        cin>>tableau[i][j];
                    }
                }  
            }
            else{
                if(temp == 0){
                    // i.e. if for minimization, constraint is greater than type, well and good
                    for(j = 0; j < numberOfVariables; j++){
                        cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                        cin>>tableau[i][j];
                    }
                }
                else if (temp == 1){
                    indexOfOppositeSign.push_back(i);
                    for(j = 0; j < numberOfVariables; j++){
                        cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                        cin>>tableau[i][j];
                        tableau[i][j] = (-1)*tableau[i][j];
                    }
                }
                else if (temp == 2){
                    indexOfEqualConstraints.push_back(i);
                    for(j = 0; j < numberOfVariables; j++){
                        cout<<"\tEnter the coefficient of X"<<j+1<<" : ";
                        cin>>tableau[i][j];
                    }

                }
            }

            cout<<"Enter the constant of the constraint "<<count+1<<" : ";
            if(isMinimization){
                if(temp == 1 || temp == 2 ){
                    cin>>b[i];
                }
                else if(temp == 0){
                    cin>>b[i];
                    b[i] = (-1)*b[i];
                }
            }
            else{
                if(temp == 0 || temp == 2){
                    cin>>b[i];
                }
                else if(temp == 1){
                    cin>>b[i];
                    b[i] = (-1)*b[i];
                }
            }
            count++;
        }
        // Calculate Cj - Zj
        // for(int i = 0; i < MAX_COUNT ; i++){
        //     cj_zj[i] = c[i] - z[i];
        // }
        cout<<"The primal LPP is now as follows :: "<<endl;
        printLPPTable();
        cout<<"Converting the LPP problem to Dual !"<<endl;
        convertToDual();
        cout<<"Dual problem is as follows ::"<<endl;
        printDualTable();
}

void Duality::printLPPTable(){

    int count = 1;
    if(isMinimization){
        cout<<"Minimize"<<endl;
    }
    else{
        cout<<"Maximize"<<endl;
    }
    cout<<"Z = ";
    for(int i = 0; i<numberOfVariables;i++){
        cout<<"("<<c[i]<<")"<<"X"<<i+1<<" ";
        if(i < (numberOfVariables-1)){
            cout<<" + ";
        }
    }
    cout<<endl;
    // Print tableau
    for(int i = 0; i < numberOfConstraints; i++){
        for(int j = 0; j<numberOfVariables;j++){
            cout<<" ("<<tableau[i][j]<<")*"<<"X"<<j+1<<" ";
            if(j < (numberOfVariables-1)){
                cout<<" + ";
            }
        }
        int ser = i;
        std::vector<int>::iterator cons = find(this->indexOfEqualConstraints.begin(), this->indexOfEqualConstraints.end(), ser); 
        if(cons != indexOfEqualConstraints.end()){
            cout<<" = ";
        }   
        else{
            if(isMinimization){
                cout<<" >= ";
            }
            else{
                cout<<" <= ";
            }
        }
        cout<<b[i]<<endl;
        count++;
    }
    cout<<endl;
}

void Duality::convertToDual(){
    dualVariables = numberOfConstraints;
    dualConstraints = numberOfVariables;

    dualTableau.resize(dualConstraints, vector <float> (dualVariables, 0.0));

    for(auto i: b){
        dualC.push_back(i);
    }

    for(int i = 0; i < tableau.size(); i++){
        for(int j =  0; j < tableau[i].size(); j++){
            dualTableau[j][i] = tableau[i][j];
        }
    }
    
    for(auto i: c){
        dualB.push_back(i);
    }
    
}

void Duality::printDualTable(){
    cout<<endl<<"Dual problem for the said question is as follows :"<<endl;
    if(isMinimization){
        cout<<"Maximize"<<endl;
    }
    else{
        cout<<"Minimize"<<endl;
    }
    cout<<"W = ";
    for(int i = 0; i < dualC.size(); i++){
        cout<<"("<<dualC[i]<<")*Y"<<i+1<<" ";
        if(i < (dualC.size() - 1)){
            cout<<"+ ";
        }
    }
    cout<<endl<<"SUbject to"<<endl;
    for(int i = 0; i < dualTableau.size(); i++){
        cout<<"\t"<<endl;
        for(int j = 0; j < dualTableau[i].size(); j++){
            cout<<"("<<dualTableau[i][j]<<")*Y"<<i+1<<" ";
            if(j < (dualTableau[i].size()-1)){
                cout<<"+";
            }
        }
        std::vector <int>::iterator it = find(indexOfEqualConstraints.begin(), indexOfEqualConstraints.end(), i);
        if(it != indexOfEqualConstraints.end()){
            cout<<" = ";
        }
        else if(isMinimization){
            cout<<" <= ";
        }
        else{
            cout<<" >= ";
        }
        cout<<dualB[i]<<endl;
    }
    cout<<"WITH"<<endl;
    cout<<"\t";
    for(auto a: indexOfEqualConstraints){
        cout<<"Y"<<a+1<<", ";
    }
    cout<<" is/are unrestricted"<<endl<<"AND\n\t";
    for(int i = 0; i < dualVariables; i++){
        std::vector <int>::iterator it = find(indexOfEqualConstraints.begin(), indexOfEqualConstraints.end(), i);
        if(it == indexOfEqualConstraints.end()){
            cout<<"Y"<<i+1<<", ";
        }
    }
    cout<<" >= 0"<<endl;
}
Duality::~Duality(){}
#endif