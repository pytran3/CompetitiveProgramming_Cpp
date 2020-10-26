#include <complex>
using namespace std;

using Vector = complex<double>;
const double EPS = 1e-8;

double dot(Vector a, Vector b) {
  return (a.real() * b.real() + a.imag() * b.imag());
}

double cross(Vector a, Vector b) {
  return a.real() * b.imag() - a.imag() * b.real();
}

int is_intersected(Vector a1, Vector a2, Vector b1, Vector b2) {
  return  ( cross(a2-a1, b1-a1) * cross(a2-a1, b2-a1) < EPS) &&
          ( cross(b2-b1, a1-b1) * cross(b2-b1, a2-b1) < EPS );
}