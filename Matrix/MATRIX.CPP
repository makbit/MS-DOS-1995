#include "Matrix.h"
#include <string.h>      // strncpy, memcpy, memcmp
#include <iostream.h>

#define MATR(i,j)    ( *(matr+i*n+j) )
Matrix::Exception::Exception(char *why)
{
	strncpy(str, why, sizeof(str));
}
//-----------------------------------------------------------------//
//            CONSTRUCTORS                                         //
//-----------------------------------------------------------------//
Matrix::Matrix(int _m, int _n)
{
	m = _m;
	n = _n;
	matr = new real[m*n];
}
		
//-----------------------------------------------------------------//
Matrix::Matrix(int _m, int _n, real *array)
{
	m = _m;
	n = _n;
	register int size = m*n;
	matr = new real[size];
	memcpy(matr, array, size*sizeof(real));
}
//-----------------------------------------------------------------//
Matrix::Matrix(Matrix& M)
{
	m = M.m;
	n = M.n;
	if( M.matr )
	{
		register size = m*n;
		matr = new real[size];
		memcpy(matr, M.matr, size*sizeof(real));
	}
	else
	{
		matr = 0L;
	}
}

//-----------------------------------------------------------------//
//         OPERATORS                                               //
//-----------------------------------------------------------------//
Matrix& Matrix::operator =(Matrix& M)
{
	if( this!=&M )
	{
		if( check_size(M) ) // the same size
		{
			memcpy(matr, M.matr, sizeof(real)*m*n);
		}
		else // maybe (if size>=M.size ) assign  else {delete;assign}
		{
			int size = M.m * M.n;
			if( m*n < size ) // we need more memory
			{
				delete[] matr;
				matr = new real[size];
			}
			m = M.m;
			n = M.n;
			if( M.matr ) // not a NULL matrix
			{
				memcpy(matr, M.matr, size*sizeof(real));
			}
			else
			{
				delete[] matr;
				matr = 0L;
			}
		}
	}
	return *this;
}

//
//   if( matr==null ) then size==0
//
//-----------------------------------------------------------------//
Matrix& Matrix::operator -()
{
	int size = m*n;
	for( int i=0; i<size; i++ ) matr[i] = -matr[i];
	return *this;
}

//-----------------------------------------------------------------//
Matrix& Matrix::operator+=(Matrix& M)
{
	if( !check_size(M) ) throw Exception("Matrix& operator +=");
	int size = m*n;
	for(int i=0; i<size; i++) matr[i] += M.matr[i];
	return *this;
}
//-----------------------------------------------------------------//
Matrix& Matrix::operator-=(Matrix& M)
{
	if( !check_size(M) ) throw Exception("Matrix& operator -=");
	int size = m*n;
	for(int i=0; i<size; i++) matr[i] -= M.matr[i];
	return *this;
}
//-----------------------------------------------------------------//
Matrix& Matrix::operator*=(real f)
{
	int size = m*n;
	for( int i=0; i<size; i++ ) matr[i] *= f;
	return *this;
}
//-----------------------------------------------------------------//
Matrix& Matrix::operator/=(real f)
{
	f = 1/f; // multiplication is faster
	return ( (*this) *=f );
}
//-----------------------------------------------------------------//
Matrix& Matrix::operator*=(Matrix& M)
{
	if( !matr || !M.matr || (n!=M.m) ) throw Exception("Matrix& operator *=(Matrix&)");
	//       n
	// Cij = S (Aiq * Bqi)    Amn * Bnk = Cmk;
	//       q                row*column
	Matrix tmp = *this;
	//m = m;
	n = M.n;
	int new_size = m * n;
	if( new_size > tmp.m*tmp.n ) // we need more memory
	{
		delete[] matr;
		matr = new real[new_size];
	}
	int maxx = tmp.n;
	for(int i=0; i<m; i++)
		for(int j=0; j<n; j++)
		{
			real &r = MATR(i,j);
			real* tmp_i = tmp[i];
			r = real(0.0);
			for(int q=0; q<maxx; q++) 
				r += tmp_i[q] * M[q][j];
		}
	return *this;
}

//-----------------------------------------------------------------//
//      Optional operations                                        //
//-----------------------------------------------------------------//
Matrix operator +(Matrix& m1, Matrix& m2) 
{ 
	Matrix M=m1; 
	return (M+=m2); 
}

Matrix operator -(Matrix& m1, Matrix& m2)
{
	Matrix M=m1; 
	return (M-=m2); 
}
	
Matrix operator *(Matrix& m1, Matrix& m2)
{
	if( !m1.matr || !m2.matr || (m1.n!=m2.m) ) 
		throw Matrix::Exception("Matrix operator *=(Matrix&, Matrix&)");
	int maxx = m1.n;
	Matrix res(m1.m, m2.n);
	for(int i=0; i<res.m; i++)
		for(int j=0; j<res.n; j++)
		{
			real &r = res[i][j];
			r = real(0.0);
			for(int q=0; q<maxx; q++) 
				r += m1[i][q] * m2[q][j];
		}
	return res;
}
	
Matrix operator *(Matrix& m1, real f)
{
	Matrix M=m1;
	return (M*=f);
}

Matrix operator *(real f, Matrix& m1)
{
	Matrix M=m1;
	return (M*=f);
}

Matrix operator /(Matrix& m1, real f)
{
	Matrix M=m1;
	f = 1/f;
	return (M*=f);
}

//-----------------------------------------------------------------//
//     I/O Operations                                              //
//-----------------------------------------------------------------//
void Matrix::read (istream& is)
{
	int old_size = m*n;
	is.read((char*)&m, sizeof(int));
	is.read((char*)&n, sizeof(int));
	int new_size = m * n;
	if( new_size > old_size )
	{
		delete[] matr;
		matr = new real[new_size];
	}
	if( new_size ) is.read((char*)matr, sizeof(real)*new_size);
}

void Matrix::write(ostream& os)
{
	os.write((char*)&m, sizeof(int));
	os.write((char*)&n, sizeof(int));
	if( m && n ) os.write((char*)matr, sizeof(real)*m*n);
}

istream& operator >>(istream& is, Matrix &M)
{
	int old_size = M.m * M.n;
	is>>M.m>>M.n;
	int new_size = M.m * M.n;
	if( new_size > old_size )
	{
		delete[] M.matr;
		M.matr = new real[new_size];
	}
	for(int i=0; i<new_size; i++) is>>M.matr[i];
	return is;
}

ostream& operator <<(ostream& os, Matrix &M)
{
	os<<M.m<<' '<<M.n<<endl;
	for(int i=0; i<M.m; i++)
	{
		for(int j=0; j<M.n; j++) os<<M[i][j]<<' ';
		os<<endl;
	}
	return os;
}

//-----------------------------------------------------------------//
//                                                                 //
//-----------------------------------------------------------------//
void Matrix::LoadIdentity()
{
	if( matr )
	{
		memset(matr, 0, m*n*sizeof(real));
		int a = (m < n ) ? m : n; // number of elements on diagonal
		for(int i=0; i<a; i++) MATR(i,i) = 1.0;
	}
}

//-----------------------------------------------------------------//
bool Matrix::equal(Matrix& M)
{
	if( (matr==M.matr) || 
	    (check_size(M) && !memcmp(matr,M.matr,sizeof(real)*m*n)) )
		return true;
	else
		return false;
}

//-----------------------------------------------------------------//
//     Special Operations                                          //
//-----------------------------------------------------------------//
void Matrix::transpose()
{
	if( matr )
	{
		if( m==n ) // this is square matrix
		{ // this is much faster!
			for(int i=0; i<m; i++)
			for(int j=i+1; j<n; j++)
			{
				real f = MATR(i,j);
				MATR(i,j) = MATR(j,i);
				MATR(j,i) = f;
			}
		}
		else
		{
			// I need more effective algorithm!
			Matrix tmp(*this);
			int a = n;
			n = m;
			m = a;
			for(int i=0; i<m; i++)
			for(int j=0; j<n; j++)
				MATR(i,j) = tmp[j][i];
		}
	}
//	return *this;
}

//-----------------------------------------------------------------//
Matrix Matrix::submatrix(int i, int j)
{
	if( !matr || (m<=1) || (n<=1) ) throw Exception("Matrix::submatrix()");
	Matrix M(m-1, n-1);

	// I should improve this loops!!!
	int a, b, x, y;
	for(a=0, y=0; y<m; a++, y++)
	{
		if( y==i )
		{
			y++; // skip(remove) row
			if( y==m ) break;
		}
		for(b=0, x=0; x<n; b++, x++)
		{
			if( x==j )
			{
				x++; // skip(remove) column
				if( x==n ) break;
			}
			M[a][b] = MATR(y, x);
		}
	}
	return M;
}
	
static real asign[] = { +1.0, -1.0 };
//-----------------------------------------------------------------//
real Matrix::det()
{
	// matrix must be square
	if( !matr || (m!=n) ) throw Exception("Matrix::det()");
	switch(m)
	{
		case 1:
			return *matr; // matr[0]
		case 2:
			return matr[0]*matr[3] - matr[1]*matr[2];
		case 3:
			// a1b2c3 + a2b3c1 + a3b1c2  -  a3b2c1 - a2b1c3 - a1b3c2;
			return matr[0]*matr[4]*matr[8] + 
							matr[1]*matr[5]*matr[6] +
							matr[2]*matr[3]*matr[7] -
			       
							matr[2]*matr[4]*matr[6] - 
							matr[1]*matr[3]*matr[8] -
							matr[0]*matr[5]*matr[7];
	}
	// recursion (better find smth faster)
	real result = 0.0;
	for(int j=0; j<n; j++)
		result += asign[j%2] * matr[j] * minor(0, j);
	return result;
}

//-----------------------------------------------------------------//
void Matrix::inverse()
{
	real determ = det(); // this will check (!matr || (m!=n))
	if( determ==0.0 ) throw Exception("Matrix::inverse()");
	determ = 1/determ; // faster
	Matrix M(*this);
	for(int i=0; i<m; i++) // square matrix
	for(int j=0; j<m; j++)
	{
		MATR(j,i) = asign[(i+j)%2] * M.minor(i,j) * determ;
	}
//	return *this;
}

//-----------------------------------------------------------------//
//	int rang();

//-----------------------------------------------------------------//
#undef MATR
// end of file