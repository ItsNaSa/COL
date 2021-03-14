#include <iostream>
#include "Transport.h"
using namespace std;

int main(){
    transportation t1;
    transportation t2(t1);
    char choice;
    cout<<"The entered problem is as follows"<<endl;
    t1.printAllocationTable();
    cout<<"Solving the problem with least cost method ::"<<endl;
    int leastCostSolution = t1.leastCost();
    cout<<"Enter Y to solve the problem with Vogel's approximation method :: ";
    cin>>choice;
    if(choice == 'Y' || choice == 'y'){
        cout<<"Problem is as follows"<<endl;
        t2.printAllocationTable();
        cout<<"Solving the problem with vogel's cost method ::"<<endl;
        int vogelsSolution = t2.vogels();
        cout<<"Vogel's method feasible solution is :: "<<vogelsSolution<<endl;
    }
    cout<<"Least cost method feasible solution is :: "<<leastCostSolution<<endl;
    return 0;
}