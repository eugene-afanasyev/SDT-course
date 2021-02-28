#include <cmath>
#include <iostream>
#include <algorithm>
#include "rational.h"

Rational::Rational(uint32_t num, uint32_t denom, bool is_positive) {
    positive_ = is_positive;
    numer_ = std::abs(static_cast<int>(num));
    denom_ = denom != 0 ? std::abs(static_cast<int>(denom)) : 1;
    ReduceFraction();
}

Rational::Rational(int32_t num) {
    positive_ = num >= 0;
    numer_ = std::abs(static_cast<int>(num));
    denom_ = 1;
    ReduceFraction();
}

Rational::Rational(const Rational &other) {
    numer_ = other.numer_;
    denom_ = other.denom_;
    positive_ = other.positive_;
    ReduceFraction();
}

Rational::Rational(double input) {

    double num = std::abs(input);

    unsigned multiplier = 1;

    while(num * multiplier - std::floor(num * multiplier) > 0) {
        multiplier *= 10;
    }

    numer_ = num * multiplier;
    denom_ = multiplier;
    positive_ = input > 0;

    ReduceFraction();
}

Rational& Rational::operator=(const Rational& other) {
    numer_ = other.numer_;
    denom_ = other.denom_;
    positive_ = other.positive_;
    return *this;
}

bool operator==(const Rational& num1, const Rational& num2) {
    return (
        num1.GetNumer() == num2.GetNumer() &&
        num1.GetDenom() == num2.GetDenom() &&
        num1.IsPositive() == num2.IsPositive());
}

bool operator>(const Rational& num1, const Rational& num2) {
    if (num1.IsPositive() ^ num2.IsPositive()) {
        return  num1.IsPositive();
    } else if (num1.IsPositive() && num2.IsPositive()) {
        return (num1.GetNumer() * num2.GetDenom() >
                num2.GetNumer() * num1.GetDenom());
    } else {
        return (num1.GetNumer() * num2.GetDenom() <
                num2.GetNumer() * num1.GetDenom());
    }
}

bool operator<(const Rational& num1, const Rational& num2) {
    return !(num1 > num2);
}

Rational Rational::operator+(const Rational& other) const {
    uint32_t result_denom = this->GetDenom() * other.GetDenom();
    uint32_t result_numer = 0;
    bool is_positive = false;

    if (this->IsPositive() == other.IsPositive()) {
        result_numer =
                this->GetNumer() * other.GetDenom() +
                other.GetNumer() * this->GetDenom();
        is_positive = this->IsPositive();
    } else if (this->abs() > other.abs()) {
        result_numer =
                this->GetNumer() * other.GetDenom() -
                other.GetNumer() * this->GetDenom();
        is_positive = this->IsPositive();
    } else {
        result_numer =
                other.GetNumer() * this->GetDenom() -
                this->GetNumer() * other.GetDenom();
        is_positive = other.IsPositive();
    }

    return Rational(result_numer, result_denom, is_positive);
}

Rational Rational::operator-(const Rational& other) const {
    return (-other) + *this;
}

Rational Rational::operator*(const Rational& other) const {
    return Rational(
            this->GetNumer() * other.GetNumer(),
            this->GetDenom() * other.GetDenom(),
            !(this->IsPositive() ^ other.IsPositive()));
}

Rational Rational::operator/(const Rational& other) const {
    return *this * Rational(
            other.GetDenom(), other.GetNumer(), other.IsPositive());
}

Rational Rational::operator-() const{
    if (numer_ > 0) {
        return Rational(numer_, denom_, !positive_);
    } else {
        return *this;
    }
}

Rational Rational::abs() const{
    return Rational(numer_, denom_, true);
}

std::ostream& operator<<(std::ostream& out, const Rational& num) {
    if (num.IsPositive()) {
        if (num.GetDenom() == 1) {
            out << num.GetNumer();
        } else {
            out << '(' << num.GetNumer() << '/' << num.GetDenom() << ')';
        }
    } else {
        if (num.GetDenom() == 1) {
            out << '-' << num.GetNumer();
        } else {
            out << "-(" << num.GetNumer() << '/' << num.GetDenom() << ')';
        }
    }
    return out;
}

int32_t Rational::GetNumer() const {
    return numer_;
}

int32_t Rational:: GetDenom() const {
    return denom_;
}

bool Rational::IsPositive() const {
    return positive_;
}

double Rational::ToDouble() const {
    return
        static_cast<double>(numer_) /
        static_cast<double>(denom_) *
        std::pow(-1, !positive_);
}

// reduces fraction using Euclidean algorithm
void Rational::ReduceFraction() {
    int result = gcd(numer_, denom_);
    numer_ /= result;
    denom_ /= result;
}

int64_t gcd(int64_t a, int64_t b)
{
    if (a == 0)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
        return gcd(a, b % a);
    else
        return gcd(b, a % b);
}

Rational exp(const Rational& num, uint32_t pow) {
    return Rational(
            std::pow(num.GetNumer(), pow),
            std::pow(num.GetDenom(), pow),
            num.IsPositive() || !(pow % 2));
}

double Rational::sqrt() const{
    return (std::sqrt(GetNumer()) / std::sqrt(GetDenom()));
}
