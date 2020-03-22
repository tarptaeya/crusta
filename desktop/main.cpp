#include "browser.h"

int main(int argc, char **argv)
{
    int status = browser->start(argc, argv);
    delete browser;
    return status;
}
