#include "json.hpp"

#include<iostream>
#include<vector>
#include<map>
using json = nlohmann::json;
using namespace std;
void func2(){
    json js;
    js["id"] = {1, 2, 3, 4};
    cout << js << endl;
}

int main(){
    func2();
    return 0;
}