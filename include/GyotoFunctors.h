/**
 * \file GyotoFunctors.h
 * \brief Classes which an operator() method


 */

/*
    Copyright 2011 Thibaut Paumard

    This file is part of Gyoto.

    Gyoto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gyoto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gyoto.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __GyotoFunctors_H_
#define __GyotoFunctors_H_

namespace Gyoto {
  namespace Functor {
    class Double_constDoubleArray;
    class Double_Double_const;
    
  }
}

class Gyoto::Functor::Double_constDoubleArray
{
 public:
  virtual double operator()(double const data[]) = 0;
};

class Gyoto::Functor::Double_Double_const
{
 public:
  int status;
  virtual double operator()(double) const = 0;
  double ridders(double, double) const;
  double secant(double, double);
};

#endif
