#include "LayoutManager.h"

using namespace wll;

void LayoutManager::SetAttributes(LayoutAttributes& attributes) {
	this->attributes = &attributes;
}

void LayoutManager::Initialize() {
	nextLine = attributes->paddingY;
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
		x = attributes->width - x - element->GetWidth();
	}

	// Adjust y position
	if (corner == LM_LOWERLEFT || corner == LM_LOWERRIGHT) { 
		y = attributes->height - y - element->GetHeight();
	}
}

void LayoutManager::AddNewLine(WindowElement* element) {
	AddNewLine(element, attributes->lineHeight);
}

void LayoutManager::AddNewLine(WindowElement* element, unsigned int lineHeight) {
	AddElement(element);
	element->SetSize(0, lineHeight);
	element->SetPosition(attributes->paddingX, nextLine);
	//element->CalculateRect(GetDC(NULL));
}

void LayoutManager::AddNewMultiline(WindowElement* element, unsigned int lines) {
	AddNewLine(element, attributes->lineHeight * lines);
}

void LayoutManager::AddBlankLine() {
	AddSpace(attributes->lineHeight);
}

void LayoutManager::AddSpace(unsigned int lineHeight) {
	nextLine += lineHeight;
}

void LayoutManager::DrawAllElements(HDC hdc) {
	LM_FOR_ALL_ELEMENTS(elements[i]->Draw(hdc););
}

void LayoutManager::Clear() {
	elements.clear();
}

LayoutManager::LayoutManager(LayoutAttributes& attributes) {
	OutputDebugString(_T("LayoutManager constructor\n"));
	SetAttributes(attributes);
	Initialize();
}

LayoutManager::~LayoutManager() {
	OutputDebugString(_T("LayoutManager destructor\n"));
}