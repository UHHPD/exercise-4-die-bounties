#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
using namespace std;
random_device r;
mt19937 generator(r());
poisson_distribution<int>random_poisson(100);

double logPoisson(int k, double mu) {
  double logL = k*log(mu)-mu-lgamma(k+1);
  return logL;
}

double poisson(int k, double mu) {
  double p = pow(mu, k)*exp(-mu)/tgamma(k+1);
  return p;
}

double Q_func(int N, int Ns, int Nb) {
  double q =-2*(logPoisson(N, Ns+Nb)-logPoisson(N, Nb));
  return q;
}

int poisson_dist() {
	int value = random_poisson(generator);
	return value;
}

int main() {
	double Qstar = -2*(logPoisson(115, 120)-logPoisson(115, 100));
	cout << "Q*: " << Qstar << "\n";

	//Q is monotonically decreasing function: -2(logL(x;120)-logL(x;100)=-2(x*log(120/100)+20))
	// therefore a termination condition can be defined.

	int i = 0;
	int signal = 20;
	int bg = 100;
	double Q = Q_func(0, signal, bg);
	//cout << "Q: " << Q << "\n";
	double numerator = 0;
	double denominator = 0;
	double CL = 1;
	while (CL>0.05) {
		while (Q>Qstar) {
			numerator += poisson(i, signal+bg);
			denominator += poisson(i, bg);
			i++;
			Q = Q_func(i, signal, bg);
		}
		if (signal == 20) {
			cout << "numerator: " << numerator << "\n";
			cout << "denominator: " << denominator << "\n";
			cout << "limit: " << numerator/denominator << " -> N signal = 20 not excluded" <<"\n";
		}
		CL = numerator/denominator;
		signal++;
		numerator = 0;
		denominator = 0;
		i = 0;
		Q = Q_func(0, signal, bg);
	}
	cout << "smallest N signal to be excluded: " << signal-1 << "\n";

	int counter_excluded = 0;
	for (int j=0; j<502; j++){
	int i = 0;
	int signal = 20;
	int bg = 100;
	double Q = Q_func(0, signal, bg);
	//cout << "Q: " << Q << "\n";
	double numerator = 0;
	double denominator = 0;
	double CL = 1;
	int N_data = poisson_dist();
	double Q_star = Q_func(N_data, signal, bg);
	while (CL>0.05) {
		while (Q>Q_star) {
			numerator += poisson(i, signal+bg);
			denominator += poisson(i, bg);
			i++;
			Q = Q_func(i, signal, bg);
		}
		if (signal==20 and numerator/denominator>0.05) {
			//cout << "numerator: " << numerator << "\n";
			//cout << "denominator: " << denominator << "\n";
			cout << "limit: " << numerator/denominator<<" " << N_data <<" "<< " -> N signal = 20 not excluded" <<"\n";
		}
		if (signal==20 and numerator/denominator<0.05){
			cout << "limit: " << numerator/denominator << " "<<N_data << " "<<" -> N signal = 20 excluded" <<"\n";
			counter_excluded++;
		}
		CL = numerator/denominator;
		signal++;
		numerator = 0;
		denominator = 0;
		i = 0;
		Q = Q_func(0, signal, bg);
	}
	cout << "smallest N signal to be excluded: " << signal-1 << "\n";
	}
	cout << "probability to exclude N signal=20: " << counter_excluded/500. << "\n";
}