#include<iostream>
#include<fstream>
#include<cassert>
#include<iomanip>

//##################################################
// #### DATA PART, STRUCTURE PART ##################
//##################################################
struct truetype{

};

struct falsetype{

};

template<typename I>
struct Matrix1{
	const unsigned int m_row;
	const unsigned int m_col;
	int * m_array;
	//##### ctor_1
	Matrix1( const unsigned int R ,const  unsigned int C)
		: m_row(R), m_col(C )
	{
		m_array = new int [R*C]; 
	}

	int get_matrix_row()
	{
		return m_row;
	}

	int get_matrix_col()
	{
		return m_col;
	}

	int & operator()( const int i,const int j)
	{
		return m_array[get_matrix_col() * i + j];
	}
};

template<typename I>
struct Matrix2{
	const unsigned int m_row;
	const unsigned int m_col;
	int * m_array;
	//##### ctor_1
	Matrix2( const unsigned int R ,const  unsigned int C)
		: m_row(R), m_col(C)
	{
		m_array = new int [R*C]; 
	}

	int get_matrix_row()
	{
		return m_row;
	}

	int get_matrix_col()
	{
		return m_col;
	}

	int & operator()( const int i,const int j)
	{
		return m_array[ get_matrix_col() * i + j ];
	}
};

template<typename T>
struct info{

};

template<typename I>
struct info<Matrix1<I> >{
	typedef truetype nshan;
};

template<typename I>
struct info<Matrix2<I> >{
	typedef falsetype nshan;
};

struct ReadFile
{
	const char * filename;
	std::ifstream file_stream;

	//#ctor_1
	ReadFile (const char * fn):filename(fn)
	{
		file_stream.open( filename, std::ios::in);
	    	assert(file_stream.is_open() &&  "File opening");
	}

	bool my_read(int &i)
	{
		if( file_stream >> i ){
			file_stream.get()>> i ;
			std::cout << i  << " ";
		} else 	{
			return false;
		  }
		return true;
	}

	//destructor
	~ReadFile()
	{
		file_stream.close();
	}
};

struct WriteFile{
	const char *filename;
	std::ofstream file_stream;	

	//construtor
	WriteFile( const char * fn):filename(fn)
	{
		file_stream.open( filename, std::ios::out);
		assert( file_stream.is_open() && " File opening");
	}

	bool my_write( int & i)
	{
		if( file_stream << i){
			std::cout << i << " ";
		} else 	{
			return false;
		  }
   		return true;
	}

	//destructor
	~WriteFile()
	{
		file_stream.close();		
	}	
};

//##################################################
//######PROCEDURE PART, ALGORITHM PART##############
//##################################################
template<typename T>
bool readMatrix(T & to, ReadFile & from)
{
	from.file_stream.ignore(1,'#');
	from.file_stream.ignore(1,EOF);
	from.file_stream.ignore(2,EOF);
	int x= 0;
	for (int i = 0; i < to.get_matrix_row(); ++i) {
		for (int j= 0; j < to.get_matrix_col(); ++j){
			if (from.my_read(x) ) {
				to(i,j) = x; 
			} else 	{
			    std::cout << "Can not read file... "<< &from <<  std::endl; 
				return false;
			  }
		} 
		  std::cout << "\n";//test
	}
		from.file_stream.ignore(1,EOF); 
		std::cout << "\n" ; // space between matrixes
	return true;
	std::cout << " ------readMatrix ---DONE \n";
}

template<typename T>
void add_matrix(T & from1, T & from2, T & to)
{
	for (int i = 0; i < from1.get_matrix_row();++i){
		for(int j = 0; j < from1.get_matrix_col();++j) {
			to(i,j) = from1(i,j) + from2(i,j);
			std::cout << std::setw(2) << to(i,j)<< " " ; //test
		}
			std::cout << std::endl; //test
	}
	std::cout << " -------add_matrix ---DONE \n";
}

template<typename T>
void mult_matrix(T &from1, T & from2, T & to)
{
	typename info<T>::nshan tags;
	fake_mult_matrix(from1, from2, to, tags);// compiler need to choose,
					// which version to use ( A or B)
	std::cout << "Depend of situation I have A or B version\n"; 
		  
}

template<typename T>
void fake_mult_matrix(T &from1, T & from2, T & to, truetype)
{
	
	for( int i = 0; i < from1.get_matrix_row(); ++i){	
		for( int j = 0; j < from2.get_matrix_col(); ++j) {
		int sum = 0;
			for( int k = 0; k < from1.get_matrix_col(); ++k){
				sum += from1(i,k) * from2( k, j);
			}
			to(i,j) = sum;
			std::cout << sum << "   "; //test pair
		}	
			std::cout << std::endl; //test pair
	}
	std::cout << "A Version" << std::endl;
}

template<typename T>
void fake_mult_matrix(T &from1, T & from2, T & to, falsetype)
{
	for( int i = 0; i < from1.get_matrix_row(); ++i){	
		for( int j = 0; j < from2.get_matrix_col(); ++j) {
		int sum = 0;
			for( int k = 0; k < from1.get_matrix_col(); ++k){
				sum += from1(i,k) * from2( k, j);
			}
			to(i,j) = sum;
			std::cout << sum << "   "; //test pair
		}	
			std::cout << std::endl; //test pair
	}
	std::cout << "B Version " << std::endl;
}

template<typename T>
bool write_matrix(T & from, WriteFile & to)
{
	static int p = 1;
	to.file_stream << "#" << p << std::endl;
	for (int i = 0; i < from.get_matrix_row(); ++i){ 
		for (int k = 0; k < from.get_matrix_col(); ++k) {
			if (to.my_write( from(i,k))){
				to.file_stream.put(' ');
			} else	{
			  std::cout <<"Can not write file..." << &from << std::endl;
				return false;
			  }
		}
		std::cout << std::endl;
			to.file_stream << std::endl;
	}
		p += 1;
	return true;
}

void input_matrix_dimensions( unsigned int & M, 
			    unsigned int & N,
			    unsigned int & K,
			    unsigned int & HOW_MANY)
{
	std::cout << "Please enter A & B matrix Row\n ";
	std::cout << " M = ";
	//std::cin >> M; //remove after test
	M = 3;
	std::cout << "Please enter A & B matrix Columns\n ";
	std::cout << " N = ";
	//std::cin >> N; //remove after test
	N = 4;	
	std::cout << "Please enter A & B matrix Columns\n ";
	std::cout << " K = ";
	//std::cin >> K; //remove after test
	K = 5;
	std::cout << std::endl;	
	std::cout << "Please how many matrix you need to check\n";
	//std::cin >> HOW_MANY;
	HOW_MANY = 10;
	std::cout << std::endl;
}


int main()
{
	unsigned int M = 0;
	unsigned int N = 0;
	unsigned int K = 0;
	unsigned int HOW_MANY = 0;
	input_matrix_dimensions(M, N, K , HOW_MANY);
	Matrix1<int> A(M,N), B(M,N), AB(M,N),
	       C(N,K), D(N,K ), CD(N,K), ABCD(M,K);
	ReadFile ra("a.txt"), rb("b.txt"), rc("c.txt"), rd("d.txt");
	WriteFile wa("result.txt");
	while (readMatrix(A,ra) && readMatrix(B,rb) && 
		readMatrix(C,rc) &&  readMatrix(D,rd)){
			add_matrix(C,D, CD);
			add_matrix(A,B, AB);
			mult_matrix(AB, CD, ABCD);
			write_matrix(ABCD, wa);
	}
	
	input_matrix_dimensions(M, N, K , HOW_MANY);
	Matrix2<int> A1(M,N), B1(M,N), AB1(M,N),
	       C1(N,K), D1(N,K ), CD1(N,K), ABCD1(M,K);
	ReadFile ra1("a.txt"), rb1("b.txt"), rc1("c.txt"), rd1("d.txt");
	WriteFile wa1("result.txt");
	while (readMatrix(A1,ra1) && readMatrix(B1,rb1) && 
		readMatrix(C1,rc1) &&  readMatrix(D1,rd1)){
			add_matrix(C1,D1, CD1);
			add_matrix(A1,B1, AB1);
			mult_matrix(AB1, CD1, ABCD1);
			write_matrix(ABCD1, wa1);
	}	
	
	std::cout << "\n+++++++++++++ ALL CURRENT +++++++++++"<< std::endl;
	return 0;
}
