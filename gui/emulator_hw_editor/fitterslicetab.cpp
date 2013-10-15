
#include "fitterslicetab.h"
using namespace elabtsaot;

#include "slice.h"

#include "fitterslicewidget.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QSizePolicy>
#include <QTabWidget>
#include <QFrame>
#include <QFormLayout>
#include <QTableWidget>
#include <QStringList>
#include <QToolBox>

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <complex>
using std::complex;
//#include <utility>
using std::pair;

FitterSliceTab::FitterSliceTab( Slice* slc, QWidget* parent) :
    QSplitter(Qt::Horizontal, parent), _slc(slc){

  // Global slice parameters box
  _init_globalParamsBox(); // initializes globalParamsBox and handles connects
  globalParamsBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
//  globalParamsBox->setMaximumWidth( globalParamsBox->minimumSizeHint().width() );

  // Pipelines pane
  _init_pipeBox(); // initializes pipeBox and handles connects
  pipeBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
//  pipeBox->setMaximumWidth( pipeBox->minimumSizeHint().width() );

  // Left-side toolbox
  QToolBox* toolbox = new QToolBox( this );
  toolbox->setSizePolicy( QSizePolicy::Minimum,QSizePolicy::Preferred );
  addWidget( toolbox );
  toolbox->addItem( globalParamsBox, QString("Slice params") );
  toolbox->addItem( pipeBox, QString("Pipelines") );

  int width = 0;
  for ( int k = 0 ; k != toolbox->count() ; ++k )
    if ( toolbox->widget(k)->minimumSizeHint().width() > width )
      width = toolbox->widget(k)->minimumSizeHint().width();
  toolbox->setMaximumWidth( width );

  // Core fitter slice widget
  _sliceCore = new FitterSliceWidget( _slc, this );
  addWidget( _sliceCore );
  connect( _sliceCore, SIGNAL(fittingPositionClicked(int,int,int,int)),
           this, SIGNAL(fittingPositionClicked(int,int,int,int)) );

  // Update view
  updt();

}

void FitterSliceTab::updt(){

  _sliceCore->update();

  // ----------------- Update global slice parameters -----------------
  gotGainForm->setValue( _slc->ana.got_gain() );
  gotOffsetForm->setValue( _slc->ana.got_offset() );
//  realVrefValForm->setValue( _slc->real_voltage_ref_val() );
  realVrefTapForm->setValue( _slc->ana.real_voltage_ref_tap() ); // updates also realVrefValForm
//  imagVrefValForm->setValue( _slc->imag_voltage_ref_val() );
  imagVrefTapForm->setValue( _slc->ana.imag_voltage_ref_tap() ); // updates also imagVrefTapForm

  // ----------------- Update pipelines -----------------
  // Generators pipeline
  vector< pair<int,int> > pos = _slc->dig.pipe_gen.position();
  vector<double> xd = _slc->dig.pipe_gen.xd_1_inverse();
  vector< complex<double> > i0 = _slc->dig.pipe_gen.i_ss();
  vector<double> pMech = _slc->dig.pipe_gen.p_mechanical();
  vector<double> g1 = _slc->dig.pipe_gen.gain_1();
  vector<double> g2 = _slc->dig.pipe_gen.gain_2();
  vector<double> g3 = _slc->dig.pipe_gen.gain_3();
  vector<double> g4 = _slc->dig.pipe_gen.gain_4();
  vector<double> g5 = _slc->dig.pipe_gen.gain_5();
  vector<double> g6 = _slc->dig.pipe_gen.gain_6();
  vector<double> pa0 = _slc->dig.pipe_gen.pa_0();
  vector<double> w0 = _slc->dig.pipe_gen.omega_0();
  vector<double> d0 = _slc->dig.pipe_gen.delta_0();
  for( size_t i = 0; i != pos.size(); ++i ){
    genPipeTable->setItem( i,0, new QTableWidgetItem( QString("%0").arg(pos[i].first) ) );
    genPipeTable->setItem( i,1, new QTableWidgetItem( QString("%0").arg(pos[i].second) ) );
    genPipeTable->setItem( i,2, new QTableWidgetItem( QString("%0").arg(xd[i]) ) );
    genPipeTable->setItem( i,3, new QTableWidgetItem( QString("%0").arg(i0[i].real()) ) );
    genPipeTable->setItem( i,4, new QTableWidgetItem( QString("%0").arg(i0[i].imag()) ) );
    genPipeTable->setItem( i,5, new QTableWidgetItem( QString("%0").arg(pMech[i]) ) );
    genPipeTable->setItem( i,6, new QTableWidgetItem( QString("%0").arg(g1[i]) ) );
    genPipeTable->setItem( i,7, new QTableWidgetItem( QString("%0").arg(g2[i]) ) );
    genPipeTable->setItem( i,8, new QTableWidgetItem( QString("%0").arg(g3[i]) ) );
    genPipeTable->setItem( i,9, new QTableWidgetItem( QString("%0").arg(g4[i]) ) );
    genPipeTable->setItem( i,10, new QTableWidgetItem( QString("%0").arg(g5[i]) ) );
    genPipeTable->setItem( i,11, new QTableWidgetItem( QString("%0").arg(g6[i]) ) );
    genPipeTable->setItem( i,12, new QTableWidgetItem( QString("%0").arg(pa0[i]) ) );
    genPipeTable->setItem( i,13, new QTableWidgetItem( QString("%0").arg(w0[i]) ) );
    genPipeTable->setItem( i,14, new QTableWidgetItem( QString("%0").arg(d0[i]) ) );
  }

  // Constant Z load pipeline
  pos = _slc->dig.pipe_zload.position();
  vector<double> real = _slc->dig.pipe_zload.real_Y();
  vector<double> imag = _slc->dig.pipe_zload.imag_Y();
  for( size_t i = 0; i != pos.size(); ++i ){
    zloadPipeTable->setItem( i,0, new QTableWidgetItem( QString("%0").arg(pos[i].first) ) );
    zloadPipeTable->setItem( i,1, new QTableWidgetItem( QString("%0").arg(pos[i].second) ) );
    zloadPipeTable->setItem( i,2, new QTableWidgetItem( QString("%0").arg(real[i]) ) );
    zloadPipeTable->setItem( i,3, new QTableWidgetItem( QString("%0").arg(imag[i]) ) );
  }

  // Constant I load pipeline
  pos = _slc->dig.pipe_iload.position();
  real = _slc->dig.pipe_iload.real_I();
  imag = _slc->dig.pipe_iload.imag_I();
  for( size_t i = 0 ; i != pos.size(); ++i ){
    iloadPipeTable->setItem( i,0, new QTableWidgetItem( QString("%0").arg(pos[i].first) ) );
    iloadPipeTable->setItem( i,1, new QTableWidgetItem( QString("%0").arg(pos[i].second) ) );
    iloadPipeTable->setItem( i,2, new QTableWidgetItem( QString("%0").arg(real[i]) ) );
    iloadPipeTable->setItem( i,3, new QTableWidgetItem( QString("%0").arg(imag[i]) ) );
  }

  // Constant P load pipeline
  pos = _slc->dig.pipe_pload.position();
  real = _slc->dig.pipe_pload.P();
  imag = _slc->dig.pipe_pload.Q();
  for( size_t i = 0 ; i != pos.size(); ++i ){
    ploadPipeTable->setItem( i,0, new QTableWidgetItem( QString("%0").arg(pos[i].first) ) );
    ploadPipeTable->setItem( i,1, new QTableWidgetItem( QString("%0").arg(pos[i].second) ) );
    ploadPipeTable->setItem( i,2, new QTableWidgetItem( QString("%0").arg(real[i]) ) );
    ploadPipeTable->setItem( i,3, new QTableWidgetItem( QString("%0").arg(imag[i]) ) );
  }

}

bool FitterSliceTab::isShowingReal() const{return _sliceCore->isShowingReal();}

void FitterSliceTab::gotGainSlot(double val){
  _slc->ana.set_got_gain( val );
}

void FitterSliceTab::gotOffsetSlot(double val){
  _slc->ana.set_got_offset( val );
}

void FitterSliceTab::realVrefValSlot(double val){
  // The following creates a cyclic dependency with the realVrefTapSlot slot
//  _slc->set_real_voltage_ref_val( val );
//  realVrefValForm->setValue( _slc->real_voltage_ref_val() );
//  realVrefTapForm->setValue( _slc->ana.real_voltage_ref_tap() );
}

void FitterSliceTab::realVrefTapSlot(int tap){
  _slc->ana.set_real_voltage_ref_tap( static_cast<unsigned int>(tap) );
  realVrefValForm->setValue( _slc->ana.real_voltage_ref_val() );
}

void FitterSliceTab::imagVrefValSlot(double val){
  // The following creates a cyclic dependency with the imagVrefTapSlot slot
//  _slc->set_imag_voltage_ref_val( val );
//  imagVrefValForm->setValue( _slc->imag_voltage_ref_val() );
//  imagVrefTapForm->setValue( _slc->ana.imag_voltage_ref_tap() );
}

void FitterSliceTab::imagVrefTapSlot(int tap){
  _slc->ana.set_imag_voltage_ref_tap( static_cast<unsigned int>(tap) );
  imagVrefValForm->setValue( _slc->ana.imag_voltage_ref_val() );
}

void FitterSliceTab::genPipeSlot(int row, int col){

  complex<double> i_0;

  // The first two rows (the position of the gen) are not editable
  switch ( col ){
  case 0:
    genPipeTable->item( row,0 )->setText( QString("%0").arg(_slc->dig.pipe_gen.position().at(row).first) );
    break;

  case 1:
    genPipeTable->item( row,1 )->setText( QString("%0").arg(_slc->dig.pipe_gen.position().at(row).second) );
    break;

  // The rest of the columns are editable; so, write changes to the model
  case 2:
    _slc->dig.pipe_gen.set_xd_1_inverse( row, genPipeTable->item(row,2)->text().toDouble() );
    break;

  case 3:
    i_0 = complex<double>( genPipeTable->item(row,3)->text().toDouble(),
                           _slc->dig.pipe_gen.i_ss().at(row).imag() );
    _slc->dig.pipe_gen.set_i_0( row, i_0 );
    break;

  case 4:
    i_0 = complex<double>( _slc->dig.pipe_gen.i_ss().at(row).real(),
                           genPipeTable->item(row,4)->text().toDouble() );
    _slc->dig.pipe_gen.set_i_0( row, i_0 );
    break;

  case 5:
    _slc->dig.pipe_gen.set_p_mechanical( row,genPipeTable->item(row,5)->text().toDouble() );
    break;

  case 6:
    _slc->dig.pipe_gen.set_gain_1( row, genPipeTable->item(row,6)->text().toDouble() );
    break;

  case 7:
    _slc->dig.pipe_gen.set_gain_2( row, genPipeTable->item(row,7)->text().toDouble() );
    break;

  case 8:
    _slc->dig.pipe_gen.set_gain_3( row, genPipeTable->item(row,8)->text().toDouble() );
    break;

  case 9:
    _slc->dig.pipe_gen.set_gain_4( row, genPipeTable->item(row,9)->text().toDouble() );
    break;

  case 10:
    _slc->dig.pipe_gen.set_gain_5( row, genPipeTable->item(row,10)->text().toDouble() );
    break;

  case 11:
    _slc->dig.pipe_gen.set_gain_6( row, genPipeTable->item(row,11)->text().toDouble() );
    break;

  case 12:
    _slc->dig.pipe_gen.set_pa_0( row, genPipeTable->item(row,12)->text().toDouble() );
    break;

  case 13:
    _slc->dig.pipe_gen.set_omega_0( row, genPipeTable->item(row,13)->text().toDouble() );
    break;

  case 14:
    _slc->dig.pipe_gen.set_delta_0( row, genPipeTable->item(row,14)->text().toDouble() );
    break;

  default:
    // Do nothing! Normally shouldn't be reached!
    break;
  }

  return;
}

void FitterSliceTab::zloadPipeSlot(int row, int col){

  // The first two rows (the position of the load) are not editable
  switch (col){
  case 0:
    zloadPipeTable->item(row,0)->setText( QString("%0").arg(_slc->dig.pipe_zload.position().at(row).first) );
    break;

  case 1:
    zloadPipeTable->item(row,1)->setText( QString("%0").arg(_slc->dig.pipe_zload.position().at(row).second) );
    break;

  // The rest of the columns are editable; so, write changes to the model
  case 2:
    _slc->dig.pipe_zload.set_real_Y( row, zloadPipeTable->item(row,2)->text().toDouble() );
    break;

  case 3:
    _slc->dig.pipe_zload.set_imag_Y( row, zloadPipeTable->item(row,3)->text().toDouble() );
    break;

  default:
    // Do nothing! Normally shouldn't be reached!
    break;
  }

  return;
}

void FitterSliceTab::iloadPipeSlot(int row, int col){

  // The first two rows (the position of the load) are not editable
  switch (col){
  case 0:
    iloadPipeTable->item(row,0)->setText( QString("%0").arg(_slc->dig.pipe_iload.position().at(row).first) );
    break;

  case 1:
    iloadPipeTable->item(row,1)->setText( QString("%0").arg(_slc->dig.pipe_iload.position().at(row).second) );
    break;

  // The rest of the columns are editable; so, write changes to the model
  case 2:
    _slc->dig.pipe_iload.set_real_I( row, iloadPipeTable->item(row,2)->text().toDouble() );
    break;

  case 3:
    _slc->dig.pipe_iload.set_imag_I( row, iloadPipeTable->item(row,3)->text().toDouble() );
    break;

  default:
    // Do nothing! Normally shouldn't be reached!
    break;
  }

  return;
}

void FitterSliceTab::ploadPipeSlot(int row, int col){

  // The first two rows (the position of the load) are not editable
  switch (col){
  case 0:
    ploadPipeTable->item(row,0)->setText( QString("%0").arg(_slc->dig.pipe_pload.position().at(row).first) );
    break;

  case 1:
    ploadPipeTable->item(row,1)->setText( QString("%0").arg(_slc->dig.pipe_pload.position().at(row).second) );
    break;

  // The rest of the columns are editable; so, write changes to the model
  case 2:
    _slc->dig.pipe_pload.set_P( row, ploadPipeTable->item(row,2)->text().toDouble() );
    break;

  case 3:
    _slc->dig.pipe_pload.set_Q( row, ploadPipeTable->item(row,3)->text().toDouble() );
    break;

  default:
    // Do nothing! Normally shouldn't be reached!
    break;
  }

  return;
}

void FitterSliceTab::toggleReal(){  _sliceCore->toggleReal(); }
void FitterSliceTab::toggleReal(bool real){ _sliceCore->toggleReal(real); }
void FitterSliceTab::zoomIn(){ _sliceCore->zoomIn(); }
void FitterSliceTab::zoomOut(){ _sliceCore->zoomOut(); }
void FitterSliceTab::zoomFit(){ _sliceCore->zoomFit(); }

void FitterSliceTab::_init_globalParamsBox(){

  globalParamsBox = new QGroupBox("Global slice parameters", this);
  QFormLayout* globalParamsLay = new QFormLayout(globalParamsBox);
  globalParamsBox->setLayout( globalParamsLay );

  // GOT gain
  QLabel* gotGainLabel = new QLabel("GOT gain");
  gotGainForm = new QDoubleSpinBox();
  gotGainForm->setDecimals(2);
  gotGainForm->setSingleStep(0.01);
  globalParamsLay->addRow( gotGainLabel, gotGainForm );

  // GOT offset
  QLabel* gotOffsetLabel = new QLabel("GOT offset");
  gotOffsetForm = new QDoubleSpinBox();
  gotOffsetForm->setDecimals(2);
  gotOffsetForm->setRange(0.0, 5.0);
  gotOffsetForm->setSingleStep(0.01);
  globalParamsLay->addRow( gotOffsetLabel, gotOffsetForm );

  // Real V ref val
  QLabel* realVrefValLabel = new QLabel("Real Vref val");
  realVrefValForm = new QDoubleSpinBox();
  realVrefValForm->setDecimals( 4 );
  realVrefValForm->setRange( _slc->ana.real_voltage_ref_val_min(),
                             _slc->ana.real_voltage_ref_val_max() );
  realVrefValForm->setSingleStep(0.01);
  globalParamsLay->addRow( realVrefValLabel, realVrefValForm );
  realVrefValForm->setEnabled( false );

  // Real V ref tap
  QLabel* realVrefTapLabel = new QLabel("Real Vref tap");
  realVrefTapForm = new QSpinBox();
  realVrefTapForm->setRange( 0, _slc->ana.real_voltage_ref_tap_max() );
  globalParamsLay->addRow( realVrefTapLabel, realVrefTapForm );

  // Imag V ref val
  QLabel* imagVrefValLabel = new QLabel("Imag Vref val");
  imagVrefValForm = new QDoubleSpinBox();
  imagVrefValForm->setDecimals( 4 );
  imagVrefValForm->setRange( _slc->ana.imag_voltage_ref_val_min(),
                             _slc->ana.imag_voltage_ref_val_max() );
  imagVrefValForm->setSingleStep(0.01);
  globalParamsLay->addRow( imagVrefValLabel, imagVrefValForm );
  imagVrefValForm->setEnabled( false );

  // Imag V ref tap
  QLabel* imagVrefTapLabel = new QLabel("Imag Vref tap");
  imagVrefTapForm = new QSpinBox();
  imagVrefTapForm->setRange( 0, _slc->ana.imag_voltage_ref_tap_max() );
  globalParamsLay->addRow( imagVrefTapLabel, imagVrefTapForm );

  // ----------------- Connect signals -----------------
  connect( gotGainForm, SIGNAL(valueChanged(double)),
           this, SLOT(gotGainSlot(double)) );
  connect( gotOffsetForm, SIGNAL(valueChanged(double)),
           this, SLOT(gotOffsetSlot(double)) );
  connect( realVrefValForm, SIGNAL(valueChanged(double)),
           this, SLOT(realVrefValSlot(double)) );
  connect( realVrefTapForm, SIGNAL(valueChanged(int)),
           this, SLOT(realVrefTapSlot(int)) );
  connect( imagVrefValForm, SIGNAL(valueChanged(double)),
           this, SLOT(imagVrefValSlot(double)) );
  connect( imagVrefTapForm, SIGNAL(valueChanged(int)),
           this, SLOT(imagVrefTapSlot(int)) );

  return;
}

void FitterSliceTab::_init_pipeBox(){

  pipeBox = new QGroupBox("Pipelines", this);
  QVBoxLayout* pipeBoxLay = new QVBoxLayout( pipeBox );
  pipeBox->setLayout( pipeBoxLay );
  QTabWidget* pipeTabWidget = new QTabWidget();
  pipeBoxLay->addWidget( pipeTabWidget );

  // Generator pipeline
  genPipeTable = new QTableWidget( _slc->dig.pipe_gen.element_count_max(), 15 );
  pipeTabWidget->addTab( genPipeTable, "Generators" );
  QStringList genPipeLabels = QStringList();
  genPipeLabels.push_back("X Pos");
  genPipeLabels.push_back("Y Pos");
  genPipeLabels.push_back("Xd 1 Inv");
  genPipeLabels.push_back("Re(I0)");
  genPipeLabels.push_back("Im(I0)");
  genPipeLabels.push_back("P Mech");
  genPipeLabels.push_back("Gain 1");
  genPipeLabels.push_back("Gain 2");
  genPipeLabels.push_back("Gain 3");
  genPipeLabels.push_back("Gain 4");
  genPipeLabels.push_back("Gain 5");
  genPipeLabels.push_back("Gain 6");
  genPipeLabels.push_back("Acc. Power");
  genPipeLabels.push_back("Omega 0");
  genPipeLabels.push_back("Delta 0");
  genPipeTable->setHorizontalHeaderLabels( genPipeLabels );
  genPipeTable->resizeColumnsToContents();
  genPipeTable->resizeRowsToContents();

  // Constant Z load pipeline
  zloadPipeTable = new QTableWidget( _slc->dig.pipe_zload.element_count_max(), 4 );
  pipeTabWidget->addTab( zloadPipeTable, "Const. Z loads" );
  QStringList zloadPipeLabels = QStringList();
  zloadPipeLabels.push_back("X Pos");
  zloadPipeLabels.push_back("Y Pos");
  zloadPipeLabels.push_back("Re(Z)");
  zloadPipeLabels.push_back("Im(Z)");
  zloadPipeTable->setHorizontalHeaderLabels( zloadPipeLabels );
  zloadPipeTable->resizeColumnsToContents();
  zloadPipeTable->resizeRowsToContents();

  // Constant I load pipeline
  iloadPipeTable = new QTableWidget( _slc->dig.pipe_iload.element_count_max(), 4 );
  pipeTabWidget->addTab( iloadPipeTable, "Const. I loads" );
  QStringList iloadPipeLabels = QStringList();
  iloadPipeLabels.push_back("X Pos");
  iloadPipeLabels.push_back("Y Pos");
  iloadPipeLabels.push_back("Re(I)");
  iloadPipeLabels.push_back("Im(I)");
  iloadPipeTable->setHorizontalHeaderLabels( iloadPipeLabels );
  iloadPipeTable->resizeColumnsToContents();
  iloadPipeTable->resizeRowsToContents();

  // Constant P load pipeline
  ploadPipeTable = new QTableWidget( _slc->dig.pipe_iload.element_count_max(), 4 );
  pipeTabWidget->addTab( ploadPipeTable, "Const. P loads" );
  QStringList ploadPipeLabels = QStringList();
  ploadPipeLabels.push_back("X Pos");
  ploadPipeLabels.push_back("Y Pos");
  ploadPipeLabels.push_back("P");
  ploadPipeLabels.push_back("Q");
  ploadPipeTable->setHorizontalHeaderLabels( ploadPipeLabels );
  ploadPipeTable->resizeColumnsToContents();
  ploadPipeTable->resizeRowsToContents();

  // ----------------- Connect signals -----------------
  connect( genPipeTable, SIGNAL(cellChanged(int,int)),
           this, SLOT(genPipeSlot(int,int)) );
  connect( zloadPipeTable, SIGNAL(cellChanged(int,int)),
           this, SLOT(zloadPipeSlot(int,int)) );
  connect( iloadPipeTable, SIGNAL(cellChanged(int,int)),
           this, SLOT(iloadPipeSlot(int,int)) );
  connect( ploadPipeTable, SIGNAL(cellChanged(int,int)),
           this, SLOT(ploadPipeSlot(int,int)) );

  return;
}