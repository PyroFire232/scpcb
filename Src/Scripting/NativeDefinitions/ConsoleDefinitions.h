#ifndef CONSOLEDEFINITIONS_H_INCLUDED
#define CONSOLEDEFINITIONS_H_INCLUDED

#include <map>
#include <vector>

#include <Color/Color.h>
#include <String/String.h>

#include "../NativeDefinition.h"

class ScriptManager;
class KeyBinds;

class ConsoleDefinitions : public NativeDefinition {
    private:
        void helpInternal(std::vector<PGE::String> params);
        void bindInternal(std::vector<PGE::String> params);
        void unbindInternal(std::vector<PGE::String> params);

        KeyBinds* keyBinds;

        enum class LogType {
            Log,
            Warning,
            Error
        };

        struct Command {
            asIScriptFunction* func;
            void(ConsoleDefinitions::*nativFunc)(std::vector<PGE::String>);
            PGE::String name;
            PGE::String helpText;
        };
        std::map<uint64_t, Command> commands;

        asIScriptContext* scriptContext;
        asIScriptContext* msgContext;
        asIScriptObject* consoleInstance;
        asIScriptFunction* addConsoleMsgFunc;

        void registerCommandInternal(const PGE::String& name, const PGE::String& helpText, void(ConsoleDefinitions::*nativFunc)(std::vector<PGE::String>));
        void registerCommand(const PGE::String& name, const PGE::String& helpText, void* f, int typeId);
        void registerCommandNoHelp(const PGE::String& name, void* f, int typeId);

        void addConsoleMessage(const PGE::String& msg, const PGE::Color& color);

        void internalLog(void* ref, int typeId, LogType type);

        void log(void* ref, int typeId);
        void warning(void* ref, int typeId);
        void error(void* ref, int typeId);

    public:
        ConsoleDefinitions(ScriptManager* mgr, KeyBinds* kb);
        ~ConsoleDefinitions();

        void executeCommand(const PGE::String& in);

        void setUp(ScriptManager* mgr);
};

#endif // CONSOLEDEFINITIONS_H_INCLUDED
