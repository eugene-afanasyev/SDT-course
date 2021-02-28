#ifndef __COMPLEX_NUMBER_H__
#define __COMPLEX_NUMBER_H__

#include <ostream>
#include "../rational/rational.h"

class ComplexNumber {
 public:
    explicit ComplexNumber(const Rational& real_part,
                           const Rational& imaginary_part);

    ComplexNumber(const ComplexNumber& other);

    ComplexNumber(int num);

    ComplexNumber(double num);

    ComplexNumber();

    ~ComplexNumber() = default;

    friend bool operator==(const ComplexNumber&, const ComplexNumber&);
    friend bool operator!=(const ComplexNumber&, const ComplexNumber&);

    ComplexNumber operator+(const ComplexNumber&);
    ComplexNumber operator-(const ComplexNumber&);
    ComplexNumber operator*(const ComplexNumber&);
    ComplexNumber operator/(const ComplexNumber&);

    ComplexNumber& operator=(const ComplexNumber&);

    ComplexNumber& operator+=(const ComplexNumber&);
    ComplexNumber& operator-=(const ComplexNumber&);
    ComplexNumber& operator*=(const ComplexNumber&);
    ComplexNumber& operator/=(const ComplexNumber&);

    ComplexNumber operator-() const;

    double abs() const;

    double arg() const;

    ComplexNumber pow(const uint32_t&) const;

    friend std::ostream& operator<<(
            std:: ostream& out, const ComplexNumber& num);

    Rational GetRealPart() const;
    void SetRealPart(const Rational&);

    Rational GetImaginaryPart() const;
    void SetImaginaryPart(const Rational&);

private:
    Rational real_pt, imag_pt;
};

#endif //__COMPLEX_NUMBER_H__
