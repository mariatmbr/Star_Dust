// Timbur Maria 313CA
#include <stdio.h>
#include <stdlib.h>
// returnez un integer cu semn pozitiv
int make_signed(int x) {
    return x > 127 ? -(256-x) : x;
}
// returnez un integer unsigned,care poate fi pozitiv/negativ
int make_unsigned(int x) {
    return x < 0 ? 256 + x : x;
}
// returnez valoare hexazecimala
char to_hex(int x) {
    if (x <= 9) return '0' + x;
    return 'A' + x - 10;
}
// returnez valoare int a unui char
int from_hex(char c) {
    if (c >= '0' && c <= '9') {
        return c-'0';
    }
    return c-'A' + 10;
}
void transform(char *s, char *a) {
    for (int i = 1; i <= 4; i++) {
        a[i-1] = make_signed(from_hex(s[8 - 2*i])*16 +
        from_hex(s[8 - 2*i + 1]));
    }
}
// modific valoarea elementului dorit
void do_modify(int dim, char *arr, char *val) {
    for (int i = 0; i < dim; i++) {
        arr[i] = val[i];
    }
}
// sterg elementul si inlocuiesc 0
void do_delete(int dim, char *arr) {
    for (int i = 0; i < dim; i++) {
        arr[i] = 0;
    }
}
// fac swap pe linia si blocul dorit
void do_swap(int dim, char *arr) {
        for (int i = 0; i < dim/2; i++) {
        char aux = arr[i];
        arr[i] = arr[dim-i-1];
        arr[dim-i-1] = aux;
    }
}
// printez vectorul de caractere
void print_val(char* val) {
    char s[9];
    s[8] = '\0';
    for (int i = 0; i < 4; i++) {
        int num = make_unsigned(val[3 - i]);
        s[2*i] = to_hex(num/16);
        s[2*i+1] = to_hex(num%16);
    }
    printf("%s ", s);
}
// citesc sirul de caractere
void read_val(char *val) {
    char s[9];
    scanf("%s", s);
    for (int i = 0; i < 4; i++) val[i] = 0;
    transform(s, val);
}
// algoritmul simplificat al lui Lee
void Lee(char** viz, int N, int *M, int* CX, int* CY, int i,
int j, int* kg, int* mig, int* mjg) {
    if (viz[i][j]) return;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int st = 0, dr = 0, k = 0, mi = i, mj = j;
    CX[0] = i;
    CY[0] = j;
    viz[i][j] = 1;
    while (st <= dr) {
        int cx = CX[st];
        int cy = CY[st];
        k++;
        if (cx < mi || (cx == mi && cy < mj)) {
            mi = cx;
            mj = cy;
        }
        for (int d = 0; d < 4; d++) {
            int x = cx + dx[d];
            int y = cy + dy[d];
            if (x >= 0 && y >= 0 && x < N && y < M[x] && !viz[x][y]) {
                viz[x][y] = 1; dr++;
                CX[dr] = x; CY[dr] = y;
            }
        }
        st++;
    }
    if (*kg < k || (*kg == k && (mi < *mig || (mi == *mig && mj < *mjg)))) {
        *kg = k; *mig = mi; *mjg = mj;
    }
}
int main() {
    char **A, **V; int N, R, *M; scanf("%d", &N);
    A = (char**) calloc (N, sizeof(char *));
    M = (int*) calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        scanf("%d", &M[i]);
        A[i] = (char*) calloc (4*M[i], sizeof(char));
        for (int j = 0; j < M[i]; j++) {
            char s[9]; scanf("%s", s);
            transform(s, A[i]+4*j);
        }
        M[i]*=4;
    }
    int sum = 0, nr = 0;
    for (int i = 0; i < N; i++) {
       for (int j = 0; j < M[i]; j++) {
            if (i == 0 || i == N - 1 || j == M[i] - 1 || j == 0) {
                sum += A[i][j]; nr++;
            }
        }
    }
    printf("task 1\n");
    // inmultesc cu 1.0 pentru ca sum e de tip int
    printf("%.8lf\n", 1.0*sum/nr);
    scanf("%d", &R);
    // pana cand nu mai sunt operatii de facut
    while (R--) {
        char tip_op, tip_val, val[4];
        int i, j, dim;
        scanf(" %c", &tip_op); scanf(" %c", &tip_val);
        scanf("%d", &i); scanf("%d", &j);
        if (tip_val == 'C') dim = 1;
        else if (tip_val == 'S') dim = 2;
        else if (tip_val == 'I') dim = 4;
        if (tip_op == 'M') {
            read_val(val); j--; j *= dim;
            if (j + dim >= M[i]) {
                A[i] = realloc(A[i], (j + dim) * sizeof(char));
                for (int k = M[i]; k < (j + dim); k++) A[i][k] = 0;
                M[i] = (j + dim);
            }
            do_modify(dim, &A[i][j], val);
        } else if (tip_op == 'D') {
            j--; j*=dim;
            if (j + dim >= M[i]) {
                // realoc memorie pentru acest caz
                A[i] = realloc(A[i], (j + dim) * sizeof(char));
                for (int k = M[i]; k < (j + dim); k++) A[i][k] = 0;
                M[i] = (j + dim);
            }
            do_delete(dim, &A[i][j]);
        } else if (tip_op == 'S') {
            j*=dim; do_swap(dim, &A[i][j]);
        }
    }
    printf("task 2\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M[i]; j += 4) {
            print_val(&A[i][j]);
        }
        printf("\n");
    }
    printf("task 3\n");
    int c_size = 0, kg = 0, mig = 1e9, mjg = 1e9;
    V = (char**) calloc (N, sizeof(char *));
    for (int i = 0; i < N; i++) {
        V[i] = (char*) calloc(M[i], sizeof(char));
        for (int j = 0; j < M[i]; j++) {
            V[i][j] = A[i][j] != 0;
        }
        c_size += M[i];
    }
    // coada care retine valorile
    int* CX = calloc(c_size, sizeof(int));
    int* CY = calloc(c_size, sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M[i]; j++) {
            Lee(V, N, M, CX, CY, i, j, &kg, &mig, &mjg);
        }
    }
    // mig este i minim, mjg este j minim
    printf("%d %d %d", mig, mjg, kg);
    // eliberam memoria alocata prin free
    for (int i = 0; i < N; i++) {
        free(A[i]); free(V[i]);
    }
    free(A); free(V); free(M); free(CX); free(CY);
}

