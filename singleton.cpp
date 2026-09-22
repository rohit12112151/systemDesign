// #include<iostream>
// #include<mutex>
// using namespace std;

// class singleton{
//     private:
//        static singleton*instance;
//        static mutex mtx;
//         singleton(){
//             cout<<"Singleton instance created!"<<endl;
//         }

//     public:

//         static singleton* getInstance(){
//             // lock_guard<mutex>lock(mtx);
//             if(instance==nullptr){
//                 lock_guard<mutex>lock(mtx);
//                 instance= new singleton();
//             }
//             return instance;
//         }


// };


// //instanciate static members
// singleton* singleton::instance=nullptr;
// mutex singleton::mtx;

// int main(){
//     singleton*s1 = singleton::getInstance();
//     singleton*s2 = singleton::getInstance();

//     cout<<(s1==s2)<<endl;
// }






//eager initialization
#include<iostream>
#include<mutex>
using namespace std;

class singleton{
    private:
       static singleton*instance;
       
        singleton(){
            cout<<"Singleton instance created!"<<endl;
        }

    public:

        static singleton* getInstance(){
            return instance;
        }


};


//instanciate static members
singleton* singleton::instance=new singleton();
// not a good practice to create instance at compile time because it will be created 
// even if it is not used in the program.Assume it as a expensive operation.

// mutex singleton::mtx;

int main(){
    singleton*s1 = singleton::getInstance();
    singleton*s2 = singleton::getInstance();

    cout<<(s1==s2)<<endl;
}