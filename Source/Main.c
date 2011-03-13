#import <Debit/Application.h>

#import <Domain/UserSession.h>
#import <Domain/Configuration.h>
#import <Domain/ArticleListing.h>

#define self Application

def(void, OnInit) {
	RdString path =
		(this->args->len == 0)
			? Blog_ConfigPath
			: this->args->buf[0];

	CategoriesInstance cats = Categories_GetInstance();
	Categories_SetLogger(cats, &this->logger);

	ConfigurationInstance config = Configuration_GetInstance();
	Configuration_SetLogger(config, &this->logger);
	Configuration_Parse(config, path);

	ArticleListingInstance listing = ArticleListing_GetInstance();
	ArticleListing_SetLogger(listing, &this->logger);
	ArticleListing_Populate(listing,
		Configuration_GetArticlePath(config));

	SessionManagerInstance sessMgr = SessionManager_GetInstance();
	SessionManager_SetBackend(sessMgr, UserSession_GetImpl());
}
