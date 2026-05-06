#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream> 
#include <iomanip>

using namespace std;
using namespace std::chrono;

long long current_aux_mem=0;
long long max_aux_mem=0;

void track_alloc(long long bytes) 
{
    current_aux_mem+=bytes;
    if (current_aux_mem > max_aux_mem) 
        max_aux_mem=current_aux_mem;
}

void track_free(long long bytes) 
{
    current_aux_mem-=bytes;
}


void bubbleSort(int v[], int n) 
{
    for (int i=0;i<n-1;i++) 
    {
        bool swapped=false;
        for (int j=0;j<n-i-1;j++) 
        {
            if (v[j] > v[j + 1]) 
            { 
                swap(v[j], v[j + 1]); 
                swapped = true; 
            }
        }
        if (!swapped) 
            break;
    }
}

void selectionSort(int v[], int n) 
{
    for (int i=0;i<n-1;i++) 
    {
        int minIndex=i;
        for (int j = i + 1; j < n; j++) 
            if (v[j] < v[minIndex]) 
                minIndex = j;
        swap(v[i], v[minIndex]);
    }
}

void insertionSort(int v[], int n) 
{
    for (int i=1;i<n;i++) 
    {
        int key=v[i],j=i-1;
        while (j >= 0 && v[j] > key) 
        {
             v[j + 1]=v[j];
             j--; 
        }
        v[j + 1]=key;
    }
}

void merge(int v[], int left, int mid, int right) 
{
    int size=right-left+1;
    track_alloc(size*sizeof(int)); 
    int* temp= new int[size];
    int i=left,j=mid+1,k=0;
    while (i<=mid && j<=right) 
        temp[k++] = (v[i] <= v[j]) ? v[i++] : v[j++];
    while (i <= mid) 
        temp[k++] = v[i++];
    while (j <= right) 
        temp[k++] = v[j++];
    for (int t=0;t<size;t++) 
        v[left + t]=temp[t];
    delete[] temp;
    track_free(size * sizeof(int));
}

void mergeSort(int v[], int left, int right) 
{
    if (left<right) 
    {
        int mid=left+(right-left)/2;
        mergeSort(v,left,mid);
        mergeSort(v,mid+1,right);
        merge(v,left,mid,right);
    }
}

void quickSort(int v[], int low, int high) 
{
    if (low>=high) 
        return;
    int pivotIndex=low+rand()%(high-low+1);
    swap(v[low], v[pivotIndex]);
    int lt=low,gt=high,i=low+1,pivot=v[low];
    while (i<=gt) 
    {
        if (v[i]<pivot) 
            swap(v[lt++], v[i++]);
        else if (v[i]>pivot) 
            swap(v[i],v[gt--]);
        else 
            i++;
    }
    quickSort(v,low,lt-1);
    quickSort(v,gt+1,high);
}

void heapify(int v[], int n, int i) 
{
    int largest=i,l=2*i+1,r=2*i+2;
    if (l<n && v[l]>v[largest])
        largest=l;
    if (r<n && v[r]>v[largest]) 
        largest=r;
    if (largest!=i) 
    { 
        swap(v[i], v[largest]); 
        heapify(v, n, largest); 
    }
}

void heapSort(int v[], int n) 
{
    for (int i=n/2-1;i>=0;i--) 
        heapify(v,n,i);
    for (int i=n-1;i>0;i--) 
    { 
        swap(v[0],v[i]);
        heapify(v,i,0); 
    }
}

void countingSort(int v[], int n) 
{
    if (n<=0) 
        return;
    int maxVal=v[0];
    for (int i=1;i<n;i++) 
        if (v[i]>maxVal)   
            maxVal=v[i];
    track_alloc((maxVal+1) * sizeof(int));
    int* count=new int[maxVal + 1]();
    for (int i=0;i<n;i++) 
        count[v[i]]++;
    int index=0;
    for (int i=0;i<=maxVal;i++) 
        while (count[i]-- > 0) 
            v[index++]=i;
    delete[] count;
    track_free((maxVal + 1) * sizeof(int));
}


void checkSort(int v[], int n, string name, nanoseconds duration, long long memory) 
{
    bool isSorted=true;
    for (int i=0;i<n-1;i++) 
    {
        if (v[i]>v[i+1]) 
        { 
            isSorted = false; 
            break; 
        }
    }
    
    double seconds=duration.count() / 1000000000.0;

    cout<<left<<setw(15)<<name 
         <<" | Status: "<<(isSorted ? "OK" : "FAIL") 
         <<" | Timp: "<<fixed<<setprecision(9)<<seconds<<" s" 
         <<" | Memorie: "<<fixed<<setprecision(2)<<memory / 1024.0<<" KB"<<'\n';
}

int main() {
    ifstream fin("liste.txt");
    int n;
    fin>>n; 
    int* original=new int[n];
    int* v=new int[n];

    for (int i=0;i<n;i++) 
    {
        fin>>original[i];
    }
    fin.close();

    auto reset_and_start = [&](int* v, int* original, int n) 
    {
        for (int i=0;i<n;i++) 
            v[i]=original[i];
        max_aux_mem=0;
        current_aux_mem=0;
        return high_resolution_clock::now();
    };

    cout <<"Rezultate Experiment N = "<<n<<"\n";
    cout <<string(85, '-')<<'\n';

    high_resolution_clock::time_point start, stop;

    start=reset_and_start(v,original,n);
    bubbleSort(v,n);
    stop=high_resolution_clock::now();
    checkSort(v,n,"BubbleSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    selectionSort(v,n);
    stop=high_resolution_clock::now();
    checkSort(v,n,"SelectionSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    insertionSort(v,n);
    stop=high_resolution_clock::now();
    checkSort(v,n,"InsertionSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    mergeSort(v,0,n-1);
    stop=high_resolution_clock::now();
    checkSort(v,n,"MergeSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    quickSort(v,0,n-1);
    stop=high_resolution_clock::now();
    checkSort(v,n,"QuickSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    heapSort(v,n);
    stop=high_resolution_clock::now();
    checkSort(v,n,"HeapSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    start=reset_and_start(v,original,n);
    countingSort(v,n);
    stop=high_resolution_clock::now();
    checkSort(v,n,"CountingSort",duration_cast<nanoseconds>(stop - start),max_aux_mem);

    delete[] v;
    delete[] original;

    return 0;
}