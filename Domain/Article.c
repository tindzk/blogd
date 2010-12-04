#import "Article.h"

#define self Article

def(void, Init) {
	this->title    = HeapString(0);
	this->path     = HeapString(0);
	this->date     = Date_Empty();
	this->descr    = Body_Empty();
	this->contents = Body_Empty();
	this->sections = Sections_New(0);
}

def(void, Destroy) {
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
