#include "ConsoleDefinition.h"
#ifdef DEBUG
    #include <iostream>
#endif
#include "../../Menus/Console.h"

ConsoleDefinition::ConsoleDefinition(ScriptManager* mgr, Console* con) {
    engine = mgr->getAngelScriptEngine();
    console = con;
    scriptContext = engine->CreateContext();

    engine->RegisterGlobalFunction("void registerCommand(?&in command, const string&in helpText=\"\")", asMETHOD(ConsoleDefinition, registerCommand), asCALL_THISCALL_ASGLOBAL, this);

    engine->SetDefaultNamespace("Debug");
    engine->RegisterGlobalFunction("void log(const string&in content)", asMETHOD(ConsoleDefinition, log), asCALL_THISCALL_ASGLOBAL, this);
    engine->RegisterGlobalFunction("void warning(const string&in content)", asMETHOD(ConsoleDefinition, warning), asCALL_THISCALL_ASGLOBAL, this);
    engine->RegisterGlobalFunction("void error(const string&in content)", asMETHOD(ConsoleDefinition, error), asCALL_THISCALL_ASGLOBAL, this);
}

void ConsoleDefinition::registerCommand(asIScriptFunction** f, const PGE::String& helpText) {
    console->registerExternalCommand(*f, scriptContext, helpText);
}

void ConsoleDefinition::log(const PGE::String& content) {
#ifdef DEBUG
    std::cout << "Debug::Log: " << content << std::endl;
#endif
    console->addConsoleMessage("Debug::Log: " + content);
}

void ConsoleDefinition::warning(const PGE::String& content) {
#ifdef DEBUG
    std::cout << "Debug::Warn: " << content << std::endl;
#endif
    console->logWarning("Debug::Log: " + content);
}

void ConsoleDefinition::error(const PGE::String& content) {
    #ifdef DEBUG
        std::cerr << "Debug::Err: " << content << std::endl; // TODO error throwing
    #endif
        console->logError("Debug::Err: " + content);
}
