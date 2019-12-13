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

# cublasSgemv

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

