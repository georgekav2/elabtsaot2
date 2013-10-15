
#include "atom.h"
using namespace elabtsaot;

//#include <vector>
using std::vector;
#include <limits>
using std::numeric_limits;

Atom::Atom( DAC const* real_voltage_ref,
            DAC const* imag_voltage_ref ) :
    _embr_exist(EMBRPOS_UL+1, false),
    _embr_real(EMBRPOS_UL+1),
    _embr_imag(EMBRPOS_UL+1),
    _real_voltage_ref(real_voltage_ref), _imag_voltage_ref(imag_voltage_ref){

  // In initialization list and hereunder: emulator branch physical existance
  // vector is initialized to false for all positions but EMBRPOS_U, EMBRPOS_UR,
  // EMBRPOS_R
  _embr_exist[EMBRPOS_U ] = true;
  _embr_exist[EMBRPOS_UR] = true;
  _embr_exist[EMBRPOS_R ] = true;

  // In initialization list: emulator branch virtual existance vector is
  // initialized to false for all positions
}

int Atom::reset( bool complete ){
  // Not touching physical existance values for the atom
  // Just reseting virtual existance to false and reinitilizing data structures

  // Reset node
  _node.reset( complete );

  // Reset branches
  int ans = 0;
  for ( size_t k = 0 ; k != _embr_real.size() ; ++k )
    ans |= _embr_real[k].reset( complete );
  for ( size_t k = 0 ; k != _embr_imag.size() ; ++k )
    ans |= _embr_imag[k].reset( complete );

  return ans;
}

size_t Atom::getEmbrCount() const{
  size_t ans = 0;
  for ( size_t k = 0 ; k != _embr_exist.size() ; ++k )
    if ( _embr_exist[k] )
      ++ans;
  return ans;
}

void Atom::calibrate(Atom const& cal_am){
  _node.set_real_adc_offset_corr( cal_am.node_real_adc_offset_corr() );
  _node.set_imag_adc_offset_corr( cal_am.node_imag_adc_offset_corr() );
  _node.set_real_adc_gain_corr( cal_am.node_real_adc_gain_corr() );
  _node.set_imag_adc_gain_corr( cal_am.node_real_adc_gain_corr() );

  _node.set_real_pot_current_rab( cal_am.node_real_pot_current_rab(), true);
  _node.set_imag_pot_current_rab( cal_am.node_imag_pot_current_rab(), true);
  _node.set_real_pot_current_rw( cal_am.node_real_pot_current_rw(), true);
  _node.set_imag_pot_current_rw( cal_am.node_imag_pot_current_rw(), true);
  _node.set_real_pot_resistance_rab( cal_am.node_real_pot_resistance_rab(), true);
  _node.set_imag_pot_resistance_rab( cal_am.node_imag_pot_resistance_rab(), true);
  _node.set_real_pot_resistance_rw( cal_am.node_real_pot_resistance_rw(), true);
  _node.set_imag_pot_resistance_rw( cal_am.node_imag_pot_resistance_rw(), true);

  if ( _embr_exist[EMBRPOS_U] ){
    set_embr_real_pot_near_rab( EMBRPOS_U, cal_am.embr_real_pot_near_rab(EMBRPOS_U), true);
    set_embr_imag_pot_near_rab( EMBRPOS_U, cal_am.embr_imag_pot_near_rab(EMBRPOS_U), true);
    set_embr_real_pot_far_rab( EMBRPOS_U, cal_am.embr_real_pot_far_rab(EMBRPOS_U), true);
    set_embr_imag_pot_far_rab( EMBRPOS_U, cal_am.embr_imag_pot_far_rab(EMBRPOS_U), true);

    set_embr_real_pot_near_rw( EMBRPOS_U, cal_am.embr_real_pot_near_rw(EMBRPOS_U), true);
    set_embr_imag_pot_near_rw( EMBRPOS_U, cal_am.embr_imag_pot_near_rw(EMBRPOS_U), true);
    set_embr_real_pot_far_rw( EMBRPOS_U, cal_am.embr_real_pot_far_rw(EMBRPOS_U), true);
    set_embr_imag_pot_far_rw( EMBRPOS_U, cal_am.embr_imag_pot_far_rw(EMBRPOS_U), true);
  }

  if ( _embr_exist[EMBRPOS_UR] ){
    set_embr_real_pot_near_rab( EMBRPOS_UR, cal_am.embr_real_pot_near_rab(EMBRPOS_UR), true);
    set_embr_imag_pot_near_rab( EMBRPOS_UR, cal_am.embr_imag_pot_near_rab(EMBRPOS_UR), true);
    set_embr_real_pot_far_rab( EMBRPOS_UR, cal_am.embr_real_pot_far_rab(EMBRPOS_UR), true);
    set_embr_imag_pot_far_rab( EMBRPOS_UR, cal_am.embr_imag_pot_far_rab(EMBRPOS_UR), true);

    set_embr_real_pot_near_rw( EMBRPOS_UR, cal_am.embr_real_pot_near_rw(EMBRPOS_UR), true);
    set_embr_imag_pot_near_rw( EMBRPOS_UR, cal_am.embr_imag_pot_near_rw(EMBRPOS_UR), true);
    set_embr_real_pot_far_rw( EMBRPOS_UR, cal_am.embr_real_pot_far_rw(EMBRPOS_UR), true);
    set_embr_imag_pot_far_rw( EMBRPOS_UR, cal_am.embr_imag_pot_far_rw(EMBRPOS_UR), true);
  }

  if ( _embr_exist[EMBRPOS_R] ){
    set_embr_real_pot_near_rab( EMBRPOS_R, cal_am.embr_real_pot_near_rab(EMBRPOS_R), true);
    set_embr_imag_pot_near_rab( EMBRPOS_R, cal_am.embr_imag_pot_near_rab(EMBRPOS_R), true);
    set_embr_real_pot_far_rab( EMBRPOS_R, cal_am.embr_real_pot_far_rab(EMBRPOS_R), true);
    set_embr_imag_pot_far_rab( EMBRPOS_R, cal_am.embr_imag_pot_far_rab(EMBRPOS_R), true);

    set_embr_real_pot_near_rw( EMBRPOS_R, cal_am.embr_real_pot_near_rw(EMBRPOS_R), true);
    set_embr_imag_pot_near_rw( EMBRPOS_R, cal_am.embr_imag_pot_near_rw(EMBRPOS_R), true);
    set_embr_real_pot_far_rw( EMBRPOS_R, cal_am.embr_real_pot_far_rw(EMBRPOS_R), true);
    set_embr_imag_pot_far_rw( EMBRPOS_R, cal_am.embr_imag_pot_far_rw(EMBRPOS_R), true);
  }

  // Should never happen with current hardware topology (as of Feb 2012)
  if ( _embr_exist[EMBRPOS_DR] ){
    set_embr_real_pot_near_rab( EMBRPOS_DR, cal_am.embr_real_pot_near_rab(EMBRPOS_DR), true);
    set_embr_imag_pot_near_rab( EMBRPOS_DR, cal_am.embr_imag_pot_near_rab(EMBRPOS_DR), true);
    set_embr_real_pot_far_rab( EMBRPOS_DR, cal_am.embr_real_pot_far_rab(EMBRPOS_DR), true);
    set_embr_imag_pot_far_rab( EMBRPOS_DR, cal_am.embr_imag_pot_far_rab(EMBRPOS_DR), true);

    set_embr_real_pot_near_rw( EMBRPOS_DR, cal_am.embr_real_pot_near_rw(EMBRPOS_DR), true);
    set_embr_imag_pot_near_rw( EMBRPOS_DR, cal_am.embr_imag_pot_near_rw(EMBRPOS_DR), true);
    set_embr_real_pot_far_rw( EMBRPOS_DR, cal_am.embr_real_pot_far_rw(EMBRPOS_DR), true);
    set_embr_imag_pot_far_rw( EMBRPOS_DR, cal_am.embr_imag_pot_far_rw(EMBRPOS_DR), true);
  }

  if ( _embr_exist[EMBRPOS_D] ){
    set_embr_real_pot_near_rab( EMBRPOS_D, cal_am.embr_real_pot_near_rab(EMBRPOS_D), true);
    set_embr_imag_pot_near_rab( EMBRPOS_D, cal_am.embr_imag_pot_near_rab(EMBRPOS_D), true);
    set_embr_real_pot_far_rab( EMBRPOS_D, cal_am.embr_real_pot_far_rab(EMBRPOS_D), true);
    set_embr_imag_pot_far_rab( EMBRPOS_D, cal_am.embr_imag_pot_far_rab(EMBRPOS_D), true);

    set_embr_real_pot_near_rw( EMBRPOS_D, cal_am.embr_real_pot_near_rw(EMBRPOS_D), true);
    set_embr_imag_pot_near_rw( EMBRPOS_D, cal_am.embr_imag_pot_near_rw(EMBRPOS_D), true);
    set_embr_real_pot_far_rw( EMBRPOS_D, cal_am.embr_real_pot_far_rw(EMBRPOS_D), true);
    set_embr_imag_pot_far_rw( EMBRPOS_D, cal_am.embr_imag_pot_far_rw(EMBRPOS_D), true);
  }

  // Should never happen with current hardware topology (as of Feb 2012)
  if ( _embr_exist[EMBRPOS_DL] ){
    set_embr_real_pot_near_rab( EMBRPOS_DL, cal_am.embr_real_pot_near_rab(EMBRPOS_DL), true);
    set_embr_imag_pot_near_rab( EMBRPOS_DL, cal_am.embr_imag_pot_near_rab(EMBRPOS_DL), true);
    set_embr_real_pot_far_rab( EMBRPOS_DL, cal_am.embr_real_pot_far_rab(EMBRPOS_DL), true);
    set_embr_imag_pot_far_rab( EMBRPOS_DL, cal_am.embr_imag_pot_far_rab(EMBRPOS_DL), true);

    set_embr_real_pot_near_rw( EMBRPOS_DL, cal_am.embr_real_pot_near_rw(EMBRPOS_DL), true);
    set_embr_imag_pot_near_rw( EMBRPOS_DL, cal_am.embr_imag_pot_near_rw(EMBRPOS_DL), true);
    set_embr_real_pot_far_rw( EMBRPOS_DL, cal_am.embr_real_pot_far_rw(EMBRPOS_DL), true);
    set_embr_imag_pot_far_rw( EMBRPOS_DL, cal_am.embr_imag_pot_far_rw(EMBRPOS_DL), true);
  }

  if ( _embr_exist[EMBRPOS_L] ){
    set_embr_real_pot_near_rab( EMBRPOS_L, cal_am.embr_real_pot_near_rab(EMBRPOS_L), true);
    set_embr_imag_pot_near_rab( EMBRPOS_L, cal_am.embr_imag_pot_near_rab(EMBRPOS_L), true);
    set_embr_real_pot_far_rab( EMBRPOS_L, cal_am.embr_real_pot_far_rab(EMBRPOS_L), true);
    set_embr_imag_pot_far_rab( EMBRPOS_L, cal_am.embr_imag_pot_far_rab(EMBRPOS_L), true);

    set_embr_real_pot_near_rw( EMBRPOS_L, cal_am.embr_real_pot_near_rw(EMBRPOS_L), true);
    set_embr_imag_pot_near_rw( EMBRPOS_L, cal_am.embr_imag_pot_near_rw(EMBRPOS_L), true);
    set_embr_real_pot_far_rw( EMBRPOS_L, cal_am.embr_real_pot_far_rw(EMBRPOS_L), true);
    set_embr_imag_pot_far_rw( EMBRPOS_L, cal_am.embr_imag_pot_far_rw(EMBRPOS_L), true);
  }

  // Should never happen with current hardware topology (as of Feb 2012)
  if ( _embr_exist[EMBRPOS_UL] ){
    set_embr_real_pot_near_rab( EMBRPOS_UL, cal_am.embr_real_pot_near_rab(EMBRPOS_UL), true);
    set_embr_imag_pot_near_rab( EMBRPOS_UL, cal_am.embr_imag_pot_near_rab(EMBRPOS_UL), true);
    set_embr_real_pot_far_rab( EMBRPOS_UL, cal_am.embr_real_pot_far_rab(EMBRPOS_UL), true);
    set_embr_imag_pot_far_rab( EMBRPOS_UL, cal_am.embr_imag_pot_far_rab(EMBRPOS_UL), true);

    set_embr_real_pot_near_rw( EMBRPOS_UL, cal_am.embr_real_pot_near_rw(EMBRPOS_UL), true);
    set_embr_imag_pot_near_rw( EMBRPOS_UL, cal_am.embr_imag_pot_near_rw(EMBRPOS_UL), true);
    set_embr_real_pot_far_rw( EMBRPOS_UL, cal_am.embr_real_pot_far_rw(EMBRPOS_UL), true);
    set_embr_imag_pot_far_rw( EMBRPOS_UL, cal_am.embr_imag_pot_far_rw(EMBRPOS_UL), true);
  }
}

double Atom::getMinMaxAchievableR() const{

  double minMaxR = numeric_limits<double>::max();

  // Take into account branches
  for ( size_t k = 0 ; k != _embr_exist.size() ; ++k ){
    if ( _embr_exist[k] ){
      // real embrs
      if ( _embr_real[k].pot_near_getRMax() < minMaxR )
        minMaxR = _embr_real[k].pot_near_getRMax();
      if ( _embr_real[k].pot_far_getRMax() < minMaxR )
        minMaxR = _embr_real[k].pot_far_getRMax();
      // imag embrs
      if ( _embr_imag[k].pot_near_getRMax() < minMaxR )
        minMaxR = _embr_imag[k].pot_near_getRMax();
      if ( _embr_imag[k].pot_far_getRMax() < minMaxR )
        minMaxR = _embr_imag[k].pot_far_getRMax();
    }
  }

  // --- Take into account node ---
  // real part for the node
  if ( _node.real_pot_current_getRMax() < minMaxR )
    minMaxR = _node.real_pot_current_getRMax();
  if ( _node.real_pot_resistance_getRMax() < minMaxR )
    minMaxR = _node.real_pot_resistance_getRMax();
  // imag part for the node
  if ( _node.imag_pot_current_getRMax() < minMaxR )
    minMaxR = _node.imag_pot_current_getRMax();
  if ( _node.imag_pot_resistance_getRMax() < minMaxR )
    minMaxR = _node.imag_pot_resistance_getRMax();

  return minMaxR;
}



/*****************************************************************************
****** GETTERS ***************************************************************
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
///// NODE RELATED ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Atom::set_node_real_adc_offset_corr( double val ){ _node.set_real_adc_offset_corr( val ); }
void Atom::set_node_real_adc_gain_corr( double val ){ _node.set_real_adc_gain_corr( val ); }
void Atom::set_node_real_sw_voltage(bool val){ _node.set_real_sw_voltage(val); }
void Atom::set_node_real_sw_current(bool val){ _node.set_real_sw_current(val); }
void Atom::set_node_real_sw_current_shunt(bool val){ _node.set_real_sw_current_shunt(val); }
void Atom::set_node_real_sw_resistance(bool val){ _node.set_real_sw_resistance(val); }
int Atom::set_node_real_pot_current_rab(double val, bool updateTap){ return _node.set_real_pot_current_rab( val, updateTap ); }
int Atom::set_node_real_pot_current_rw(double val, bool updateTap){ return _node.set_real_pot_current_rw( val, updateTap ); }
int Atom::set_node_real_pot_current_r(double val){ return _node.set_real_pot_current_r(val); }
int Atom::set_node_real_pot_current_tap( unsigned int val){ return _node.set_real_pot_current_tap(val); }
void Atom::set_node_real_pot_current_sw(bool val){ return _node.set_real_pot_current_sw(val); }
int Atom::set_node_real_pot_current_swA( bool val, bool updateTap){ return _node.set_real_pot_current_swA(val,updateTap); }
int Atom::set_node_real_pot_resistance_rab(double val, bool updateTap){ return _node.set_real_pot_resistance_rab( val, updateTap ); }
int Atom::set_node_real_pot_resistance_rw(double val, bool updateTap){ return _node.set_real_pot_resistance_rw( val, updateTap ); }
int Atom::set_node_real_pot_resistance_r(double val){ return _node.set_real_pot_resistance_r(val); }
int Atom::set_node_real_pot_resistance_tap(unsigned int val){ return _node.set_real_pot_resistance_tap(val); }
void Atom::set_node_real_pot_resistance_sw(bool val){ return _node.set_real_pot_resistance_sw(val); }
int Atom::set_node_real_pot_resistance_swA( bool val, bool updateTap){ return _node.set_real_pot_resistance_swA(val,updateTap); }

void Atom::set_node_imag_adc_offset_corr( double val ){ _node.set_imag_adc_offset_corr( val ); }
void Atom::set_node_imag_adc_gain_corr( double val ){ _node.set_imag_adc_gain_corr( val ); }
void Atom::set_node_imag_sw_voltage(bool val){ _node.set_imag_sw_voltage(val); }
void Atom::set_node_imag_sw_current(bool val){ _node.set_imag_sw_current(val); }
void Atom::set_node_imag_sw_current_shunt(bool val){ _node.set_imag_sw_current_shunt(val); }
void Atom::set_node_imag_sw_resistance(bool val){ _node.set_imag_sw_resistance(val); }
int Atom::set_node_imag_pot_current_rab(double val, bool updateTap){ return _node.set_imag_pot_current_rab( val, updateTap ); }
int Atom::set_node_imag_pot_current_rw(double val, bool updateTap){ return _node.set_imag_pot_current_rw( val, updateTap ); }
int Atom::set_node_imag_pot_current_r(double val){ return _node.set_imag_pot_current_r(val); }
int Atom::set_node_imag_pot_current_tap( unsigned int val){ return _node.set_imag_pot_current_tap(val); }
void Atom::set_node_imag_pot_current_sw(bool val){ return _node.set_imag_pot_current_sw(val); }
int Atom::set_node_imag_pot_current_swA( bool val, bool updateTap){ return _node.set_imag_pot_current_swA(val,updateTap); }
int Atom::set_node_imag_pot_resistance_rab(double val, bool updateTap){ return _node.set_imag_pot_resistance_rab( val, updateTap ); }
int Atom::set_node_imag_pot_resistance_rw(double val, bool updateTap){ return _node.set_imag_pot_resistance_rw( val, updateTap ); }
int Atom::set_node_imag_pot_resistance_r(double val){ return _node.set_imag_pot_resistance_r(val); }
int Atom::set_node_imag_pot_resistance_tap(unsigned int val){ return _node.set_imag_pot_resistance_tap(val); }
void Atom::set_node_imag_pot_resistance_sw(bool val){ return _node.set_imag_pot_resistance_sw(val); }
int Atom::set_node_imag_pot_resistance_swA( bool val, bool updateTap){ return _node.set_imag_pot_resistance_swA(val,updateTap); }

//////////////////////////////////////////////////////////////////////////////
////BRANCH RELATED ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Atom::set_embr_exist(size_t pos, bool val){ _embr_exist[pos] = val; }

void Atom::set_embr_real_sw_sc(size_t pos, bool val){ _embr_real[pos].set_sw_sc(val); }
void Atom::set_embr_real_sw_mid(size_t pos, bool val){ _embr_real[pos].set_sw_mid(val); }
int Atom::set_embr_real_pot_near_rab(size_t pos, double val, bool updateTap){ return _embr_real[pos].set_pot_near_rab( val, updateTap ); }
int Atom::set_embr_real_pot_near_rw( size_t pos, double val, bool updateTap){ return _embr_real[pos].set_pot_near_rw( val, updateTap ); }
int Atom::set_embr_real_pot_near_r(size_t pos, double val){
  double rMismatch_near;
  return _embr_real[pos].set_pot_near_r(val, &rMismatch_near);
}
int Atom::set_embr_real_pot_near_tap(size_t pos, unsigned int val){ return _embr_real[pos].set_pot_near_tap(val); }
void Atom::set_embr_real_pot_near_sw(size_t pos, bool val){ return _embr_real[pos].set_pot_near_sw(val); }
int Atom::set_embr_real_pot_near_swA(size_t pos, bool val, bool updateTap){ return _embr_real[pos].set_pot_near_swA(val,updateTap); }
int Atom::set_embr_real_pot_far_rab(size_t pos, double val, bool updateTap){ return _embr_real[pos].set_pot_far_rab( val, updateTap ); }
int Atom::set_embr_real_pot_far_rw( size_t pos, double val, bool updateTap){ return _embr_real[pos].set_pot_far_rw( val, updateTap ); }
int Atom::set_embr_real_pot_far_r(size_t pos, double val){
  double rMismatch_far;
  return _embr_real[pos].set_pot_far_r(val, &rMismatch_far);
}
int Atom::set_embr_real_pot_far_tap(size_t pos, unsigned int val){ return _embr_real[pos].set_pot_far_tap(val); }
void Atom::set_embr_real_pot_far_sw(size_t pos, bool val){ return _embr_real[pos].set_pot_far_sw(val); }
int Atom::set_embr_real_pot_far_swA(size_t pos, bool val, bool updateTap){ return _embr_real[pos].set_pot_far_swA(val,updateTap); }

void Atom::set_embr_imag_sw_sc(size_t pos, bool val){ _embr_imag[pos].set_sw_sc(val); }
void Atom::set_embr_imag_sw_mid(size_t pos, bool val){ _embr_imag[pos].set_sw_mid(val); }
int Atom::set_embr_imag_pot_near_rab(size_t pos, double val, bool updateTap){ return _embr_imag[pos].set_pot_near_rab( val, updateTap ); }
int Atom::set_embr_imag_pot_near_rw( size_t pos, double val, bool updateTap){ return _embr_imag[pos].set_pot_near_rw( val, updateTap ); }
int Atom::set_embr_imag_pot_near_r(size_t pos, double val){
  double rMismatch_near;
  return _embr_imag[pos].set_pot_near_r(val, &rMismatch_near);
}
int Atom::set_embr_imag_pot_near_tap(size_t pos, unsigned int val){ return _embr_imag[pos].set_pot_near_tap(val); }
void Atom::set_embr_imag_pot_near_sw( size_t pos, bool val){ return _embr_imag[pos].set_pot_near_sw(val); }
int Atom::set_embr_imag_pot_near_swA( size_t pos, bool val, bool updateTap){ return _embr_imag[pos].set_pot_near_swA(val,updateTap); }
int Atom::set_embr_imag_pot_far_rab(size_t pos, double val, bool updateTap){ return _embr_imag[pos].set_pot_far_rab( val, updateTap ); }
int Atom::set_embr_imag_pot_far_rw( size_t pos, double val, bool updateTap){ return _embr_imag[pos].set_pot_far_rw( val, updateTap ); }
int Atom::set_embr_imag_pot_far_r(size_t pos, double val){
  double rMismatch_far;
  return _embr_imag[pos].set_pot_far_r(val, &rMismatch_far);
}
int Atom::set_embr_imag_pot_far_tap(size_t pos, unsigned int val){ return _embr_imag[pos].set_pot_far_tap(val); }
void Atom::set_embr_imag_pot_far_sw(size_t pos, bool val){ return _embr_imag[pos].set_pot_far_sw(val); }
int Atom::set_embr_imag_pot_far_swA(size_t pos, bool val, bool updateTap){ return _embr_imag[pos].set_pot_far_swA(val, updateTap); }

/*******************************************************************************
******** GETTERS ***************************************************************
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/////// NODE RELATED ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

double Atom::node_real_adc_offset_corr() const{ return _node.real_adc_offset_corr(); }
double Atom::node_real_adc_gain_corr() const{ return _node.real_adc_gain_corr(); }
bool Atom::node_real_sw_voltage() const{ return _node.real_sw_voltage(); }
bool Atom::node_real_sw_current() const{ return _node.real_sw_current(); }
bool Atom::node_real_sw_current_shunt() const{ return _node.real_sw_current_shunt(); }
bool Atom::node_real_sw_resistance() const{ return _node.real_sw_resistance(); }
double Atom::node_real_pot_current_rab() const{ return _node.real_pot_current_rab(); }
double Atom::node_real_pot_current_rw() const{ return _node.real_pot_current_rw(); }
double Atom::node_real_pot_current_r() const{ return _node.real_pot_current_r(); }
unsigned int Atom::node_real_pot_current_tap() const{ return _node.real_pot_current_tap(); }
bool Atom::node_real_pot_current_sw() const{ return _node.real_pot_current_sw(); }
bool Atom::node_real_pot_current_swA() const{ return _node.real_pot_current_swA(); }
double Atom::node_real_pot_resistance_rab() const{ return _node.real_pot_resistance_rab(); }
double Atom::node_real_pot_resistance_rw() const{ return _node.real_pot_resistance_rw(); }
double Atom::node_real_pot_resistance_r() const{ return _node.real_pot_resistance_r(); }
unsigned int Atom::node_real_pot_resistance_tap() const{ return _node.real_pot_resistance_tap(); }
bool Atom::node_real_pot_resistance_sw() const{ return _node.real_pot_resistance_sw(); }
bool Atom::node_real_pot_resistance_swA() const{ return _node.real_pot_resistance_swA(); }

double Atom::node_imag_adc_offset_corr() const{ return _node.imag_adc_offset_corr(); }
double Atom::node_imag_adc_gain_corr() const{ return _node.imag_adc_gain_corr(); }
bool Atom::node_imag_sw_voltage() const{ return _node.imag_sw_voltage(); }
bool Atom::node_imag_sw_current() const{ return _node.imag_sw_current(); }
bool Atom::node_imag_sw_current_shunt() const{ return _node.imag_sw_current_shunt(); }
bool Atom::node_imag_sw_resistance() const{ return _node.imag_sw_resistance(); }
double Atom::node_imag_pot_current_rab() const{ return _node.imag_pot_current_rab(); }
double Atom::node_imag_pot_current_rw() const{ return _node.imag_pot_current_rw(); }
double Atom::node_imag_pot_current_r() const{ return _node.imag_pot_current_r(); }
unsigned int Atom::node_imag_pot_current_tap() const{ return _node.imag_pot_current_tap(); }
bool Atom::node_imag_pot_current_sw() const{ return _node.imag_pot_current_sw(); }
bool Atom::node_imag_pot_current_swA() const{ return _node.imag_pot_current_swA(); }
double Atom::node_imag_pot_resistance_rab() const{ return _node.imag_pot_resistance_rab(); }
double Atom::node_imag_pot_resistance_rw() const{ return _node.imag_pot_resistance_rw(); }
double Atom::node_imag_pot_resistance_r() const{ return _node.imag_pot_resistance_r(); }
unsigned int Atom::node_imag_pot_resistance_tap() const{ return _node.imag_pot_resistance_tap(); }
bool Atom::node_imag_pot_resistance_sw() const{ return _node.imag_pot_resistance_sw(); }
bool Atom::node_imag_pot_resistance_swA() const{ return _node.imag_pot_resistance_swA(); }

//////////////////////////////////////////////////////////////////////////////
////BRANCH RELATED ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EmulatorBranch const& Atom::embr_real(size_t pos) const{ return _embr_real[pos]; }
EmulatorBranch const& Atom::embr_imag(size_t pos) const{  return _embr_imag[pos]; }

vector<bool> Atom::embr_exist() const{ return _embr_exist; }
bool Atom::embr_exist(size_t pos) const{ return _embr_exist[pos]; }

bool Atom::embr_real_sw_sc(size_t pos) const{ return _embr_real[pos].sw_sc(); }
bool Atom::embr_real_sw_mid(size_t pos) const{ return _embr_real[pos].sw_mid(); }
double Atom::embr_real_pot_near_rab(size_t pos) const{ return _embr_real[pos].pot_near_rab(); }
double Atom::embr_real_pot_near_rw(size_t pos) const{ return _embr_real[pos].pot_near_rw(); }
double Atom::embr_real_pot_near_r(size_t pos) const{ return _embr_real[pos].pot_near_r(); }
unsigned int Atom::embr_real_pot_near_tap(size_t pos) const{ return _embr_real[pos].pot_near_tap(); }
bool Atom::embr_real_pot_near_sw(size_t pos) const{ return _embr_real[pos].pot_near_sw(); }
bool Atom::embr_real_pot_near_swA(size_t pos) const{ return _embr_real[pos].pot_near_swA(); }
double Atom::embr_real_pot_far_rab(size_t pos) const{ return _embr_real[pos].pot_far_rab(); }
double Atom::embr_real_pot_far_rw(size_t pos) const{ return _embr_real[pos].pot_far_rw(); }
double Atom::embr_real_pot_far_r(size_t pos) const{ return _embr_real[pos].pot_far_r(); }
unsigned int Atom::embr_real_pot_far_tap(size_t pos) const{ return _embr_real[pos].pot_far_tap(); }
bool Atom::embr_real_pot_far_sw(size_t pos) const{ return _embr_real[pos].pot_far_sw(); }
bool Atom::embr_real_pot_far_swA(size_t pos) const{ return _embr_real[pos].pot_far_swA(); }

bool Atom::embr_imag_sw_sc(size_t pos) const{ return _embr_imag[pos].sw_sc(); }
bool Atom::embr_imag_sw_mid(size_t pos) const{ return _embr_imag[pos].sw_mid(); }
double Atom::embr_imag_pot_near_rab(size_t pos) const{ return _embr_imag[pos].pot_near_rab(); }
double Atom::embr_imag_pot_near_rw(size_t pos) const{ return _embr_imag[pos].pot_near_rw(); }
double Atom::embr_imag_pot_near_r(size_t pos) const{ return _embr_imag[pos].pot_near_r(); }
unsigned int Atom::embr_imag_pot_near_tap(size_t pos) const{ return _embr_imag[pos].pot_near_tap(); }
bool Atom::embr_imag_pot_near_sw(size_t pos) const{ return _embr_imag[pos].pot_near_sw(); }
bool Atom::embr_imag_pot_near_swA(size_t pos) const{ return _embr_imag[pos].pot_near_swA(); }
double Atom::embr_imag_pot_far_rab(size_t pos) const{ return _embr_imag[pos].pot_far_rab(); }
double Atom::embr_imag_pot_far_rw(size_t pos) const{ return _embr_imag[pos].pot_far_rw(); }
double Atom::embr_imag_pot_far_r(size_t pos) const{ return _embr_imag[pos].pot_far_r(); }
unsigned int Atom::embr_imag_pot_far_tap(size_t pos) const{ return _embr_imag[pos].pot_far_tap(); }
bool Atom::embr_imag_pot_far_sw(size_t pos) const{ return _embr_imag[pos].pot_far_sw(); }
bool Atom::embr_imag_pot_far_swA(size_t pos) const{ return _embr_imag[pos].pot_far_swA(); }