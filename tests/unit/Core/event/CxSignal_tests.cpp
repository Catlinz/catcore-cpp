#include "core/event/CxSignal.h"
#include "core/CxTestCore.h"

namespace cat {

	CxI32 g_counter = 0;
	
	class Cx_Obj : public CxSignalEmitter {
	  public:
		Cx_Obj() : CxSignalEmitter() {}
		~Cx_Obj() {}
		void click() { emit("clicked()"); }
		void setValue(CxI32 in_value) {
			m_value = in_value;
			emit(CxHash("valueChanged(CxI32)"), CxArgs1<CxI32>(m_value));
		}
		CxI32 value() const { return m_value; }
	  private:
		CxI32 m_value;
		
	};

	class Cx_Recv {
	  public:
		Cx_Recv() : m_value(0) {}
		static void setValue(void *in_recv, const CxArgs &in_arg) {
			((Cx_Recv *)in_recv)->setValue(in_arg[0].i32);
		}
		void setValue(CxI32 in_value) { m_value = in_value; }
		CxI32 value() const { return m_value; }
		
	  private:
		CxI32 m_value;
	};

	void testOnClick1(void *in_recv, const CxArgs &) {
		g_counter++;
	}
	void testOnClick2(void *in_recv, const CxArgs &) {
		g_counter += 2;
	}

	void testCxSignal() {
		BEGIN_TEST;

		Cx_Obj s0;
		s0.click();
		ass_eq(g_counter, 0);

		s0.connect("clicked()", testOnClick1, 0);
		s0.click();
		ass_eq(g_counter, 1);

		s0.connect("clicked()", testOnClick2, 0);
		s0.click();
		ass_eq(g_counter, 4);

		Cx_Recv r0;
		s0.connect("valueChanged(CxI32)", Cx_Recv::setValue, &r0);
		ass_eq(r0.value(), 0);
		s0.setValue(100);
		ass_eq(r0.value(), 100);
		ass_eq(g_counter, 4);
		s0.click();
		ass_eq(g_counter, 7);
		ass_eq(r0.value(), 100);

		s0.disconnect("clicked()", testOnClick1, 0);
		s0.click();
		ass_eq(g_counter, 9);

		s0.connect("clicked()", testOnClick1, 0);
		s0.click();
		ass_eq(g_counter, 12);

		s0.disconnectAll("clicked()");
		s0.click();
		ass_eq(g_counter, 12);

		s0.setValue(-2093);
		ass_eq(r0.value(), -2093);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxSignal();
	return 0;
}
