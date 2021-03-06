#ifndef JCPP_NIO_JBYTEBUFFERASCHARBUFFERB_H
#define JCPP_NIO_JBYTEBUFFERASCHARBUFFERB_H

#include "jcpp/native/api/NativeInclude.h"
#include "jcpp/lang/JString.h"
#include "jcpp/nio/JCharBuffer.h"
#include "jcpp/nio/JByteBuffer.h"
#include "jcpp/io/JBits.h"
#include "jcpp/lang/JStringIndexOutOfBoundsException.h"
#include <cassert>

namespace jcpp {
	namespace nio {

	// @Class(canonicalName="java.nio.ByteBufferAsCharBufferB", simpleName="ByteBufferAsCharBufferB");
	class JCPP_EXPORT JByteBufferAsCharBufferB : public JCharBuffer {
	protected:
		JByteBuffer* bb;
		const jint offset;

		jint ix(jint i);

	public:
		JByteBufferAsCharBufferB(JByteBuffer* bb);
		JByteBufferAsCharBufferB(JByteBuffer* bb, jint mark, jint pos, jint lim, jint cap, jint off);

		static jcpp::lang::JClass* getClazz();

		JCharBuffer* duplicate();
		virtual jchar get();
		virtual jchar get(jint i);
		virtual JCharBuffer* put(jchar x);
		virtual JCharBuffer* put(jint i, jchar x);
		virtual jbool isDirect();
		jbool isReadOnly();
		virtual JCharBuffer* slice();
		virtual JString* toString(jint start, jint end);
		virtual JCharBuffer* subSequence(jint start, jint end);
		virtual JByteOrder* order();

		virtual ~JByteBufferAsCharBufferB();

	};

	}
}

#endif /* JBYTEBUFFERASCHARBUFFERB_H_ */
