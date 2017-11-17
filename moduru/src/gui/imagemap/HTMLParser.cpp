#include <gui/imagemap/HTMLParser.hpp>

#include <gui/imagemap/HTMLTag.hpp>
#include <gui/imagemap/Shape.hpp>
#include <gui/imagemap/ShapeList.hpp>
#include <gui/Point.hpp>
#include <gui/Polygon.hpp>
#include <lang/Integer.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>

#include <lang/StrUtil.hpp>

#include <sstream>

using namespace moduru::gui::imagemap;
using namespace moduru::lang;
using namespace std;

HTMLParser::HTMLParser(String* html) 
{
	this->html = new String(html);
	parseIndex = 0;
}

String HTMLParser::getImageSrc()
{
	parseIndex = 0;
	HTMLTag* tag{ nullptr };
	if ((tag = nextTag()) != nullptr) {
		if (tag->getTagType().compareToIgnoreCase(String("img")) == 0 && tag->getArgumentValue(String("usemap")).length() != 0) {
			auto res = tag->getArgumentValue(String("src"));
			delete tag;
			return res;
		}
	}
	return String("");
}

String HTMLParser::getMapName()
{
	parseIndex = 0;
	HTMLTag* tag{ nullptr };
	if ((tag = nextTag()) != nullptr) {
		if (tag->getTagType().compareToIgnoreCase(String("map")) == 0 && tag->getArgumentValue(String("name")).length() != 0) {
			auto res = tag->getArgumentValue(String("name"));
			delete tag;
			return res;
		}
	}
	return String("");
}

void HTMLParser::createShapeList(std::weak_ptr<ShapeList> sl)
{
	auto lSl = sl.lock();
	lSl->clear();
	HTMLTag* tag{ nullptr };
	parseIndex = 0;
	tag = nextTag();

	while (tag != nullptr) {
		std::shared_ptr<Shape> sh;
		auto tagTypeStr = tag->getTagType().to_cppstring();
		bool tagType = StrUtil::eqIgnoreCase(tagTypeStr, "area");
		auto argValue = tag->getArgumentValue(String("shape"));
		bool tagArg = StrUtil::eqIgnoreCase(argValue.to_cppstring(), "rect");
		if (tagType && tagArg) {
			istringstream is(tag->getArgumentValue(String("coords")).to_cppstring());
			string part;
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

			if (getline(is, part, ','))
				x1 = stoi(StrUtil::trim(part));

			if (getline(is, part, ','))
				y1 = stoi(StrUtil::trim(part));

			if (getline(is, part, ','))
				x2 = stoi(StrUtil::trim(part));

			if (getline(is, part, ','))
				y2 = stoi(StrUtil::trim(part));
			sh = std::make_shared<Shape>(x1, y1, x2, y2);
		}
		else if (StrUtil::eqIgnoreCase(tag->getTagType().to_cppstring(), "area") && StrUtil::eqIgnoreCase(tag->getArgumentValue(String("shape")).to_cppstring(), "circle")) {
			istringstream is(tag->getArgumentValue(String("coords")).to_cppstring());
			string part;
			int x = 0, y = 0, r = 0;
			if (getline(is, part, ','))
				x = stoi(StrUtil::trim(part));

			if (getline(is, part, ','))
				y = stoi(StrUtil::trim(part));

			if (getline(is, part, ','))
				r = stoi(StrUtil::trim(part));
			sh = std::make_shared<Shape>(make_shared<moduru::gui::Point>(x, y), r);
		}
		else if (StrUtil::eqIgnoreCase(tag->getTagType().to_cppstring(), "area") && (StrUtil::eqIgnoreCase(tag->getArgumentValue(String("shape")).to_cppstring(), "poly") || StrUtil::eqIgnoreCase(tag->getArgumentValue(String("shape")).to_cppstring(), "polygon"))) {
			istringstream is(tag->getArgumentValue(String("coords")).to_cppstring());
			string part;
			auto poly = make_shared<moduru::gui::Polygon>();
			while (getline(is, part, ',')) {
				int x = 0, y = 0;
				x = stoi(StrUtil::trim(part));
				if (getline(is, part, ','))
					y = stoi(StrUtil::trim(part));
				poly->addPoint(x, y);
			}
			sh = std::make_shared<Shape>(poly);
		}
		if (sh != nullptr) {
            auto argVal = tag->getArgumentValue(String("href"));
			sh->set_href(&argVal);
            argVal = tag->getArgumentValue(String("alt"));
			sh->set_alt(&argVal);
            argVal = tag->getArgumentValue(String("onMouseOver"));
			sh->set_onMouseOver(&argVal);
            argVal = tag->getArgumentValue(String("onMouseOut"));
			sh->set_onMouseOut(&argVal);
            argVal = tag->getArgumentValue(String("onClick"));
			sh->set_onClick(&argVal);
			lSl->add_shape(sh);
		}
		delete tag;
		tag = nextTag();
	}
}

HTMLTag* HTMLParser::nextTag()
{
	String newtag;
	int i = 0, j = 0;
	for (i = parseIndex; i < html->length(); i++) {
		if (html->charAt(i) == '<') {
			for (j = i; j < html->length() && html->charAt(j) != '>'; j++) {
				string s1 = newtag.to_cppstring();
				string s2 = html->to_cppstring().substr(j, 1);
				string res = s1 + s2;
				newtag = String(res);
			}
			newtag = String(newtag.to_cppstring() + ">");
			parseIndex = j + 1;
			return new HTMLTag(new String(newtag));
		}
	}
	return nullptr;
}

HTMLParser::~HTMLParser() {
	delete html;
	html = nullptr;
}
