#include "LayoutManager.h"

using namespace wll;

//LayoutAttributes& LayoutManager::GetAttributes() const {
//	return *attributes;
//}

//void LayoutManager::SetAttributes(LayoutAttributes& attributes) {
//	this->attributes = &attributes;
//}

void LayoutManager::Initialize() {
	nextLine = attributes.paddingY;
	
	width = height = 0;

	if (attributes.autoSize) {
		attributes.width = attributes.minWidth;
		attributes.height = attributes.minHeight;
	}
}

void LayoutManager::AddSpace(unsigned int lineHeight) {
	nextLine += lineHeight;
	height += lineHeight;

	if (attributes.autoSize) {
		attributes.width = max(attributes.width, width + attributes.paddingX * 2);
		attributes.height = max(attributes.height, height + attributes.paddingY * 2);
	}
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
	
	if (lineHeight == 0) lineHeight = element->GetHeight();
	int offset = attributes.indentLevel * attributes.indentWidth;

	element->SetSize(element->GetWidth(), lineHeight);
	element->SetPosition(attributes.paddingX + offset, nextLine);

	int totalWidth = element->GetWidth() + offset;
	if (totalWidth > width)
		width = totalWidth;
	
	AddSpace(lineHeight);
}

void LayoutManager::AddNewMultiline(WindowElement* element, unsigned int lines) {
	AddNewLine(element, attributes.lineHeight * lines);
}

void LayoutManager::AddBlankLine() {
	AddSpace(attributes.lineHeight);
}

void LayoutManager::DrawAllElements(HDC hdc) {
	LM_FOR_ALL_ELEMENTS(elements[i]->Draw(hdc););
}

void LayoutManager::Clear() {
	elements.clear();
}

void LayoutManager::Reset() {
	Clear();
	Initialize();
}

void LayoutManager::Refresh() {
	std::vector<WindowElement*> swap(elements);
	Reset();
	elements.swap(swap);
}

LayoutManager::LayoutManager(LayoutAttributes& attributes) : attributes(attributes) {
	OutputDebugString(_T("LayoutManager constructor\n"));
	//SetAttributes(attributes);
	Initialize();
}

LayoutManager::~LayoutManager() {
	OutputDebugString(_T("LayoutManager destructor\n"));
}