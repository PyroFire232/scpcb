#ifndef SCRIPTMODULE_H_INCLUDED
#define SCRIPTMODULE_H_INCLUDED

#include <angelscript.h>
#include <Misc/FilePath.h>
#include <Misc/String.h>
#include <map>
#include <inttypes.h>
#include "Type.h"

class ScriptManager;
class ScriptFunction;
class ScriptClass;
class Script;

class ScriptModule {
    private:
        PGE::String name;
        ScriptManager* scriptManager;
        asIScriptModule* scriptModule;
        bool built;

        std::vector<Script*> scripts;
        std::vector<ScriptFunction*> functions;
        std::vector<ScriptClass*> classes;
    public:
        ScriptModule(ScriptManager* mgr, const PGE::String& nm);

        void addScript(const PGE::String& sectionName, Script* script);
        void build();

        asIScriptModule* getAngelScriptModule() const;
        ScriptManager* getScriptManager() const;

        ScriptClass* getClassByName(const PGE::String& name) const;
        ScriptClass* getClassByTypeId(int typeId) const;

        ScriptFunction* getFunctionByName(const PGE::String& name) const;

        Type* typeFromTypeId(int typeId) const;
        Type* typeFromTypeId(int typeId, bool& isClssType) const;
};

#endif