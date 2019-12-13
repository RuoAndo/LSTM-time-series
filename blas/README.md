# cublasSgemv

<pre>
cublasStatus_t cublasSgemv(cublasHandle_t handle, cublasOperation_t trans,
                           int m, int n,
                           const float           *alpha,
                           const float           *A, int lda,
                           const float           *x, int incx,
                           const float           *beta,
                           float           *y, int incy)
</pre>

y = α op ( A ) x + β y

<pre>
 66    int M = 100;
 67    int N = 5;
 
 89    cublasSgemv(handle, CUBLAS_OP_N, M, N, &alpha, dA, M, dX, 1, &beta, dY, 1);
</pre>

<img src="cublasSgemv-1.png" width=70% height=70%>

<pre>
# nvcc cublasSgemv_1.cu -lcublas
</pre>

<pre>
# ./a.out
6590.10
10041.88
17133.61
12606.56
7975.49
11884.11
14154.83
13064.94
14770.13
9690.33
...
elapsed time = 0.681640245 sec
</pre>

# cublasSgemm

<pre>
cublasStatus_t cublasSgemm(cublasHandle_t handle,
                           cublasOperation_t transa, cublasOperation_t transb,
                           int m, int n, int k,
                           const float           *alpha,
                           const float           *A, int lda,
                           const float           *B, int ldb,
                           const float           *beta,
                           float           *C, int ldc)
</pre>

<pre>
C = α op ( A ) op ( B ) + β C
</pre>

<pre>
8 int const A_ROW = 5;
9 int const A_COL = 6;
10 int const B_ROW = 6;
11 int const B_COL = 7;       
</pre>

<pre>
  float a = 1, b = 0;
  cublasSgemm(
          handle,
          CUBLAS_OP_T,   
          CUBLAS_OP_T,  
          A_ROW,   // 5
          B_COL,   // 6      
          A_COL,   // 5      
          &a,           
          d_A,          
          A_COL,   // 5      
          d_B,          
          B_COL,   // 6     
          &b,           
          d_C,     // 7     
          A_ROW         
  );
</pre>

<pre>
cublasStatus_t cublasSgemm(cublasHandle_t handle,
                           cublasOperation_t transa, cublasOperation_t transb,
                           int m, int n, int k,
                           const float           *alpha,
                           const float           *A, int lda,
                           const float           *B, int ldb,
                           const float           *beta,
                           float           *C, int ldc)

C = α op ( A ) op ( B ) + β C

handle: handle to the cuBLAS library context.

transa: operation op(A) that is non- or (conj.) transpose.
transb: operation op(B) that is non- or (conj.) transpose.

m: number of rows of matrix op(A) and C.
n: number of columns of matrix op(B) and C.

k: number of columns of op(A) and rows of op(B).

alpha: scalar used for multiplication.
A: array of dimensions lda x k with lda>=max(1,m) if transa == CUBLAS_OP_N and lda x m with lda>=max(1,k) otherwise.

lda: leading dimension of two-dimensional array used to store the matrix A.
B: array of dimension ldb x n with ldb>=max(1,k) if transb == CUBLAS_OP_N and ldb x k with ldb>=max(1,n) otherwise.

ldb: leading dimension of two-dimensional array used to store matrix B.
beta: scalar used for multiplication. If beta==0, C does not have to be a valid input.

C: rray of dimensions ldc x n with ldc>=max(1,m).
ldc: leading dimension of a two-dimensional array used to store the matrix C.
</pre>

<pre>
# ./a.out

A : row 5: col 6
4 7 8 6 4 6
7 3 10 2 3 8
1 10 4 7 1 7
3 7 2 9 8 10
3 1 3 4 8 6

B : row 6: col 5
 10 3 3 9 10 8 4
 7 2 3 10 4 2 10
 5 8 9 5 6 1 4
 7 2 1 7 4 3 1
 7 2 6 6 5 8 7
 6 7 10 4 8 5 6

A*B :row 5:col 7
[GPU] 0.000061689 sec
235 224 198 268 172 152 173
120 143 101 195 220 152 205
151 236 207 212 258 152 208
229 163 226 156 134 142 96
181 135 188 169 176 215 130
</pre>

