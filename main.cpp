#include "utils.h"
#include "interpreter.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    INIT();

    if( argc < 2 || argc > 3 )
    {
        string greetings =
                "BrainFuck Language Interpreter\n"
                "Usage:\n\n"
                "1- " + string(argv[0]) + " \"Brainfuck code\"\n\n"
                "2- " + string(argv[0]) + " -f file.b\n\n"
                "3- " + string(argv[0]) + " -dbg \"Brainfuck code\"\n\n"
                "4- " + string(argv[0]) + " -dbg -f file.b\n\n"
                "\n\nDeveloped by Guga.\nSkype: gugaloks\n";

        PRINT(greetings);

        FINISH();
        return EXIT_FAILURE;
    }

    try
    {
        Interpreter obj;
        obj.start(argv, argc);
    }

    catch(runtime_error &e)
    {
        string error = "An Error Has Occurred During Performance:\n";
        PRINT_ERR(error, e);
    }

    catch(invalid_argument &e)
    {
        string error = "An Error Has Occurred:\n";
        PRINT_ERR(error, e);
    }

    FINISH();

    return EXIT_SUCCESS;
}
