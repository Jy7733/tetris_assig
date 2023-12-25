#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** CTetris::setOfColorBlockObjects = NULL;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function

  // write the rest of this function!!
  setOfColorBlockObjects = new Matrix**[numTypes];

  for (int t=0; t<numTypes; t++){
    setOfColorBlockObjects[t] = new Matrix*[numDegrees];
  }

  for (int t=0; t<numTypes; t++){
    for (int d=0; d<numDegrees; d++){
      Matrix *object = setOfBlockObjects[t][d];
      int size = object->get_dy();
      int *array = new int[size*size +1];
      int k;
      for (k=0; k<size*size; k++)
        array[k] = (setOfColorBlockArrays[numDegrees*t + d][k]);
      array[k] = -1;
      setOfColorBlockObjects[t][d] = new Matrix(array,size,size);
      delete[] array;
      
    }
  }

}

void CTetris::deinit(void) {
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL;
}

CTetris::~CTetris() {
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key) {

  TetrisState _state = Tetris::accept(key); // call superclass' function

  currCBlk = setOfColorBlockObjects[type][degree];
  Matrix *tempBlk = iCScreen->clip(top,left,top+currCBlk->get_dy(),left+currCBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currCBlk);
  delete tempBlk;

  oCScreen->paste(iCScreen,0,0);
  oCScreen->paste(tempBlk2,top,left);
  delete tempBlk2;

  if (_state == TetrisState::NewBlock) {
    oCScreen = deleteFullLines(oCScreen,currCBlk,top,wallDepth);
    iCScreen -> paste(oCScreen,0,0);
  }

  return _state;
}
