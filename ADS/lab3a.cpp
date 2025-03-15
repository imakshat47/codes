#include<iostream>
using namespace std;

int N = 10;

struct studentNode{
    int roll;
    string name,stream;
    studentNode* next;

    studentNode(int roll, string name, string stream){
        this->roll = roll;
        this->name = name;
        this->stream = stream;
        this->next=NULL;
    }
};

int main(){
    int roll[N], record_count=0, _roll;
    string name[N], stream[N], _name,_stream;
    char next;
    studentNode* st;
    cout<<"Enter details below: \n 1. Student Roll No \n 2. Student Name \n 3. Student Stream \nEnter records and each entry detials seperated by space:\n";
    
    do{
        cin>>_roll>>_name>>_stream;
        // cin>>roll[record_count]>>name[record_count]>>stream[record_count++];
        if(record_count<=N){
            roll[record_count]=_roll;
            name[record_count]=_name;
            stream[record_count]=_stream;
        }else cout<<"Array was of fixed size and we've exhausted the space.\nLinked list dynamic so still accepting entries...";
        if(record_count)
            st->next = new studentNode(_roll,_name,_stream);
        else 
            st=new studentNode(_roll,_name,_stream);
        ++record_count;
        cout<<"Want to enter more records: (y/n) ";
        cin>>next;
    }while (next!='n');

    printf("Entered Student Detials: \n");
    printf("S.No \tRoll No\t Name\t Stream\n");
    for(int i=0;i<record_count;++i){
        cout<<i+1<<"\t"<<roll[i]<<"\t"<<name[i]<<"\t"<<stream[i]<<endl;
    }

    // print all student name in O(n)
    cout<<"All Student name list: (in O(n))"<<endl;
    for(int i=0;i<record_count;++i){
        cout<<i+1<<"\t"<<name[i]<<endl;
    }

    // print any student name in O(1)
    cout<<"Name of Student record by S. No.: (in O(1) array access) ";
    do{    
        int idx;cin>>idx;    
        cout<<idx<<"\t"<<name[--idx]<<endl;
        cout<<"Want to view more records: (y/n) ";
        cin>>next;
    }while (next!='n');

    // print any student name in O(n)
    cout<<"Name of Student record by S. No.: (in O(n) Linked List access) ";
    do{    
        int idx;cin>>idx;    
        studentNode* temp = st;        
        while(temp->next!=NULL){
            if(temp->roll==idx)
                break;
            temp=temp->next;
        }
        cout<<idx<<"\t"<<temp->name<<endl;
        cout<<"Want to view more records: (y/n) ";
        cin>>next;
    }while (next!='n');    
   
    return 0;
}