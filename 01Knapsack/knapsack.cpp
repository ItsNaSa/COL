#include<bits/stdc++.h> 
using namespace std;
 
//Using Dynamic Programming
int max(int a, int b){ return (a > b)? a : b; }
 
int knapsack(int W, int wt[], int value[], int n){
   int i, w;
   int K[n+1][W+1];
 
   for (i = 0; i <= n; i++){
       for (w = 0; w <= W; w++){
           if (i==0 || w==0)
               K[i][w] = 0;
           else if (wt[i-1] <= w)
                 K[i][w] = max(value[i-1] + K[i-1][w-wt[i-1]],  K[i-1][w]);
           else
                 K[i][w] = K[i-1][w];
       }
   }
 
   return K[n][W];
}
 
int main(){
    int i, n, value[20], wt[20], W;
    
    cout<<"\nEnter number of items ::";
    cin>>n;
    
    cout<<"\nEnter weight and value of items::\n";
    for(i = 0;i < n; ++i)
    {
    	cin>>wt[i]>>value[i];
    }
 
    cout<<"\nEnter maximum weight of the knapsack ::";
    cin>>W;
    
    cout<<"\nAnswer :: "<<knapsack(W, wt, value, n)<<"\n";
    return 0;
}
