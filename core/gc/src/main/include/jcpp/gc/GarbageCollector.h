#ifndef JCPP_GC_GARBAGE_COLLECTOR_H
#define JCPP_GC_GARBAGE_COLLECTOR_H

#include "jcpp/native/api/NativeInclude.h"
#include "jcpp/gc/Stack.h"
#include "jcpp/gc/Heap.h"
#include "jcpp/gc/DestructorThread.h"
#include "jcpp/native/api/nthread/NativeMutex.h"
#include "jcpp/gc/visitor/IGCVisitor.h"

using namespace jcpp::native::api::nthread;
using namespace jcpp::gc::visitor;

namespace jcpp {
	namespace gc {

		class JCPP_EXPORT GarbageCollector {
		private:
			static GarbageCollector* garbageCollector;


			Heap* heap;
			Stack* stack;
			DestructorThread* destructorThread;
			NativeMutex* mutex;

			TraverseContext* lastContext;

			GarbageCollector();

			void acceptVisitor(IClassInfoVisitor *classVisitor, IObjectInfoGroupVisitor * objectInfoGroupVisitor, IMethodCallVisitor *methodCallVisitor);
		public:
			static GarbageCollector* getGarbageCollector();

			void gc();
			void acceptGCVisitor(IGCVisitor *visitor);
			void acceptClassVisitor(IClassInfoVisitor *v);
			void acceptObjectVisitor(IObjectInfoGroupVisitor *v);
			void acceptMethodCallVisitor(IMethodCallVisitor *v);

			virtual ~GarbageCollector();
		};

	}
}

#endif
