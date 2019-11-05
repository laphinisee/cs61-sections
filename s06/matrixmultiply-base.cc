#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cinttypes>
#include <cmath>
#include <getopt.h>
#include <cassert>
#include <time.h>

// sqmatrix -- type representing square matrices
struct sqmatrix {
    double* v;    // pointer to matrix elements, in row-major order
    size_t sz;    // matrix dimension

    // sqmatrix(size) constructor
    //    Initialize a `size * size` matrix.
    sqmatrix(size_t size)
        : v(new double[size * size]), sz(size) {
    }
    // ~sqmatrix() destructor
    //    Free memory allocated for elements.
    ~sqmatrix() {
        delete[] v;
    }
    sqmatrix(const sqmatrix&) = delete;
    sqmatrix& operator=(const sqmatrix&) = delete;


    // size()
    //    Return the matrix dimension.
    size_t size() {
        return sz;
    }

    // at(i, j)
    //    Return the matrix element in row `i` and column `j`.
    //    Call like this:
    //    ```
    //    sqmatrix m(10);      // 10 * 10 matrix
    //    m.at(1, 2) = 100.0;  // sets element 1,2 to 100.0
    //    ```
    //    Returns a reference type, which means you can *assign*
    //    the return value if you want.
    double& at(size_t i, size_t j) {
        return v[i * sz + j];
    }

    // operator()(i, j)
    //    Synonym for `at(i, j)`, but easier to type.
    double& operator()(size_t i, size_t j) {
        return at(i, j);
    }

    // transpose()
    //    Transpose this matrix in place.
    void transpose() {
        for (size_t i = 0; i != sz; ++i) {
            for (size_t j = 0; j != i; ++j) {
                std::swap(at(i, j), at(j, i));
            }
        }
    }

    // statistics()
    //    Return some statistics about the matrix.
    struct statistics_type {
        double corner[4];
        double diagonal_sum;
    };
    statistics_type statistics();
};



// matrix_multiply(c, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `sz`.
//    Computes the matrix product `a * b` and stores it in `c`.
void matrix_multiply(sqmatrix& c, sqmatrix& a, sqmatrix& b) {
    assert(c.size() == a.size() && c.size() == b.size());

    // clear `c`
    for (size_t i = 0; i < c.size(); ++i) {
        for (size_t j = 0; j < c.size(); ++j) {
            c.at(i, j) = 0;
        }
    }

    // compute product and update `c`
    for (size_t i = 0; i < c.size(); ++i) {
        for (size_t j = 0; j < c.size(); ++j) {
            for (size_t k = 0; k < c.size(); ++k) {
                c.at(i, j) += a.at(i, k) * b.at(k, j);
            }
        }
    }
}


// timestamp()
//    Return the current time.
double timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + 1e-9 * ts.tv_nsec;
}


// xrandom()
//    Return a pseudo-random number in the range [0, XRAND_MAX].
//    We use our own generator to ensure values computed on different
//    OSes will follow the same sequence.
#define XRAND_MAX 0x7FFFFFFF
static uint64_t xrandom_seed;
unsigned xrandom() {
    xrandom_seed = xrandom_seed * 6364136223846793005U + 1U;
    return (xrandom_seed >> 32) & XRAND_MAX;
}


// compute_statistics(m, sz)
//    Compute and return some statistics about matrix `m`.
sqmatrix::statistics_type sqmatrix::statistics() {
    statistics_type mstat;
    mstat.corner[0] = at(0, 0);
    mstat.corner[1] = at(0, size() - 1);
    mstat.corner[2] = at(size() - 1, 0);
    mstat.corner[3] = at(size() - 1, size() - 1);
    mstat.diagonal_sum = 0;
    for (size_t i = 0; i != size(); ++i) {
        mstat.diagonal_sum += at(i, i);
    }
    return mstat;
}


int main(int argc, char* argv[]) {
    size_t sz = 1000;
    int has_seed = 0;

    // read options
    int opt;
    while ((opt = getopt(argc, argv, "n:d:")) != -1) {
        switch (opt) {
        case 'n':
            sz = strtoul(optarg, nullptr, 0);
            break;
        case 'd':
            xrandom_seed = strtoul(optarg, nullptr, 0);
            has_seed = 1;
            break;
        default:
            fprintf(stderr, "Usage: ./matrixmultiply [-n SIZE] [-d SEED]\n");
            exit(EXIT_FAILURE);
        }
    }

    assert(sz > 0);
    assert(sz < (size_t) sqrt(SIZE_MAX / sizeof(double)));
    if (!has_seed) {
        union { double d; uint64_t x; } u;
        u.d = timestamp();
        xrandom_seed = u.x;
    }
    printf("size %zu, seed %" PRIu64 "\n", sz, xrandom_seed);

    // allocate matrices
    sqmatrix a(sz);
    sqmatrix b(sz);
    sqmatrix c(sz);

    // fill in source matrices
    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = 0; j < sz; ++j) {
            a(i, j) = xrandom() / (double) XRAND_MAX;
        }
    }

    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = 0; j < sz; ++j) {
            b(i, j) = xrandom() / (double) XRAND_MAX;
        }
    }

    // compute `c = a * b`
    double t0 = timestamp();
    matrix_multiply(c, a, b);
    double t1 = timestamp();

    // compute times, print times and ratio
    printf("multiply time %.09f\n", t1 - t0);

    // print statistics and differences
    sqmatrix::statistics_type mstat = c.statistics();
    for (int i = 0; i < 4; ++i) {
        printf("corner statistic %d: %g (%a)\n",
               i, mstat.corner[i], mstat.corner[i]);
    }
    printf("diagonal sum statistic: %g (%a)\n",
           mstat.diagonal_sum, mstat.diagonal_sum);
}
