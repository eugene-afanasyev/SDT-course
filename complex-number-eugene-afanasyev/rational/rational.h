#ifndef __RATIONAL_H__
#define __RATIONAL_H__


#include <cstdint>
#include <iostream>
#include <cstdlib>

class Rational {
 public:
    explicit Rational(uint32_t num = 0, uint32_t denom = 1, bool is_positive = true);

    Rational(double num);

    Rational(int32_t num);

    Rational(const Rational& other);

    ~Rational() = default;

    Rational& operator=(const Rational&);

    friend bool operator==(const Rational&, const Rational&);

    friend bool operator>(const Rational&, const  Rational&);

    friend bool operator<(const Rational&, const  Rational&);

    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;

    Rational operator-() const;

    Rational abs() const;


    friend std::ostream& operator<<(std::ostream& out, const Rational& num);

    int32_t GetNumer() const;
    int32_t GetDenom() const;
    bool IsPositive() const;

    double ToDouble() const;

    double sqrt() const;

    void ReduceFraction();

 private:
    int32_t numer_, denom_;
    bool positive_;
};

int64_t gcd(int64_t a, int64_t b);

Rational exp(const Rational& num, uint32_t pow);

#endif  // __RATIONAL_H__
