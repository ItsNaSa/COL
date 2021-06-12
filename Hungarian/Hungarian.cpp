#include<iostream>
#include<stdio.h>
#include<iomanip>
using namespace std;
#define MAX 50
enum boolean{FALSE,TRUE};

class Hungarian{
    int data[MAX][MAX];
    int allocs[MAX][MAX];
    int n_rows, n_cols;
    int bal_stat;

    public:
        Hungarian(){
            int i,j;
            for(i=0;i<MAX;i++){
                for(j=0;j<MAX;j++){
                    data[i][j]=0;
                    allocs[i][j]=0;
                }
            }
            n_rows=n_cols=bal_stat=0;
        }
        void setRow(int no){n_rows=no;}
        void setColumn(int no){n_cols=no;}
        void getData();
        void makeallocs();
        void display();
        void rowMinima(int [][MAX],int,int);
        void columnMinima(int [][MAX],int,int);
        boolean checkValue(int,int,int [][MAX]);
};
void Hungarian::getData(){
    int i,j;
    cout<<"enter cost matrix :\n";
    for(i=0;i<n_rows;i++){
        cout<<"enter "<<i+1<<" row :";
        for(j=0;j<n_cols;j++)
            cin>>data[i][j];
    }
}
void copyArray(int startRow,int startCol,int endRow,int endCol,int temp[][MAX],int start1row,int start1col,int ans[][MAX]){
    int i,j,k,l;
    for(i=startRow,k=start1row;i<endRow;i++,k++)
        for(j=startCol,l=start1col;j<endCol;j++,l++)
            ans[k][l]=temp[i][j];
}
int getMinVal(int temp[],int no){
    int min=temp[0];
    for(int i=0;i<no;i++)
        if(min>temp[i])
            min=temp[i];
    return min;
}
int getPosition(int temp[],int no,int value){
    for(int i=0;i<no;i++)
        if(temp[i]==value)
            return i;
    return -1;
}
int countVal(int temp[],int no,int value){
    int i,sum=0;
    for(i=0;i<no;i++)
        if(temp[i]==value)
            sum++;
    return sum;
}
void Hungarian::rowMinima(int temp[][MAX],int row,int col){
    int i,j,min;
    for(i=0;i<row;i++){
        min=9999;
        for(j=0;j<col;j++)
            if(min>temp[i][j])
                min=temp[i][j];
        for(j=0;j<col;j++)
            temp[i][j]-=min;
    }
}
void Hungarian::columnMinima(int temp[][MAX],int row,int col){
    int i,j,min;
    for(i=0;i<row;i++){
        min=9999;
        for(j=0;j<col;j++)
            if(min>temp[j][i])
                min=temp[j][i];
        for(j=0;j<col;j++)
            temp[j][i]-=min;
    }
}
boolean Hungarian::checkValue(int row,int col,int temp[][MAX]){
    int i,j;
    for(i=0;i<row;i++)
        for(j=0;j<col;j++)
            if(temp[i][j]==0)
                return TRUE;
    return FALSE;

}
void Hungarian::makeallocs(){
    int temp_data[MAX][MAX]={0};
    int i,j;
    if(n_rows>n_cols){
        for(i=0;i<n_rows;i++)
            data[i][n_cols]=0;
        n_cols++;
        bal_stat=1;
    }else if(n_rows<n_cols){
        for(i=0;i<n_cols;i++)
            data[n_rows][i]=0;
        n_rows++;
        bal_stat=2;
    }
    copyArray(0,0,n_rows,n_cols,data,0,0,temp_data);
    rowMinima(temp_data,n_rows,n_cols);
    columnMinima(temp_data,n_rows,n_cols);
    int min,pos,count;
    int tempCol[MAX]={0};
    while(checkValue(n_rows,n_cols,temp_data)){
        for(i=0;i<n_rows;i++){
            count=countVal(temp_data[i],n_cols,0);
            if(count==1){
                pos=getPosition(temp_data[i],n_cols,0);
                allocs[i][pos]=data[i][pos];
                for(j=0;j<n_rows;j++)
                    if(temp_data[j][pos]==0)
                        temp_data[j][pos]=9999;
            }
        }
        for(i=0;i<n_rows;i++){
            for(j=0;j<n_cols;j++)
                tempCol[j]=temp_data[j][i];
            count=countVal(tempCol,n_rows,0);
            if(count==1){
                pos=getPosition(tempCol,n_rows,0);
                allocs[i][pos]=data[i][pos];
                for(j=0;j<n_cols;j++)
                    if(temp_data[pos][j]==0)
                        temp_data[pos][j]=9999;
            }

        }
    }
}
void Hungarian::display(){
    int i,j;
    cout<<"\nGiven Cost matrix :\n";
    for(i=0;i<n_rows;i++)
        cout<<"\t"<<char(65+i);
    cout<<endl;
    for(i=0;i<n_rows;i++){
        cout<<i+1;
        for(j=0;j<n_cols;j++)
            cout<<"\t"<<data[i][j];
        cout<<endl;
    }
    if(bal_stat!=0){
        cout<<"\n\nhere the give cost matrix is not squar Matrix\n";
        cout<<"so this is a unbalance problem and as a solution";
        cout<<"\n we have add an extra "<<((bal_stat==1)?"column":"row")<<" with 0 value in each\n";
    }
    cout<<"\n\nOpportunity Matrix :\n";
    rowMinima(data,n_rows,n_cols);
    columnMinima(data,n_rows,n_cols);
    for(i=0;i<n_rows;i++){
        for(j=0;j<n_cols;j++)
            cout<<"\t"<<data[i][j];
        cout<<endl;
    }
    int sum=0;
    cout<<"\n\nJobs\t:\tMachine\t:\tCost\n";
    for(i=0;i<n_rows;i++)
        for(j=0;j<n_cols;j++)
            if(allocs[i][j]!=0){
                cout<<i+1<<"\t:\t"<<char(65+j)<<"\t:\t"<<allocs[i][j];
                sum+=allocs[i][j];
                cout<<endl;
            }
    cout<<"\nTotal Assignment Cost = "<<sum;
}
int main(){
    Hungarian hm;
    int row,col;

    cout<<"Enter no of row :";
    cin>>row;
    cout<<"Enter no of column :";
    cin>>col;

    hm.setRow(row);
    hm.setColumn(col);
    hm.getData();
    hm.makeallocs();
    hm.display();
}
