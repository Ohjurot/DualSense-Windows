#include <Windows.h>

#include <string>
#include <sstream>

#include <DualSenseWindows/IO.h>
#include <DualSenseWindows/Device.h>

typedef std::wstringstream wstrBuilder;

class Console {
	public:
		Console() {
			AllocConsole();
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		~Console() {
			FreeConsole();
		}

		void writeLine(LPCWSTR text) {
			write(text);
			write(L"\n");
		}

		void writeLine(wstrBuilder& builder) {
			writeLine(builder.str().c_str());
			builder.str(L"");
		}

		void write(LPCWSTR text) {
			WriteConsoleW(consoleHandle, text, wcslen(text), NULL, NULL);
		}

		void write(wstrBuilder& builder) {
			write(builder.str().c_str());
			builder.str(L"");
		}

	private:
		HANDLE consoleHandle;
};

INT WINAPI wWinMain(HINSTANCE _In_ hInstance, HINSTANCE _In_opt_ hPrevInstance, LPWSTR _In_ cmdLine, INT _In_ nCmdShow) {
	// Console
	Console console;
	wstrBuilder builder;
	console.writeLine(L"DualShock 5 Windows Test\n========================\n");
	
	// Enum all controlers presentf
	DS5W::DeviceEnumInfo infos[16];
	unsigned int controlersCount = 0;
	DS5W_ReturnValue rv = DS5W::enumDevices(infos, 16, true, &controlersCount);

	// check size
	if (controlersCount == 0) {
		console.writeLine(L"No PS5 controler found!");
		system("pause");
		return -1;
	}

	// Print all controler
	builder << L"Found " << controlersCount << L" PS5 Controler(s):";
	console.writeLine(builder);

	// Iterate controlers
	for (unsigned int i = 0; i < controlersCount; i++) {
		if (infos[i]._internal.connection == DS5W::DeviceConnection::BT) {
			builder << L"Wireless (Blotooth) controler (";
		}
		else {
			builder << L"Wired (USB) controler (";
		}

		builder << infos[i]._internal.path << L")";
		console.writeLine(builder);
	}

	// Create first controler
	DS5W::DeviceContext con;
	if (DS5W_SUCCESS(DS5W::initDeviceContext(&infos[0], &con))) {
		console.writeLine(L"DualSense 5 controller connected");

		// State object
		DS5W::DS5InputState inState;

		// Application infinity loop
		bool keepRunning = false;
		while (keepRunning) {
			// Get input state
			if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con, &inState))) {
				if (inState.headPhoneConnected) {
					console.writeLine(L"Connected");
				}
				else {
					console.writeLine(L"Not Connected");
				}
			}
			else {
				// Device disconnected show error and try to reconnect
				console.writeLine(L"Device removed!");
				DS5W::reconnectDevice(&con);
			}
		}

		// Free state
		DS5W::freeDeviceContext(&con);
	}
	else {
		console.writeLine(L"Failed to connect to controler!");
		system("pause");
		return -1;
	}
	system("pause");

	return 0;
}
