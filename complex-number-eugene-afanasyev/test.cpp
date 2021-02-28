#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
#include "rational/rational.h"
#include "complex/ComplexNumber.h"

using std::stringstream;

TEST_CASE("rational class init correctly", "[Rational]") {
    {
        Rational a(1, 10), b(2, 20, true), c(12, 0, false);
        REQUIRE(a.GetNumer() == 1);
        REQUIRE(a.GetDenom() == 10);
        REQUIRE(a.IsPositive() == true);
        REQUIRE(a.GetNumer() == 1);
        REQUIRE(a.GetDenom() == 10);
        REQUIRE(b.IsPositive() == true);
        REQUIRE(c.GetNumer() == 12);
        REQUIRE(c.GetDenom() == 1);
        REQUIRE(c.IsPositive() == false);
    }

    {
        Rational c = 0.5;
        REQUIRE(c == Rational(1, 2));
        Rational d = -2.5;
        REQUIRE(d == Rational(5, 2, false));
        Rational f = -0.9;
        REQUIRE(f == Rational(9, 10, false));
    }

    {
        Rational g = 1;
        REQUIRE(g == Rational(1, 1));
        Rational h = 0;
        REQUIRE(h == Rational(0, 1));
    }
}

TEST_CASE("copy constructor works correctly", "[Rational]") {
    Rational a(1, 12), b(10, 11, false);
    a = b;
    REQUIRE((a = b) == Rational(10, 11, false));
    REQUIRE(b == Rational(10, 11, false));
}

TEST_CASE("rational numbers compares correctly", "[Rational::operator==]") {
    {
        REQUIRE(Rational(10, 1) == Rational(10, 1));
        REQUIRE(Rational(10, 2) == Rational(500, 100));
        REQUIRE(!(Rational(500, 100, false) == Rational(500, 100)));
        REQUIRE(!(Rational(500, 100, false) == Rational(500, 100)));
        REQUIRE(!(Rational(10, 2, false) == Rational(500, 100, true)));
        REQUIRE(Rational(10, 2, false) == Rational(500, 100, false));
        REQUIRE(Rational(10, 2) == Rational(500, 100, false).abs());
    }
    {
        REQUIRE(!(Rational(10, 1) > Rational(10, 1)));
        REQUIRE(Rational(9, 1) < Rational(10, 1));
        REQUIRE(Rational(9, 1) > Rational (10, 1, false));
        REQUIRE(Rational(1, 1) > Rational(1, 10));
        REQUIRE(Rational(10, 15, false) < Rational(9, 15, false));
    }
}

TEST_CASE("rational numbers add up correctly", "[Rational]") {
    {
        Rational a(1, 10), b(1, 10);
        REQUIRE(a + b == Rational(2, 10));
        REQUIRE(a + a == Rational(2, 10));
    }
    {
        REQUIRE(Rational(1, 5) + Rational(1, 2) == Rational(7, 10));
        REQUIRE(Rational(10, 1) + Rational(100, 10) == Rational(20, 1));
        REQUIRE(Rational(3, 4) + Rational(2, 4, false) == Rational(1, 4));
        REQUIRE(Rational(3, 4) + Rational(5, 4, false) == Rational(1, 2, false));
        REQUIRE(Rational(3, 4, false) + Rational(2, 4, false) == Rational(5, 4, false));
        REQUIRE(Rational(3, 4, false) + Rational(2, 4) == Rational(1, 4, false));
        REQUIRE(Rational(1, 4, false) + Rational(2, 4) == Rational(1, 4));
    }
}

TEST_CASE("rational numbers are subtracted correctly", "[Rational]") {
    {
        Rational a(2, 5), b(3, 5),
            c(1, 5, false);
        REQUIRE(a - b == Rational(1, 5, false));
        REQUIRE(a - a == Rational(0, 1));
        REQUIRE(a - c - b == Rational(0, 5));
        REQUIRE(b - c == Rational(4, 5, true));
    }
}

TEST_CASE("rational numbers are multiplied correctly", "[Rational]") {
    REQUIRE(Rational(1, 2) * Rational(1, 2) == Rational(1,4));
    REQUIRE(Rational(1, 2) * Rational(1, 2, false) == Rational(1, 4, false));
    REQUIRE(Rational(1, 3, false) * Rational(3, 4, false) == Rational(1, 4));
    REQUIRE(Rational(5, 10,false) * Rational(3, 2) == Rational(3, 4, false));
}

TEST_CASE("rational numbers are divided correctly", "[Rational]") {
    REQUIRE(Rational(1, 2) / Rational(1, 2) == Rational(1, 1));
    REQUIRE(Rational(3, 2) / Rational(1, 2, false) == Rational(3, 1, false));
    REQUIRE(Rational(1, 2, false) / Rational(2, 2, false) == Rational(1, 2));
}


TEST_CASE("unary minus works correctly", "[Rational]") {
    REQUIRE(-Rational(1, 2) == Rational(1, 2, false));
    REQUIRE(-Rational(1, 2, false) == Rational(1, 2, true));
}

TEST_CASE("rational numbers convert to double correctly") {
    REQUIRE(Rational(1, 1).ToDouble() == 1.0);
    REQUIRE(Rational(1, 2, false) == -0.5);
    REQUIRE(Rational(0).ToDouble() == 0);
    REQUIRE(Rational(4, 5) == 0.8);
    REQUIRE(Rational(650, 1000) == 0.65);
    REQUIRE(Rational(6, 3) == 2.0);
    REQUIRE(Rational(13, 2) == 6.5);
}

TEST_CASE("standard stream works correctly with rational numbers") {
    Rational a(2, 5), b(10, 20, false);
    std::stringstream ss;
    ss << a;
    REQUIRE(ss.str() == "(2/5)");
    ss << b;
    REQUIRE(ss.str() == "(2/5)-(1/2)");
    ss.str(std::string());
    ss << b;
    REQUIRE(ss.str() == "-(1/2)");

    ss.str(std::string());
    Rational c(12, 1), d(10, 1, false);
    ss << c;
    REQUIRE(ss.str() == "12");

    ss.str(std::string());
    ss << d;
    REQUIRE(ss.str() == "-10");
}

TEST_CASE("gcd function works correctly") {
    REQUIRE(gcd(10, 120) == 10);
    REQUIRE(gcd(0,0) == 0);
    REQUIRE(gcd(1, 12) == 1);
    REQUIRE(gcd(32, 6) == 2);
    REQUIRE(gcd(3, 33) == 3);
}

TEST_CASE("the exponentiation function is working correctly") {
    Rational a(2, 3), b(1, 4), c(3, 2, false);
    REQUIRE(exp(a, 2) == Rational(4, 9));
    REQUIRE(exp(b, 3) == Rational(1, 64));
    REQUIRE(exp(c, 2) == Rational(9, 4, true));
    REQUIRE(exp(c, 3) == Rational(27, 8, false));
    REQUIRE_FALSE(exp(c, 1) == Rational(3, 2));
    REQUIRE(exp(c, 0) == 1);
}

TEST_CASE("root calculating correctly on rational numbers") {
    Rational a(144, 256), b(1, 16), c(4096, 625);
    REQUIRE(a.sqrt() == 0.75);
    REQUIRE(b.sqrt() == 0.25);
    REQUIRE(c.sqrt() == 2.56);
}

TEST_CASE("complex numbers init correctly") {
    ComplexNumber a(1, 3);
    REQUIRE(a.GetRealPart() == 1);
    REQUIRE(a.GetImaginaryPart() == 3);

    ComplexNumber b(10);
    REQUIRE(b.GetRealPart() == 10);
    REQUIRE(b.GetImaginaryPart() == 0);

    ComplexNumber c(2.1);
    REQUIRE(c.GetRealPart() == 2.1);
    REQUIRE(c.GetImaginaryPart() == 0);

    ComplexNumber d;
    REQUIRE(d.GetRealPart() == 0);
    REQUIRE(d.GetImaginaryPart() == 0);
}

TEST_CASE("ComplexNumber class setters works correctly") {
    ComplexNumber a(10, 1);
    a.SetRealPart(2);
    a.SetImaginaryPart(-1);
    REQUIRE(a.GetRealPart() == 2);
    REQUIRE(a.GetImaginaryPart() == -1);
}

TEST_CASE("complex nums copy constructor works correctly") {
    ComplexNumber b(-20);
    ComplexNumber a(b);
    a = b;
    REQUIRE(a == ComplexNumber(-20));
}

TEST_CASE("complex number compares correctly") {
    {
        REQUIRE(ComplexNumber(0, 1) == ComplexNumber(0, 1));
        REQUIRE(ComplexNumber(100, 200) == ComplexNumber(Rational(100, 1), 200));

        ComplexNumber a = 1.2;
        REQUIRE(a == ComplexNumber(1.2, 0));
        ComplexNumber b = -3.4;
        REQUIRE(b == ComplexNumber(-3.4, 0));
        REQUIRE(ComplexNumber(0) == ComplexNumber(0));
        REQUIRE(ComplexNumber(1) == ComplexNumber(1, 0));
    }

    {
        REQUIRE(ComplexNumber(1) != ComplexNumber(0, 1));
        REQUIRE(ComplexNumber(2) != ComplexNumber(2, 1));
        REQUIRE(ComplexNumber(10, 1) != ComplexNumber(10, 0));
        REQUIRE(ComplexNumber(20, 20) != ComplexNumber(20, -20));
        REQUIRE(ComplexNumber(-20, -20) != ComplexNumber(20, -20));
    }
}

TEST_CASE("complex numbers add up correctly") {

    {
        ComplexNumber a(11, 2), b(-2.5, -12), c(9.432, -100.7);
        REQUIRE(a + b == ComplexNumber(8.5, -10));
        REQUIRE(a + c == ComplexNumber(20.43200, -98.7));
        REQUIRE(a + b + c == ComplexNumber(17.93200, -110.7));
        REQUIRE(b + c == ComplexNumber(6.932, -112.7));
        REQUIRE(ComplexNumber(0) + ComplexNumber(0) == ComplexNumber(0, 0));
    }
}

TEST_CASE("complex numbers subtracted correctly") {
    {
        ComplexNumber a(10, 0.4), b(90.3, -71), c(-3, -1);
        REQUIRE(a - b == ComplexNumber(-80.3, 71.4));
        REQUIRE(a - c == ComplexNumber(13, 1.4));
        REQUIRE(c - b == ComplexNumber(-93.3, 70));
        REQUIRE(c - a == ComplexNumber(-13, -1.4));
        REQUIRE(b - c == ComplexNumber(93.3, -70));
        REQUIRE(b - a == ComplexNumber(80.3, -71.4));
        REQUIRE(a - b - c == ComplexNumber(-77.3, 72.4));
        REQUIRE(ComplexNumber(0) - ComplexNumber(0) == ComplexNumber(0, 0));
    }
}

TEST_CASE("complex numbers multiplied correctly") {
    {
        ComplexNumber a(1, -4), b(1.5, 1.2), c(-10.1, -4);
        REQUIRE(a * b == ComplexNumber(6.3, -4.8));
        REQUIRE(a * c == ComplexNumber(-26.1, 36.4));
        REQUIRE(b * c == ComplexNumber(-10.35, -18.12));
        REQUIRE(a * b * c == ComplexNumber(-82.83, 23.28));
    }

    {
        ComplexNumber a(2), b(11, -7), c(-0.5, 10);
        REQUIRE(a * b == ComplexNumber(22, -14));
        REQUIRE(a * c == ComplexNumber(-1, 20));
        REQUIRE(b * c == ComplexNumber(64.5, 113.5));
        REQUIRE(a * b * c == ComplexNumber(129, 227));
    }
}

TEST_CASE("complex numbers are divided correctly") {
    {
        ComplexNumber a(-2, 1), b (1, -1);
        REQUIRE(a / b == ComplexNumber(-1.5, -0.5));
        REQUIRE(b / a == ComplexNumber(-0.6, 0.2));
    }

    {
        ComplexNumber a(-1, 3), b(1, 2);
        REQUIRE(a / b == ComplexNumber(1, 1));
        REQUIRE(b / a == ComplexNumber(0.5, -0.5));
    }

    {
        ComplexNumber a(2, 5), b(3, -2);
        REQUIRE(a / b == ComplexNumber(
                -Rational(4, 13), Rational(19, 13)));
        REQUIRE(b / a == ComplexNumber(-Rational(4, 29), -Rational(19, 29)));
        REQUIRE(b / 1 == b);
    }
}

TEST_CASE("getters and setters works correctly") {
    ComplexNumber a(10, 2), b(-1, 1);
    a.SetRealPart(b.GetRealPart());
    b.SetRealPart(3);
    REQUIRE(a == ComplexNumber(-1, 2));
    REQUIRE(b == ComplexNumber(3, 1));
    REQUIRE(a.GetImaginaryPart() == 2);
    REQUIRE(b.GetImaginaryPart() == 1);
}

TEST_CASE("assigning operators works correctly", "[ComplexNumber]") {
    //  assigning
    {
        ComplexNumber a(10, 11), b(20, -1.24), c(1, 4);
        a = b = c;
        REQUIRE(b == ComplexNumber(1, 4));
        REQUIRE(a == ComplexNumber(1, 4));
    }

    //  add up
    {
        ComplexNumber a(10, 11), b(1, -2);
        REQUIRE((a += b) == ComplexNumber(11, 9));
        REQUIRE((b += a) == ComplexNumber(12, 7));
    }

    {
        ComplexNumber a(4, -4), b(1, -3), c(1, 2);
        a += b += c;
        REQUIRE(b == ComplexNumber(2, -1));
        REQUIRE(a == ComplexNumber(6, -5));
    }

    //  subtracting
    {
        ComplexNumber a(1, 2), b(3, -3), c(-2, 1);
        a -= b -= c;
        REQUIRE(b == ComplexNumber(5, -4));
        REQUIRE(a == ComplexNumber(-4, 6));

        c -= a -= b;
        REQUIRE(a == ComplexNumber(-9, 10));
        REQUIRE(c == ComplexNumber(7, -9));
    }

    //  multiplying
    {
        ComplexNumber a(2, -4), b(10, -7), c(-19, 2);
        a *= b *= c;
        REQUIRE(b == ComplexNumber(-176, 153));
        REQUIRE(a == ComplexNumber(260, 1010));

        c *= a *= ComplexNumber(0.01, 0.002);
        REQUIRE(a == ComplexNumber(0.58, 10.62));
        REQUIRE(c == ComplexNumber(-32.26, -200.62));
    }

    // dividing
    {
        ComplexNumber a(1, -4), b(2, -2), c(2, 1);
        a /= c /= b;
        REQUIRE(c == ComplexNumber(0.25, 0.75));
        REQUIRE(a == ComplexNumber(-4.4, -2.8));
        c /= b /= a;
        REQUIRE(b == ComplexNumber(-Rational(2, 17), Rational(9, 17)));
        REQUIRE(c == ComplexNumber(1.25, -0.75));
    }
}

TEST_CASE("abs and arg functions work correctly") {
    {
        ComplexNumber a(1, -2), b(4, 3), c(-2, -5);
        REQUIRE(a.abs() == std::sqrt(5));
        REQUIRE(b.abs() == 5);
        REQUIRE(c.abs() == std::sqrt(29));
        REQUIRE(a.arg() == std::atan(-2 / 1));
        REQUIRE(b.arg() == std::atan(3.0 / 4.0));
        REQUIRE(c.arg() == std::atan(-5.0 / -2.0));
    }
}

TEST_CASE("complex numbers are raised to natural power correctly") {
    REQUIRE(ComplexNumber(1, -2).pow(2) == ComplexNumber(-3, -4));
    REQUIRE(ComplexNumber(1, -2).pow(20) == ComplexNumber(-9653287, 1476984));
    REQUIRE(ComplexNumber(-4, -7).pow(10) == ComplexNumber(-534842913, -1029668584));
    REQUIRE(ComplexNumber(3, -3).pow(9) == ComplexNumber(314928, -314928));
    REQUIRE(ComplexNumber(21, 12).pow(1) == ComplexNumber(21, 12));
    REQUIRE(ComplexNumber(3, 2).pow(0) == 1);
}

TEST_CASE("unary minus operator works correctly") {
    REQUIRE(ComplexNumber(3, 4) == -ComplexNumber(-3, -4));
    ComplexNumber a(-1, 32);
    REQUIRE(ComplexNumber(1, -2) == -ComplexNumber(-1, 2));
    REQUIRE(ComplexNumber(0) == -ComplexNumber(0));
    REQUIRE(-ComplexNumber(12, -2) == -ComplexNumber(12, -2));
}

TEST_CASE("standard stream works correctly with complex numbers") {
    ComplexNumber a(2, 5), b(0.52, 0.13), c(-1), d(0, -2), f(3, -1.2);
    std::stringstream ss;

    ss << a;
    REQUIRE(ss.str() == "2+5i");
    ss.str(std::string());

    ss << b;
    REQUIRE(ss.str() == "(13/25)+(13/100)i");
    ss.str(std::string());

    ss << c;
    REQUIRE(ss.str() == "-1");
    ss.str(std::string());

    ss << d;
    REQUIRE(ss.str() == "-2i");
    ss.str(std::string());

    ss << f;
    REQUIRE(ss.str() == "3-(6/5)i");
}

TEST_CASE("implicit integer numbers converting in expressions with complex numbers") {
    {
        ComplexNumber a(1, -4), b(2, -2);
        REQUIRE((a + 1 - 8) * 0.5 + b == ComplexNumber(-1, -4));
    }
}
