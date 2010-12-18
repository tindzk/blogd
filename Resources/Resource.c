#import "Resource.h"

MainTemplate GetMainTemplate(SessionInstance sess) {
	UserSessionInstance usrSess = (UserSessionInstance) Session_GetData(sess);

	ConfigurationInstance config = Configuration_GetInstance();

	MainTemplate main;

	main.page       = pgNone;
	main.blogName   = Configuration_GetTitle(config);
	main.flattr     = Configuration_GetFlattr(config).len != 0;
	main.url        = Configuration_GetUrl(config);
	main.descr      = Configuration_GetDescr(config);
	main.copyright  = Configuration_GetCopyright(config);
	main.mail       = Configuration_GetMail(config);
	main.author     = Configuration_GetAuthor(config);
	main.title      = Configuration_GetTitle(config);
	main.external   = Configuration_GetExternal(config);
	main.body       = (Template) EmptyCallback();
	main.footer     = (Template) EmptyCallback();
	main.categories = Categories_GetInstance();
	main.cats       = Categories_GetRoot(main.categories);
	main.catId      = -1;
	main.style      = UserSession_GetStyle(usrSess);

	if (main.style.len == 0) {
		main.style = Blog_DefaultStyle;
	}

	return main;
}
