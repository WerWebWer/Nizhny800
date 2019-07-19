#include "Nizhny800.h"
//#using "source.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThread]
void main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Nizhny800::Nizhny800 form;
	Application::Run(% form);
}
