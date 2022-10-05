#include <iostream>
#include "Doublelist.hpp"
#include <list>
#include <numeric>

int main()
{
    Doublelist<std::string> slist;
    //std::list<std::string> slist;
    slist.push_front("Tachibana Arisu");
    slist.push_back("Satou Shin");
    slist.push_front("Katagiri Sanae");
    slist.push_back("Sasaki Chie");
    slist.push_front("Tsukioka Kogane");
    slist.push_back("Komiya Kaho");

    std::cout << slist.size() << std::endl;

    auto name = *(slist.begin());
   
   // Tsukioka Kogane; Katagiri Sanae; Tachibana Arisu; Satou Shin; Sasaki Chie; Komiya Kaho;
   for (const auto& s : slist) {
       std::cout << s << std::endl;
   }

    std::cout << "\n\n";

    std::cout << slist.front() << std::endl;
    std::cout << slist.back() << std::endl;

    Doublelist<int> ilist;
    ilist.push_back(17);
    ilist.push_back(23);
    ilist.push_back(78);
    ilist.push_back(4);
    ilist.push_back(34);
    ilist.push_back(19);
    ilist.push_back(3);
    ilist.push_back(5);
    ilist.push_back(23);
    ilist.push_back(37);
    ilist.push_back(43);

    std::cout << std::accumulate(ilist.begin(), ilist.end(), 0)
              << std::endl;
}
