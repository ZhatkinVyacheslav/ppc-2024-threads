// Copyright 2024 Loginov Maxim

#pragma once

#include <cmath>
#include <random>

namespace LoginovTbb {
struct LoginovPoint {
 public:
  double x;
  double y;

  LoginovPoint();
  LoginovPoint(double X, double Y);
  LoginovPoint(const LoginovPoint& p2);

  LoginovPoint& operator=(const LoginovPoint& p2);
  bool operator==(const LoginovPoint& p2) const;
  bool operator()(LoginovPoint& p0, LoginovPoint& p1) const;

  double angle(const LoginovPoint& p) const;
  double Distance(const LoginovPoint& p) const;
  void swap(LoginovPoint& p2);
  int Compare(const LoginovPoint& pivot, const LoginovPoint& P) const;
  static LoginovPoint aRandomPoint(double min, double max);
};
}  // namespace LoginovTbb
