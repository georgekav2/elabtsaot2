
#ifndef SSENGINE_H
#define SSENGINE_H

#include "propertyholder.h"
#include "powersystem.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;

#include <complex>
typedef std::complex<double> complex;

namespace elabtsaot{

class Logger;

class SSEngine : public PropertyHolder {
 public:
  SSEngine(std::string description, Logger* log);
  virtual ~SSEngine(){}
  std::string SSDescription() const;
  void setLogger(Logger* log);
  int solvePowerFlow(Powersystem& pws) const;

 protected:
  std::string _SSdescription;
  Logger* _SSlog;

 private:
  virtual int do_solvePowerFlow(Powersystem const& pws,
                                ublas::matrix<complex>& Y,
                                ublas::vector<complex>& V) const = 0;
};

namespace ssengine{
  void buildY(Powersystem const& pws, ublas::matrix<complex>& Y);
  void calculatePower(ublas::matrix<complex> const& Y,
                      ublas::vector<complex> const& V,
                      ublas::vector<complex>& S);
  void calculateCurrent(ublas::matrix<complex> const& Y,
                        ublas::vector<complex> const& V,
                        ublas::vector<complex>& I);
  ublas::vector<complex> absComplex(ublas::vector<complex> const& v);
  void calculateDSdV(ublas::matrix<complex> const& Y,
                     ublas::vector<complex> const& V,
                     ublas::matrix<complex>& dSdVm,
                     ublas::matrix<complex>& dSdVa);
  void updatePowersystem(Powersystem& pws,
                         ublas::matrix<complex> const& Y,
                         ublas::vector<complex> const& V);
} // end of namespace ssengine

} // end of namespace elabtsaot

#endif // SSENGINE_H
