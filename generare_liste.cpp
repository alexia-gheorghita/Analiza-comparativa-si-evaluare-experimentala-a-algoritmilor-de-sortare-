#include <bits/stdc++.h>
#include <fstream>
using namespace std;
ofstream fout("liste.txt");

void randomVector(int n)
{
    fout<<'\n'<<"elemente random :\n";
    for (int i = 0; i < n; i++)
        fout<<rand() % 10000<<' ';
}

void sortedVector(int n)
{
    fout<<'\n'<<"elemente sortate :\n";
    for (int i = 0; i < n; i++)
        fout<<i + 1<<' ';
}

void reversedVector(int n)
{
    fout<<'\n'<<"elemente sortate invers :\n";
    for (int i = 0; i < n; i++)
        fout<<n - i<<' ';
}

void flatVector(int n)
{
    fout<<'\n'<<"elemente plate :\n";
    for (int i = 0; i < n; i++)
        fout<<rand() % 5<<' ';
}

void halfSortedVector(int n)
{
    fout<<'\n'<<"elemente sortate 50% :\n";
    for (int i = 0; i < n/2; i++)
        fout<<i + 1<<' ';
    for (int i = n / 2; i < n; i++)
        fout<<rand() % 10000<<' ';
}

void almostSortedVector(int n)
{
    fout << '\n' << "elemente aproape sortate:\n";

    int* v = new int[n];

    for (int i = 0; i < n; i++)
        v[i] = i + 1;

    int swaps = max(1, n * 5 / 100);

    for (int i = 0; i < swaps; i++) {
        int a = rand() % n;
        int b = rand() % n;
        swap(v[a], v[b]);
    }

    for (int i = 0; i < n; i++)
        fout << v[i] << ' ';

    delete[] v;
}

int main()
{
    int n=10000000;
    fout<<"liste "<<n<<" elemente:"<<'\n';

    //randomVector(n);
    sortedVector(n);
    //reversedVector(n);
    //flatVector(n);
    //halfSortedVector(n);
    //almostSortedVector(n);
    return 0;
}
