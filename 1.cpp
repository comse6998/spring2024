
#include <iostream>
#include <iomanip>

#include <ctime>   // 包含了time()
using namespace std;

void test()
{
int n = rand() % 256;
cout <<n<<endl;
}

int main()
{
srand(114514);
    for (int i = 0; i < 20; i++)
    {
        test();
    }
    return 0;
}