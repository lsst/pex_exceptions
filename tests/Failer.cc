#include "Failer.h"

failer::Failer::Failer(void) {
}

void failer::Failer::fail(void) {
    throw LSST_EXCEPT(failer::MyException, "message");
};
