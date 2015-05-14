#include "LayoutManager.h"

using namespace appwin;

void LayoutManager::MoveNextLine(int lineHeight) {
	if (nextLine == 0) nextLine += attributes.paddingY;
	nextLine += lineHeight;
}

void LayoutManager::AddElement(WindowElement* element) {
	elements.push_back(element);
}

void LayoutManager::AddElement(WindowElement* element, LONG x, LONG y) {
	AddElement(element);
	element->SetPosition(x, y);
}

void LayoutManager::AddElement(WindowElement* element, LONG x, LONG y, unsigned int corner) {
	// Adjust x position
	if (corner == LM_UPPERRIGHT || corner == LM_LOWERRIGHT) {
		x = attributes.width - x - element->GetWidth();
	}

	// Adjust y position
	if (corner == LM_LOWERLEFT || corner == LM_LOWERRIGHT) { 
		y = attributes.height - y - element->GetHeight();
	}
}

void LayoutManager::AddNewLine(WindowElement* element) {
	AddNewLine(element, attributes.lineHeight);
}

void LayoutManager::AddNewLine(WindowElement* element, unsigned int lineHeight) {
	AddElement(element);
	element->SetSize(attributes.width - attributes.paddingX * 2, lineHeight);
	element->SetPosition(attributes.paddingX, nextLine);
	MoveNextLine(lineHeight);
}

void LayoutManager::AddNewMultiline(WindowElement* element, unsigned int lines) {
	AddNewLine(element, attributes.lineHeight * lines);
}

void LayoutManager::AddBlankLine() {
	AddSpace(attributes.lineHeight);
}

void LayoutManager::AddSpace(unsigned int lineHeight) {
	MoveNextLine(lineHeight);
}

void LayoutManager::DrawAllElements(HDC hdc) {
	for (unsigned int i = 0; i < elements.size(); i++) {
		elements[i]->Draw(hdc);
	}
}

LayoutManager::LayoutManager() {
	OutputDebugString(_T("LayoutManager constructor\n"));
	nextLine = 0;
}

LayoutManager::~LayoutManager() {
	OutputDebugString(_T("LayoutManager destructor\n"));
}