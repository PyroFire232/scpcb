#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <map>
#include <vector>

#include <angelscript.h>

#include <String/String.h>

class ScriptModule;
class ScriptManager;
class ScriptObject;
class Type;

class ScriptFunction {
    public:
        struct Signature {
            Type* returnType;
            PGE::String functionName;

            struct Argument {
                Type* type;
                PGE::String name;

                Argument(Type* t, const PGE::String& n);
            };
            std::vector<Argument> arguments;

            PGE::String toString() const;
        };

    private:
        Signature signature;

        asIScriptFunction* scriptFunction;

        std::map<int, PGE::String> stringArgs;

        ScriptModule* scriptModule;

        ScriptObject* returnedObject;

        asIScriptContext* scriptContext;
        bool pushedState;

    public:
        ScriptFunction(ScriptModule* module,asIScriptFunction* asScriptFunction,asIScriptFunction* asFuncWithSignature=nullptr);
        ~ScriptFunction();

        asIScriptFunction* getAngelScriptFunction() const;

        int getArgumentIndex(const PGE::String& argument) const;

        const Signature& getSignature() const;

        void prepare();
        void unprepare();
        void setObject(ScriptObject* obj);
        void setObjectNative(void* obj);
        void setArgument(const PGE::String& argument, int32_t i32);
        void setArgument(const PGE::String& argument, uint32_t u32);
        void setArgument(const PGE::String& argument, float f);
        void setArgument(const PGE::String& argument, double d);
        void setArgument(const PGE::String& argument, const PGE::String& s);
        void setArgument(const PGE::String& argument, ScriptObject* obj);
        void setArgumentNative(const PGE::String& argument, void* obj);
        void execute();
        int32_t getReturnInt32() const;
        uint32_t getReturnUInt32() const;
        float getReturnFloat() const;
        double getReturnDouble() const;
        PGE::String getReturnString() const;
        ScriptObject* getReturnObject() const;
};

#endif // FUNCTION_H_INCLUDED
