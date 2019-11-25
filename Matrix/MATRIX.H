#ifndef MATRIX_H__
#define MATRIX_H__

// forward declarations of I/O classes
class istream;
class ostream;

#ifndef real
	#define real double
#endif

// the Matrix: m-rows, n-columns
class Matrix
{
public:
	~Matrix();
	Matrix();
	Matrix(int _m, int _n);
	Matrix(int _m, int _n, real *array);
	Matrix(Matrix&);

	// assignment
	Matrix& operator =(Matrix&); 
	// operations
	real* operator [](int row); 
	int operator ==(Matrix&);
	int operator !=(Matrix&);
	Matrix& operator -();
	Matrix& operator+=(Matrix&);
	Matrix& operator-=(Matrix&);
	Matrix& operator*=(real f);
	Matrix& operator/=(real f);
	Matrix& operator*=(Matrix&);
	friend Matrix operator +(Matrix&m1, Matrix& m2);
	friend Matrix operator -(Matrix&m1, Matrix& m2);
	friend Matrix operator *(Matrix&m1, Matrix& m2);
	friend Matrix operator *(Matrix&,   real f);
	friend Matrix operator *(real f,   Matrix&);
	friend Matrix operator /(Matrix&,   real f);

	// binary I/O mode 
	void read (istream& is);
	void write(ostream& os);
	// text I/O mode
	friend istream& operator >>(istream& is, Matrix &M);
	friend ostream& operator <<(ostream& os, Matrix &M);

	void transpose();
	Matrix submatrix(int i, int j);
	real minor(int i, int j);
	real det();
	void inverse();
	//int rang();        // --------
	void LoadIdentity();


	struct Exception
	{
		Exception(char *why);
		char str[80];
		operator const char*() { return str; }
	};

private:
	int m, n;
	real *matr;
	bool equal(Matrix&);
	bool check_size(Matrix& M);
};

//-----------------------------------------------------------------//
inline Matrix::~Matrix()                   { delete[] matr; }
inline Matrix::Matrix()                    { m=n=0; matr = 0L; }
inline real* Matrix::operator [](int row)  { return (matr+row*n); }
inline int Matrix::operator ==(Matrix& M)  { return equal(M); }
inline int Matrix::operator !=(Matrix& M)  { return !equal(M); }
inline bool Matrix::check_size(Matrix& M)  { return (M.m==m && M.n==n); }
inline real Matrix::minor(int i, int j)    { return submatrix(i, j).det(); }


#endif // end of file