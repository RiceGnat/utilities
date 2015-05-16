#include <string>
#include <unordered_map>

#include <windows.h>

#ifndef _INC_WINFONT
#define _INC_WINFONT

#define FONTPARAM_DEFS basic_string<TCHAR> fontName, int fontSize, int fontWeight
#define FONTPARAMS fontName, fontSize, fontWeight

namespace wll {
	using std::unordered_map;
	using std::basic_string;
	
	struct FontKey {
	/// MEMBERS ----------------------------------------------
	public:
		basic_string<TCHAR> name;
		int size;
		int weight;
		
		bool operator==(const FontKey &other) const;
		
	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
	public: FontKey(FONTPARAM_DEFS);
	};

	struct FontKeyHasher
	{
		std::size_t operator()(const FontKey& k) const;
	};

	typedef unordered_map<FontKey, HFONT, FontKeyHasher> FontLookup;

	class FontProvider {
	/// STATIC MEMBERS ---------------------------------------
	private:
		static FontLookup lookup;

	public:
		static HFONT GetFont(FONTPARAM_DEFS);
		static void Flush();

	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
		// Hide constructor and destructor
	private: FontProvider();
	private: ~FontProvider();
	};
}

#endif //!_INC_WINFONT