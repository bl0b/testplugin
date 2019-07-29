#include "qtguiclass.h"
#include <Qthread>
#include "rxregsvc.h"
#include "acutads.h"
// Simple acrxEntryPoint code. Normally intialization and cleanup
// (such as registering and removing commands) should be done here.
//

static char* argv[] = { "QtPlugin", NULL };
static int argc = 1;

// Issue: Qt life cycle not strongly tied to the plugin lifecycle
struct MyQt {
	QApplication m_app;
	QtGuiClass m_gui;
	QThread* m_thread;

	MyQt()
		: m_app(argc, argv)
		, m_gui()
	{
		m_thread = QThread::create([this]() { m_app.exec(); });
		m_thread->start();
		m_gui.show();
	}

	~MyQt()
	{
		m_gui.hide();
		m_app.quit();
		m_thread->quit();
	}
};

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	static MyQt app;

	switch (msg) {
	case AcRx::kInitAppMsg:
		// Allow application to be unloaded
		// Without this statement, AutoCAD will
		// not allow the application to be unloaded
		// except on AutoCAD exit.
		//
		acrxUnlockApplication(appId);
		// Register application as MDI aware. 
		// Without this statement, AutoCAD will
		// switch to SDI mode when loading the
		// application.
		//
		acrxRegisterAppMDIAware(appId);
		acutPrintf(L"\nExample Application Loaded");
		break;
	case AcRx::kUnloadAppMsg:
		acutPrintf(L"\nExample Application Unloaded");
		break;
	}
	return AcRx::kRetOK;
}