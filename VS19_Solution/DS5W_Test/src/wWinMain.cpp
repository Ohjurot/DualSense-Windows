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
		bool keepRunning = true;
		while (keepRunning) {
			// Get input state
			if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con, &inState))) {
				// Build all universal buttons (USB and BT) as text
				builder << std::endl << L" === Universal input ===" << std::endl;

				builder << L"Left Stick\tX: " << (int)inState.leftStick.x << L"\tY: " << (int)inState.leftStick.y << (inState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_STICK ? L"\tPUSH" : L"") << std::endl;
				builder << L"Right Stick\tX: " << (int)inState.rightStick.x << L"\tY: " << (int)inState.rightStick.y << (inState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_STICK ? L"\tPUSH" : L"") << std::endl << std::endl;
				
				builder << L"Left Trigger:  " << (int)inState.leftTrigger << L"\tBinary active: " << (inState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_TRIGGER ? L"Yes" : L"No") << (inState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_BUMPER ? L"\tBUMPER" : L"") << std::endl;
				builder << L"Right Trigger: " << (int)inState.rightTrigger << L"\tBinary active: " << (inState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_TRIGGER ? L"Yes" : L"No") << (inState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_BUMPER ? L"\tBUMPER" : L"") << std::endl << std::endl;
				
				builder << L"DPAD: " << (inState.buttonsAndDpad & DS5W_ISTATE_DPAD_LEFT ? L"L " : L"  ") << (inState.buttonsAndDpad & DS5W_ISTATE_DPAD_UP ? L"U " : L"  ") << 
					(inState.buttonsAndDpad & DS5W_ISTATE_DPAD_DOWN ? L"D " : L"  ") << (inState.buttonsAndDpad & DS5W_ISTATE_DPAD_RIGHT ? L"R " : L"  ");
				builder << L"\tButtons: " << (inState.buttonsAndDpad & DS5W_ISTATE_BTX_SQUARE ? L"S " : L"  ") << (inState.buttonsAndDpad & DS5W_ISTATE_BTX_CROSS ? L"X " : L"  ") <<
					(inState.buttonsAndDpad & DS5W_ISTATE_BTX_CIRCLE ? L"O " : L"  ") << (inState.buttonsAndDpad & DS5W_ISTATE_BTX_TRIANGLE ? L"T " : L"  ") << std::endl;
				builder << (inState.buttonsA & DS5W_ISTATE_BTN_A_MENUE ? L"MENUE" : L"") << (inState.buttonsA & DS5W_ISTATE_BTN_A_SELECT ? L"\tSELECT" : L"") << std::endl;;

				// Check if controler support USB
				// TODO: Add a "user mode" connection query
				if (con._internal.connection == DS5W::DeviceConnection::USB) {
					builder << std::endl << L" === USB Exclusive (for now) ===" << std::endl;
					builder << L"Tochpad" << (inState.buttonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON ? L" (pushed):" : L":") << std::endl;

					builder << L"Finger 1\tX: " << inState.touchPoint1.x << L"\t Y: " << inState.touchPoint1.y << std::endl;
					builder << L"Finger 2\tX: " << inState.touchPoint2.x << L"\t Y: " << inState.touchPoint2.y << std::endl << std::endl;

					builder << (inState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO ? L"PLAYSTATION" : L"") << (inState.buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON ? L"\tMIC" : L"") << std::endl;;

					// Ommited accel and gyro
				}

				// Print to console
				console.writeLine(builder);
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
