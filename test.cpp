#include<fstream>
#include<iostream>
#include<cassert>

////////////////////////////////////////////////
// STRUCTURES , 
////////////////////////////////////////////////

struct Matrix
{
	int * m_array;

	unsigned int m_col;
	unsigned int m_row;

	//constructor
	Matrix(unsigned row , unsigned col) : m_row( row), m_col( col)
	{
		m_array = new int[m_row * m_col];	
	}


	int getMatrixRow() const 
	{
		return m_row;
	}

	int getMatrixCol() const
	{
		return m_col;	
	}

	//operator() overload
	int & operator() ( unsigned int i, unsigned int j)
	{
		return m_array[ i * getMatrixCol() + j];	
	}

	
      //destructor
	~Matrix()
	{
		delete []  m_array;
	}			
};


struct ReadFile
{
	const char * filename;
	std::ifstream like_pipe;

	//constructor
	ReadFile( const char * fn):filename(fn)
	{
	 	like_pipe.open( filename, std::ios::in);
		assert( like_pipe.is_open() && "File opening");
	}

	bool my_read(int &x)
	{
		if(like_pipe >> x )
		{
			std::cout << x  <<  " " ;		
			
		}
		else
		{
			return false;
		}		
		return true;
	}

	//destructor
	~ReadFile()
	{
		like_pipe.close();
	}

};

struct WriteFile
{
	const char * filename;
	std::ofstream like_pipe;
	
	//constructor
	WriteFile(const char * fn) : filename( fn )
	{
		like_pipe.open( filename, std::ios::out);		
		assert( like_pipe.is_open() && "File opening");
	}

	// processing
	bool my_write( int & x)
	{
  	//	like_pipe << x << " ";
		if( like_pipe << x){
			return true;
		}
		else {
			return false;
		}
		return true;
	}

	//destructor
	~WriteFile() 
	{
		like_pipe.close();
	}
};	


////////////////////////////////////////////////
// OPERATION , ALGORITMS , DATA
////////////////////////////////////////////////

bool readMatrix( Matrix & to, ReadFile & from)
{
	
	from.like_pipe.ignore();
	from.like_pipe.ignore();
	int x = 0;
	for(int i = 0; i < to.getMatrixRow(); ++i)	
	{
		for(int j = 0; j < to.getMatrixCol(); ++j)
		{ 
			if( from.my_read(x))
			{
				to( i, j ) = x;
			}
			else
			{
				return false;
			}
		}//end for(j;j;j) 
	std::cout << std::endl;
	}//end for(i;i;i)
	
	std::cout << std::endl;
	return true;
}

bool check( Matrix & from_result, Matrix & from_expect , WriteFile & to_log  )
{
	int x = 0;
	for(int i = 0; i < from_result.getMatrixRow(); ++i)
	{
		for(int j = 0; j < from_result.getMatrixCol(); ++j)
		{
			if( 0 != (from_result( i , j ) - from_result( i , j ))  )	
			{
		       		x = from_result( i, j );
				to_log.my_write(x);
				to_log.like_pipe << "(" << i << ','
			 		 << j << ")" << std::endl;
			}//end if
			else
			{
				return false ;
			}//end if
		}//end for(j;j;j)
	}//end for(i;i;i)
	return true;
}




int main()
{
//Data part
	unsigned int M = 3;
	unsigned int N = 5;


	Matrix res( M, N );
	Matrix exp( M, N );
	ReadFile r_RES("result.txt");
	ReadFile r_EXP("expect.txt");
	WriteFile w_LOG("log.txt");

//Procedure part
	readMatrix( res, r_RES);
	readMatrix( exp, r_EXP);
	check( res, exp, w_LOG);

	std::cout << " I am from main test.cpp ... GOOD LUCK"<< std::endl;	
}
