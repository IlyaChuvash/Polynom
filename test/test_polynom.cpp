#include "gtest.h"

#include "polynomial.h"

TEST(Polynomial, AddSubMerge_O_n) {
    Polynomial a;
    a.add_monom(2, 1, 0, 0); // 2x
    a.add_monom(3, 0, 1, 0); // 3y

    Polynomial b;
    b.add_monom(5, 1, 0, 0);  // 5x
    b.add_monom(-3, 0, 1, 0); // -3y
    b.add_monom(7, 0, 0, 2);  // 7z^2

    auto sum = a + b;
    EXPECT_EQ(sum.to_string(), "7*x + 7*z^2");

    auto diff = a - b;
    EXPECT_EQ(diff.to_string(), "-3*x + 6*y - 7*z^2");
}

TEST(Polynomial, MulConst_O_n) {
    Polynomial p;
    p.add_monom(1, 2, 0, 0); // x^2
    p.add_monom(-2, 0, 0, 1); // -2z

    auto q = p * 3;
    EXPECT_EQ(q.to_string(), "3*x^2 - 6*z");
}

TEST(Polynomial, MulPoly_O_n2) {
    Polynomial a;
    a.add_monom(1, 1, 0, 0); // x
    a.add_monom(1, 0, 1, 0); // y

    Polynomial b;
    b.add_monom(1, 1, 0, 0); // x
    b.add_monom(-1, 0, 1, 0); // -y

    auto prod = a * b; // (x+y)(x-y) = x^2 - y^2
    EXPECT_EQ(prod.to_string(), "x^2 - y^2");
}

TEST(Polynomial, CombineLikeTermsAndDropZero) {
    Polynomial p;
    p.add_monom(2, 1, 2, 3);
    p.add_monom(-2, 1, 2, 3);
    EXPECT_EQ(p.to_string(), "0");
}

TEST(Polynomial, ZeroPolynomialOperations) {
    Polynomial zero;
    Polynomial p;
    p.add_monom(3, 2, 0, 0); // 3x^2
    auto sum = zero + p;
    EXPECT_EQ(sum.to_string(), "3*x^2");
    auto sum2 = p + zero;
    EXPECT_EQ(sum2.to_string(), "3*x^2");
    auto prod = zero * p;
    EXPECT_EQ(prod.to_string(), "0");
    auto prod2 = p * zero;
    EXPECT_EQ(prod2.to_string(), "0");
    auto diff = p - zero;
    EXPECT_EQ(diff.to_string(), "3*x^2");
    auto diff2 = zero - p;
    EXPECT_EQ(diff2.to_string(), "-3*x^2");
}

TEST(Polynomial, ComplexMultiplication) {
    // (x^2 + y) * (x - z) = x^3 - x^2 z + xy - yz
    Polynomial p1;
    p1.add_monom(1, 2, 0, 0); // x^2
    p1.add_monom(1, 0, 1, 0); // y

    Polynomial p2;
    p2.add_monom(1, 1, 0, 0); // x
    p2.add_monom(-1, 0, 0, 1); // -z

    auto res = p1 * p2;
    EXPECT_EQ(res.to_string(), "x^3 - x^2*z + x*y - y*z");
}

TEST(Polynomial, TestSortingOrder) {
    Polynomial p;
    p.add_monom(1, 0, 0, 1); // z
    p.add_monom(1, 1, 0, 0); // x
    p.add_monom(1, 0, 1, 0); // y

    // Based on operator<: x (i=1) > y (i=0, j=1) > z (i=0, j=0, k=1)
    // Should be printed as x + y + z
    EXPECT_EQ(p.to_string(), "x + y + z");
}

TEST(Polynomial, HighPowers) {
    Polynomial p;
    p.add_monom(2.5, 10, 5, 2);
    p.add_monom(-1.0, 10, 5, 2);

    // 2.5 - 1.0 = 1.5
    EXPECT_EQ(p.to_string(), "1.5*x^10*y^5*z^2");
}

TEST(Polynomial, SelfOperations) {
    Polynomial p;
    p.add_monom(1, 1, 1, 1); // xyz
    // p - p = 0
    auto zero = p - p;
    EXPECT_EQ(zero.to_string(), "0");
    // p + p = 2xyz
    auto dou = p + p;
    EXPECT_EQ(dou.to_string(), "2*x*y*z");
}
