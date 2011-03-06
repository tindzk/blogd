#import "Categories.h"
#import "Article.h"

#define self Article

rsdef(self *, New) {
	self *res = Pool_Alloc(Pool_GetInstance(), sizeof(self));

	res->lng      = String_New(0);
	res->tags     = String_New(0);
	res->title    = String_New(0);
	res->path     = String_New(0);
	res->date     = Date_Empty();
	res->descr    = Body_New();
	res->contents = Body_New();
	res->sections = Sections_New(0);

	return res;
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

	Pool_Free(Pool_GetInstance(), this);
}

def(void, SetPath, String path) {
	String_Assign(&this->path, path);
}

def(ProtString, GetPath) {
	return this->path.prot;
}

def(void, SetTitle, String title) {
	String_Assign(&this->title, title);
}

def(ProtString, GetTitle) {
	return this->title.prot;
}

def(void, SetLanguage, String lng) {
	String_Assign(&this->lng, lng);
}

def(ProtString, GetLanguage) {
	if (this->lng.len == 0) {
		return Blog_DefaultLanguage;
	}

	return this->lng.prot;
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

def(ProtString, GetTags) {
	if (this->tags.buf == NULL) {
		this->tags = String_New(32);

		CategoryArray *cats = Categories_GetCategories(Categories_GetInstance());

		foreach (cat, cats) {
			foreach (article, cat->articles) {
				if (Article_Equals(*article, this)) {
					String_Append(&this->tags, cat->name.prot);
					String_Append(&this->tags, ',');

					break;
				}
			}
		}

		if (this->tags.len > 0) {
			String_Crop(&this->tags, 0, -1);
		}
	}

	return this->tags.prot;
}

def(void, AddSection, String title, Body body) {
	Sections_Push(&this->sections, (Section) {
		.title = title,
		.body  = body
	});
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
