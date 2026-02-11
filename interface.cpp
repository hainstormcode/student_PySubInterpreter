#include <iostream>
#include <string>
#include <fstream>
#include "interface.h"
#include "lexAnalysis.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;
std::fstream fs;
lexicalAnalyser lexAnalysis;
Expevaluator math;
Interpreter pysubi;


int lineSum = 0;

void Interface::setColor(int textColor)
{
    //cout << "\033[" << textColor << "m";
}

void Interface::mintOn()
{
    //cout << "\033[38;5;121m";
}

void Interface::mint2On()
{
    //cout << "\033[4;38;5;158m";
}


void Interface::resetColor()
{
    //cout << "\033[0m";
}


bool running = true;
bool runningHelp = false;

void Interface::startInterface()
{
    string openCommand;
    cout << "PySUB Interpreter 1.0 Testing Version on Windows (works on unix, but no colors displayed usually) (February 2026)" << endl;
    cout << "Enter program lines or read (<filename>.py) at command line interface" << endl;
    cout << "Type 'help' for more information or 'quit' to exit" << endl;
    while (running)
    {
        cout << ">>>";
        //initial input system for commands.
        getline(cin, openCommand);
        if (openCommand == "quit" || openCommand == "quit()")
            {
            quit();
            }
        else if (openCommand == "help" || openCommand == "help()")
        {
            help();
        }
        //tricky substring for help(<text here>)
        else if (openCommand.substr(0, 5) == "help(" && openCommand.length() != 6)
        {
            string helpInquiry = openCommand.substr(5, openCommand.length() - 6);
            if (helpInquiry == "clear") {
                setColor(33);
                clearHelp();
                resetColor();
            }
            else if (helpInquiry == "help") {
                setColor(33);
                helpHelp();
                resetColor();
            }
            else if (helpInquiry == "quit") {
                setColor(33);
                quitHelp();
                resetColor();
            }
            else if (helpInquiry == "read") {
                setColor(33);
                readHelp();
                resetColor();
            }
            else if (helpInquiry == "show") {
                setColor(33);
                showHelp();
                resetColor();
            }
            else if (helpInquiry == "clear") {
                setColor(33);
                clearHelp();
                resetColor();
            }
            else if (helpInquiry == "commands") {
                setColor(33);
                commandsHelp();
                resetColor();
            }
            else {
                setColor(33);
                cout << "Not a definable command." << endl;
                resetColor();
            }
            
            
        }
        //tricky substring for read(file name)
        else if (openCommand.substr(0, 5) == "read(" && openCommand.length() != 6)
        {
            setColor(36);
            string fileName = openCommand.substr(5, openCommand.length() - 6);
            cout << endl << "attempting to open: " << fileName << endl <<endl;
            read(fileName);
            resetColor();
        }
        //read without file specified
        else if (openCommand == "read" || openCommand == "read()")
        {
            setColor(36);
            cout << "Specify a file in between the parenthesis, and try again." << endl;
            resetColor();
        }
        else if (openCommand.substr(0, 5) == "show(" && openCommand.length() != 6)
        {
            resetColor();
            string showName = openCommand.substr(5, openCommand.length() - 6);
            if (showName == "tokens")
            {
                lexAnalysis.tokenGenerate(programCode);
                lexAnalysis.tokenShow();
                lexAnalysis.tokenClear();
            }
            if (showName == "variables")
            {
                //when you add this back, you will have errors in lexanalysis...   pysubi.displayTable();
                pysubi.symbol.displayTable();
            }
        }
        else if (openCommand == "show" || openCommand == "show()")
        {
            setColor(32);
            show();
            resetColor();
        }
        else if (openCommand == "clear" || openCommand == "clear()")
        {
            clear();
        }
        else if (isdigit(openCommand[0]) || openCommand.substr(0, 3) == "not" || openCommand[0] == '(') {
            setColor(93);
            cout << "Your answer: " << math.processEquation(openCommand) << endl;
            resetColor();
        }
        //calls run function from interpreter.cpp
        else if (openCommand == "run" || openCommand == "run()")
        {
            lexAnalysis.tokenGenerate(programCode);
            pysubi.run(lexAnalysis.getTokens());
        }
        else if (isalpha(openCommand[0]))
        {
            int j = 0;
            string keyMode = "";
            while (isalpha(openCommand[j]))
            {
                keyMode += openCommand[j];
                j++;
            }
            
            string valueMode = "";
            int k = 0;
            for (int w = 0; w < openCommand.size(); w++)
            {
                if (isdigit(openCommand[w]))
                {
                    k = w;
                    break;
                }
            }
            while (isdigit(openCommand[k]))
            {
                valueMode += openCommand[k];
                k++;
            }
            

            
            pysubi.assignVariables(keyMode, valueMode);
        }
        //catch for if all command possibilities are not fullfilled.
        else {
            setColor(31);
            cout << "Invalid command entered. Use 'help' and then 'commands' to see options for commands." << endl;
            resetColor();
        }
    }


}



void Interface::helpUtility()
{
    //alternative version of the interface, specifically for help/definitions.
    string helpCommand;
    setColor(33);
    cout << "Welcome to the help utility!" << endl;
    cout << "*To exit and return to the interpreter, type 'exit'" << endl
        << "* To get a list of commands, type 'commands'" << endl
        << "* To get a description of any command, just type the command at the help > prompt" << endl;
    resetColor();
    while (runningHelp)
    {
        //new version of >>> to make it obvious EVERY INPUT that you are in the Help Utility,
        //without having to introduce it every time.
        
        cout << endl << "help>";
        cin >> helpCommand;
        if (helpCommand == "exit" || helpCommand == "exit()")
        {
            exitHelp();
        }
        if (helpCommand == "quit" || helpCommand == "quit()")
        {
            setColor(33);
            quitHelp();
            resetColor();
        }
        if (helpCommand == "help" || helpCommand == "help()")
        {
            setColor(33);
            helpHelp();
            resetColor();
        }
        if (helpCommand == "read" || helpCommand == "read()")
        {
            setColor(33);
            readHelp();
            resetColor();
        }
        if (helpCommand == "show" || helpCommand == "show()")
        {
            setColor(33);
            showHelp();
            resetColor();
        }
        if (helpCommand == "clear" || helpCommand == "clear()")
        {
            setColor(33);
            clearHelp();
            resetColor();
        }
        if (helpCommand == "commands" || helpCommand == "commands()")
        {
            setColor(33);
            commandsHelp();
            resetColor();
        }
        
    }


}


void Interface::quit() {
    //kills running code.
    running = false;
}
void Interface::help() {
    //switches which "engine" is running.
    running = false;
    runningHelp = true;
    helpUtility();
}
void Interface::read(string file) {
    lineSum = 0;
    //storing lines system.
    if (programCode.empty())
    {
        ifstream readFile(file);
        string line;
        
        while (getline(readFile,line)) {
            programCode.push_back(line);
            lineSum++;
        }
        if (!programCode.empty())
        {
            cout << "Success." << endl;
        }
        else
        {
            setColor(31);
            cout << "Error. Try again, and check your spelling." << endl;
            resetColor();
        }
    }
    else
    {
        clear();
        read(file);
    }
}




void Interface::show() {
    //accessing stored lines system
    if (!programCode.empty())
    {
        int count = 1;
        for (int i = 0; i < lineSum; i++) {
            cout << "[" << count << "] ";
            cout << programCode[i] << endl;
            count++;
        }
    }
    else
    {
        cout << "No code currently in memory, try using 'read' again." << endl;
    }
}
void Interface::clear() {
    programCode.clear();
}


void Interface::exitHelp() {
    //switches which "engine" is running.
    runningHelp = false;
    running = true;
}
void Interface::quitHelp() {
    cout << "The 'quit' command simply quits the interpreter application." << endl;
}
void Interface::helpHelp() {
    cout << "The 'help' command opens the Help Utility...which you're using right now!" << endl;
}
void Interface::readHelp() {
    cout << "The 'read' command asks for a python file, and then commits it to memory. This can be used by the show function!" << endl;
}
void Interface::showHelp() {
    
    cout << "The 'show' command takes the python file you've given it with the 'read' command, and displays its content line by line." << endl;
    cout << "Also, if you use the 'show' command with 'tokens' like so: 'show(tokens)', it will show all the categories for each token in your script!" << endl;
    cout << "Lastly, if you use the 'show' command with 'variables', like 'show(variables', it will show all the variables in your Symbol Table." << endl;
}
void Interface::clearHelp() {
    cout << "The 'clear' command removes whatever file you've committed to memory (with 'read') from memory," <<endl << "allowing you to then replace the file you're examining with a new one." << endl;
}
void Interface::commandsHelp() {
    cout << "Below is a list of commands. Enter any command at the next prompt to get a definition" 
        << endl
        << endl
        << "clear    " << "help    " << "quit"
        << endl
        << "read    " << "show    "
        << endl
        << endl
        << "NOTE: all commands can also be entered as functions:"
        << endl
        << endl
        << "clear()    " << "help()    " << "quit()"
        << endl
        << "read()    " << "show()    "
        << endl;

}
