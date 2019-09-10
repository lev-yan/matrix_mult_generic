#include<iostream>

struct Matrix1{};

struct Matrix2{};


template<typename T>
void sum_matrix(const T & m1,const T & m2, T & m)
{
	std::cout << "function sum_matrix\n";
}




int main()
{
	Matrix1 m1;
	Matrix1 m2;
	Matrix1 m;
	sum_matrix(m1, m2, m);
	std::cout << "##############" << std::endl;
	return 0;
}
