
#include "fittereditor.h"
using namespace elabtsaot;

#include "powersystem.h"
#include "auxiliary.h"
#include "emulator.h"

#include "guiauxiliary.h"
#include "fitteremulatoreditor.h"
#include "nodedialog.h"
#include "emulatorbranchdialog.h"

#include <QToolBar>
#include <QAction>

#include <ctime>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;

FitterEditor::FitterEditor( Emulator* emu, QWidget* parent ) :
    QSplitter(Qt::Vertical, parent), _emu(emu), _emuhw(emu->emuhw()){

  // -- Emulator editor (fitter edition) --
  // fee does not exist, so create it
  fee = new FitterEmulatorEditor( _emuhw, this );
  this->addWidget( fee );
  connect( fee, SIGNAL(fittingPositionClicked(int,int,int,int)),
           this, SLOT(fittingPositionClickedSlot(int,int,int,int)) );

  // -- Fitter emulator toolbar --
  QToolBar* fitToolbar = new QToolBar("Fitter editor toolbar", this);
  this->addWidget( fitToolbar );
  fitToolbar->setFixedHeight(30);

  // Update view act
  QAction* updtAct = new QAction( QIcon(":/images/update.png"),
                                  "Update fitter editor view", fitToolbar );
  fitToolbar->addAction( updtAct );
  connect( updtAct, SIGNAL(triggered()),
           this, SLOT(updt()) );

  fitToolbar->addSeparator(); // -- Separator

  // Reset fitting (GPF/TD) act
  QAction* resetFittingTDAct = new QAction( QIcon(":/images/reset.png"),
                                            "Reset fitting (GPF/TD)", fitToolbar );
  fitToolbar->addAction(resetFittingTDAct);
  connect( resetFittingTDAct, SIGNAL(triggered()), this, SLOT(resetFittingTDSlot()) );

  // Reset fitting (DCPF) act
  QAction* resetFittingDCPFAct = new QAction( QIcon(":/images/reset.png"),
                                              "Reset fitting (DCPF)", fitToolbar );
  fitToolbar->addAction(resetFittingDCPFAct);
  connect( resetFittingDCPFAct, SIGNAL(triggered()), this, SLOT(resetFittingDCPFSlot()) );

  // Auto fitting (GPF) act
  QAction* autoFittingGPFAct = new QAction( QIcon(":/images/execute.png"),
                                            "Auto fitting (GPF)", fitToolbar );
  fitToolbar->addAction(autoFittingGPFAct);
  connect(autoFittingGPFAct, SIGNAL(triggered()), this, SLOT(autoFittingGPFSlot()));

  // Auto fitting (DCPF) act
  QAction* autoFittingDCPFAct = new QAction( QIcon(":/images/execute.png"),
                                             "Auto fitting (DCPF)", fitToolbar );
  fitToolbar->addAction(autoFittingDCPFAct);
  connect(autoFittingDCPFAct, SIGNAL(triggered()), this, SLOT(autoFittingDCPFSlot()));

  // Auto fitting (TD) act
  QAction* autoFittingTDAct = new QAction( QIcon(":/images/execute.png"),
                                           "Auto fitting (TD)", fitToolbar );
  fitToolbar->addAction(autoFittingTDAct);
  connect(autoFittingTDAct, SIGNAL(triggered()), this, SLOT(autoFittingTDSlot()));

  // Validate fitting (GPF) act
  QAction* validateFittingGPFAct = new QAction( QIcon(":/images/validate.png"),
                                               "Validate fitting (GPF)", fitToolbar );
  fitToolbar->addAction(validateFittingGPFAct);
  connect(validateFittingGPFAct, SIGNAL(triggered()), this, SLOT(validateFittingGPFSlot()));

  // Validate fitting (DCPF) act
  QAction* validateFittingDCPFAct = new QAction( QIcon(":/images/validate.png"),
                                                 "Validate fitting (DCPF)", fitToolbar );
  fitToolbar->addAction(validateFittingDCPFAct);
  connect(validateFittingDCPFAct, SIGNAL(triggered()), this, SLOT(validateFittingDCPFSlot()));

  // Validate fitting (TD) act
  QAction* validateFittingTDAct = new QAction( QIcon(":/images/validate.png"),
                                               "Validate fitting (TD)", fitToolbar );
  fitToolbar->addAction(validateFittingTDAct);
  connect( validateFittingTDAct, SIGNAL(triggered()), this, SLOT(validateFittingTDSlot()) );

  fitToolbar->addSeparator(); // -- Separator

  // Toggle Re/Im act
  QAction* toggleRealAct = new QAction( QIcon(":/images/reim.png"),
                                        "Toggle Re/Im", fitToolbar );
  fitToolbar->addAction(toggleRealAct);
  connect( toggleRealAct, SIGNAL(triggered()), fee, SLOT(toggleReal()) );

  // Zoom in act (button)
  QAction* zoomInAct = new QAction( QIcon(":/images/zoom-in.png"),
                                    "Zoom in", fitToolbar );
  fitToolbar->addAction(zoomInAct);
  connect( zoomInAct, SIGNAL(triggered()), fee, SLOT(zoomIn()) );

  // Zoom out act (button)
  QAction* zoomOutAct = new QAction( QIcon(":/images/zoom-out.png"),
                                     "Zoom out", fitToolbar );
  fitToolbar->addAction(zoomOutAct);
  connect( zoomOutAct, SIGNAL(triggered()), fee, SLOT(zoomOut()) );

  // Zoom fit act (button)
  QAction* zoomFitAct = new QAction( QIcon(":/images/zoom-fit.png"),
                                     "Zoom fit", fitToolbar );
  fitToolbar->addAction(zoomFitAct);
  connect( zoomFitAct, SIGNAL(triggered()), fee, SLOT(zoomFit()) );
}

int FitterEditor::init(){ return fee->init(); }

void FitterEditor::updt(){ return fee->updt(); }

void FitterEditor::resetFittingTDSlot(){
  int ans = _emu->reset(false,EMU_OPTYPE_TD);
  if (ans) cout << "Reset the emulator (GPF/TD) failed with code: " << ans << endl;
  else     cout << "Reseting the emulator (GPF/TD) succeded" << endl;
  fee->updt();
}

void FitterEditor::resetFittingDCPFSlot(){
  int ans = _emu->reset(false,EMU_OPTYPE_DCPF);
  if (ans) cout << "Reset the emulator (DCPF) failed with code: " << ans << endl;
  else     cout << "Reseting the emulator (DCPF) succeded" << endl;
  fee->updt();
}

void FitterEditor::autoFittingGPFSlot(){
  vector<string> outputMsg;
  int ans = _emu->autoFitting(EMU_OPTYPE_GPF, &outputMsg);
  if ( ans ){
    cout << "Auto-fitting (GPF) failed with code " << ans << endl;
  } else {
    cout << "Auto-fitting (GPF) succeeded!" << endl;
    fee->updt();
  }
  cout << "Auto-fitting (GPF) warning messages: " << endl;
  for (size_t k=0; k!=outputMsg.size(); ++k)
    cout << outputMsg[k] << endl;
}

void FitterEditor::autoFittingDCPFSlot(){
  vector<string> outputMsg;
  int ans = _emu->autoFitting(EMU_OPTYPE_DCPF, &outputMsg);
  if ( ans ){
    cout << "Auto-fitting (DCPF) failed with code " << ans << endl;
  } else {
    cout << "Auto-fitting (DCPF) succeeded!" << endl;
    fee->updt();
  }
  cout << "Auto-fitting (DCPF) warning messages: " << endl;
  for (size_t k=0; k!=outputMsg.size(); ++k)
    cout << outputMsg[k] << endl;
}


void FitterEditor::autoFittingTDSlot(){
  vector<string> outputMsg;
  int ans = _emu->autoFitting(EMU_OPTYPE_TD, &outputMsg);
  if ( ans ){
    cout << "Auto-fitting (TD) failed with code " << ans << endl;
  } else {
    cout << "Auto-fitting (TD) succeeded!" << endl;
    fee->updt();
  }
  cout << "Auto-fitting (TD) warning messages: " << endl;
  for (size_t k=0; k!=outputMsg.size(); ++k)
    cout << outputMsg[k] << endl;
}

void FitterEditor::validateFittingGPFSlot(){
  int ans = _emu->validateFitting(EMU_OPTYPE_GPF);
  if (ans) cout << "Validate fitting (GPF) failed with code " << ans << endl;
  else cout << "Validate fitting (GPF) succeeded!" << endl;
}

void FitterEditor::validateFittingDCPFSlot(){
  int ans = _emu->validateFitting(EMU_OPTYPE_DCPF);
  if (ans) cout << "Validate fitting (DCPF) failed with code " << ans << endl;
  else cout << "Validate fitting (DCPF) succeeded!" << endl;
}

void FitterEditor::validateFittingTDSlot(){
  int ans = _emu->validateFitting(EMU_OPTYPE_TD);
  if (ans) cout << "Validate fitting (TD) failed with code " << ans << endl;
  else cout << "Validate fitting (TD) succeeded!" << endl;
}

void FitterEditor::fittingPositionClickedSlot(int emulator_tab,
                                              int emulator_row,
                                              int emulator_col,
                                              int emulator_elm){
  int ans(0);
  /*
    emulator_elm follows the following convention for the element clicked:
    0b0..0zyyyxxx

    z   - 0: non-node; 1: node
    yyy - 0: near pot sw; 1: near pot res; 2: far pot sw; 3: far pot res;
          4: short circt sw; 5: mid gnd sw
    xxx - embr pos according to EmbrPosition 0: EMBRPOS_ U; 1: UR ...; 7: UL
  */
  int isNode = (1<<6) & emulator_elm;           // extracting z
  int embrPos = 7 & emulator_elm;               // extracting xxx
  int embrElm = ((7<<3) & emulator_elm) >> 3;   // extracting yyy
  bool real = fee->isShowingReal();
  Atom* atom = &_emuhw->sliceSet[emulator_tab].ana._atomSet[emulator_row][emulator_col];

  if (isNode){
    NodeDialog dialog(atom, real);
    ans = dialog.exec();
    return;
  } else { // Emulator branch or nothing
    // Get pointer to EmulatorBranch
    EmulatorBranch* embr = real ? &atom->embr_real[embrPos] : &atom->embr_imag[embrPos];
    // Execute dialog for the EmulatorBranch
    EmulatorBranchDialog  dialog(embr);
    ans = dialog.exec();
  }
  // If dialog successfully executed update FitterEmulatorEditor
  if (ans) return;
  else fee->updt();

//  QString text;
//  double valDouble;
//  bool valBool;
//  switch (embrElm){
//  case 0: // near pot sw
//    valBool = atom->embr(embrPos,real).pot_near_sw();
//    if (real)
//      atom->embr_real[embrPos].set_pot_near_sw(!valBool);
//    else
//      atom->embr_imag[embrPos].set_pot_near_sw(!valBool);
//    break;

//  case 1: // near pot res
//    text = QString("Near pot res of embr %0 of atom [tab,row,col]: [%1,%2,%3]")
//            .arg(embrPos).arg(emulator_tab).arg(emulator_row).arg(emulator_col);
//    valDouble = atom->embr(embrPos,real).pot_near_r();
//    ans = guiauxiliary::askDouble(text, valDouble);
//    if (!ans){
//      if (real)
//        ans = atom->embr_real[embrPos].set_pot_near_r(valDouble, NULL);
//      else
//        ans = atom->embr_imag[embrPos].set_pot_near_r(valDouble, NULL);
//      if (ans) cout << "Failed to set: " << text.toStdString() << endl;
//    }
//    break;

//  case 2: // far pot sw
//    valBool = atom->embr(embrPos,real).pot_far_sw();
//    if (real)
//      atom->embr_real[embrPos].set_pot_far_sw(!valBool);
//    else
//      atom->embr_imag[embrPos].set_pot_far_sw(!valBool);
//    break;

//  case 3: // far pot res
//    text = QString("Far pot res of embr %0 of atom [tab,row,col]: [%1,%2,%3]")
//            .arg(embrPos).arg(emulator_tab).arg(emulator_row).arg(emulator_col);
//    valDouble=atom->embr(embrPos,real).pot_far_r();
//    ans = guiauxiliary::askDouble( text, valDouble );
//    if (!ans){
//      if (real)
//        ans = atom->embr_real[embrPos].set_pot_far_r(valDouble, NULL);
//      else
//        ans = atom->embr_imag[embrPos].set_pot_far_r(valDouble, NULL);
//      if (ans) cout << "Failed to set: " << text.toStdString() << endl;
//    }
//    break;

//  case 4: // short circt sw
//    valBool = atom->embr(embrPos,real).sw_sc();
//    if (real)
//      atom->embr_real[embrPos].set_sw_sc(!valBool);
//    else
//      atom->embr_imag[embrPos].set_sw_sc(!valBool);
//    break;

//  case 5: // mid gnd sw
//    valBool = atom->embr(embrPos,real).sw_mid();
//    if (real)
//      atom->embr_real[embrPos].set_sw_mid(!valBool);
//    else
//      atom->embr_imag[embrPos].set_sw_mid(!valBool);
//    break;
//  }

  return;
}
