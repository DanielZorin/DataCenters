#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

int
check_correction(int n_resources, int n_requests, int *sizes_resources, int *sizes_requests, int *a)
{
	//std::cout << "!!!" << std::endl;
	int *size_cur = new int[n_resources + 1];
	for (int i = 1; i <= n_requests; ++i) {
		size_cur[a[i]] += sizes_requests[i];
	}
	for (int i = 1; i <= n_resources; ++i) {
		//std::cout << size_cur[i] << " ";
		if (size_cur[i] > sizes_resources[i]) {
			return 0;
		}
	}
	return 1;
} 
	
int *
generate_distribution(int n_resources, int n_requests, int *sizes_resources, int *sizes_requests)
{
	int *a = new int[n_requests + 1]; //numbers of resources
	//srand(time(NULL)); 
	do 
	{
		//std::cout << "..." << std::endl;
        for (int i = 1; i <= n_requests; ++i) {
			a[i] = rand() % (n_resources + 1);
		}
		
	} while (!check_correction(n_resources, n_requests, sizes_resources, sizes_requests, a));
	//std::cout << "all" << std::endl;	 
	return a;   
}

void
display(int *a, int n)
{
	for (int i = 1; i <= n; ++i) {
		std::cout << i << " " << a[i] << std::endl;
	}
	std::cout << std::endl;
}

int
func(int *a, int n_requests)
{
	int sum = 0;
	for (int i = 1; i <= n_requests; ++i) {
		if (!a[i]) {
			++sum;
		}
	}
	return sum;
}

int *
get_load(int n_resources, int n_requests, int *sizes_requests, int *a)
{
	int *size_cur = new int[n_requests + 1];
	for (int i = 1; i <= n_requests; ++i) {
		size_cur[a[i]] += sizes_requests[i];
	}
	return size_cur;
}

double 
func1(int n_requests, int n_resources, int *sizes_requests, int *a)
{
	int *b = get_load(n_resources, n_requests, sizes_requests, a);
	int sum = 0;
	for (int i = 1; i <= n_requests; ++i) {
		sum += sizes_requests[i];
	}
	double av_t = double(sum) / n_resources;
	double res = 0;
	for (int i = 1; i <= n_resources; ++i) {
		res += (double(b[i]) - av_t);
	}
	return res;
}

int *
maximize_by_n(int n_resources, int n_requests, int *sizes_resources, int *sizes_requests)
{
	int *a0 = new int[n_requests + 1];
	int *a = new int[n_requests + 1];
	int *best = new int[n_requests + 1];
	a0 = generate_distribution(n_resources, n_requests, sizes_resources, sizes_requests);
	//std::cout << "generated ";
	memcpy(best, a0, sizeof(int) * (n_requests + 1));
	double delta = 0, temperature, start_temperature = 50;
	double p = 1; 
	int step = 0;
	do {
		//std::cout << "step " << step << std::endl;
		temperature = start_temperature / log(1 + step);
		for (int i = 0; i < 10; ++i) {
			a = generate_distribution(n_resources, n_requests, sizes_resources, sizes_requests);
			delta = func(a, n_requests) - func(a0, n_requests); // > 0, если нулей стало больше 
			 //+ func1(n_requests, n_resources, sizes_requests, a) - func1(n_requests, n_resources, sizes_requests, a0);
			double h = (double)rand() / RAND_MAX;
			p = exp(-delta / temperature);
			if (delta <= 0 || h > p) { // cтало лучше
				a0 = a;
			}
			if (func(a0, n_requests) < func(best, n_requests)) {
				memcpy(best,a0, sizeof(int) * (n_requests + 1));
			}
		}
		++step;
	} while (temperature > 7 && func(best, n_requests) != 0 && step < 5000);
	return best;
} 	

int 
main(void)
{
	
	srand(static_cast <unsigned> (time(0)));
	int n_requests = 0, n_resources = 0; 
	std::cout << "N resources = "; std::cin >> n_resources;
	std::cout << "N requests = "; std::cin >> n_requests; 
	int sizes_requests[n_requests + 1], sizes_resources[n_resources + 1];
	std::cout << "Resources sizes: ";
	for (int i = 1; i <= n_resources; ++i) {
		std::cin >> sizes_resources[i];
	}
	std::cout << "Requests sizes: ";
	for (int i = 1; i <= n_requests; ++i) {
		std::cin >> sizes_requests[i];
	}
	std::cout << "..." << std::endl;
	int *a = maximize_by_n(n_resources, n_requests, sizes_resources, sizes_requests);
	display(a, n_requests);

	return 0;
}
