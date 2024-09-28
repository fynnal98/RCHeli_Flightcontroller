#include "Util.h"

bool Util::correctionEnabled(unsigned int channel10Pulse) {
    return channel10Pulse > 1500;
}