#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <angelscript.h>
#include <tinyxml2.h>

#include <String/String.h>

class ScriptClass;
class ScriptModule;

class ScriptObject {
    private:
        ScriptClass* scriptClass;
        asIScriptObject* angelScriptObject;

        int getPropertyIndex(const PGE::String& name) const;

    public:
        ScriptObject(ScriptClass* clss, asIScriptObject* asObj);
        ~ScriptObject();

        asIScriptObject* getAngelScriptObject() const;

        PGE::String getPropertyString(const PGE::String& propertyName) const;
        void setProperty(const PGE::String& propertyName, const PGE::String& val) const;

        void saveXML(tinyxml2::XMLElement* element, const ScriptModule* module) const;
        void loadXML(tinyxml2::XMLElement* element, const ScriptModule* module) const;
};

#endif // OBJECT_H_INCLUDED
