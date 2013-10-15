
#ifndef MOTEURFENGTIAN_H
#define MOTEURFENGTIAN_H

#include "ssengine.h"

namespace elabtsaot{

class Emulator;

class MoteurFengtian : public SSEngine {

 public:

  MoteurFengtian(Emulator* emu, Logger* log = NULL);
  virtual ~MoteurFengtian(){}

private:

  int do_solveLoadflow(Powersystem const& pws,
                       boost::numeric::ublas::vector<double>& x,
                       boost::numeric::ublas::vector<double>& F ) const;
  void _getOptions( double& beta1,
                    double& beta2,
                    double& Ptolerance,
                    double& Qtolerance,
                    size_t& maxIterCount ) const;

  // ---------- Variables ----------
  Emulator* _emu;

};

} // end of namespace elabtsaot

#endif // MOTEURFENGTIAN_H