#ifndef DAEMON_H_
#define DAEMON_H_

#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>

#include <fstream>

#include "nodeDoc.h"
#include "xmlEngine.h"
#include "selectFile.h"
#include "regExp.h"
#include "metaRank.h"
#include "UIDSession.h"
#include "shttpd.h"

#define WEBINTERFACE_URI "/WebInterface/*"
#define WEBINTERFACE_PATH "/home/olivier/workspace/tddaemonclient/www/com.teardrop.WebInterface/"
#define WEBINTERFACE_ROOT_SIZE 14
#define WEBINTERFACE_ENGINES_ICONS_URI "/engines_icons/*"
#define WEBINTERFACE_ENGINES_ICONS "/home/olivier/workspace/tdengines/icons/"
#define WEBINTERFACE_ENGINES_ICONS_SIZE 15
#define NEXT_RESULTS "/services/get_next_results"
#define QUERY_POST "/services/query_post"
#define REQUEST_TREE "/services/request_tree"

using namespace std;

class TdDaemon {

public:
	static int launchDaemon(tdParam *tdp);
	
private:
	static string createJSON(bool final, metaRank *mr);
	static string newQuery(struct shttpd_arg *arg, string query,string engines,string limit);
	static string get_next_results(struct shttpd_arg *arg);
	static void show_index(struct shttpd_arg *arg);
	static string show_tree();
	static void show_404(struct shttpd_arg *arg);
	static void query_process(struct shttpd_arg *arg);
	static void show_file(struct shttpd_arg *arg,string filename);
	static void show_engines_icons(struct shttpd_arg *arg);
	static void show_wi(struct shttpd_arg *arg);
	
	
	//The map that will hold sessions
    static map<string, metaRank*> globalSearches;
    //The teardrop parameters
	static tdParam *tdp;
};

#endif /*DAEMON_H_*/
