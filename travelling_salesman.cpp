#include <iostream>  
#include <vector>  
#include <cmath>  
#include <cstdlib>  
#include <fstream>  
#include <ctime>  
#include <algorithm>  
#include <string>  
#include <random>  
  
using namespace std;  
  
// Structure to store the coordinates of cities  
struct City {  
   int x, y;  
};  
  
// Function to calculate the Euclidean distance between two cities  
double calculateDistance(const City &city1, const City &city2) {  
   return sqrt((city1.x - city2.x) * (city1.x - city2.x) + (city1.y - city2.y) * (city1.y - city2.y));  
}  
  
// Function to calculate the total cost of a tour  
double calculateTourCost(const vector<int> &tour, const vector<City> &cities) {  
   double totalCost = 0;  
   int numCities = tour.size();  
   for (int i = 0; i < numCities - 1; ++i) {  
      totalCost += calculateDistance(cities[tour[i]], cities[tour[i+1]]);  
   }  
   totalCost += calculateDistance(cities[tour[numCities-1]], cities[tour[0]]); // Return to start  
   return totalCost;  
}  
  
// Function to generate a neighboring solution using 2-Opt  
void generateNeighbor2Opt(vector<int> &tour) {  
   std::random_device rd;  
   std::mt19937 gen(rd());  
   std::uniform_int_distribution<int> dis(0, tour.size() - 1);  
   int i = dis(gen);  
   int j = dis(gen);  
   if (i > j) {  
      swap(i, j);  
   }  
   reverse(tour.begin() + i, tour.begin() + j + 1);  
}  
  
// Function to read city coordinates from a file  
vector<City> readCitiesFromFile(const string &filename) {  
   vector<City> cities;  
   ifstream file(filename);  
  
   if (!file.is_open()) {  
      cerr << "Error: Unable to open file!" << endl;  
      exit(1);  
   }  
  
   int x, y;  
   while (file >> x >> y) {  
      if (file.fail()) {  
        file.clear();  
        file.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip non-numeric lines  
        continue;  
      }  
      cities.push_back({x, y});  
   }  
  
   file.close();  
   return cities;  
}  
  
// Simulated Annealing function to solve the TSP  
vector<int> simulatedAnnealing(const vector<City> &cities, int maxIterations, double initialTemperature, double coolingRate) {  
   int numCities = cities.size();  
  
   // Initial solution: Random permutation of cities  
   vector<int> currentSolution(numCities);  
   for (int i = 0; i < numCities; ++i) {  
      currentSolution[i] = i;  
   }  
   std::random_device rd;  
   std::mt19937 gen(rd());  
   std::shuffle(currentSolution.begin(), currentSolution.end(), gen);  
  
   double currentCost = calculateTourCost(currentSolution, cities);  
   vector<int> bestSolution = currentSolution;  
   double bestCost = currentCost;  
  
   double temperature = initialTemperature;  
  
   for (int iteration = 0; iteration < maxIterations; ++iteration) {  
      // Generate a new solution using 2-opt  
      vector<int> newSolution = currentSolution;  
      generateNeighbor2Opt(newSolution);  
      double newCost = calculateTourCost(newSolution, cities);  
  
      // Calculate the acceptance probability  
      double acceptanceProbability = std::exp(min(700.0, max(-700.0, (currentCost - newCost) / temperature)));  
  
      // Accept the new solution with a probability  
      if (newCost < currentCost || acceptanceProbability > ((double) rand() / RAND_MAX)) {  
        currentSolution = newSolution;  
        currentCost = newCost;  
  
        // Update the best solution  
        if (newCost < bestCost) {  
           bestSolution = newSolution;  
           bestCost = newCost;  
        }  
      }  
  
      // Cool down the temperature  
      temperature *= coolingRate;  
   }  
  
   return bestSolution;  
}  
  
// Function to print the tour and its total cost  
void printTour(const vector<int> &tour, const vector<City> &cities) {  
   for (int i : tour) {  
      if (!(cout << i << " ")) {  
        cerr << "Error: Unable to print tour!" << endl;  
        exit(1);  
      }  
   }  
   cout << endl;  
   cout << "Total Cost: " << calculateTourCost(tour, cities) << endl;  
}  
  
int main() {  
   std::random_device rd;  
   std::mt19937 gen(rd());  
  
   // Read cities from file  
   string filename = "xqf131.tsp";  // You can change the file name based on the dataset  
   vector<City> cities = readCitiesFromFile(filename);  
  
   // Edge case check  
   if (cities.size() < 2) {  
      cerr << "Error: Not enough cities to form a tour." << endl;  
      return 1;  
   }  
  
   // Simulated Annealing parameters  
   int maxIterations = 10000;  
   double initialTemperature = 1000.0;  
   double coolingRate = 0.99;  
  
   // Solve TSP using Simulated Annealing  
   vector<int> bestTour = simulatedAnnealing(cities, maxIterations, initialTemperature, coolingRate);  
  
   // Print the best tour and its cost  
   printTour(bestTour, cities);  
  
   return 0;  
}