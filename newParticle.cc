#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include "MC.h"
using namespace std;
random_device r;
mt19937 generator(r());
uniform_real_distribution <> distribution(0, 1);
normal_distribution<>normal(2000, 200);

double normalGen() {
	double value = normal(generator);
	return value;
}

double custom(double x) {
	double value = 1000/sqrt(1-2000000*(5e-7)*x);
	return value;
}

Vec randomVector(mt19937 &generator, double norm)
{
    // Fill me in question 1.d!
    uniform_real_distribution <> distributionPhi(0, 2*M_PI);
    double phi = distributionPhi(generator);
    uniform_real_distribution <> distributionCos(-1, 1);
    double cos_theta = distributionCos(generator);
    double theta = acos(cos_theta);
    double r = norm;
    Vec result;
    result.x = r*cos(phi)*sin(theta);
    result.y = r*sin(phi)*sin(theta);
    result.z = r*cos_theta;
    return result;
}

int main() {
	cout << custom(distribution(generator)) << "\n";
	Histogram mass_values(1000, 3000, 40);

	ofstream outFile1;
	ofstream outFile2;
	outFile1.open("hist_mass.txt");
	outFile2.open("filtered_hist_mass.txt");
	double counter = 0;
	for (int i=0; i<1000; i++) {
		double mass = normalGen();
		Vec result = randomVector(generator, mass/2);
		outFile1 << mass << "\n";
		double pT = sqrt(pow(result.x, 2)+pow(result.y, 2));
		if (pT<500 or abs(2*result.z/mass)>0.98) {continue;}
		counter++;
		outFile2 << mass << "\n";
		mass_values.fill(mass);
	}
	ofstream outFile3;
	ofstream outFile4;
	outFile3.open("hist_mass_bg.txt");
	outFile4.open("filtered_hist_mass_bg.txt");
	double counter_bg = 0;
	for (int i=0; i<30000; i++) {
		double mass_bg = custom(distribution(generator));
		Vec result_bg = randomVector(generator, mass_bg/2);
		outFile3 << mass_bg << "\n";
		double pT = sqrt(pow(result_bg.x, 2)+pow(result_bg.y, 2));
		if (pT<500 or abs(2*result_bg.z/mass_bg)>0.98) {continue;}
		counter_bg++;
		outFile4 << mass_bg << "\n";
	}
	cout << "fraction satisfying detector conditions: " << counter/10 << "%" << "\n";
	cout << "fraction satisfying detector conditions (Background): " << counter_bg*100/30000 << "%" << "\n";
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	mass_values.write(cout);
}