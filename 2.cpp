#include<iostream>

using namespace std;

namespace test
{
    class Complex{
    public:double real, imag; static double s;
    Complex(double a=0.0,double b=0.0):real(a), imag(b) {}

    Complex operator+(const Complex& r) const {
    return Complex(real+r.real, imag+r.imag);
    }

    Complex operator=(double a) {real=a;imag=2;return *this;}

    operator int() {return real;}

    double func() const{return real;}
    double& func() {return real;}

    Complex operator()(bool);
    void print_real(double);

    double& get() {return real;}
    };

    void Complex::print_real(double c) {cout << Complex::real+c <<endl;}
    Complex Complex::operator()(bool b) {
        if (b) return Complex(real, imag);
        else return Complex();
    }


} // namespace test

int main()
{
test::Complex a(1,2);
a.print_real(1.0);
test::Complex b = a(false);
b.print_real(1.0);
cout << a.get()<<endl;
}