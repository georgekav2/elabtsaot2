/*!
\file encoder.h
\brief Definition file for namespace encoder

This class is part of the elab-tsaot project in the Electronics Laboratory of
the Ecole Polytechnique Federal de Lausanne.

\author Theodoros Kyriakidis, thekyria at gmail dot com, Electronics Laboratory
EPFL
*/

#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <stdint.h>

namespace elabtsaot{

class Slice;
class Emulator;
class Scenario;

//!  Encoder namespace
/*!
  Namespace encompassing encoder functions. The task of the encoder is to encode
  a configuration of an emulator slice (FPGA and analog elements) into a stream
  that can be understood be the emulator FPGA.

  In turn the latter parses this stream, generated by the encoder and received
  via the USB interface, and according to it, it configures the physical
  electrical elements that constitute the hardware emulated version of the power
  system.

  Conceptual flow:
  - Real world power system representation
  - Software mapping onto the emulator topology
  - Encoding of the emulator-mapped power system into a stream *
  - Communication of the stream via the USB interface
  - Hardware takes action from then on .. (configures its analog & digital
    elements etc)

  * where the encoder functions operates

  \author thekyria
  \date March 2012
*/
namespace encoder{


//! Performs the encoding of one of the slices of the emulator hardware into a
//! bitstream suitable for PF analysis using the method of Guillaume Lanz
int encodeSlicePF(const Slice &sl, std::vector<uint32_t>& sliceConf);
//! Performs the encoding of one of the slices of the emulator hardware into a
//! bitstream suitable for TD emulation
int encodeSliceTD( Slice const& sl, std::vector<uint32_t>& sliceConf );

//! Stamps (bitwise OR) an unsigned 32 bit integer word with the confirm stamp
void stamp_NIOS_confirm( uint32_t& word, uint32_t confirm_stamp = 0xC0000000 );
//! Stamps (bitwise OR) an signed 32 bit integer word with the confirm stamp
void stamp_NIOS_confirm( int32_t& word, uint32_t confirm_stamp = 0xC0000000 );

namespace detail{

// General encoding functions
void encode_atomsGot(Slice const& sl, std::vector<uint32_t>& got_conf);
int encode_vref( Slice const& sl, std::vector<uint32_t>& vref_conf );
int encode_resistors( Slice const& sl, std::vector<uint32_t>& res_conf, std::vector<uint32_t>& res_tcon_conf );
int encode_switches( Slice const& sl, std::vector<uint32_t>& switches_conf );

// PF specific encoding functions
int encode_PFgot( Slice const& sl, std::vector<uint32_t>& got_conf );
int encode_PFpositions( Slice const& sl, std::vector<uint32_t>& pos_conf, std::vector<uint32_t>& slpos_conf );
void encode_PFauxiliary( Slice const& sl,
                         std::vector<uint32_t>& conf_conf,
                         std::vector<uint32_t>& starter_conf,
                         std::vector<uint32_t>& nios_conf );
void encode_PFIinit( Slice const& sl, std::vector<uint32_t>& icar_conf, std::vector<uint32_t>& ipol_conf );
void encode_PQsetpoints( Slice const& sl, std::vector<uint32_t>& pqset_conf );

// TD specific encoding functions
int encode_TDgenerators( Slice const& sl,
                         std::vector<uint32_t>& ggot_conf,
                         std::vector<uint32_t>& gen_conf1,
                         std::vector<uint32_t>& gen_conf2,
                         std::vector<uint32_t>& gen_conf3 );
int encode_TDzloads( Slice const& sl, std::vector<uint32_t>& zgot_conf, std::vector<uint32_t>& zloads_conf );
int encode_TDiloads( Slice const& sl, std::vector<uint32_t>& igot_conf, std::vector<uint32_t>& iloads_conf );
int encode_TDploads( Slice const& sl, std::vector<uint32_t>& ploads_conf );
int encode_TDpositions( Slice const& sl, std::vector<uint32_t>& pos_conf );
int encode_TDauxiliary( Slice const& sl, std::vector<uint32_t>& pert_conf );

/*! Creates a 32 bit fixed point arithmetic word
  The result is written at the position pointed to by the pointer argument
  pword.

  Example for creating a signed Q3.10 number:
    int32_t temp;
    form_word( floatval, 13, 10, true, &temp );

  \param val floating point (double) number to be converted to fixed point
  \param total_bits total number of bits; X+Y in Q number format QX.Y
  \param decimal_bits number of decimal bits; Y in Q number format QX.Y
  \param isSigned flag; if true number created is signed (2's complement)
  \param pword output argument; pointer to the created 32bit fixed point
               number: [padding_with_0s][total-decimal_bits].[decimal_bits]

  \return integer exit code; 0 when successful
*/
int form_word( double val,
               size_t total_bits,
               size_t decimal_bits,
               bool isSigned,
               int32_t* pword);
//! Creates a 64 bit fixed point arithmetic word
/*!
  The result is written at the position pointed to by the pointer argument
  pword.

  Example for creating a signed Q13.23 number:
    int64_t temp64;
    form_word( floatval, 36, 23, true, &temp64 );

  \param val floating point (double) number to be converted to fixed point
  \param total_bits total number of bits; X+Y in Q number format QX.Y
  \param decimal_bits number of decimal bits; Y in Q number format QX.Y
  \param isSigned flag; if true number created is signed (2's complement)
  \param pword output argument; pointer to the created 32bit fixed point
               number: [padding_with_0s][total-decimal_bits].[decimal_bits]

  \return integer exit code; 0 when successful
*/
int form_word( double val,
               size_t total_bits,
               size_t decimal_bits,
               bool isSigned,
               int64_t* pword);

} // end of namespace encoder::detail

} // end of namespace encoder

} // end of namespace elabtsaot

#endif // ENCODER_H
