#pragma once
#include <lang/Subset.hpp>

#include <unordered_map>
#include <vector>

namespace moduru {
	namespace lang {

		class Character;

		class UnicodeBlock final
			: public Subset
		{

		public:
			typedef Subset super;

		private:
			static std::unordered_map<String*, Character*> map_;

		private:
			static UnicodeBlock* BASIC_LATIN_;
			static UnicodeBlock* LATIN_1_SUPPLEMENT_;
			static UnicodeBlock* LATIN_EXTENDED_A_;
			static UnicodeBlock* LATIN_EXTENDED_B_;
			static UnicodeBlock* IPA_EXTENSIONS_;
			static UnicodeBlock* SPACING_MODIFIER_LETTERS_;
			static UnicodeBlock* COMBINING_DIACRITICAL_MARKS_;
			static UnicodeBlock* GREEK_;
			static UnicodeBlock* CYRILLIC_;
			static UnicodeBlock* ARMENIAN_;
			static UnicodeBlock* HEBREW_;
			static UnicodeBlock* ARABIC_;
			static UnicodeBlock* DEVANAGARI_;
			static UnicodeBlock* BENGALI_;
			static UnicodeBlock* GURMUKHI_;
			static UnicodeBlock* GUJARATI_;
			static UnicodeBlock* ORIYA_;
			static UnicodeBlock* TAMIL_;
			static UnicodeBlock* TELUGU_;
			static UnicodeBlock* KANNADA_;
			static UnicodeBlock* MALAYALAM_;
			static UnicodeBlock* THAI_;
			static UnicodeBlock* LAO_;
			static UnicodeBlock* TIBETAN_;
			static UnicodeBlock* GEORGIAN_;
			static UnicodeBlock* HANGUL_JAMO_;
			static UnicodeBlock* LATIN_EXTENDED_ADDITIONAL_;
			static UnicodeBlock* GREEK_EXTENDED_;
			static UnicodeBlock* GENERAL_PUNCTUATION_;
			static UnicodeBlock* SUPERSCRIPTS_AND_SUBSCRIPTS_;
			static UnicodeBlock* CURRENCY_SYMBOLS_;
			static UnicodeBlock* COMBINING_MARKS_FOR_SYMBOLS_;
			static UnicodeBlock* LETTERLIKE_SYMBOLS_;
			static UnicodeBlock* NUMBER_FORMS_;
			static UnicodeBlock* ARROWS_;
			static UnicodeBlock* MATHEMATICAL_OPERATORS_;
			static UnicodeBlock* MISCELLANEOUS_TECHNICAL_;
			static UnicodeBlock* CONTROL_PICTURES_;
			static UnicodeBlock* OPTICAL_CHARACTER_RECOGNITION_;
			static UnicodeBlock* ENCLOSED_ALPHANUMERICS_;
			static UnicodeBlock* BOX_DRAWING_;
			static UnicodeBlock* BLOCK_ELEMENTS_;
			static UnicodeBlock* GEOMETRIC_SHAPES_;
			static UnicodeBlock* MISCELLANEOUS_SYMBOLS_;
			static UnicodeBlock* DINGBATS_;
			static UnicodeBlock* CJK_SYMBOLS_AND_PUNCTUATION_;
			static UnicodeBlock* HIRAGANA_;
			static UnicodeBlock* KATAKANA_;
			static UnicodeBlock* BOPOMOFO_;
			static UnicodeBlock* HANGUL_COMPATIBILITY_JAMO_;
			static UnicodeBlock* KANBUN_;
			static UnicodeBlock* ENCLOSED_CJK_LETTERS_AND_MONTHS_;
			static UnicodeBlock* CJK_COMPATIBILITY_;
			static UnicodeBlock* CJK_UNIFIED_IDEOGRAPHS_;
			static UnicodeBlock* HANGUL_SYLLABLES_;
			static UnicodeBlock* PRIVATE_USE_AREA_;
			static UnicodeBlock* CJK_COMPATIBILITY_IDEOGRAPHS_;
			static UnicodeBlock* ALPHABETIC_PRESENTATION_FORMS_;
			static UnicodeBlock* ARABIC_PRESENTATION_FORMS_A_;
			static UnicodeBlock* COMBINING_HALF_MARKS_;
			static UnicodeBlock* CJK_COMPATIBILITY_FORMS_;
			static UnicodeBlock* SMALL_FORM_VARIANTS_;
			static UnicodeBlock* ARABIC_PRESENTATION_FORMS_B_;
			static UnicodeBlock* HALFWIDTH_AND_FULLWIDTH_FORMS_;
			static UnicodeBlock* SPECIALS_;
			static UnicodeBlock* SURROGATES_AREA_;
			static UnicodeBlock* SYRIAC_;
			static UnicodeBlock* THAANA_;
			static UnicodeBlock* SINHALA_;
			static UnicodeBlock* MYANMAR_;
			static UnicodeBlock* ETHIOPIC_;
			static UnicodeBlock* CHEROKEE_;
			static UnicodeBlock* UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_;
			static UnicodeBlock* OGHAM_;
			static UnicodeBlock* RUNIC_;
			static UnicodeBlock* KHMER_;
			static UnicodeBlock* MONGOLIAN_;
			static UnicodeBlock* BRAILLE_PATTERNS_;
			static UnicodeBlock* CJK_RADICALS_SUPPLEMENT_;
			static UnicodeBlock* KANGXI_RADICALS_;
			static UnicodeBlock* IDEOGRAPHIC_DESCRIPTION_CHARACTERS_;
			static UnicodeBlock* BOPOMOFO_EXTENDED_;
			static UnicodeBlock* CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_;
			static UnicodeBlock* YI_SYLLABLES_;
			static UnicodeBlock* YI_RADICALS_;
			static UnicodeBlock* CYRILLIC_SUPPLEMENTARY_;
			static UnicodeBlock* TAGALOG_;
			static UnicodeBlock* HANUNOO_;
			static UnicodeBlock* BUHID_;
			static UnicodeBlock* TAGBANWA_;
			static UnicodeBlock* LIMBU_;
			static UnicodeBlock* TAI_LE_;
			static UnicodeBlock* KHMER_SYMBOLS_;
			static UnicodeBlock* PHONETIC_EXTENSIONS_;
			static UnicodeBlock* MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A_;
			static UnicodeBlock* SUPPLEMENTAL_ARROWS_A_;
			static UnicodeBlock* SUPPLEMENTAL_ARROWS_B_;
			static UnicodeBlock* MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B_;
			static UnicodeBlock* SUPPLEMENTAL_MATHEMATICAL_OPERATORS_;
			static UnicodeBlock* MISCELLANEOUS_SYMBOLS_AND_ARROWS_;
			static UnicodeBlock* KATAKANA_PHONETIC_EXTENSIONS_;
			static UnicodeBlock* YIJING_HEXAGRAM_SYMBOLS_;
			static UnicodeBlock* VARIATION_SELECTORS_;
			static UnicodeBlock* LINEAR_B_SYLLABARY_;
			static UnicodeBlock* LINEAR_B_IDEOGRAMS_;
			static UnicodeBlock* AEGEAN_NUMBERS_;
			static UnicodeBlock* OLD_ITALIC_;
			static UnicodeBlock* GOTHIC_;
			static UnicodeBlock* UGARITIC_;
			static UnicodeBlock* DESERET_;
			static UnicodeBlock* SHAVIAN_;
			static UnicodeBlock* OSMANYA_;
			static UnicodeBlock* CYPRIOT_SYLLABARY_;
			static UnicodeBlock* BYZANTINE_MUSICAL_SYMBOLS_;
			static UnicodeBlock* MUSICAL_SYMBOLS_;
			static UnicodeBlock* TAI_XUAN_JING_SYMBOLS_;
			static UnicodeBlock* MATHEMATICAL_ALPHANUMERIC_SYMBOLS_;
			static UnicodeBlock* CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_;
			static UnicodeBlock* CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT_;
			static UnicodeBlock* TAGS_;
			static UnicodeBlock* VARIATION_SELECTORS_SUPPLEMENT_;
			static UnicodeBlock* SUPPLEMENTARY_PRIVATE_USE_AREA_A_;
			static UnicodeBlock* SUPPLEMENTARY_PRIVATE_USE_AREA_B_;
			static UnicodeBlock* HIGH_SURROGATES_;
			static UnicodeBlock* HIGH_PRIVATE_USE_SURROGATES_;
			static UnicodeBlock* LOW_SURROGATES_;
			static std::vector<int32_t>* blockStarts_;
			static std::vector<UnicodeBlock*>* blocks_;

		public:
			static UnicodeBlock* of(char16_t c);
			static UnicodeBlock* of(int32_t codePoint);
			static UnicodeBlock* forName(String* blockName);

		private:
			UnicodeBlock(String* idName);
			UnicodeBlock(String* idName, String* alias);
			UnicodeBlock(String* idName, std::vector<String*> aliasName);

		private:
			//static unordered_map<String*, Character*>*& map();

		public:
			static UnicodeBlock*& BASIC_LATIN();
			static UnicodeBlock*& LATIN_1_SUPPLEMENT();
			static UnicodeBlock*& LATIN_EXTENDED_A();
			static UnicodeBlock*& LATIN_EXTENDED_B();
			static UnicodeBlock*& IPA_EXTENSIONS();
			static UnicodeBlock*& SPACING_MODIFIER_LETTERS();
			static UnicodeBlock*& COMBINING_DIACRITICAL_MARKS();
			static UnicodeBlock*& GREEK();
			static UnicodeBlock*& CYRILLIC();
			static UnicodeBlock*& ARMENIAN();
			static UnicodeBlock*& HEBREW();
			static UnicodeBlock*& ARABIC();
			static UnicodeBlock*& DEVANAGARI();
			static UnicodeBlock*& BENGALI();
			static UnicodeBlock*& GURMUKHI();
			static UnicodeBlock*& GUJARATI();
			static UnicodeBlock*& ORIYA();
			static UnicodeBlock*& TAMIL();
			static UnicodeBlock*& TELUGU();
			static UnicodeBlock*& KANNADA();
			static UnicodeBlock*& MALAYALAM();
			static UnicodeBlock*& THAI();
			static UnicodeBlock*& LAO();
			static UnicodeBlock*& TIBETAN();
			static UnicodeBlock*& GEORGIAN();
			static UnicodeBlock*& HANGUL_JAMO();
			static UnicodeBlock*& LATIN_EXTENDED_ADDITIONAL();
			static UnicodeBlock*& GREEK_EXTENDED();
			static UnicodeBlock*& GENERAL_PUNCTUATION();
			static UnicodeBlock*& SUPERSCRIPTS_AND_SUBSCRIPTS();
			static UnicodeBlock*& CURRENCY_SYMBOLS();
			static UnicodeBlock*& COMBINING_MARKS_FOR_SYMBOLS();
			static UnicodeBlock*& LETTERLIKE_SYMBOLS();
			static UnicodeBlock*& NUMBER_FORMS();
			static UnicodeBlock*& ARROWS();
			static UnicodeBlock*& MATHEMATICAL_OPERATORS();
			static UnicodeBlock*& MISCELLANEOUS_TECHNICAL();
			static UnicodeBlock*& CONTROL_PICTURES();
			static UnicodeBlock*& OPTICAL_CHARACTER_RECOGNITION();
			static UnicodeBlock*& ENCLOSED_ALPHANUMERICS();
			static UnicodeBlock*& BOX_DRAWING();
			static UnicodeBlock*& BLOCK_ELEMENTS();
			static UnicodeBlock*& GEOMETRIC_SHAPES();
			static UnicodeBlock*& MISCELLANEOUS_SYMBOLS();
			static UnicodeBlock*& DINGBATS();
			static UnicodeBlock*& CJK_SYMBOLS_AND_PUNCTUATION();
			static UnicodeBlock*& HIRAGANA();
			static UnicodeBlock*& KATAKANA();
			static UnicodeBlock*& BOPOMOFO();
			static UnicodeBlock*& HANGUL_COMPATIBILITY_JAMO();
			static UnicodeBlock*& KANBUN();
			static UnicodeBlock*& ENCLOSED_CJK_LETTERS_AND_MONTHS();
			static UnicodeBlock*& CJK_COMPATIBILITY();
			static UnicodeBlock*& CJK_UNIFIED_IDEOGRAPHS();
			static UnicodeBlock*& HANGUL_SYLLABLES();
			static UnicodeBlock*& PRIVATE_USE_AREA();
			static UnicodeBlock*& CJK_COMPATIBILITY_IDEOGRAPHS();
			static UnicodeBlock*& ALPHABETIC_PRESENTATION_FORMS();
			static UnicodeBlock*& ARABIC_PRESENTATION_FORMS_A();
			static UnicodeBlock*& COMBINING_HALF_MARKS();
			static UnicodeBlock*& CJK_COMPATIBILITY_FORMS();
			static UnicodeBlock*& SMALL_FORM_VARIANTS();
			static UnicodeBlock*& ARABIC_PRESENTATION_FORMS_B();
			static UnicodeBlock*& HALFWIDTH_AND_FULLWIDTH_FORMS();
			static UnicodeBlock*& SPECIALS();
			static UnicodeBlock*& SURROGATES_AREA();
			static UnicodeBlock*& SYRIAC();
			static UnicodeBlock*& THAANA();
			static UnicodeBlock*& SINHALA();
			static UnicodeBlock*& MYANMAR();
			static UnicodeBlock*& ETHIOPIC();
			static UnicodeBlock*& CHEROKEE();
			static UnicodeBlock*& UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS();
			static UnicodeBlock*& OGHAM();
			static UnicodeBlock*& RUNIC();
			static UnicodeBlock*& KHMER();
			static UnicodeBlock*& MONGOLIAN();
			static UnicodeBlock*& BRAILLE_PATTERNS();
			static UnicodeBlock*& CJK_RADICALS_SUPPLEMENT();
			static UnicodeBlock*& KANGXI_RADICALS();
			static UnicodeBlock*& IDEOGRAPHIC_DESCRIPTION_CHARACTERS();
			static UnicodeBlock*& BOPOMOFO_EXTENDED();
			static UnicodeBlock*& CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A();
			static UnicodeBlock*& YI_SYLLABLES();
			static UnicodeBlock*& YI_RADICALS();
			static UnicodeBlock*& CYRILLIC_SUPPLEMENTARY();
			static UnicodeBlock*& TAGALOG();
			static UnicodeBlock*& HANUNOO();
			static UnicodeBlock*& BUHID();
			static UnicodeBlock*& TAGBANWA();
			static UnicodeBlock*& LIMBU();
			static UnicodeBlock*& TAI_LE();
			static UnicodeBlock*& KHMER_SYMBOLS();
			static UnicodeBlock*& PHONETIC_EXTENSIONS();
			static UnicodeBlock*& MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A();
			static UnicodeBlock*& SUPPLEMENTAL_ARROWS_A();
			static UnicodeBlock*& SUPPLEMENTAL_ARROWS_B();
			static UnicodeBlock*& MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B();
			static UnicodeBlock*& SUPPLEMENTAL_MATHEMATICAL_OPERATORS();
			static UnicodeBlock*& MISCELLANEOUS_SYMBOLS_AND_ARROWS();
			static UnicodeBlock*& KATAKANA_PHONETIC_EXTENSIONS();
			static UnicodeBlock*& YIJING_HEXAGRAM_SYMBOLS();
			static UnicodeBlock*& VARIATION_SELECTORS();
			static UnicodeBlock*& LINEAR_B_SYLLABARY();
			static UnicodeBlock*& LINEAR_B_IDEOGRAMS();
			static UnicodeBlock*& AEGEAN_NUMBERS();
			static UnicodeBlock*& OLD_ITALIC();
			static UnicodeBlock*& GOTHIC();
			static UnicodeBlock*& UGARITIC();
			static UnicodeBlock*& DESERET();
			static UnicodeBlock*& SHAVIAN();
			static UnicodeBlock*& OSMANYA();
			static UnicodeBlock*& CYPRIOT_SYLLABARY();
			static UnicodeBlock*& BYZANTINE_MUSICAL_SYMBOLS();
			static UnicodeBlock*& MUSICAL_SYMBOLS();
			static UnicodeBlock*& TAI_XUAN_JING_SYMBOLS();
			static UnicodeBlock*& MATHEMATICAL_ALPHANUMERIC_SYMBOLS();
			static UnicodeBlock*& CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B();
			static UnicodeBlock*& CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT();
			static UnicodeBlock*& TAGS();
			static UnicodeBlock*& VARIATION_SELECTORS_SUPPLEMENT();
			static UnicodeBlock*& SUPPLEMENTARY_PRIVATE_USE_AREA_A();
			static UnicodeBlock*& SUPPLEMENTARY_PRIVATE_USE_AREA_B();
			static UnicodeBlock*& HIGH_SURROGATES();
			static UnicodeBlock*& HIGH_PRIVATE_USE_SURROGATES();
			static UnicodeBlock*& LOW_SURROGATES();

		private:
			static std::vector<int>*& blockStarts();
			static std::vector<UnicodeBlock*>*& blocks();
			//friend class Character;
			//friend class Subset;
			//friend class CharacterCache;
		};

	}
}
