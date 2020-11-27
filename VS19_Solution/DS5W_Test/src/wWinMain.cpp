#include <Windows.h>

#include <string>
#include <sstream>

#include <DualSenseWindows/IO.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/Helpers.h>

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
	console.writeLine(L"DualSense Controller Windows Test\n========================\n");
	
	// Enum all controllers presentf
	DS5W::DeviceEnumInfo infos[16];
	unsigned int controllersCount = 0;
	DS5W_ReturnValue rv = DS5W::enumDevices(infos, 16, &controllersCount);

	// check size
	if (controllersCount == 0) {
		console.writeLine(L"No DualSense controller found!");
		system("pause");
		return -1;
	}

	// Print all controller
	builder << L"Found " << controllersCount << L" DualSense Controller(s):";
	console.writeLine(builder);

	// Iterate controllers
	for (unsigned int i = 0; i < controllersCount; i++) {
		if (infos[i]._internal.connection == DS5W::DeviceConnection::BT) {
			builder << L"Wireless (Bluetooth) controller (";
		}
		else {
			builder << L"Wired (USB) controller (";
		}

		builder << infos[i]._internal.path << L")";
		console.writeLine(builder);
	}

	// Create first controller
	DS5W::DeviceContext con;
	if (DS5W_SUCCESS(DS5W::initDeviceContext(&infos[0], &con))) {
		console.writeLine(L"DualSense controller connected");

		// Title
		builder << L"DS5 (";
		if (con._internal.connection == DS5W::DeviceConnection::BT) {
			builder << L"BT";
		}
		else {
			builder << L"USB";
		}
		builder << L") Press L1 and R1 to exit";
		SetConsoleTitle(builder.str().c_str());
		builder.str(L"");

		// State object
		DS5W::DS5InputState inState;
		DS5W::DS5OutputState outState;
		ZeroMemory(&inState, sizeof(DS5W::DS5InputState));
		ZeroMemory(&outState, sizeof(DS5W::DS5OutputState));

		// Color intentsity
		float intensity = 1.0f;
		uint16_t lrmbl = 0.0;
		uint16_t rrmbl = 0.0;

		// Force
		DS5W::TriggerEffectType rType = DS5W::TriggerEffectType::NoResitance;

		int btMul = con._internal.connection == DS5W::DeviceConnection::BT ? 10 : 1;

		// Application infinity loop
		while (!(inState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_BUMPER && inState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_BUMPER)) {
			// Get input state
			if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con, &inState))) {
				// === Read Input ===
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
				builder << (inState.buttonsA & DS5W_ISTATE_BTN_A_MENU ? L"MENU" : L"") << (inState.buttonsA & DS5W_ISTATE_BTN_A_SELECT ? L"\tSELECT" : L"") << std::endl;;

				builder << L"Trigger Feedback:\tLeft: " << (int)inState.leftTriggerFeedback << L"\tRight: " << (int)inState.rightTriggerFeedback << std::endl << std::endl;

				builder << L"Touchpad" << (inState.buttonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON ? L" (pushed):" : L":") << std::endl;

				builder << L"Finger 1\tX: " << inState.touchPoint1.x << L"\t Y: " << inState.touchPoint1.y << std::endl;
				builder << L"Finger 2\tX: " << inState.touchPoint2.x << L"\t Y: " << inState.touchPoint2.y << std::endl << std::endl;

				builder << L"Battery: " << inState.battery.level << (inState.battery.chargin ? L" Charging" : L"") << (inState.battery.fullyCharged ? L"  Fully charged" : L"") << std::endl << std::endl;

				builder << (inState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO ? L"PLAYSTATION" : L"") << (inState.buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON ? L"\tMIC" : L"") << std::endl;;

				// Ommited accel and gyro

				// Print to console
				console.writeLine(builder);

				// === Write Output ===
				// Rumbel
				lrmbl = max(lrmbl - 0x200 / btMul, 0);
				rrmbl = max(rrmbl - 0x100 / btMul, 0);

				outState.leftRumble = (lrmbl & 0xFF00) >> 8UL;
				outState.rightRumble = (rrmbl & 0xFF00) >> 8UL;

				// Lightbar
				outState.lightbar = DS5W::color_R8G8B8_UCHAR_A32_FLOAT(255, 0, 0, intensity);
				intensity -= 0.0025f / btMul;
				if (intensity <= 0.0f) {
					intensity = 1.0f;

					lrmbl = 0xFF00;
					rrmbl = 0xFF00;
				}

				// Player led
				if (outState.rightRumble) {
					outState.playerLeds.playerLedFade = true;
					outState.playerLeds.bitmask = DS5W_OSTATE_PLAYER_LED_MIDDLE;
					outState.playerLeds.brightness = DS5W::LedBrightness::HIGH;
				}
				else {
					outState.playerLeds.bitmask = 0;
				}

				// Set force
				if (inState.rightTrigger == 0xFF) {
					rType = DS5W::TriggerEffectType::ContinuousResitance;
				} else if (inState.rightTrigger == 0x00) {
					rType = DS5W::TriggerEffectType::NoResitance;
				}

				// Mic led
				if (inState.buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON) {
					outState.microphoneLed = DS5W::MicLed::ON;
				}
				else if (inState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO) {
					outState.microphoneLed = DS5W::MicLed::OFF;
				}

				// Left trigger is clicky / section
				outState.leftTriggerEffect.effectType = DS5W::TriggerEffectType::SectionResitance;
				outState.leftTriggerEffect.Section.startPosition = 0x00;
				outState.leftTriggerEffect.Section.endPosition = 0x60;

				// Right trigger is forcy
				outState.rightTriggerEffect.effectType = rType;
				outState.rightTriggerEffect.Continuous.force = 0xFF;
				outState.rightTriggerEffect.Continuous.startPosition = 0x00;				

				DS5W::setDeviceOutputState(&con, &outState);
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
		console.writeLine(L"Failed to connect to controller!");
		system("pause");
		return -1;
	}

	return 0;
}
