#include <iostream>
#include <boost/locale.hpp>

int main(){
    std::string c("ação");

    std::cout << boost::locale::normalize(c) << std::endl;
}