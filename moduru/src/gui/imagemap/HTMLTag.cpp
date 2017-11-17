#include <gui/imagemap/HTMLTag.hpp>

#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>
#include <lang/StrUtil.hpp>

using namespace moduru::lang;
using namespace moduru::gui::imagemap;
using namespace std;

HTMLTag::HTMLTag(moduru::lang::String* tag)
{
	this->tag = tag;
	tag_lc = new String(StrUtil::toLower(tag->to_cppstring()));
}

moduru::lang::String HTMLTag::getArgumentValue(moduru::lang::String arg)
{
	if (arg.length() > tag->length() + 5)
		return String("");

	auto i = tag_lc->indexOf(String(StrUtil::toLower(arg.to_cppstring())));
	if (i < 0)
		return String("");

	i = tag->indexOf(moduru::lang::String("="), i);
	if (i < 0)
		return String("");

	i++;
	for (; i < tag->length() && tag->charAt(i) == u' '; i++)
		;
	String val;
	if (tag->charAt(i) == u'"')
		for (i++; i < tag->length() && tag->charAt(i) != u'"'; i++) {
			string s1 = val.to_cppstring() + tag->to_cppstring().substr(i, 1);
			val = moduru::lang::String(s1);
		}

	else
		for (; i < tag->length() && tag->charAt(i) != u' ' && tag->charAt(i) != u'>'; i++) {
			//val = moduru::lang::StringBuilder(val).append(tag->substring(i, i + 1))->toString();
			string s1 = val.to_cppstring() + tag->to_cppstring().substr(i, 1);
			val = moduru::lang::String(s1);
		}

	return val;
}

moduru::lang::String HTMLTag::getTagType()
{
	int i = tag->indexOf(moduru::lang::String(" "));
	if (i < 0)
		i = tag->indexOf(moduru::lang::String(">"));

	if (i < 0)
		i = tag->length() - 1;
	auto res = String(tag->to_cppstring());
	res = res.substring(1, i);
	return res;
}

HTMLTag::~HTMLTag() {
	delete tag;
	tag = nullptr;
	delete tag_lc;
	tag_lc = nullptr;
}
