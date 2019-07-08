#include "app.h"
#include <exception>
#include <iostream>

int main()
{
    App app;
    try{
        app.run();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}