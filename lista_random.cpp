#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector> 
#include <iomanip>
#include <ctime>

using namespace std;
using namespace std::chrono;

// Variabile pentru tracking memorie
long long current_aux_mem = 0;
long long max_aux_mem = 0;

void track_alloc(long long bytes) {
    current_aux_mem += bytes;
    if (current_aux_mem > max_aux_mem) 
        max_aux_mem = current_aux_mem;
}

void track_free(long long bytes) {
    current_aux_mem -= bytes;
}

//IMPLEMENTARI ALGORITMI

void bubbleSort(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) { 
                swap(v[j], v[j + 1]); 
                swapped = true; 
            }
        }
        if (!swapped) break;
    }
}

void selectionSort(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) 
            if (v[j] < v[minIndex]) minIndex = j;
        swap(v[i], v[minIndex]);
    }
}

void insertionSort(int v[], int n) {
    for (int i = 1; i < n; i++) {
        int key = v[i], j = i - 1;
        while (j >= 0 && v[j] > key) {
             v[j + 1] = v[j];
             j--; 
        }
        v[j + 1] = key;
    }
}

void merge(int v[], int left, int mid, int right) {
    int size = right - left + 1;
    track_alloc(size * sizeof(int)); 
    int* temp = new int[size];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) 
        temp[k++] = (v[i] <= v[j]) ? v[i++] : v[j++];
    while (i <= mid) temp[k++] = v[i++];
    while (j <= right) temp[k++] = v[j++];
    for (int t = 0; t < size; t++) v[left + t] = temp[t];
    delete[] temp;
    track_free(size * sizeof(int));
}

void mergeSort(int v[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(v, left, mid);
        mergeSort(v, mid + 1, right);
        merge(v, left, mid, right);
    }
}

void quickSort(int v[], int low, int high) {
    if (low >= high) return;
    int pivotIndex = low + rand() % (high - low + 1);
    swap(v[low], v[pivotIndex]);
    int lt = low, gt = high, i = low + 1, pivot = v[low];
    while (i <= gt) {
        if (v[i] < pivot) swap(v[lt++], v[i++]);
        else if (v[i] > pivot) swap(v[i], v[gt--]);
        else i++;
    }
    quickSort(v, low, lt - 1);
    quickSort(v, gt + 1, high);
}

void heapify(int v[], int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && v[l] > v[largest]) largest = l;
    if (r < n && v[r] > v[largest]) largest = r;
    if (largest != i) { 
        swap(v[i], v[largest]); 
        heapify(v, n, largest); 
    }
}

void heapSort(int v[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(v, n, i);
    for (int i = n - 1; i > 0; i--) { 
        swap(v[0], v[i]);
        heapify(v, i, 0); 
    }
}

void countingSort(int v[], int n) {
    if (n <= 0) return;
    int maxVal = v[0];
    for (int i = 1; i < n; i++) if (v[i] > maxVal) maxVal = v[i];
    track_alloc((maxVal + 1) * sizeof(int));
    int* count = new int[maxVal + 1]();
    for (int i = 0; i < n; i++) count[v[i]]++;
    int index = 0;
    for (int i = 0; i <= maxVal; i++) 
        while (count[i]-- > 0) v[index++] = i;
    delete[] count;
    track_free((maxVal + 1) * sizeof(int));
}

//FUNCTII EXPERIMENT

void generateRandomVector(int v[], int n) {
    for (int i = 0; i < n; i++)
        v[i] = rand() % 10000;
}

int main() {
    srand(time(0)); // Seed pentru numere aleatorii
    
    const int n = 1000;           // Dimensiunea listei
    const int num_tests = 1000; // Numarul de liste testate
    
    // Structuri pentru a stoca sumele totale pentru medii
    double total_time[7] = {0};
    double total_mem[7] = {0};
    string names[7] = {"BubbleSort", "SelectionSort", "InsertionSort", "MergeSort", "QuickSort", "HeapSort", "CountingSort"};

    int* original = new int[n];
    int* v = new int[n];

    cout << "Rulare experiment academic: " << num_tests << " teste pentru N = " << n << endl;
    cout << "Se genereaza medii statistice..." << endl << endl;

    for (int t = 0; t < num_tests; t++) {
        generateRandomVector(original, n);

        auto run_test = [&](int alg_idx, auto sort_func) {
            for (int i = 0; i < n; i++) v[i] = original[i]; // Reset list
            max_aux_mem = 0; current_aux_mem = 0; // Reset memory tracking
            
            auto start = high_resolution_clock::now();
            sort_func();
            auto stop = high_resolution_clock::now();
            
            total_time[alg_idx] += duration_cast<nanoseconds>(stop - start).count();
            total_mem[alg_idx] += max_aux_mem;
        };

        run_test(0, [&]() { bubbleSort(v, n); });
        run_test(1, [&]() { selectionSort(v, n); });
        run_test(2, [&]() { insertionSort(v, n); });
        run_test(3, [&]() { mergeSort(v, 0, n - 1); });
        run_test(4, [&]() { quickSort(v, 0, n - 1); });
        run_test(5, [&]() { heapSort(v, n); });
        run_test(6, [&]() { countingSort(v, n); });

        if ((t + 1) % 1000 == 0) cout << "Progres: " << t + 1 << "/" << num_tests << " liste procesate." << endl;
    }

    // Afisare Rezultate Finale (Medii)
    cout << "\n" << string(70, '=') << "\n";
    cout << left << setw(20) << "ALGORITM" << " | " 
         << setw(18) << "TIMP MEDIU (s)" << " | " 
         << "MEMORIE MEDIE (KB)" << "\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < 7; i++) {
        double avg_time = (total_time[i] / num_tests) / 1000000000.0;
        double avg_mem = (total_mem[i] / num_tests) / 1024.0;
        
        cout << left << setw(20) << names[i] << " | " 
             << fixed << setprecision(9) << setw(18) << avg_time << " | " 
             << fixed << setprecision(2) << avg_mem << " KB" << "\n";
    }
    cout << string(70, '=') << "\n";

    delete[] v;
    delete[] original;
    return 0;
}