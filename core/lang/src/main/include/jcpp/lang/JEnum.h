#ifndef JCPP_LANG_JENUM_H
#define JCPP_LANG_JENUM_H

#include "jcpp/lang/JObject.h"
#include "jcpp/lang/JClass.h"
#include "jcpp/lang/JPrimitiveInt.h"
#include "jcpp/lang/JString.h"
#include "jcpp/lang/JComparable.h"
#include "jcpp/io/JSerializable.h"

using namespace jcpp::io;

namespace jcpp{
    namespace lang{

    	// @IgnoreReflection()
        class JCPP_EXPORT JEnum: public JObject, public JComparable, public JSerializable{
        private:

        	static jcpp::lang::JClass* clazz;

        public:
        	// @IgnoreReflection()
			class JCPP_EXPORT JEnumClass : public jcpp::lang::JClass{
			  public:
				JEnumClass();

				virtual void initialize();

				virtual jcpp::lang::JClass* getSuperclass();
			};
        protected:
            JPrimitiveInt* ord;
            JString* name;

            JEnum(JEnumClass* _class,JString* name,JPrimitiveInt* ord);

        public:
            static jcpp::lang::JClass* getClazz();

            static JEnum* valueOf(jcpp::lang::JClass* enumType,JString* name);
            
            virtual JString* getName();
            
            virtual jint ordinal();

            virtual jcpp::lang::JClass* getDeclaringClass();
            
            virtual jint compareTo(JObject* o);
            
            virtual jbool equals(JObject* other);
            
            virtual jint hashCode();
            
            virtual JString* toString();

            virtual ~JEnum();
        };
    }
}

#endif
