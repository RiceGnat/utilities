#include "FontProvider.h"

using namespace appwin;


// Equality comparator
bool FontKey::operator==(const FontKey &other) const {
	return (name == other.name &&
		size == other.size && 
		weight == other.weight);
}

// Fontkey constructor
FontKey::FontKey(FONTPARAM_DEFS) {
	name = fontName;
	size = fontSize;
	weight = fontWeight;
}

// Hash function
std::size_t FontKeyHasher::operator()(const FontKey& k) const {
	using std::size_t;
	using std::hash;
	using std::string;

	// Compute individual hash values and combine them using XOR and bit shifting:
	return ((hash<basic_string<TCHAR>>()(k.name)
		^ (hash<int>()(k.size) << 1)) >> 1)
		^ (hash<int>()(k.weight) << 1);
}

// Lookup object
FontLookup FontProvider::lookup;

// Get (or create) font
HFONT FontProvider::GetFont(FONTPARAM_DEFS) {
	// Make font key
	FontKey key = FontKey(FONTPARAMS);

	// See if font exists in lookup
	if (!lookup.count(key)) {
		// Create a new font if not found
		HFONT font = CreateFont(
			fontSize,	// Font height
			0,						// Font width
			0,						// Escapement (line angle)
			0,						// Orientation (character angle)
			fontWeight,				// Font weight
			FALSE,					// Italic
			FALSE,					// Underline
			FALSE,					// Strikeout
			ANSI_CHARSET,			// Character set
			OUT_DEFAULT_PRECIS,		// Output precision
			CLIP_DEFAULT_PRECIS,	// Clipping precision
			ANTIALIASED_QUALITY,	// Output quality
			DEFAULT_PITCH | FF_MODERN, // Pitch and family
			fontName.c_str()
			);

		lookup.insert(std::pair<FontKey, HFONT>(key, font));
	}

	// Return the font
	return lookup[key];
}

// Flush font cache
void FontProvider::Flush() {
	lookup.clear();
}