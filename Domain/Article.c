#import "Categories.h"
#import "Article.h"

#define self Article

def(void, Init) {
	this->lng      = $("");
	this->tags     = $("");
	this->title    = $("");
	this->path     = $("");
	this->date     = Date_Empty();
	this->descr    = Body_Empty();
	this->contents = Body_Empty();
	this->sections = Sections_New(0);
}

def(void, Destroy) {
	String_Destroy(&this->lng);
	String_Destroy(&this->tags);
	String_Destroy(&this->title);
	String_Destroy(&this->path);
	Body_Destroy(&this->descr);
	Body_Destroy(&this->contents);

	foreach (sect, this->sections) {
		String_Destroy(&sect->title);
		Body_Destroy(&sect->body);
	}

	Sections_Free(this->sections);
}

def(void, SetPath, String path) {
	String_Copy(&this->path, path);
}

def(String, GetPath) {
	return String_Disown(this->path);
}

def(void, SetTitle, String title) {
	String_Copy(&this->title, title);
}

def(String, GetTitle) {
	return String_Disown(this->title);
}

def(void, SetLanguage, String lng) {
	String_Copy(&this->lng, lng);
}

def(String, GetLanguage) {
	if (this->lng.len == 0) {
		return Blog_DefaultLanguage;
	}

	return String_Disown(this->lng);
}

def(void, SetDate, Date date) {
	this->date = date;
}

def(Date, GetDate) {
	return this->date;
}

def(void, SetDescr, Body descr) {
	Body_Destroy(&this->descr);
	this->descr = descr;
}

def(Body *, GetDescr) {
	return &this->descr;
}

def(void, SetContents, Body contents) {
	Body_Destroy(&this->contents);
	this->contents = contents;
}

def(Body *, GetContents) {
	return &this->contents;
}

def(String, GetTags) {
	if (this->tags.buf == NULL) {
		this->tags = String_New(32);

		CategoryArray *cats = Categories_GetCategories(Categories_GetInstance());

		foreach (cat, cats) {
			foreach (article, cat->articles) {
				if (Article_Equals(*article, this)) {
					String_Append(&this->tags, cat->name);
					String_Append(&this->tags, ',');

					break;
				}
			}
		}

		if (this->tags.len > 0) {
			String_Crop(&this->tags, 0, -1);
		}
	}

	return String_Disown(this->tags);
}

def(void, AddSection, String title, Body body) {
	Section sect = {
		.title = String_Clone(title),
		.body  = body
	};

	Sections_Push(&this->sections, sect);
}

def(Sections *, GetSections) {
	return this->sections;
}

def(bool, IsEmpty) {
	return this->sections->len == 0
		&& this->contents.type == Body_Type_Empty;
}

sdef(short, ComparePath, ArticleInstance *a, ArticleInstance *b) {
	return String_NaturalCompare(Article_GetPath(*b), Article_GetPath(*a));
}

sdef(short, CompareDate, ArticleInstance *a, ArticleInstance *b) {
	return Date_Compare(Article_GetDate(*b), Article_GetDate(*a));
}
