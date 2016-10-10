#include "interpreter.h"
#include "utils.h"

#include <cstdio>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <new>
#include <cstring>

Interpreter::Interpreter() :
    m_cx(0),
    m_cell(new(nothrow) unsigned char[30000])
{

    memset(m_cell, 0, 30000 );
    m_iterators.reserve(100);
}

Interpreter::~Interpreter()
{
    m_runtime.clear();
    delete[] m_cell;
}

Token_e Interpreter::inferTokenType(char ch) const
{
    if( ch == '+' ) return _Plus;
    if( ch == '-' ) return _Minus;
    if( ch == '>' ) return _Great;
    if( ch == '<' ) return _Less;
    if( ch == ',' ) return _Comma;
    if( ch == '.' ) return _Dot;
    if( ch == '[' ) return _OpenBracket;
    if( ch == ']' ) return _CloseBracket;

    return _COMMENT_TOKEN;
}

void Interpreter::start(char** argv, int argc )
{

    if( argc == 3 )
    {
        if( ( !strcmp( argv[1], "-f" ) || !strcmp( argv[2], "-f" ) ) )
        {
            m_inputfile.open(argv[2], ios_base::in);

            if(m_inputfile.fail())
            {
                m_inputfile.close();
                m_inputfile.clear();

                m_inputfile.open( argv[1], ios_base::in);

                if(m_inputfile.fail())
                {
                    m_inputfile.close();
                    m_inputfile.clear();
                    throw invalid_argument( "Could not open the file" );
                }
            }


            while(!m_inputfile.eof())
            {
                m_cmdLine.push_back(m_inputfile.get());
            }

            m_inputfile.close();

        }
        else
        {
            throw invalid_argument("Invalid Command-Line Parameter");
        }
    }
    else
    {

        m_cmdLine.operator =( argv[1] );
    }

      //string cmdLine = ">>>>+>+++>+++>>>>>+++[>,+>++++++++[<[-[-[-[->>>]]]]>[>>>]<<<-]<[<<<<+>+>+>>>>[<]>[+>]>+>+<<<+[>>>>>>>>>>>+<<<<<<<<<<<-]+<+<-[<<+>>>+>+>>>+<<<<<--------[<<<<+>>->>>>->>+<<<<-[<<<<->+>>>>->>>->-<<<<<----[<<<->>>>+<-[<<<+>>>>->+>>+<<<<-[<<<<+>->+>>>+>>>>+<<<<<-[<<->>>->->>>-<<<<<-[<<<<->+>>>>+>+>>>+<<<<<-[<<<<+>>>>>>-<<-[<<+>>>->>>>-<<<<<-[>+>>>->+<<<<<-[>>+<<-[<<<->->>>->->>+<<<<-[<<<+>+>>>+>+<<-[>->-<<-[<<->>>++++[<-------->-]++>+<<[<<+>>>>>>->-<<<<<-[<<<<->>->>-[<<<<+>>>>>>>>+<<<<-[<<<<->>+>>>>>>>+<<<<<-[>->>>-<<<<-]]]]]]]]]]]]]]]]]]]]]]>[[[<<<<<<<<<<<+>>>>>>>>>>>-]>]+>>>>>>>+>]<]<[-]<<<<<<<++<+++<+++[>]<[>>>>>++++++++[<++++++<++++>>-]>>>[-[<+<<<<.>>>>>-]]<<+<<-<<<<[-[-[>+<-]>]>>>[.[>]]<<[<+>-]>>>[<<-[<++>-]>>-]<<[++[<+>--]>+<]>>>[<+>-]<<<<<<<<]>>>>>++++++++++.>+[[-]<]<<<]";
    //string cmdLine = ",>++[<----->-]<[----------------------.,----------],";

//	string cmdLine = "+[>+]";

    if( m_cmdLine.empty() ) return;

    parse( m_cmdLine );

    m_dequeIterator = m_runtime.begin();

    execute();
}

void Interpreter::parse(string &cmdLine)
{

    signed open_brackets = 0;
    stack< int, vector<int> > loop_position_stack;

    string::size_type count = 0;
    string::size_type pos = 1;

    while( count < cmdLine.size() )
    {
        char ch = cmdLine.at( count );

        switch( inferTokenType( ch ) )
        {
        case _Plus:
            {
                m_runtime.push_back( _Plus );
                pos++;
                break;
            }

        case _Minus:
            {
                m_runtime.push_back( _Minus );
                pos++;
                break;
            }

        case _Great:
            {
                m_runtime.push_back( _Great );
                pos++;
                break;
            }

        case _Less:
            {
                m_runtime.push_back( _Less );
                pos++;
                break;
            }

        case _Comma:
            {
                m_runtime.push_back( _Comma );
                pos++;
                break;
            }

        case _Dot:
            {
                m_runtime.push_back( _Dot );
                pos++;
                break;
            }

        case _OpenBracket:
            {
                m_runtime.push_back( _OpenBracket );
                open_brackets++;
                loop_position_stack.push( pos++ );
                break;
            }

        case _CloseBracket:
            {
                m_runtime.push_back( _CloseBracket );

                if( !open_brackets )
                {
                    char buffer[10];
                    memset(buffer, 0, 10);

                    itoa( pos, buffer );

                    throw runtime_error( string("Unexpected \']\' token at position (maybe): ") + buffer);
                }

                open_brackets--;
                loop_position_stack.pop();

                pos++;
                break;
            }

        case _COMMENT_TOKEN:
            {
                break;
            }

         default:
            break;

        }

        count++;
    }

   m_runtime.push_back( _EOL );

    if( open_brackets )
    {
        string buff("");
        char buffer[10];
        memset(buffer, 0, 10);

        while( !loop_position_stack.empty() )
        {
            signed num = loop_position_stack.top();

            itoa( num, buffer );

            buff.push_back( '\n' );

            buff += "Loop at position: ";

            buff += buffer;

            loop_position_stack.pop();
        }

        throw runtime_error( string("Missing \']\' token in following positions (maybe):\n\n") + buff );
    }
}

void Interpreter::instruction(Token_e opcode)
{
    // Each iterator represents the begining of a loop
    static stack<deque<Token_e>::const_iterator, vector<deque<Token_e>::const_iterator> > loop_stack( m_iterators );

    switch( opcode )
    {
    case _Plus:
        {
            if(m_cell[m_cx] == 255) m_cell[m_cx] = 0;
            else m_cell[m_cx]++;
            break;
        }

    case _Minus:
        {
            if(!m_cell[m_cx]) m_cell[m_cx] = 255;
            else m_cell[m_cx]--;
            break;
        }

    case _Great:
        {
            if( ++m_cx > 30000 ) m_cx = 0;
            break;
        }

    case _Less:
        {
            if( --m_cx < 0 ) m_cx = 30000;
            break;
        }

    case _Comma:
        {
             m_cell[m_cx] = getch();

#if defined(_WIN32) || defined(WIN32)

             if( m_cell[m_cx] == 13 )
             {
                 PUTCHAR( 13 );
                 PUTCHAR( ( m_cell[ m_cx ] = 10 ) );
             }
             else
             {
                 PUTCHAR( m_cell[m_cx] );
             }
#else
             __fpurge(stdin);
#endif
             break;
        }


    case _Dot:
        {
            PUTCHAR(m_cell[m_cx]);
            break;
        }

    case _OpenBracket:
        {
            m_dequeIterator++;

            if( !m_cell[m_cx] )
            {

                unsigned short scope_counter = 0;

                while( true )
                {

                    if( *m_dequeIterator == _CloseBracket )
                    {

                        if( scope_counter )
                        {
                            scope_counter--;
                            m_dequeIterator++;

                            continue;
                        }


                        break;

                    }

                    if( *m_dequeIterator == _OpenBracket )
                    {
                        scope_counter++;
                        m_dequeIterator++;

                        continue;
                    }

                    m_dequeIterator++;

                }

            }
            else
            {
                m_dequeIterator--;
                loop_stack.push( m_dequeIterator );
            }

           /*deque<Token_e>::const_iterator ptr = m_dequeIterator;

            while( m_cell[m_cx] )
            {
                m_dequeIterator = ptr;

                while( *m_dequeIterator != _CloseBracket )
                {
                    instruction( *m_dequeIterator );
                }

            }*/

            break;

        }

    case _CloseBracket:
        {

            m_dequeIterator = loop_stack.top();

            loop_stack.pop();
            return;

        }

     default:
        break;

    }

    ++m_dequeIterator;
}

void Interpreter::execute()
{

    for(;m_dequeIterator != m_runtime.end();)
    {
        instruction(*m_dequeIterator);
    }

}
