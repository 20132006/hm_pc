#include <iostream>
using namespace std;
int main()
{
    int sum=0,a,i,n;
    cin>>n;
    for (i=0;i<n;i++)
    {
      cin>>a;
      sum+=a;
    }
    cout<<sum<<endl;
    return 0;
}
