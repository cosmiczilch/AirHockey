#include "ctextinput.h"

void CTextInput::init(int Id, float Width, float Height, float X, float Y,
		      float Z)
{
	id = Id;

	width = Width;
	height = Height;
	x = X;
	y = Y;
	z = Z;

	font.fontWidth = 20;
	font.fontHeight = 30;

	enabled = false;
	visible = false;

	inputText = "";

	return;
}

void CTextInput::setFont(SFont Font)
{
	font.fontWidth = Font.fontWidth;
	font.fontHeight = Font.fontHeight;
	font.foreColor.r = Font.foreColor.r;
	font.foreColor.g = Font.foreColor.g;
	font.foreColor.b = Font.foreColor.b;
	font.foreColor.a = Font.foreColor.a;

	return;
}

void CTextInput::draw()
{
	if (!enabled || !visible) {
		return;
	}
	// Draw the string now
	fontPrinter.setFont(font);
	fontPrinter.print(inputText, x, y, z);

	return;
}

char keysym_to_char(SDLKey keysym)
{
	switch (keysym) {
	case SDLK_0:
		return '0';
	case SDLK_1:
		return '1';
	case SDLK_2:
		return '2';
	case SDLK_3:
		return '3';
	case SDLK_4:
		return '4';
	case SDLK_5:
		return '5';
	case SDLK_6:
		return '6';
	case SDLK_7:
		return '7';
	case SDLK_8:
		return '8';
	case SDLK_9:
		return '9';
	case SDLK_PERIOD:
		return '.';
	default:
		return '\0';
	}

	return '\0';
}

void CTextInput::keyPressHandler(SDLKey keysym)
{
	if ((keysym >= SDLK_0 && keysym <= SDLK_9) || keysym == SDLK_PERIOD) {
		inputText.push_back(keysym_to_char(keysym));
	}

	return;
}

void CTextInput::inputDoneHandler()
{
	if (onInputDone) {
		onInputDone();
	}

	return;
}
