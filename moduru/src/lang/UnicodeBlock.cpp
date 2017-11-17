#include <lang/UnicodeBlock.hpp>

//#include <java/io/Serializable.hpp>
//#include <lang/ArrayStoreException.hpp>
#include <lang/CharSequence.hpp>
#include <lang/Subset.hpp>
#include <lang/Character.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
//#include <lang/IllegalArgumentException.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
//#include <java/util/HashMap.hpp>
//#include <java/util/Locale.hpp>
//#include <java/util/Map.hpp>
//#include <Array.hpp>
//#include <SubArray.hpp>
//#include <ObjectArray.hpp>

using namespace moduru::lang;
using namespace std;

UnicodeBlock::UnicodeBlock(String* idName) : super(idName)
{
	//map_[idName->toUpperCase(moduru::util::Locale::US())] = this;
}

UnicodeBlock::UnicodeBlock(String* idName, String* alias) : UnicodeBlock(idName)
{
	//map_[alias->toUpperCase(moduru::util::Locale::US())] = this;
}

UnicodeBlock::UnicodeBlock(String* idName, vector<String*> aliasName) : UnicodeBlock(idName)
{
	for (auto x = int32_t(0); x < aliasName.size(); ++x) {
		//map_->put(aliasName[x]->toUpperCase(:moduru::util::Locale::US()), this);
	}
}

/*
java::util::Map*& UnicodeBlock::map()
{
    
    return map_;
}
java::util::Map* UnicodeBlock::map_;
*/

UnicodeBlock*& UnicodeBlock::BASIC_LATIN()
{
    
    return BASIC_LATIN_;
}
UnicodeBlock* UnicodeBlock::BASIC_LATIN_;

UnicodeBlock*& UnicodeBlock::LATIN_1_SUPPLEMENT()
{
    
    return LATIN_1_SUPPLEMENT_;
}
UnicodeBlock* UnicodeBlock::LATIN_1_SUPPLEMENT_;

UnicodeBlock*& UnicodeBlock::LATIN_EXTENDED_A()
{
    
    return LATIN_EXTENDED_A_;
}
UnicodeBlock* UnicodeBlock::LATIN_EXTENDED_A_;

UnicodeBlock*& UnicodeBlock::LATIN_EXTENDED_B()
{
    
    return LATIN_EXTENDED_B_;
}
UnicodeBlock* UnicodeBlock::LATIN_EXTENDED_B_;

UnicodeBlock*& UnicodeBlock::IPA_EXTENSIONS()
{
    
    return IPA_EXTENSIONS_;
}
UnicodeBlock* UnicodeBlock::IPA_EXTENSIONS_;

UnicodeBlock*& UnicodeBlock::SPACING_MODIFIER_LETTERS()
{
    
    return SPACING_MODIFIER_LETTERS_;
}
UnicodeBlock* UnicodeBlock::SPACING_MODIFIER_LETTERS_;

UnicodeBlock*& UnicodeBlock::COMBINING_DIACRITICAL_MARKS()
{
    
    return COMBINING_DIACRITICAL_MARKS_;
}
UnicodeBlock* UnicodeBlock::COMBINING_DIACRITICAL_MARKS_;

UnicodeBlock*& UnicodeBlock::GREEK()
{
    
    return GREEK_;
}
UnicodeBlock* UnicodeBlock::GREEK_;

UnicodeBlock*& UnicodeBlock::CYRILLIC()
{
    
    return CYRILLIC_;
}
UnicodeBlock* UnicodeBlock::CYRILLIC_;

UnicodeBlock*& UnicodeBlock::ARMENIAN()
{
    
    return ARMENIAN_;
}
UnicodeBlock* UnicodeBlock::ARMENIAN_;

UnicodeBlock*& UnicodeBlock::HEBREW()
{
    
    return HEBREW_;
}
UnicodeBlock* UnicodeBlock::HEBREW_;

UnicodeBlock*& UnicodeBlock::ARABIC()
{
    
    return ARABIC_;
}
UnicodeBlock* UnicodeBlock::ARABIC_;

UnicodeBlock*& UnicodeBlock::DEVANAGARI()
{
    
    return DEVANAGARI_;
}
UnicodeBlock* UnicodeBlock::DEVANAGARI_;

UnicodeBlock*& UnicodeBlock::BENGALI()
{
    
    return BENGALI_;
}
UnicodeBlock* UnicodeBlock::BENGALI_;

UnicodeBlock*& UnicodeBlock::GURMUKHI()
{
    
    return GURMUKHI_;
}
UnicodeBlock* UnicodeBlock::GURMUKHI_;

UnicodeBlock*& UnicodeBlock::GUJARATI()
{
    
    return GUJARATI_;
}
UnicodeBlock* UnicodeBlock::GUJARATI_;

UnicodeBlock*& UnicodeBlock::ORIYA()
{
    
    return ORIYA_;
}
UnicodeBlock* UnicodeBlock::ORIYA_;

UnicodeBlock*& UnicodeBlock::TAMIL()
{
    
    return TAMIL_;
}
UnicodeBlock* UnicodeBlock::TAMIL_;

UnicodeBlock*& UnicodeBlock::TELUGU()
{
    
    return TELUGU_;
}
UnicodeBlock* UnicodeBlock::TELUGU_;

UnicodeBlock*& UnicodeBlock::KANNADA()
{
    
    return KANNADA_;
}
UnicodeBlock* UnicodeBlock::KANNADA_;

UnicodeBlock*& UnicodeBlock::MALAYALAM()
{
    
    return MALAYALAM_;
}
UnicodeBlock* UnicodeBlock::MALAYALAM_;

UnicodeBlock*& UnicodeBlock::THAI()
{
    
    return THAI_;
}
UnicodeBlock* UnicodeBlock::THAI_;

UnicodeBlock*& UnicodeBlock::LAO()
{
    
    return LAO_;
}
UnicodeBlock* UnicodeBlock::LAO_;

UnicodeBlock*& UnicodeBlock::TIBETAN()
{
    
    return TIBETAN_;
}
UnicodeBlock* UnicodeBlock::TIBETAN_;

UnicodeBlock*& UnicodeBlock::GEORGIAN()
{
    
    return GEORGIAN_;
}
UnicodeBlock* UnicodeBlock::GEORGIAN_;

UnicodeBlock*& UnicodeBlock::HANGUL_JAMO()
{
    
    return HANGUL_JAMO_;
}
UnicodeBlock* UnicodeBlock::HANGUL_JAMO_;

UnicodeBlock*& UnicodeBlock::LATIN_EXTENDED_ADDITIONAL()
{
    
    return LATIN_EXTENDED_ADDITIONAL_;
}
UnicodeBlock* UnicodeBlock::LATIN_EXTENDED_ADDITIONAL_;

UnicodeBlock*& UnicodeBlock::GREEK_EXTENDED()
{
    
    return GREEK_EXTENDED_;
}
UnicodeBlock* UnicodeBlock::GREEK_EXTENDED_;

UnicodeBlock*& UnicodeBlock::GENERAL_PUNCTUATION()
{
    
    return GENERAL_PUNCTUATION_;
}
UnicodeBlock* UnicodeBlock::GENERAL_PUNCTUATION_;

UnicodeBlock*& UnicodeBlock::SUPERSCRIPTS_AND_SUBSCRIPTS()
{
    
    return SUPERSCRIPTS_AND_SUBSCRIPTS_;
}
UnicodeBlock* UnicodeBlock::SUPERSCRIPTS_AND_SUBSCRIPTS_;

UnicodeBlock*& UnicodeBlock::CURRENCY_SYMBOLS()
{
    
    return CURRENCY_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::CURRENCY_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS()
{
    
    return COMBINING_MARKS_FOR_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::LETTERLIKE_SYMBOLS()
{
    
    return LETTERLIKE_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::LETTERLIKE_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::NUMBER_FORMS()
{
    
    return NUMBER_FORMS_;
}
UnicodeBlock* UnicodeBlock::NUMBER_FORMS_;

UnicodeBlock*& UnicodeBlock::ARROWS()
{
    
    return ARROWS_;
}
UnicodeBlock* UnicodeBlock::ARROWS_;

UnicodeBlock*& UnicodeBlock::MATHEMATICAL_OPERATORS()
{
    
    return MATHEMATICAL_OPERATORS_;
}
UnicodeBlock* UnicodeBlock::MATHEMATICAL_OPERATORS_;

UnicodeBlock*& UnicodeBlock::MISCELLANEOUS_TECHNICAL()
{
    
    return MISCELLANEOUS_TECHNICAL_;
}
UnicodeBlock* UnicodeBlock::MISCELLANEOUS_TECHNICAL_;

UnicodeBlock*& UnicodeBlock::CONTROL_PICTURES()
{
    
    return CONTROL_PICTURES_;
}
UnicodeBlock* UnicodeBlock::CONTROL_PICTURES_;

UnicodeBlock*& UnicodeBlock::OPTICAL_CHARACTER_RECOGNITION()
{
    
    return OPTICAL_CHARACTER_RECOGNITION_;
}
UnicodeBlock* UnicodeBlock::OPTICAL_CHARACTER_RECOGNITION_;

UnicodeBlock*& UnicodeBlock::ENCLOSED_ALPHANUMERICS()
{
    
    return ENCLOSED_ALPHANUMERICS_;
}
UnicodeBlock* UnicodeBlock::ENCLOSED_ALPHANUMERICS_;

UnicodeBlock*& UnicodeBlock::BOX_DRAWING()
{
    
    return BOX_DRAWING_;
}
UnicodeBlock* UnicodeBlock::BOX_DRAWING_;

UnicodeBlock*& UnicodeBlock::BLOCK_ELEMENTS()
{
    
    return BLOCK_ELEMENTS_;
}
UnicodeBlock* UnicodeBlock::BLOCK_ELEMENTS_;

UnicodeBlock*& UnicodeBlock::GEOMETRIC_SHAPES()
{
    
    return GEOMETRIC_SHAPES_;
}
UnicodeBlock* UnicodeBlock::GEOMETRIC_SHAPES_;

UnicodeBlock*& UnicodeBlock::MISCELLANEOUS_SYMBOLS()
{
    
    return MISCELLANEOUS_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::MISCELLANEOUS_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::DINGBATS()
{
    
    return DINGBATS_;
}
UnicodeBlock* UnicodeBlock::DINGBATS_;

UnicodeBlock*& UnicodeBlock::CJK_SYMBOLS_AND_PUNCTUATION()
{
    
    return CJK_SYMBOLS_AND_PUNCTUATION_;
}
UnicodeBlock* UnicodeBlock::CJK_SYMBOLS_AND_PUNCTUATION_;

UnicodeBlock*& UnicodeBlock::HIRAGANA()
{
    
    return HIRAGANA_;
}
UnicodeBlock* UnicodeBlock::HIRAGANA_;

UnicodeBlock*& UnicodeBlock::KATAKANA()
{
    
    return KATAKANA_;
}
UnicodeBlock* UnicodeBlock::KATAKANA_;

UnicodeBlock*& UnicodeBlock::BOPOMOFO()
{
    
    return BOPOMOFO_;
}
UnicodeBlock* UnicodeBlock::BOPOMOFO_;

UnicodeBlock*& UnicodeBlock::HANGUL_COMPATIBILITY_JAMO()
{
    
    return HANGUL_COMPATIBILITY_JAMO_;
}
UnicodeBlock* UnicodeBlock::HANGUL_COMPATIBILITY_JAMO_;

UnicodeBlock*& UnicodeBlock::KANBUN()
{
    
    return KANBUN_;
}
UnicodeBlock* UnicodeBlock::KANBUN_;

UnicodeBlock*& UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS()
{
    
    return ENCLOSED_CJK_LETTERS_AND_MONTHS_;
}
UnicodeBlock* UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS_;

UnicodeBlock*& UnicodeBlock::CJK_COMPATIBILITY()
{
    
    return CJK_COMPATIBILITY_;
}
UnicodeBlock* UnicodeBlock::CJK_COMPATIBILITY_;

UnicodeBlock*& UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS()
{
    
    return CJK_UNIFIED_IDEOGRAPHS_;
}
UnicodeBlock* UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_;

UnicodeBlock*& UnicodeBlock::HANGUL_SYLLABLES()
{
    
    return HANGUL_SYLLABLES_;
}
UnicodeBlock* UnicodeBlock::HANGUL_SYLLABLES_;

UnicodeBlock*& UnicodeBlock::PRIVATE_USE_AREA()
{
    
    return PRIVATE_USE_AREA_;
}
UnicodeBlock* UnicodeBlock::PRIVATE_USE_AREA_;

UnicodeBlock*& UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS()
{
    
    return CJK_COMPATIBILITY_IDEOGRAPHS_;
}
UnicodeBlock* UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_;

UnicodeBlock*& UnicodeBlock::ALPHABETIC_PRESENTATION_FORMS()
{
    
    return ALPHABETIC_PRESENTATION_FORMS_;
}
UnicodeBlock* UnicodeBlock::ALPHABETIC_PRESENTATION_FORMS_;

UnicodeBlock*& UnicodeBlock::ARABIC_PRESENTATION_FORMS_A()
{
    
    return ARABIC_PRESENTATION_FORMS_A_;
}
UnicodeBlock* UnicodeBlock::ARABIC_PRESENTATION_FORMS_A_;

UnicodeBlock*& UnicodeBlock::COMBINING_HALF_MARKS()
{
    
    return COMBINING_HALF_MARKS_;
}
UnicodeBlock* UnicodeBlock::COMBINING_HALF_MARKS_;

UnicodeBlock*& UnicodeBlock::CJK_COMPATIBILITY_FORMS()
{
    
    return CJK_COMPATIBILITY_FORMS_;
}
UnicodeBlock* UnicodeBlock::CJK_COMPATIBILITY_FORMS_;

UnicodeBlock*& UnicodeBlock::SMALL_FORM_VARIANTS()
{
    
    return SMALL_FORM_VARIANTS_;
}
UnicodeBlock* UnicodeBlock::SMALL_FORM_VARIANTS_;

UnicodeBlock*& UnicodeBlock::ARABIC_PRESENTATION_FORMS_B()
{
    
    return ARABIC_PRESENTATION_FORMS_B_;
}
UnicodeBlock* UnicodeBlock::ARABIC_PRESENTATION_FORMS_B_;

UnicodeBlock*& UnicodeBlock::HALFWIDTH_AND_FULLWIDTH_FORMS()
{
    
    return HALFWIDTH_AND_FULLWIDTH_FORMS_;
}
UnicodeBlock* UnicodeBlock::HALFWIDTH_AND_FULLWIDTH_FORMS_;

UnicodeBlock*& UnicodeBlock::SPECIALS()
{
    
    return SPECIALS_;
}
UnicodeBlock* UnicodeBlock::SPECIALS_;

UnicodeBlock*& UnicodeBlock::SURROGATES_AREA()
{
    
    return SURROGATES_AREA_;
}
UnicodeBlock* UnicodeBlock::SURROGATES_AREA_;

UnicodeBlock*& UnicodeBlock::SYRIAC()
{
    
    return SYRIAC_;
}
UnicodeBlock* UnicodeBlock::SYRIAC_;

UnicodeBlock*& UnicodeBlock::THAANA()
{
    
    return THAANA_;
}
UnicodeBlock* UnicodeBlock::THAANA_;

UnicodeBlock*& UnicodeBlock::SINHALA()
{
    
    return SINHALA_;
}
UnicodeBlock* UnicodeBlock::SINHALA_;

UnicodeBlock*& UnicodeBlock::MYANMAR()
{
    
    return MYANMAR_;
}
UnicodeBlock* UnicodeBlock::MYANMAR_;

UnicodeBlock*& UnicodeBlock::ETHIOPIC()
{
    
    return ETHIOPIC_;
}
UnicodeBlock* UnicodeBlock::ETHIOPIC_;

UnicodeBlock*& UnicodeBlock::CHEROKEE()
{
    
    return CHEROKEE_;
}
UnicodeBlock* UnicodeBlock::CHEROKEE_;

UnicodeBlock*& UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS()
{
    
    return UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_;
}
UnicodeBlock* UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_;

UnicodeBlock*& UnicodeBlock::OGHAM()
{
    
    return OGHAM_;
}
UnicodeBlock* UnicodeBlock::OGHAM_;

UnicodeBlock*& UnicodeBlock::RUNIC()
{
    
    return RUNIC_;
}
UnicodeBlock* UnicodeBlock::RUNIC_;

UnicodeBlock*& UnicodeBlock::KHMER()
{
    
    return KHMER_;
}
UnicodeBlock* UnicodeBlock::KHMER_;

UnicodeBlock*& UnicodeBlock::MONGOLIAN()
{
    
    return MONGOLIAN_;
}
UnicodeBlock* UnicodeBlock::MONGOLIAN_;

UnicodeBlock*& UnicodeBlock::BRAILLE_PATTERNS()
{
    
    return BRAILLE_PATTERNS_;
}
UnicodeBlock* UnicodeBlock::BRAILLE_PATTERNS_;

UnicodeBlock*& UnicodeBlock::CJK_RADICALS_SUPPLEMENT()
{
    
    return CJK_RADICALS_SUPPLEMENT_;
}
UnicodeBlock* UnicodeBlock::CJK_RADICALS_SUPPLEMENT_;

UnicodeBlock*& UnicodeBlock::KANGXI_RADICALS()
{
    
    return KANGXI_RADICALS_;
}
UnicodeBlock* UnicodeBlock::KANGXI_RADICALS_;

UnicodeBlock*& UnicodeBlock::IDEOGRAPHIC_DESCRIPTION_CHARACTERS()
{
    
    return IDEOGRAPHIC_DESCRIPTION_CHARACTERS_;
}
UnicodeBlock* UnicodeBlock::IDEOGRAPHIC_DESCRIPTION_CHARACTERS_;

UnicodeBlock*& UnicodeBlock::BOPOMOFO_EXTENDED()
{
    
    return BOPOMOFO_EXTENDED_;
}
UnicodeBlock* UnicodeBlock::BOPOMOFO_EXTENDED_;

UnicodeBlock*& UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A()
{
    
    return CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_;
}
UnicodeBlock* UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_;

UnicodeBlock*& UnicodeBlock::YI_SYLLABLES()
{
    
    return YI_SYLLABLES_;
}
UnicodeBlock* UnicodeBlock::YI_SYLLABLES_;

UnicodeBlock*& UnicodeBlock::YI_RADICALS()
{
    
    return YI_RADICALS_;
}
UnicodeBlock* UnicodeBlock::YI_RADICALS_;

UnicodeBlock*& UnicodeBlock::CYRILLIC_SUPPLEMENTARY()
{
    
    return CYRILLIC_SUPPLEMENTARY_;
}
UnicodeBlock* UnicodeBlock::CYRILLIC_SUPPLEMENTARY_;

UnicodeBlock*& UnicodeBlock::TAGALOG()
{
    
    return TAGALOG_;
}
UnicodeBlock* UnicodeBlock::TAGALOG_;

UnicodeBlock*& UnicodeBlock::HANUNOO()
{
    
    return HANUNOO_;
}
UnicodeBlock* UnicodeBlock::HANUNOO_;

UnicodeBlock*& UnicodeBlock::BUHID()
{
    
    return BUHID_;
}
UnicodeBlock* UnicodeBlock::BUHID_;

UnicodeBlock*& UnicodeBlock::TAGBANWA()
{
    
    return TAGBANWA_;
}
UnicodeBlock* UnicodeBlock::TAGBANWA_;

UnicodeBlock*& UnicodeBlock::LIMBU()
{
    
    return LIMBU_;
}
UnicodeBlock* UnicodeBlock::LIMBU_;

UnicodeBlock*& UnicodeBlock::TAI_LE()
{
    
    return TAI_LE_;
}
UnicodeBlock* UnicodeBlock::TAI_LE_;

UnicodeBlock*& UnicodeBlock::KHMER_SYMBOLS()
{
    
    return KHMER_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::KHMER_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::PHONETIC_EXTENSIONS()
{
    
    return PHONETIC_EXTENSIONS_;
}
UnicodeBlock* UnicodeBlock::PHONETIC_EXTENSIONS_;

UnicodeBlock*& UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A()
{
    
    return MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A_;
}
UnicodeBlock* UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A_;

UnicodeBlock*& UnicodeBlock::SUPPLEMENTAL_ARROWS_A()
{
    
    return SUPPLEMENTAL_ARROWS_A_;
}
UnicodeBlock* UnicodeBlock::SUPPLEMENTAL_ARROWS_A_;

UnicodeBlock*& UnicodeBlock::SUPPLEMENTAL_ARROWS_B()
{
    
    return SUPPLEMENTAL_ARROWS_B_;
}
UnicodeBlock* UnicodeBlock::SUPPLEMENTAL_ARROWS_B_;

UnicodeBlock*& UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B()
{
    
    return MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B_;
}
UnicodeBlock* UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B_;

UnicodeBlock*& UnicodeBlock::SUPPLEMENTAL_MATHEMATICAL_OPERATORS()
{
    
    return SUPPLEMENTAL_MATHEMATICAL_OPERATORS_;
}
UnicodeBlock* UnicodeBlock::SUPPLEMENTAL_MATHEMATICAL_OPERATORS_;

UnicodeBlock*& UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_ARROWS()
{
    
    return MISCELLANEOUS_SYMBOLS_AND_ARROWS_;
}
UnicodeBlock* UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_ARROWS_;

UnicodeBlock*& UnicodeBlock::KATAKANA_PHONETIC_EXTENSIONS()
{
    
    return KATAKANA_PHONETIC_EXTENSIONS_;
}
UnicodeBlock* UnicodeBlock::KATAKANA_PHONETIC_EXTENSIONS_;

UnicodeBlock*& UnicodeBlock::YIJING_HEXAGRAM_SYMBOLS()
{
    
    return YIJING_HEXAGRAM_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::YIJING_HEXAGRAM_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::VARIATION_SELECTORS()
{
    
    return VARIATION_SELECTORS_;
}
UnicodeBlock* UnicodeBlock::VARIATION_SELECTORS_;

UnicodeBlock*& UnicodeBlock::LINEAR_B_SYLLABARY()
{
    
    return LINEAR_B_SYLLABARY_;
}
UnicodeBlock* UnicodeBlock::LINEAR_B_SYLLABARY_;

UnicodeBlock*& UnicodeBlock::LINEAR_B_IDEOGRAMS()
{
    
    return LINEAR_B_IDEOGRAMS_;
}
UnicodeBlock* UnicodeBlock::LINEAR_B_IDEOGRAMS_;

UnicodeBlock*& UnicodeBlock::AEGEAN_NUMBERS()
{
    
    return AEGEAN_NUMBERS_;
}
UnicodeBlock* UnicodeBlock::AEGEAN_NUMBERS_;

UnicodeBlock*& UnicodeBlock::OLD_ITALIC()
{
    
    return OLD_ITALIC_;
}
UnicodeBlock* UnicodeBlock::OLD_ITALIC_;

UnicodeBlock*& UnicodeBlock::GOTHIC()
{
    
    return GOTHIC_;
}
UnicodeBlock* UnicodeBlock::GOTHIC_;

UnicodeBlock*& UnicodeBlock::UGARITIC()
{
    
    return UGARITIC_;
}
UnicodeBlock* UnicodeBlock::UGARITIC_;

UnicodeBlock*& UnicodeBlock::DESERET()
{
    
    return DESERET_;
}
UnicodeBlock* UnicodeBlock::DESERET_;

UnicodeBlock*& UnicodeBlock::SHAVIAN()
{
    
    return SHAVIAN_;
}
UnicodeBlock* UnicodeBlock::SHAVIAN_;

UnicodeBlock*& UnicodeBlock::OSMANYA()
{
    
    return OSMANYA_;
}
UnicodeBlock* UnicodeBlock::OSMANYA_;

UnicodeBlock*& UnicodeBlock::CYPRIOT_SYLLABARY()
{
    
    return CYPRIOT_SYLLABARY_;
}
UnicodeBlock* UnicodeBlock::CYPRIOT_SYLLABARY_;

UnicodeBlock*& UnicodeBlock::BYZANTINE_MUSICAL_SYMBOLS()
{
    
    return BYZANTINE_MUSICAL_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::BYZANTINE_MUSICAL_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::MUSICAL_SYMBOLS()
{
    
    return MUSICAL_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::MUSICAL_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::TAI_XUAN_JING_SYMBOLS()
{
    
    return TAI_XUAN_JING_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::TAI_XUAN_JING_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::MATHEMATICAL_ALPHANUMERIC_SYMBOLS()
{
    
    return MATHEMATICAL_ALPHANUMERIC_SYMBOLS_;
}
UnicodeBlock* UnicodeBlock::MATHEMATICAL_ALPHANUMERIC_SYMBOLS_;

UnicodeBlock*& UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B()
{
    
    return CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_;
}
UnicodeBlock* UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_;

UnicodeBlock*& UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT()
{
    
    return CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT_;
}
UnicodeBlock* UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT_;

UnicodeBlock*& UnicodeBlock::TAGS()
{
    
    return TAGS_;
}
UnicodeBlock* UnicodeBlock::TAGS_;

UnicodeBlock*& UnicodeBlock::VARIATION_SELECTORS_SUPPLEMENT()
{
    
    return VARIATION_SELECTORS_SUPPLEMENT_;
}
UnicodeBlock* UnicodeBlock::VARIATION_SELECTORS_SUPPLEMENT_;

UnicodeBlock*& UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_A()
{
    
    return SUPPLEMENTARY_PRIVATE_USE_AREA_A_;
}
UnicodeBlock* UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_A_;

UnicodeBlock*& UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_B()
{
    
    return SUPPLEMENTARY_PRIVATE_USE_AREA_B_;
}
UnicodeBlock* UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_B_;

UnicodeBlock*& UnicodeBlock::HIGH_SURROGATES()
{
    
    return HIGH_SURROGATES_;
}
UnicodeBlock* UnicodeBlock::HIGH_SURROGATES_;

UnicodeBlock*& UnicodeBlock::HIGH_PRIVATE_USE_SURROGATES()
{
    
    return HIGH_PRIVATE_USE_SURROGATES_;
}
UnicodeBlock* UnicodeBlock::HIGH_PRIVATE_USE_SURROGATES_;

UnicodeBlock*& UnicodeBlock::LOW_SURROGATES()
{
    
    return LOW_SURROGATES_;
}
UnicodeBlock* UnicodeBlock::LOW_SURROGATES_;

vector<int32_t>*& UnicodeBlock::blockStarts()
{
    
    return blockStarts_;
}
vector<int32_t>* UnicodeBlock::blockStarts_;

vector<UnicodeBlock*>*& UnicodeBlock::blocks()
{
    
    return blocks_;
}
vector<UnicodeBlock*>* UnicodeBlock::blocks_;

UnicodeBlock* UnicodeBlock::of(char16_t c)
{
    
    return of(static_cast< int32_t >(c));
}

UnicodeBlock* UnicodeBlock::of(int32_t codePoint)
{

	if (!Character::isValidCodePoint(codePoint)) {
		//throw new IllegalArgumentException();
		throw std::invalid_argument("Illegal argument");
	}
	int32_t top, bottom, current;
	bottom = 0;
	top = blockStarts_->size();
	current = top / int32_t(2);
	while (top - bottom > 1) {
		if (codePoint >= (*blockStarts_)[current]) {
			bottom = current;
		}
		else {
			top = current;
		}
		current = (top + bottom) / int32_t(2);
	}
	return (*blocks_)[current];
}

UnicodeBlock* UnicodeBlock::forName(String* blockName)
{

	//auto block = dynamic_cast<UnicodeBlock*>(dynamic_cast<Object*>(npc(map_)->get(npc(blockName)->toUpperCase(::java::util::Locale::US()))));
	auto block = nullptr;
	if (block == nullptr) {
		//throw new IllegalArgumentException();
		throw std::invalid_argument("Illegal argument");
	}
	return block;
}

/*
map_ = new ::java::util::HashMap();
BASIC_LATIN_ = new UnicodeBlock(u"BASIC_LATIN"_j, new StringArray({
u"Basic Latin"_j
, u"BasicLatin"_j
}));
LATIN_1_SUPPLEMENT_ = new UnicodeBlock(u"LATIN_1_SUPPLEMENT"_j, new StringArray({
u"Latin-1 Supplement"_j
, u"Latin-1Supplement"_j
}));
LATIN_EXTENDED_A_ = new UnicodeBlock(u"LATIN_EXTENDED_A"_j, new StringArray({
u"Latin Extended-A"_j
, u"LatinExtended-A"_j
}));
LATIN_EXTENDED_B_ = new UnicodeBlock(u"LATIN_EXTENDED_B"_j, new StringArray({
u"Latin Extended-B"_j
, u"LatinExtended-B"_j
}));
IPA_EXTENSIONS_ = new UnicodeBlock(u"IPA_EXTENSIONS"_j, new StringArray({
u"IPA Extensions"_j
, u"IPAExtensions"_j
}));
SPACING_MODIFIER_LETTERS_ = new UnicodeBlock(u"SPACING_MODIFIER_LETTERS"_j, new StringArray({
u"Spacing Modifier Letters"_j
, u"SpacingModifierLetters"_j
}));
COMBINING_DIACRITICAL_MARKS_ = new UnicodeBlock(u"COMBINING_DIACRITICAL_MARKS"_j, new StringArray({
u"Combining Diacritical Marks"_j
, u"CombiningDiacriticalMarks"_j
}));
GREEK_ = new UnicodeBlock(u"GREEK"_j, new StringArray({
u"Greek and Coptic"_j
, u"GreekandCoptic"_j
}));
CYRILLIC_ = new UnicodeBlock(u"CYRILLIC"_j);
ARMENIAN_ = new UnicodeBlock(u"ARMENIAN"_j);
HEBREW_ = new UnicodeBlock(u"HEBREW"_j);
ARABIC_ = new UnicodeBlock(u"ARABIC"_j);
DEVANAGARI_ = new UnicodeBlock(u"DEVANAGARI"_j);
BENGALI_ = new UnicodeBlock(u"BENGALI"_j);
GURMUKHI_ = new UnicodeBlock(u"GURMUKHI"_j);
GUJARATI_ = new UnicodeBlock(u"GUJARATI"_j);
ORIYA_ = new UnicodeBlock(u"ORIYA"_j);
TAMIL_ = new UnicodeBlock(u"TAMIL"_j);
TELUGU_ = new UnicodeBlock(u"TELUGU"_j);
KANNADA_ = new UnicodeBlock(u"KANNADA"_j);
MALAYALAM_ = new UnicodeBlock(u"MALAYALAM"_j);
THAI_ = new UnicodeBlock(u"THAI"_j);
LAO_ = new UnicodeBlock(u"LAO"_j);
TIBETAN_ = new UnicodeBlock(u"TIBETAN"_j);
GEORGIAN_ = new UnicodeBlock(u"GEORGIAN"_j);
HANGUL_JAMO_ = new UnicodeBlock(u"HANGUL_JAMO"_j, new StringArray({
u"Hangul Jamo"_j
, u"HangulJamo"_j
}));
LATIN_EXTENDED_ADDITIONAL_ = new UnicodeBlock(u"LATIN_EXTENDED_ADDITIONAL"_j, new StringArray({
u"Latin Extended Additional"_j
, u"LatinExtendedAdditional"_j
}));
GREEK_EXTENDED_ = new UnicodeBlock(u"GREEK_EXTENDED"_j, new StringArray({
u"Greek Extended"_j
, u"GreekExtended"_j
}));
GENERAL_PUNCTUATION_ = new UnicodeBlock(u"GENERAL_PUNCTUATION"_j, new StringArray({
u"General Punctuation"_j
, u"GeneralPunctuation"_j
}));
SUPERSCRIPTS_AND_SUBSCRIPTS_ = new UnicodeBlock(u"SUPERSCRIPTS_AND_SUBSCRIPTS"_j, new StringArray({
u"Superscripts and Subscripts"_j
, u"SuperscriptsandSubscripts"_j
}));
CURRENCY_SYMBOLS_ = new UnicodeBlock(u"CURRENCY_SYMBOLS"_j, new StringArray({
u"Currency Symbols"_j
, u"CurrencySymbols"_j
}));
COMBINING_MARKS_FOR_SYMBOLS_ = new UnicodeBlock(u"COMBINING_MARKS_FOR_SYMBOLS"_j, new StringArray({
u"Combining Diacritical Marks for Symbols"_j
, u"CombiningDiacriticalMarksforSymbols"_j
, u"Combining Marks for Symbols"_j
, u"CombiningMarksforSymbols"_j
}));
LETTERLIKE_SYMBOLS_ = new UnicodeBlock(u"LETTERLIKE_SYMBOLS"_j, new StringArray({
u"Letterlike Symbols"_j
, u"LetterlikeSymbols"_j
}));
NUMBER_FORMS_ = new UnicodeBlock(u"NUMBER_FORMS"_j, new StringArray({
u"Number Forms"_j
, u"NumberForms"_j
}));
ARROWS_ = new UnicodeBlock(u"ARROWS"_j);
MATHEMATICAL_OPERATORS_ = new UnicodeBlock(u"MATHEMATICAL_OPERATORS"_j, new StringArray({
u"Mathematical Operators"_j
, u"MathematicalOperators"_j
}));
MISCELLANEOUS_TECHNICAL_ = new UnicodeBlock(u"MISCELLANEOUS_TECHNICAL"_j, new StringArray({
u"Miscellaneous Technical"_j
, u"MiscellaneousTechnical"_j
}));
CONTROL_PICTURES_ = new UnicodeBlock(u"CONTROL_PICTURES"_j, new StringArray({
u"Control Pictures"_j
, u"ControlPictures"_j
}));
OPTICAL_CHARACTER_RECOGNITION_ = new UnicodeBlock(u"OPTICAL_CHARACTER_RECOGNITION"_j, new StringArray({
u"Optical Character Recognition"_j
, u"OpticalCharacterRecognition"_j
}));
ENCLOSED_ALPHANUMERICS_ = new UnicodeBlock(u"ENCLOSED_ALPHANUMERICS"_j, new StringArray({
u"Enclosed Alphanumerics"_j
, u"EnclosedAlphanumerics"_j
}));
BOX_DRAWING_ = new UnicodeBlock(u"BOX_DRAWING"_j, new StringArray({
u"Box Drawing"_j
, u"BoxDrawing"_j
}));
BLOCK_ELEMENTS_ = new UnicodeBlock(u"BLOCK_ELEMENTS"_j, new StringArray({
u"Block Elements"_j
, u"BlockElements"_j
}));
GEOMETRIC_SHAPES_ = new UnicodeBlock(u"GEOMETRIC_SHAPES"_j, new StringArray({
u"Geometric Shapes"_j
, u"GeometricShapes"_j
}));
MISCELLANEOUS_SYMBOLS_ = new UnicodeBlock(u"MISCELLANEOUS_SYMBOLS"_j, new StringArray({
u"Miscellaneous Symbols"_j
, u"MiscellaneousSymbols"_j
}));
DINGBATS_ = new UnicodeBlock(u"DINGBATS"_j);
CJK_SYMBOLS_AND_PUNCTUATION_ = new UnicodeBlock(u"CJK_SYMBOLS_AND_PUNCTUATION"_j, new StringArray({
u"CJK Symbols and Punctuation"_j
, u"CJKSymbolsandPunctuation"_j
}));
HIRAGANA_ = new UnicodeBlock(u"HIRAGANA"_j);
KATAKANA_ = new UnicodeBlock(u"KATAKANA"_j);
BOPOMOFO_ = new UnicodeBlock(u"BOPOMOFO"_j);
HANGUL_COMPATIBILITY_JAMO_ = new UnicodeBlock(u"HANGUL_COMPATIBILITY_JAMO"_j, new StringArray({
u"Hangul Compatibility Jamo"_j
, u"HangulCompatibilityJamo"_j
}));
KANBUN_ = new UnicodeBlock(u"KANBUN"_j);
ENCLOSED_CJK_LETTERS_AND_MONTHS_ = new UnicodeBlock(u"ENCLOSED_CJK_LETTERS_AND_MONTHS"_j, new StringArray({
u"Enclosed CJK Letters and Months"_j
, u"EnclosedCJKLettersandMonths"_j
}));
CJK_COMPATIBILITY_ = new UnicodeBlock(u"CJK_COMPATIBILITY"_j, new StringArray({
u"CJK Compatibility"_j
, u"CJKCompatibility"_j
}));
CJK_UNIFIED_IDEOGRAPHS_ = new UnicodeBlock(u"CJK_UNIFIED_IDEOGRAPHS"_j, new StringArray({
u"CJK Unified Ideographs"_j
, u"CJKUnifiedIdeographs"_j
}));
HANGUL_SYLLABLES_ = new UnicodeBlock(u"HANGUL_SYLLABLES"_j, new StringArray({
u"Hangul Syllables"_j
, u"HangulSyllables"_j
}));
PRIVATE_USE_AREA_ = new UnicodeBlock(u"PRIVATE_USE_AREA"_j, new StringArray({
u"Private Use Area"_j
, u"PrivateUseArea"_j
}));
CJK_COMPATIBILITY_IDEOGRAPHS_ = new UnicodeBlock(u"CJK_COMPATIBILITY_IDEOGRAPHS"_j, new StringArray({
u"CJK Compatibility Ideographs"_j
, u"CJKCompatibilityIdeographs"_j
}));
ALPHABETIC_PRESENTATION_FORMS_ = new UnicodeBlock(u"ALPHABETIC_PRESENTATION_FORMS"_j, new StringArray({
u"Alphabetic Presentation Forms"_j
, u"AlphabeticPresentationForms"_j
}));
ARABIC_PRESENTATION_FORMS_A_ = new UnicodeBlock(u"ARABIC_PRESENTATION_FORMS_A"_j, new StringArray({
u"Arabic Presentation Forms-A"_j
, u"ArabicPresentationForms-A"_j
}));
COMBINING_HALF_MARKS_ = new UnicodeBlock(u"COMBINING_HALF_MARKS"_j, new StringArray({
u"Combining Half Marks"_j
, u"CombiningHalfMarks"_j
}));
CJK_COMPATIBILITY_FORMS_ = new UnicodeBlock(u"CJK_COMPATIBILITY_FORMS"_j, new StringArray({
u"CJK Compatibility Forms"_j
, u"CJKCompatibilityForms"_j
}));
SMALL_FORM_VARIANTS_ = new UnicodeBlock(u"SMALL_FORM_VARIANTS"_j, new StringArray({
u"Small Form Variants"_j
, u"SmallFormVariants"_j
}));
ARABIC_PRESENTATION_FORMS_B_ = new UnicodeBlock(u"ARABIC_PRESENTATION_FORMS_B"_j, new StringArray({
u"Arabic Presentation Forms-B"_j
, u"ArabicPresentationForms-B"_j
}));
HALFWIDTH_AND_FULLWIDTH_FORMS_ = new UnicodeBlock(u"HALFWIDTH_AND_FULLWIDTH_FORMS"_j, new StringArray({
u"Halfwidth and Fullwidth Forms"_j
, u"HalfwidthandFullwidthForms"_j
}));
SPECIALS_ = new UnicodeBlock(u"SPECIALS"_j);
SURROGATES_AREA_ = new UnicodeBlock(u"SURROGATES_AREA"_j);
SYRIAC_ = new UnicodeBlock(u"SYRIAC"_j);
THAANA_ = new UnicodeBlock(u"THAANA"_j);
SINHALA_ = new UnicodeBlock(u"SINHALA"_j);
MYANMAR_ = new UnicodeBlock(u"MYANMAR"_j);
ETHIOPIC_ = new UnicodeBlock(u"ETHIOPIC"_j);
CHEROKEE_ = new UnicodeBlock(u"CHEROKEE"_j);
UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_ = new UnicodeBlock(u"UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS"_j, new StringArray({
u"Unified Canadian Aboriginal Syllabics"_j
, u"UnifiedCanadianAboriginalSyllabics"_j
}));
OGHAM_ = new UnicodeBlock(u"OGHAM"_j);
RUNIC_ = new UnicodeBlock(u"RUNIC"_j);
KHMER_ = new UnicodeBlock(u"KHMER"_j);
MONGOLIAN_ = new UnicodeBlock(u"MONGOLIAN"_j);
BRAILLE_PATTERNS_ = new UnicodeBlock(u"BRAILLE_PATTERNS"_j, new StringArray({
u"Braille Patterns"_j
, u"BraillePatterns"_j
}));
CJK_RADICALS_SUPPLEMENT_ = new UnicodeBlock(u"CJK_RADICALS_SUPPLEMENT"_j, new StringArray({
u"CJK Radicals Supplement"_j
, u"CJKRadicalsSupplement"_j
}));
KANGXI_RADICALS_ = new UnicodeBlock(u"KANGXI_RADICALS"_j, new StringArray({
u"Kangxi Radicals"_j
, u"KangxiRadicals"_j
}));
IDEOGRAPHIC_DESCRIPTION_CHARACTERS_ = new UnicodeBlock(u"IDEOGRAPHIC_DESCRIPTION_CHARACTERS"_j, new StringArray({
u"Ideographic Description Characters"_j
, u"IdeographicDescriptionCharacters"_j
}));
BOPOMOFO_EXTENDED_ = new UnicodeBlock(u"BOPOMOFO_EXTENDED"_j, new StringArray({
u"Bopomofo Extended"_j
, u"BopomofoExtended"_j
}));
CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_ = new UnicodeBlock(u"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A"_j, new StringArray({
u"CJK Unified Ideographs Extension A"_j
, u"CJKUnifiedIdeographsExtensionA"_j
}));
YI_SYLLABLES_ = new UnicodeBlock(u"YI_SYLLABLES"_j, new StringArray({
u"Yi Syllables"_j
, u"YiSyllables"_j
}));
YI_RADICALS_ = new UnicodeBlock(u"YI_RADICALS"_j, new StringArray({
u"Yi Radicals"_j
, u"YiRadicals"_j
}));
CYRILLIC_SUPPLEMENTARY_ = new UnicodeBlock(u"CYRILLIC_SUPPLEMENTARY"_j, new StringArray({
u"Cyrillic Supplementary"_j
, u"CyrillicSupplementary"_j
}));
TAGALOG_ = new UnicodeBlock(u"TAGALOG"_j);
HANUNOO_ = new UnicodeBlock(u"HANUNOO"_j);
BUHID_ = new UnicodeBlock(u"BUHID"_j);
TAGBANWA_ = new UnicodeBlock(u"TAGBANWA"_j);
LIMBU_ = new UnicodeBlock(u"LIMBU"_j);
TAI_LE_ = new UnicodeBlock(u"TAI_LE"_j, new StringArray({
u"Tai Le"_j
, u"TaiLe"_j
}));
KHMER_SYMBOLS_ = new UnicodeBlock(u"KHMER_SYMBOLS"_j, new StringArray({
u"Khmer Symbols"_j
, u"KhmerSymbols"_j
}));
PHONETIC_EXTENSIONS_ = new UnicodeBlock(u"PHONETIC_EXTENSIONS"_j, new StringArray({
u"Phonetic Extensions"_j
, u"PhoneticExtensions"_j
}));
MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A_ = new UnicodeBlock(u"MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A"_j, new StringArray({
u"Miscellaneous Mathematical Symbols-A"_j
, u"MiscellaneousMathematicalSymbols-A"_j
}));
SUPPLEMENTAL_ARROWS_A_ = new UnicodeBlock(u"SUPPLEMENTAL_ARROWS_A"_j, new StringArray({
u"Supplemental Arrows-A"_j
, u"SupplementalArrows-A"_j
}));
SUPPLEMENTAL_ARROWS_B_ = new UnicodeBlock(u"SUPPLEMENTAL_ARROWS_B"_j, new StringArray({
u"Supplemental Arrows-B"_j
, u"SupplementalArrows-B"_j
}));
MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B_ = new UnicodeBlock(u"MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B"_j, new StringArray({
u"Miscellaneous Mathematical Symbols-B"_j
, u"MiscellaneousMathematicalSymbols-B"_j
}));
SUPPLEMENTAL_MATHEMATICAL_OPERATORS_ = new UnicodeBlock(u"SUPPLEMENTAL_MATHEMATICAL_OPERATORS"_j, new StringArray({
u"Supplemental Mathematical Operators"_j
, u"SupplementalMathematicalOperators"_j
}));
MISCELLANEOUS_SYMBOLS_AND_ARROWS_ = new UnicodeBlock(u"MISCELLANEOUS_SYMBOLS_AND_ARROWS"_j, new StringArray({
u"Miscellaneous Symbols and Arrows"_j
, u"MiscellaneousSymbolsandArrows"_j
}));
KATAKANA_PHONETIC_EXTENSIONS_ = new UnicodeBlock(u"KATAKANA_PHONETIC_EXTENSIONS"_j, new StringArray({
u"Katakana Phonetic Extensions"_j
, u"KatakanaPhoneticExtensions"_j
}));
YIJING_HEXAGRAM_SYMBOLS_ = new UnicodeBlock(u"YIJING_HEXAGRAM_SYMBOLS"_j, new StringArray({
u"Yijing Hexagram Symbols"_j
, u"YijingHexagramSymbols"_j
}));
VARIATION_SELECTORS_ = new UnicodeBlock(u"VARIATION_SELECTORS"_j, new StringArray({
u"Variation Selectors"_j
, u"VariationSelectors"_j
}));
LINEAR_B_SYLLABARY_ = new UnicodeBlock(u"LINEAR_B_SYLLABARY"_j, new StringArray({
u"Linear B Syllabary"_j
, u"LinearBSyllabary"_j
}));
LINEAR_B_IDEOGRAMS_ = new UnicodeBlock(u"LINEAR_B_IDEOGRAMS"_j, new StringArray({
u"Linear B Ideograms"_j
, u"LinearBIdeograms"_j
}));
AEGEAN_NUMBERS_ = new UnicodeBlock(u"AEGEAN_NUMBERS"_j, new StringArray({
u"Aegean Numbers"_j
, u"AegeanNumbers"_j
}));
OLD_ITALIC_ = new UnicodeBlock(u"OLD_ITALIC"_j, new StringArray({
u"Old Italic"_j
, u"OldItalic"_j
}));
GOTHIC_ = new UnicodeBlock(u"GOTHIC"_j);
UGARITIC_ = new UnicodeBlock(u"UGARITIC"_j);
DESERET_ = new UnicodeBlock(u"DESERET"_j);
SHAVIAN_ = new UnicodeBlock(u"SHAVIAN"_j);
OSMANYA_ = new UnicodeBlock(u"OSMANYA"_j);
CYPRIOT_SYLLABARY_ = new UnicodeBlock(u"CYPRIOT_SYLLABARY"_j, new StringArray({
u"Cypriot Syllabary"_j
, u"CypriotSyllabary"_j
}));
BYZANTINE_MUSICAL_SYMBOLS_ = new UnicodeBlock(u"BYZANTINE_MUSICAL_SYMBOLS"_j, new StringArray({
u"Byzantine Musical Symbols"_j
, u"ByzantineMusicalSymbols"_j
}));
MUSICAL_SYMBOLS_ = new UnicodeBlock(u"MUSICAL_SYMBOLS"_j, new StringArray({
u"Musical Symbols"_j
, u"MusicalSymbols"_j
}));
TAI_XUAN_JING_SYMBOLS_ = new UnicodeBlock(u"TAI_XUAN_JING_SYMBOLS"_j, new StringArray({
u"Tai Xuan Jing Symbols"_j
, u"TaiXuanJingSymbols"_j
}));
MATHEMATICAL_ALPHANUMERIC_SYMBOLS_ = new UnicodeBlock(u"MATHEMATICAL_ALPHANUMERIC_SYMBOLS"_j, new StringArray({
u"Mathematical Alphanumeric Symbols"_j
, u"MathematicalAlphanumericSymbols"_j
}));
CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_ = new UnicodeBlock(u"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B"_j, new StringArray({
u"CJK Unified Ideographs Extension B"_j
, u"CJKUnifiedIdeographsExtensionB"_j
}));
CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT_ = new UnicodeBlock(u"CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT"_j, new StringArray({
u"CJK Compatibility Ideographs Supplement"_j
, u"CJKCompatibilityIdeographsSupplement"_j
}));
TAGS_ = new UnicodeBlock(u"TAGS"_j);
VARIATION_SELECTORS_SUPPLEMENT_ = new UnicodeBlock(u"VARIATION_SELECTORS_SUPPLEMENT"_j, new StringArray({
u"Variation Selectors Supplement"_j
, u"VariationSelectorsSupplement"_j
}));
SUPPLEMENTARY_PRIVATE_USE_AREA_A_ = new UnicodeBlock(u"SUPPLEMENTARY_PRIVATE_USE_AREA_A"_j, new StringArray({
u"Supplementary Private Use Area-A"_j
, u"SupplementaryPrivateUseArea-A"_j
}));
SUPPLEMENTARY_PRIVATE_USE_AREA_B_ = new UnicodeBlock(u"SUPPLEMENTARY_PRIVATE_USE_AREA_B"_j, new StringArray({
u"Supplementary Private Use Area-B"_j
, u"SupplementaryPrivateUseArea-B"_j
}));
HIGH_SURROGATES_ = new UnicodeBlock(u"HIGH_SURROGATES"_j, new StringArray({
u"High Surrogates"_j
, u"HighSurrogates"_j
}));
HIGH_PRIVATE_USE_SURROGATES_ = new UnicodeBlock(u"HIGH_PRIVATE_USE_SURROGATES"_j, new StringArray({
u"High Private Use Surrogates"_j
, u"HighPrivateUseSurrogates"_j
}));
LOW_SURROGATES_ = new UnicodeBlock(u"LOW_SURROGATES"_j, new StringArray({
u"Low Surrogates"_j
, u"LowSurrogates"_j
}));
blockStarts_ = (new ::vector<int32_t>({
int32_t(0)
, int32_t(128)
, int32_t(256)
, int32_t(384)
, int32_t(592)
, int32_t(688)
, int32_t(768)
, int32_t(880)
, int32_t(1024)
, int32_t(1280)
, int32_t(1328)
, int32_t(1424)
, int32_t(1536)
, int32_t(1792)
, int32_t(1872)
, int32_t(1920)
, int32_t(1984)
, int32_t(2304)
, int32_t(2432)
, int32_t(2560)
, int32_t(2688)
, int32_t(2816)
, int32_t(2944)
, int32_t(3072)
, int32_t(3200)
, int32_t(3328)
, int32_t(3456)
, int32_t(3584)
, int32_t(3712)
, int32_t(3840)
, int32_t(4096)
, int32_t(4256)
, int32_t(4352)
, int32_t(4608)
, int32_t(4992)
, int32_t(5024)
, int32_t(5120)
, int32_t(5760)
, int32_t(5792)
, int32_t(5888)
, int32_t(5920)
, int32_t(5952)
, int32_t(5984)
, int32_t(6016)
, int32_t(6144)
, int32_t(6320)
, int32_t(6400)
, int32_t(6480)
, int32_t(6528)
, int32_t(6624)
, int32_t(6656)
, int32_t(7424)
, int32_t(7552)
, int32_t(7680)
, int32_t(7936)
, int32_t(8192)
, int32_t(8304)
, int32_t(8352)
, int32_t(8400)
, int32_t(8448)
, int32_t(8528)
, int32_t(8592)
, int32_t(8704)
, int32_t(8960)
, int32_t(9216)
, int32_t(9280)
, int32_t(9312)
, int32_t(9472)
, int32_t(9600)
, int32_t(9632)
, int32_t(9728)
, int32_t(9984)
, int32_t(10176)
, int32_t(10224)
, int32_t(10240)
, int32_t(10496)
, int32_t(10624)
, int32_t(10752)
, int32_t(11008)
, int32_t(11264)
, int32_t(11904)
, int32_t(12032)
, int32_t(12256)
, int32_t(12272)
, int32_t(12288)
, int32_t(12352)
, int32_t(12448)
, int32_t(12544)
, int32_t(12592)
, int32_t(12688)
, int32_t(12704)
, int32_t(12736)
, int32_t(12784)
, int32_t(12800)
, int32_t(13056)
, int32_t(13312)
, int32_t(19904)
, int32_t(19968)
, int32_t(40960)
, int32_t(42128)
, int32_t(42192)
, int32_t(44032)
, int32_t(55216)
, int32_t(55296)
, int32_t(56192)
, int32_t(56320)
, int32_t(57344)
, int32_t(63744)
, int32_t(64256)
, int32_t(64336)
, int32_t(65024)
, int32_t(65040)
, int32_t(65056)
, int32_t(65072)
, int32_t(65104)
, int32_t(65136)
, int32_t(65280)
, int32_t(65520)
, int32_t(65536)
, int32_t(65664)
, int32_t(65792)
, int32_t(65856)
, int32_t(66304)
, int32_t(66352)
, int32_t(66384)
, int32_t(66432)
, int32_t(66464)
, int32_t(66560)
, int32_t(66640)
, int32_t(66688)
, int32_t(66736)
, int32_t(67584)
, int32_t(67648)
, int32_t(118784)
, int32_t(119040)
, int32_t(119296)
, int32_t(119552)
, int32_t(119648)
, int32_t(119808)
, int32_t(120832)
, int32_t(131072)
, int32_t(173792)
, int32_t(194560)
, int32_t(195104)
, int32_t(917504)
, int32_t(917632)
, int32_t(917760)
, int32_t(918000)
, int32_t(983040)
, int32_t(1048576)
}));
blocks_ = (new ::vector<UnicodeBlock*>({
BASIC_LATIN_
, LATIN_1_SUPPLEMENT_
, LATIN_EXTENDED_A_
, LATIN_EXTENDED_B_
, IPA_EXTENSIONS_
, SPACING_MODIFIER_LETTERS_
, COMBINING_DIACRITICAL_MARKS_
, GREEK_
, CYRILLIC_
, CYRILLIC_SUPPLEMENTARY_
, ARMENIAN_
, HEBREW_
, ARABIC_
, SYRIAC_
, static_cast< UnicodeBlock* >(nullptr)
, THAANA_
, static_cast< UnicodeBlock* >(nullptr)
, DEVANAGARI_
, BENGALI_
, GURMUKHI_
, GUJARATI_
, ORIYA_
, TAMIL_
, TELUGU_
, KANNADA_
, MALAYALAM_
, SINHALA_
, THAI_
, LAO_
, TIBETAN_
, MYANMAR_
, GEORGIAN_
, HANGUL_JAMO_
, ETHIOPIC_
, static_cast< UnicodeBlock* >(nullptr)
, CHEROKEE_
, UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_
, OGHAM_
, RUNIC_
, TAGALOG_
, HANUNOO_
, BUHID_
, TAGBANWA_
, KHMER_
, MONGOLIAN_
, static_cast< UnicodeBlock* >(nullptr)
, LIMBU_
, TAI_LE_
, static_cast< UnicodeBlock* >(nullptr)
, KHMER_SYMBOLS_
, static_cast< UnicodeBlock* >(nullptr)
, PHONETIC_EXTENSIONS_
, static_cast< UnicodeBlock* >(nullptr)
, LATIN_EXTENDED_ADDITIONAL_
, GREEK_EXTENDED_
, GENERAL_PUNCTUATION_
, SUPERSCRIPTS_AND_SUBSCRIPTS_
, CURRENCY_SYMBOLS_
, COMBINING_MARKS_FOR_SYMBOLS_
, LETTERLIKE_SYMBOLS_
, NUMBER_FORMS_
, ARROWS_
, MATHEMATICAL_OPERATORS_
, MISCELLANEOUS_TECHNICAL_
, CONTROL_PICTURES_
, OPTICAL_CHARACTER_RECOGNITION_
, ENCLOSED_ALPHANUMERICS_
, BOX_DRAWING_
, BLOCK_ELEMENTS_
, GEOMETRIC_SHAPES_
, MISCELLANEOUS_SYMBOLS_
, DINGBATS_
, MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A_
, SUPPLEMENTAL_ARROWS_A_
, BRAILLE_PATTERNS_
, SUPPLEMENTAL_ARROWS_B_
, MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B_
, SUPPLEMENTAL_MATHEMATICAL_OPERATORS_
, MISCELLANEOUS_SYMBOLS_AND_ARROWS_
, static_cast< UnicodeBlock* >(nullptr)
, CJK_RADICALS_SUPPLEMENT_
, KANGXI_RADICALS_
, static_cast< UnicodeBlock* >(nullptr)
, IDEOGRAPHIC_DESCRIPTION_CHARACTERS_
, CJK_SYMBOLS_AND_PUNCTUATION_
, HIRAGANA_
, KATAKANA_
, BOPOMOFO_
, HANGUL_COMPATIBILITY_JAMO_
, KANBUN_
, BOPOMOFO_EXTENDED_
, static_cast< UnicodeBlock* >(nullptr)
, KATAKANA_PHONETIC_EXTENSIONS_
, ENCLOSED_CJK_LETTERS_AND_MONTHS_
, CJK_COMPATIBILITY_
, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_
, YIJING_HEXAGRAM_SYMBOLS_
, CJK_UNIFIED_IDEOGRAPHS_
, YI_SYLLABLES_
, YI_RADICALS_
, static_cast< UnicodeBlock* >(nullptr)
, HANGUL_SYLLABLES_
, static_cast< UnicodeBlock* >(nullptr)
, HIGH_SURROGATES_
, HIGH_PRIVATE_USE_SURROGATES_
, LOW_SURROGATES_
, PRIVATE_USE_AREA_
, CJK_COMPATIBILITY_IDEOGRAPHS_
, ALPHABETIC_PRESENTATION_FORMS_
, ARABIC_PRESENTATION_FORMS_A_
, VARIATION_SELECTORS_
, static_cast< UnicodeBlock* >(nullptr)
, COMBINING_HALF_MARKS_
, CJK_COMPATIBILITY_FORMS_
, SMALL_FORM_VARIANTS_
, ARABIC_PRESENTATION_FORMS_B_
, HALFWIDTH_AND_FULLWIDTH_FORMS_
, SPECIALS_
, LINEAR_B_SYLLABARY_
, LINEAR_B_IDEOGRAMS_
, AEGEAN_NUMBERS_
, static_cast< UnicodeBlock* >(nullptr)
, OLD_ITALIC_
, GOTHIC_
, static_cast< UnicodeBlock* >(nullptr)
, UGARITIC_
, static_cast< UnicodeBlock* >(nullptr)
, DESERET_
, SHAVIAN_
, OSMANYA_
, static_cast< UnicodeBlock* >(nullptr)
, CYPRIOT_SYLLABARY_
, static_cast< UnicodeBlock* >(nullptr)
, BYZANTINE_MUSICAL_SYMBOLS_
, MUSICAL_SYMBOLS_
, static_cast< UnicodeBlock* >(nullptr)
, TAI_XUAN_JING_SYMBOLS_
, static_cast< UnicodeBlock* >(nullptr)
, MATHEMATICAL_ALPHANUMERIC_SYMBOLS_
, static_cast< UnicodeBlock* >(nullptr)
, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_
, static_cast< UnicodeBlock* >(nullptr)
, CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT_
, static_cast< UnicodeBlock* >(nullptr)
, TAGS_
, static_cast< UnicodeBlock* >(nullptr)
, VARIATION_SELECTORS_SUPPLEMENT_
, static_cast< UnicodeBlock* >(nullptr)
, SUPPLEMENTARY_PRIVATE_USE_AREA_A_
, SUPPLEMENTARY_PRIVATE_USE_AREA_B_
}));

*/
