# Arbitrary Precision Arithmetic

## Assignment 3
Arbitrary-Precision arithmetic, also known as "bignum" or simply "long arithmetic" is a set of data structures and algorithms which allows to process much greater numbers than can be fit in standard data types.
It should also work for negative numbers. You can use either sign magnitude or two's complement that you are have learnt/will learn in computer architecture course to do the assignment.

It should also support fractions (decimals).

This will be the basis for extending the codebase to support complex numbers when we do the next assignment.

Refer the pdf in *assignment 3* directory for more details.

## Assignment 4
Extends assignment 3

Refer the problem and test cases in *assignment 4* directory.

### Using user defined Libraries

In this assignment, you will use the same to implement complex arithmetic which works with numbers having arbitrary precison.

1. Create a library called arbprecision. (Reference : https://pages.hep.wisc.edu/~pinghc/generate_your_own_library.htm , https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/developer_guide/creating-libraries-gcc)

2. Implement complex number arithmetic and link arbprecision. This should support addition, subtraction, abs, product, quotient.

3. Note the difference in run time for your implementation of complex arithmetic (using arbprecision) and the standard C library implementation.

4. Plot the observations in GNUPlot by calling GNUPlot from the C file where you may need to use this.

```
FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
```
The key requirement is that a make should show the graphs/plots. 

Example C code for complex numbers is given below:

```
#include <stdio.h>      /* Standard Library of Input and Output */
#include <complex.h>    /* Standard Library of Complex Numbers */

int main() {

    double complex z1 = 1.0 + 3.0 * I;
    double complex z2 = 1.0 - 4.0 * I;

    printf("Working with complex numbers:\n\v");

    printf("Starting values: Z1 = %.2f + %.2fi\tZ2 = %.2f %+.2fi\n", creal(z1), cimag(z1), creal(z2), cimag(z2));

    double complex sum = z1 + z2;
    printf("The sum: Z1 + Z2 = %.2f %+.2fi\n", creal(sum), cimag(sum));

    double complex difference = z1 - z2;
    printf("The difference: Z1 - Z2 = %.2f %+.2fi\n", creal(difference), cimag(difference));

    double complex product = z1 * z2;
    printf("The product: Z1 x Z2 = %.2f %+.2fi\n", creal(product), cimag(product));

    double complex quotient = z1 / z2;
    printf("The quotient: Z1 / Z2 = %.2f %+.2fi\n", creal(quotient), cimag(quotient));

    double complex conjugate = conj(z1);
    printf("The conjugate of Z1 = %.2f %+.2fi\n", creal(conjugate), cimag(conjugate));

    return 0;
}
```