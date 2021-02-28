#include <iostream>
#include <string>
#include <cmath>
#include "ComplexNumber.h"

ComplexNumber::ComplexNumber
    (const Rational& real_part, const Rational& imaginary_part) {
    real_pt = real_part;
    imag_pt = imaginary_part;
}

ComplexNumber::ComplexNumber(const ComplexNumber &other) {
    real_pt = other.GetRealPart();
    imag_pt = other.GetImaginaryPart();
}

ComplexNumber::ComplexNumber() {
    real_pt = 0;
    imag_pt = 0;
}

ComplexNumber::ComplexNumber(int num) {
    real_pt = num;
    imag_pt = 0;
}

ComplexNumber::ComplexNumber(double num) {
    real_pt = num;
    imag_pt = 0;
}

Rational ComplexNumber::GetRealPart() const{
    return real_pt;
}

Rational ComplexNumber::GetImaginaryPart() const {
    return imag_pt;
}

bool operator==(const ComplexNumber& num1, const ComplexNumber& num2) {
    return (num1.GetRealPart() == num2.GetRealPart() &&
            num1.GetImaginaryPart() == num2.GetImaginaryPart());
}

bool operator!=(const ComplexNumber& num1, const ComplexNumber& num2) {
    return !(num1 == num2);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) {
    Rational result_real_pt(this->GetRealPart() + other.GetRealPart());
    Rational result_img_pt(
            this->GetImaginaryPart() + other.GetImaginaryPart());
    return ComplexNumber(result_real_pt, result_img_pt);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber &other) {
    Rational result_real_pt(this->GetRealPart() - other.GetRealPart());
    Rational result_img_pt(
            this->GetImaginaryPart() - other.GetImaginaryPart());
    return ComplexNumber(result_real_pt, result_img_pt);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber &other) {
    auto result_real_pt =
            this->GetRealPart() * other.GetRealPart() -
            this->GetImaginaryPart() * other.GetImaginaryPart();

    auto result_img_pt =
            this->GetRealPart() * other.GetImaginaryPart() +
            other.GetRealPart() * this->GetImaginaryPart();

    return ComplexNumber(result_real_pt, result_img_pt);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber &other) {
    auto divider = (other.GetRealPart() * other.GetRealPart() +
                   other.GetImaginaryPart() * other.GetImaginaryPart());
    auto result_real_pt =
            (this->GetRealPart() * other.GetRealPart() +
            this->GetImaginaryPart() * other.GetImaginaryPart()) / divider;
    auto result_img_pt =
            (this->GetImaginaryPart() * other.GetRealPart() -
            this->GetRealPart() * other.GetImaginaryPart()) / divider;

    return ComplexNumber(result_real_pt, result_img_pt);
}

ComplexNumber& ComplexNumber::operator=(const ComplexNumber &other) {
    this->SetRealPart(other.GetRealPart());
    this->SetImaginaryPart(other.GetImaginaryPart());

    return *this;
}

ComplexNumber& ComplexNumber::operator+=(const ComplexNumber& other) {
    *this = (*this) + other;
    return *this;
}

ComplexNumber& ComplexNumber::operator-=(const ComplexNumber& other) {
    *this = (*this) - other;
    return *this;
}

ComplexNumber& ComplexNumber::operator*=(const ComplexNumber& other) {
    *this = (*this) * other;
    return *this;
}

ComplexNumber& ComplexNumber::operator/=(const ComplexNumber& other) {
    *this = (*this) / other;
    return *this;
}

ComplexNumber ComplexNumber::operator-() const {
    return ComplexNumber(-real_pt, -imag_pt);
}

void ComplexNumber::SetRealPart(const Rational& num) {
    this->real_pt = num;
}

void ComplexNumber::SetImaginaryPart(const Rational& num) {
    this->imag_pt = num;
}

ComplexNumber ComplexNumber::pow(const uint32_t &num) const {
    ComplexNumber result = 1;
    for (unsigned i = 0; i < num; i++) {
        result *= *this;
    }

    return result;
}

double ComplexNumber::abs() const{
    return (exp(real_pt, 2) + exp(imag_pt, 2)).sqrt();
}

double ComplexNumber::arg() const{
    return std::atan(imag_pt.ToDouble() / real_pt.ToDouble());
}

std::ostream& operator<<(std::ostream& out, const ComplexNumber& num) {
    auto img_part_sign = [num]() -> std::string {
        if (num.GetImaginaryPart().IsPositive()) {
            return "+";
        } else {
            return "-";
        }
    };

    if (num.GetImaginaryPart().GetNumer() != 0) {
        if (num.GetRealPart().GetNumer() != 0) {
            out << num.GetRealPart() << img_part_sign() <<
                num.GetImaginaryPart().abs() << 'i';
        } else {
            out << num.GetImaginaryPart() << 'i';
        }
    } else {
        out << num.GetRealPart();
    }

    return out;
}
