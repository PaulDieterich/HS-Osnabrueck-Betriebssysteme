#include <iostream>
#include <thread>

void DoWork(){
    while(true)
    std::cout << "Hello arch\n";

    return NULL;
}

int main() {
    std::thread worker(DoWork);
    return 0;
}
