#include "../include/Utils.h"

int main() {
    Utils::logInfo("Utils is working!");
    int random = Utils::randomInt(1, 10);
    Utils::logInfo("Random number: " + Utils::toString(random));
    return 0;
}
